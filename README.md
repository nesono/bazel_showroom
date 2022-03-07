# Building with Bazel Walk-Through

## Introduction

### Abstract

Bazel is a very powerful and comprehensive build tool that was released by Google as open source. It breaking with many of the existing build tooling habits and requires a lot of relearning. We try to compress the learning time by this guide as much as sensible.

The bazel documentation also has a [C++ tutorial](https://bazel.build/tutorials/cpp) you might want to check out first. The tutorial is not a prerequisite for this walk through, but you might find it interesting nevertheless.

### Basics

A bazel repository usually consists of a `WORKSPACE` file in the root directory of the repository. For now, you can just leave this file blank, but it needs to be there. It tell Bazel that everything within this directory belongs to one workspace. All sources of the project should be placed below that directory.

Bazel also uses a `BUILD` file in each sub directory in which you can specify your targets (i.e. libraries, tests, binaries, etc.) using a language called [Starlark](https://bazel.build/rules/language). In short, Starlark is a dialect of Python with some limitations.

Make sure you have Bazel, or even better: [Bazelisk](https://github.com/bazelbuild/bazelisk) installed. Bazelisk will wrap Bazel and help you manage versions and consumption of Bazel in your projects.


If you ever run into issues with checking out or need to clean your changes, please try the following commands for cleaning and resetting your workspace:

```bash
git clean -xf
git reset --hard master
```

### Targets, Packages, Labels

Bazel has excellent [documentation](https://bazel.build/) about the specific terms. Please make sure you understand the terms [Targets, Packages and Labels](https://bazel.build/concepts/build-ref) since we are using them constantly throughout this tutorial. As a quick reference for the future, you can get some details on targets using the following command:

```bash
bazelisk help target-syntax
```

## C/C++ Related Bazel Usage

### Creating a C++ library

To get the code we need in this step has the following structure:

```
.
├── BUILD
├── pilib
│   ├── BUILD
│   ├── include
│   │   └── pilib.h
│   └── src
│       ├── constants.h
│       └── pilib.cc
└── WORKSPACE
```

Please note that we chose a somewhat special directory structure here to make the example a little more interesting. Normally, one would keep all includes and source files that contribute to one library on the same directory level. 

Feel free to have a quick look at the code - the code is fairly minimal.

Now let's build the repository using the target label "//pilib:pi"

```bash
bazel build //pilib:pi
```

The command should exit cleanly after some progress information. This is how it looked on my machine:

```bash
bazel build //pilib:pi
```
The results are as follows:
```
INFO: Analysed target //pilib:pi (7 packages loaded).
INFO: Found 1 target...
Target //pilib:pi up-to-date:
  bazel-bin/pilib/libpi.a
  bazel-bin/pilib/libpi.so
INFO: Elapsed time: 0.872s, Critical Path: 0.24s
INFO: 3 processes, linux-sandbox.
INFO: Build completed successfully, 6 total actions
AIDLL25733097:bazel_showroom (git:bazel_showroom.git:(
```

Now let's have a look at the file pilib/BUILD:

```Python
cc_library(
        name = "pi",
        srcs = glob([
            "src/*.cc",
            "src/*.h",
            "include/*.h",
        ]),
        includes = ["include"],
        visibility = ["//visibility:public"],
 )
 ```

First parameter in library rule is the name `pi`, which must be unique within the package.

Next we define the source files using the `glob` function, where we include all sources and headers that we don't want to have available for our consumers (sometimes called private headers). The headers that will be made available to the consumers of our little library are defined using the `hdrs` parameter. 

Note: Do not expose internal headers with the `hdrs` parameter but keep those in `srcs`.

A glob can be instructed to recursively select files. An example is `glob(["src/**/*.cc"])` which would select every cc file in the src folder including subfolders.

**Caveat**

Recursive globs are ment to not cross package boundaries. Example: given `a/BUILD` with a `glob([**])` won't select anything in folder `a/b` if there is a BUILD file `a/b/BUILD`.

Next, we specify the include path using `includes`. Note that `includes` is not a mandatory parameter. We will have a look at the effect if this parameter when we consume the library later.

The last parameter defines the `visibility` of the target. We set the visibility of it to public for now and will have a closer look at the effects of `visibility` later. In general it's a good practice to limit the visibility of a target as much as possible.

You can find more details about cc_library, cc_binary in the [online documentation](https://bazel.build/reference/be/c-cpp).

### C++ Compiler options used when compiling libraries

The `copts` parameter adds preprocessor directives that are **not** exposed to the library's consumers:

```Python
cc_library(
    name = "foo",
    copts = ["-DUSE_VARIADIC_MACROS"]
)
```

In case you want to provide your library consumers a way have specific preprocessor definitions, use the defines parameter. The defines are also used during compilation of the library itself. Be careful with that, though. It's not the recommended way unless specifically desired.

```Python
cc_library(
    name = "foo",
    defines = ["CHESS_ENABLED"]
)
```

### Creating a C++ binary

Let's add a binary that consumes our library. You can get to that by checking out commit `25176e2`:


The file main/BUILD reads as follows:

```Python
cc_binary (
        name = "main",
        srcs = ["main.cc"],
        deps = ["//pilib:pi"],
        linkstatic = 1,
)
```

Most of the parameters in this rule should be familiar to you. Note that we added the dependency `//pilib:pi` to the binary, to consume the library in our application. Since we want to make things simple, we also added the parameter linkstatic to the binary, which prevents bazel from creating dynamic (dll/so) objects.

Let's look at what the flags do by trying out some changes and see the effects. If you want to see the full effect, you might want to do a bazel clean in between the steps to make sure you get everything rebuilt.

### Redaction with C++ or Making Compilation Reproducible

Let's check one of the special moves of Bazel - redaction. Bazel claims to produce fast and correct builds. Fast in this case probably refers to the well defined dependency model, which makes rebuilds unnecessary most of the time. Now what does correct mean? Maybe we could use the word "reproducible" instead? What bazel does is, that it takes control of all of your projects dependencies and encodes them in a tree. Every node has an associated hash and the hash of the node depends on all inputs of that node, including compilers and dependencies.

For a simple demonstration, check out the contents of `main.cc`:

```cpp
#include <iostream>
 
//#include "pilib.h"
 
int main(int argc, char** argv)
{
    std::cout << "Today: " << __DATE__ << std::endl;
    std::cout << "Time:  " << __TIME__ << std::endl;
    std::cout << "Hello Ma(i)n" << std::endl;
    return 0;
}
```
Let's see what happens when we run this code:

```bash
bazel run //main
```
This produces the output:
```
INFO: Analysed target //main:main (5 packages loaded).
INFO: Found 1 target...
Target //main:main up-to-date:
  bazel-bin/main/main
INFO: Elapsed time: 0.842s, Critical Path: 0.32s
INFO: 2 processes, linux-sandbox.
INFO: Build completed successfully, 3 total actions
 
INFO: Running command line: bazel-bin/main/main
Today: redacted
Time:  redacted
Hello Ma(i)n
```

Seems like Bazel replaced these common macros with a fixed string, snap! If you still wonder why think of a large source base, where some source files use these macros to encode the data into the binaries. These files will have to be built every time you hit build and two objects will never be the same - what a disater for caching! You might think that you sometimes want these things to be placed in your code. 

If you're curious how that would work: Bazel allows that with so called linkstamps, but that is a topic for another time.

### C++ Library Header Configuration

Bazel has some parameters that control how consumers of a `cc_library` shall include the library headers. The parameters allow you to unlink the include path from the actual file system location for the headers. You can only use the settings once per `cc_library`.

For system and external libraries, it makes sense to use the `includes` parameter, which adds the specified directory available to consumers using the `-isystem` compiler flag. Note that this flag suppresses all warnings from these headers, that includes templates declared in those headers.

For all other libraries (especially libraries developed in house), we usually don't specify the search paths explicitely but let Bazel manage it for us.
All headers will be available relative to the `BUILD` file where the target is specified. If you look at the directory structure for pilib for example, the header would have to be consumed using

```cpp
#include "include/pilib.h"
```

That is of course an ugly example, but it explains the result well.

`strip_include_prefix`: The specified string will be stripped from the required include statement. That means that if we use
```Python
strip_include_prefix = "include"
```
in `pilib`, the header would be available using
```cpp
#include "pilib.h"
```
`include_prefix`: The specified string will be added to the required include statement. If we use
```Python
include_prefix = "pilib"
```
in addition to the `strip_include_prefix` in `pilib`, the header will be available using
```cpp
#include "pilib/pilib.h"
```
If  we omit the `strip_include_prefix` option, the header will be available using
```cpp
#include "pilib/include/pilib.h"
```


## Managing Dependencies in Bazel

### Managing Visibility in Bazel

Managing visibility in Bazel provides us with a way to allow the consumption only by specific targets or packages. You can find an example in pilib/BUILD:

```Python
cc_library(
        name = "pi",
        srcs = glob(
                [ "src/*.cc",
                  "src/*.h" ],
                ),
        hdrs = glob(
                ["include/*.h"],
                ),
        includes = ["include"],
        visibility = ["//main:__pkg__"],
)
```

In this example we only allow the package `//main` to consume `//pilib:pi`.

### Consuming External Dependencies

In this example we use Google Test for testing C++ code. So let's see how we can consume Google Test in our workspace. The easiest way is to add something like to the `WORKSPACE` file:

```Python
git_repository(
    name = "googletest",
    commit = "b4676595c03a26bb84f68542c8b74d3d89b38b68",
    remote = "https://github.com/google/googletest",
    shallow_since = "1560978287 -0400",
)

```
The rule `git_repository` is a so-called "`WORKSPACE` rule", i.e. rules that can be used in `WORKSPACE` files (or files sourced from `WORKSPACE`). Note that there also exists `http_archive`, which is usually better performing than `git_repository` since it does not have to clone the repository first.

You might want to check the [`BUILD`](https://github.com/google/googletest/blob/c9461a9b55ba954df0489bab6420eb297bed846b/BUILD.bazel#L80) file of Google Test that is consumed implicitely using the above declaration. It starts with the following:

```Python
cc_library(
    name = "gtest",
    srcs = glob(
        include = [
            "googletest/src/*.cc",
            "googletest/src/*.h",
            "googletest/include/gtest/**/*.h",
            "googlemock/src/*.cc",
            "googlemock/include/gmock/**/*.h",
        ],
        exclude = [
            "googletest/src/gtest-all.cc",
            "googletest/src/gtest_main.cc",
            "googlemock/src/gmock-all.cc",
            "googlemock/src/gmock_main.cc",
        ],
    ...
)
```

This should look somewhat familiar to you now. Note the usage of the include and exclude parameters for the `glob` function.

In the checked out revision, we use Google Test for an acceptance test in `//main`.

```Python
cc_binary (
    name = "main",
    srcs = ["main.cc"],
    deps = ["//pilib:pi"],
    linkstatic = 1,
)
 
cc_test (
    name = "pi_acceptance",
    srcs = ["test.cc"],
    linkstatic = 1,
    deps = [
        "@googletest//:gtest",
        "//pilib:pi"],
    size = "small",
)
```

As you can see, a `cc_test` was added with two dependencies, one of which is Google Test. The dependency is composed of multiple indicators:

The `@` indicates an external package
The string that follows until the `//` refers to the name of the `git_repository` target.
After that, the target label is given as usual (:gtest)
The `size` attribute indicates how "heavy" the test is. The [Bazel documentation](https://bazel.build/reference/test-encyclopedia) has a table that maps the size string to default timeouts for each size. 

### Using Data Dependencies

Sometimes we depend on data in our tests or binaries and bazel needs to know about them as well, of course. Otherwise they won't show up when running in the sandbox.

We added a directory called datatest that contains a file named tag, which in turn contains a magic number. Now let's assume we want to check our code against the contents of that file. Let's have a look at `datatest/BUILD`:

```Python
cc_test(
    name = "datatest",
    srcs = ["datatest.cc"],
    deps = ["@googletest//:gtest"],
    data = [":tagfile"],
)
 
filegroup(
    name = "tagfile",
    srcs = ["tag"],
)
```

Note the use of the filegroup rule for declaring the data to Bazel. We consume that data file in our test using the `data` keyword, very similar as we did with the `deps` keyword.

Now where does Bazel put that file to? I am sure you now have the means to find that out, but let us introduce another common practice to do that. We can ask Bazel to not actual run the binary but put everything it would have done into a script using the following command:

```bash
bazel build //... # might be required before calling run below (probably a bug)
```

```bash
bazel run --script_path=run.sh //datatest
```

This is what `run.sh` contains on my machine:

```bash
#!/bin/sh
cd /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/datatest/datatest.runfiles/__main__ && \
  exec /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/datatest/datatest "$@"
```

The structure of the working directory seems familiar, doesn't it? Let's check the contents of the directory:

```
/private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/datatest/datatest.runfiles/__main__
└── datatest
    ├── datatest -> /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/datatest/datatest
    └── tag -> /Users/kuze/Developer/bazel_showroom/datatest/tag
```

We can see that it contains a directory datatest that in turn contains a link to the binary called datatest and a link to the file tag. Feel free to remove the data dependency from datatest and run it again, the file should have been vanished.

### Transitive Data Dependencies

There is actually nothing specific to transitive data dependencies, since they work exactly like other transitive dependencies, but we made an example anyways, because it's so much fun.

You see that the datalib exposes an interface to fetch it's data called data lib::consume_data(). If you have a look into the implementation of consume_data() you will see that it's simply opening a file (exposing a runtime dependency to the consuming application) and return the contents of that data file.

Let's have a look at `main/main.cc`, which consumes the data and prints it's contents:

```cpp
#include <iostream>
 
#include "pilib/include/pilib.h"
#include "platformlib/platform.h"
#include "datalib/datalib.h"
 
int main(int argc, char** argv)
{
    std::cout << "Today: " << __DATE__ << std::endl;
    std::cout << "Time:  " << __TIME__ << std::endl;
    std::cout << "Hello Ma(i)n" << std::endl;
    pi::printPi();
    platform::printPlatform();
    std::cout << "Consumed data: " << datalib::consume_data() << std::endl;
    return 0;
}
```

Now, if the data file wasn't found, we'd expect a `runtime_error` exception being thrown. Feel free to deactivate the data dependency and see what will happen.

## Bazel Tricks and Licks

### Bazel Subcommands

Use the option -s to have bazel print out it's sub commands, to see what's happening under the hood, e.g. as in

```bash
bazel build -s //...
```

Running Bazel with sub commands, you should try and see the effect of the following changes:

* Disable or remove `linkstatic` and re-enable it again.
* Enable the actual inclusion of the header in `main.cc` and see what happens when you
    * Remove and add the `pilib` dependency (check the sandbox contents!)
    * Remove the `includes` statement in the library and add it back again

When you do these changes, check out the following section that helps you to look into the bazel sandbox.

### Looking into the Bazel Sandbox

Pro tip: If you want to understand the lay out of the bazel sandbox, where bazel builds the software have a look at the sub commands, especially where the commands are executed. This is how one compilation command looks on my machine:

```bash
SUBCOMMAND: # //main:main [action 'Compiling main/main.cc']
(cd /home/jochen/.cache/bazel/_bazel_jochen/d043aeb0db5f7f2a949d47d676de24a8/execroot/__main__ && \
  exec env - \
    PWD=/proc/self/cwd \
  /usr/bin/gcc -U_FORTIFY_SOURCE -fstack-protector -Wall -B/usr/bin -B/usr/bin -Wunused-but-set-parameter -Wno-free-nonheap-object -fno-omit-frame-pointer '-std=c++0x' -MD -MF bazel-out/k8-fastbuild/bin/main/_objs/main/main/main.pic.d '-frandom-seed=bazel-out/k8-fastbuild/bin/main/_objs/main/main/main.pic.o' -fPIC -iquote . -iquote bazel-out/k8-fastbuild/genfiles -iquote external/bazel_tools -iquote bazel-out/k8-fastbuild/genfiles/external/bazel_tools -isystem pilib/include -isystem bazel-out/k8-fastbuild/genfiles/pilib/include -isystem bazel-out/k8-fastbuild/bin/pilib/include -fno-canonical-system-headers -Wno-builtin-macro-redefined '-D__DATE__="redacted"' '-D__TIMESTAMP__="redacted"' '-D__TIME__="redacted"' -c main/main.cc -o bazel-out/k8-fastbuild/bin/main/_objs/main/main/main.pic.o)
```

First important piece is the line where we change the working directory into the cache:

```bash
cd /home/jochen/.cache/bazel/_bazel_jochen/d043aeb0db5f7f2a949d47d676de24a8/execroot/__main__
```

Let's change into that directory and see what we find there.

Bonus tip: Install and use the `tree` command to see the full directory structure printed to the console. What you will see are quite some symbolic links pointing into other places in the cache as well as into the workspace of our little example.

Note also that you can see the full compilation/linkage commands in the output!

### Working Directories

Now let's examine in which directory our tests and runs are actually executed. We of course prepared something for you here as well, check it out like this:

Let's have a quick look at the file `main/test.cc`:

```cpp
#include <gtest/gtest.h>
#include <unistd.h>
 
TEST(main, get_cwd)
{
    char cwd[1024];
 
    ASSERT_NE(nullptr, getcwd(cwd, sizeof(cwd)));
    std::cout << "Current Working Directory" << std::endl;
    std::cout << cwd << std::endl;
}
```

You can see that we get the current working directory with `getcwd` and print it to the console.

### Bazel Test Working Directories

Let's run our simple test and see what it will print!

```Bash
bazel test --test_output=all //...
```
This will produce the following output:
```
...
INFO: From Testing //main:test:
==================== Test output for //main:test:
Running main() from gtest_main.cc
[==========] Running 1 test from 1 test case.
[----------] Global test environment set-up.
[----------] 1 test from main
[ RUN      ] main.get_cwd
Current Working Directory
/private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/bazel-sandbox/1599912547847164232/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/test.runfiles/__main__
[       OK ] main.get_cwd (0 ms)
[----------] 1 test from main (0 ms total)
 
[----------] Global test environment tear-down
[==========] 1 test from 1 test case ran. (0 ms total)
[  PASSED  ] 1 test.
```

From here you have a perfect view on the efforts that Bazel puts into getting your executions run in a sandbox. Let's try to look what's inside the directory:

```bash
ls /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/bazel-sandbox/1599912547847164232/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/test.runfiles/__main__
```
Which is resulting in:
```
ls: /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/bazel-sandbox/1599912547847164232/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/test.runfiles/__main__: No such file or directory
```

Hm, bazel cleaned it up already. You can enable the option `--sandbox_debug` to keep it after Bazel finishes, but just for fun, let's inject some code that tells us what the contents of our working directory are during runtime. To get the contents I simply put the following line to the end of `TEST(main, get_cwd)`:

```cpp
system("ls -lR > /tmp/bazel_test_lsR.txt");
```

Then run the tests again using:

```bash
bazel test //...
```

And let's check what we wrote into the temp file.

```bash
cat /tmp/bazel_test_lsR.txt
```
Here is what I've got:
```
total 0
drwxr-xr-x  3 kuze  wheel  96 May 28 05:39 main
 
./main:
total 0
lrwxr-xr-x  1 kuze  wheel  120 May 28 05:39 test -> /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/test
```

That means that we have one directory `main` that contains one binary `test` (which is in turn a symbolic link). Pretty straight forward and it really contains all we need to run the test. You can even try to run the executable directly now, it should still be there and it will print out your current working directory.

It's a good time to delete the temp file before running the next step.

### Working Directories With Multiple Runs Per Tests

Let's change the system call at the end of `TEST(main, get_cwd)` to check where tests are run when tests are run multiple times. This is how it looks like in my editor:

```cpp
system("pwd > /tmp/bazel_test_lsR_$$.txt");
```

Note the `$$` at the end of the temp file - it makes sure that we don't overwrite the same file over and over again, but use the process' ID as a file name distinction.

Now let's check what happens, when we run 

```bash
bazel test --runs_per_test=3 //...
```

We now should see three output files. Let's show what they contain:

```bash
grep ""  /tmp/bazel_test_lsR_*
```
Gives me:
```
/tmp/bazel_test_lsR_25353.txt:/private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/bazel-sandbox/3585304366782971843/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/test.runfiles/__main__
/tmp/bazel_test_lsR_25354.txt:/private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/bazel-sandbox/1798063428567112278/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/test.runfiles/__main__
/tmp/bazel_test_lsR_25355.txt:/private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/bazel-sandbox/4982931142553821730/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/test.runfiles/__main__
```

You might have to scroll to the right to see that they are all executed in a different directory. Their content is all the same just as with the single test shown above, i.e. one directory main containing a link to the binary test.

### Bazel Run Working Directory

Let's add the code for getting the current working directory to the main function so that it looks like the following:

```cpp
#include <iostream>
#include <unistd.h>
#include <cassert>
 
#include "pilib/include/pilib.h"
#include "platformlib/platform.h"
 
int main(int argc, char** argv)
{
    char cwd[1024];
 
    assert(getcwd(cwd, sizeof(cwd)));
    std::cout << "Current Working Directory" << std::endl;
    std::cout << cwd << std::endl;
    std::cout << "Today: " << __DATE__ << std::endl;
    std::cout << "Time:  " << __TIME__ << std::endl;
    std::cout << "Hello Ma(i)n" << std::endl;
    pi::printPi();
    platform::printPlatform();
    return 0;
}
```
When I ran the binary

```bash
bazel run //main
```
I got the following output
```
INFO: Analysed target //main:main (0 packages loaded).
INFO: Found 1 target...
Target //main:main up-to-date:
  bazel-bin/main/main
INFO: Elapsed time: 0.138s, Critical Path: 0.00s
INFO: Build completed successfully, 1 total action
 
INFO: Running command line: bazel-bin/main/main
Current Working Directory
/private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/main.runfiles/__main__
Today: redacted
Time:  redacted
Hello Ma(i)n
Pi is: 3.14159
platform: osx
```
In the working directory, I find another symbolic link by executing:

```bash
ls -l /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/main.runfiles/__main__/main/main
```
Which is printing:
```
lrwxr-xr-x  1 kuze  wheel  120 May 25 16:26 /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/main.runfiles/__main__/main/main -> /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/main
```

The target of that link is the actual executable:

```bash
file /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/main
```
Giving me:
```
/private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/main: Mach-O 64-bit executable x86_64
```

After finishing your investigations, don't forget to undo all your changes using the following command before proceeding with this walk through:

```bash
git reset --hard HEAD
```

## Platform Specific Configurations

Bazel has built-in support for platform specifics configurations.

The first notable setting can be found in common/BUILD, where we define config settings based on bazel's provided platform values.

```Python 
# create config settings based on bazel provided platform values
config_setting(
    name = "osx",
    values = { "cpu": "darwin" },
    visibility = ["//visibility:public"],
)
config_setting(
    name = "linux",
    values = { "cpu": "k8" },
    visibility = ["//visibility:public"],
)
```

Note also here the usage of the visibility parameter. The label `k8` is Bazel's way of identifying the `x86_64` platform. All we did with these declarations is telling Bazel that we want to switch declarations based on these two settings somewhere in our `BUILD` files.

We apply these configurations in the file `platformlib/BUILD` using the function select:

```Python
cc_library(
    name = "platform",
    srcs = select({
        "//common:osx": ["platform_macos.cc"],
        "//common:linux": ["platform_linux.cc"],
        "//conditions:default": ["platform_unknown.cc"],
        }),
    hdrs = ["platform.h"],
    includes = ["."],
    visibility = ["//visibility:public"],
)
```

We select a specific source file based on the configuration settings defined in `common/BUILD`.

Let's run the code and check what platform you work on! I've got the following (including the bonus information number pi):

```bash
bazel run //main
```
Which produces the output:
```
INFO: Analysed target //main:main (0 packages loaded).
INFO: Found 1 target...
Target //main:main up-to-date:
  bazel-bin/main/main
INFO: Elapsed time: 1.581s, Critical Path: 1.36s
INFO: Build completed successfully, 5 total actions
 
INFO: Running command line: bazel-bin/main/main
Today: redacted
Time:  redacted
Hello Ma(i)n
Pi is: 3.14159
platform: osx
```

### Platform-specific targets

Sometimes you want to declare a rule to be built/run on only one platform, and by default Bazel assumes all targets are available for the current target platform. Use `restricted_to` to define on which platform a target should be built or run.

```Python
cc_binary(
    name = "platform.specific",
    restricted_to = "//:arm" # Select :arm for ARM aarch64 targets, and :k8 for x86_64 specific targets
)
```

Just for fun we removed the include directives from the `cc_library` calls just to see how it will look like - this seems to be the recommended way of how things work inside Google and common practice in the Bazel universe.

