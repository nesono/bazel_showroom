# Building with Bazel Walk-Through

## Abstract

Bazel is a very powerful and comprehensive build tool that was released by Google as open source. It breaking with many of the existing build tooling habits and requires a lot of relearning. We try to compress the learning time by this guide as much as sensible.

The bazel documentation also has a [C++ tutorial](https://bazel.build/tutorials/cpp) you might want to check out first. The tutorial is not a prerequisite for this walk through, but you might find it interesting nevertheless.

## Basics

A bazel repository usually consists of a `WORKSPACE` file in the root directory of the repository. For now, you can just leave this file blank, but it needs to be there. It tell Bazel that everything within this directory belongs to one workspace. All sources of the project should be placed below that directory.

Bazel also uses a `BUILD` file in each sub directory in which you can specify your targets (i.e. libraries, tests, binaries, etc.) using a language called [Starlark](https://bazel.build/rules/language). In short, Starlark is a dialect of Python with some limitations.

Make sure you have Bazel, or even better: [Bazelisk](https://github.com/bazelbuild/bazelisk) installed. Bazelisk will wrap Bazel and help you manage versions and consumption of Bazel in your projects.

## Targets, Packages, Labels

Bazel has excellent documentation about the specific terms, which can be found on bazel.build. Please make sure to make yourself familiar with the terms. We use them throughout our little tour. As a quick reference for the future, you can get some details on targets using the following command:

```bash
$ bazelisk help target-syntax
```

## Creating a library

To get the code we need in this step, please checkout the specific commit using:

```bash
$ git checkout 82781b2
```

If you ever run into issues with checking out or need to clean your changes, please try the following commands for cleaning and resetting your workspace:

```bash
$ git clean -xf
$ git reset --hard master
```

The repository should now have the following structure:

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

Please note that we chose a somewhat special directory structure here to make the example a little more interesting. Feel free to have a quick look at the code - it's fairly minimal.

Now let's build the repository using the target label "//pilib:pi"

```bash
$ bazel build //pilib:pi
```

The command should exit cleanly after some progress information. This is how it looked on my machine:

```bash
$ bazel build //pilib:pi
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

First parameter in library rule is the name "pi", which must be unique within the package.

Next we define the source files using the glob function, where we include all sources and headers that we don't want to have available for our consumers (sometimes called private headers). The headers that will be made available to the consumers of our little library are defined using the hdrs parameter. Note: you normally don't want to expose internal headers with the hdrs parameter.

---

A glob can be instructed to recursively select files. An example is `glob(["src/**/*.cc"])` which would select every cc file in the src folder including subfolders.

**Caveat**

Recursive globs don't select files through other package boundaries. Example: given `a/BUILD` with a `glob([**])` won't select anything in folder `a/b` if there is a BUILD file `a/b/BUILD`.

---

Next, we specify the include path using `includes`. Note that `includes` is not a mandatory parameter. We will have a look at the effect when we consume the library later.

The last parameter defines the `visibility` of the target. We set the visibility of it to public for now and will have a closer look at the effects of `visibility` later.

On bazel.io, you can find more details about cc_library, cc_binary, etc.

## Compiler options used when compiling libraries

The `copts` parameter adds preprocessor directives that are **not** exposed to the library's consumers:

```Python
cc_library(
    name = "foo",
    copts = ["-DUSE_VARIADIC_MACROS"]
)
```

In case you want to provide your library consumers a way have specific preprocessor definitions, use the defines parameter. The defines are also used during compilation of the library itself.

```Python
cc_library(
    name = "foo",
    defines = ["CHESS_ENABLED"]
)
```

## Creating a binary

Let's add a binary that consumes our library. You can get to that by checking out commit `25176e2`:

```bash
$ git checkout 25176e2
```

The file main/BUILD reads as follows:

```Python
cc_binary (
        name = "main",
        srcs = ["main.cc"],
        deps = ["//pilib:pi"],
        linkstatic = 1,
)
```

Most of the parameters in this rule should be familiar to you. Note that we added the dependency "//pilib:pi" to the binary, to consume the library in our application. Since we want to make things simple, we also added the parameter linkstatic to the binary, which prevents bazel from creating dynamic (dll/so) objects.

Let's look what the flags do by trying out some changes and see the effects. If you want to see the full effect, you might want to do a bazel clean in between the steps to make sure you get everything rebuilt.

---

Use the option -s to have bazel print out it's sub commands, to see what's happening under the hood, e.g. as in

```bash
$ bazel build -s //...
```

---

Now for the things you should try and see their effect:

* Disable or remove linkstatic  and re-enable it again.
* Enable the actual inclusion of the header in main.cc and see what happens when you
    * Remove and add the pilib dependency (check the sandbox contents!)
    * Remove the includes statement in the library and add it back again

When you do these changes, check out the following section that helps you to look into the bazel sandbox.

## Looking into the Bazel Sandbox

Pro tip: If you want to understand the lay out of the bazel sandbox, where bazel builds the software have a look at the sub commands, especially where the commands are executed. This is how one compilation command looks on my machine:

```bash
SUBCOMMAND: # //main:main [action 'Compiling main/main.cc']
(cd /home/jochen/.cache/bazel/_bazel_jochen/d043aeb0db5f7f2a949d47d676de24a8/execroot/__main__ && \
  exec env - \
    PWD=/proc/self/cwd \
  /usr/bin/gcc -U_FORTIFY_SOURCE -fstack-protector -Wall -B/usr/bin -B/usr/bin -Wunused-but-set-parameter -Wno-free-nonheap-object -fno-omit-frame-pointer '-std=c++0x' -MD -MF bazel-out/k8-fastbuild/bin/main/_objs/main/main/main.pic.d '-frandom-seed=bazel-out/k8-fastbuild/bin/main/_objs/main/main/main.pic.o' -fPIC -iquote . -iquote bazel-out/k8-fastbuild/genfiles -iquote external/bazel_tools -iquote bazel-out/k8-fastbuild/genfiles/external/bazel_tools -isystem pilib/include -isystem bazel-out/k8-fastbuild/genfiles/pilib/include -isystem bazel-out/k8-fastbuild/bin/pilib/include -fno-canonical-system-headers -Wno-builtin-macro-redefined '-D__DATE__="redacted"' '-D__TIMESTAMP__="redacted"' '-D__TIME__="redacted"' -c main/main.cc -o bazel-out/k8-fastbuild/bin/main/_objs/main/main/main.pic.o)
```

First important piece is the line where we change directory into the cache:

```bash
cd /home/jochen/.cache/bazel/_bazel_jochen/d043aeb0db5f7f2a949d47d676de24a8/execroot/__main__
```

Let's change into that directory and see what we find there.

Bonus pro-tip: Install and use the tree command to see the full directory structure printed to the console. What you will see are quite some symbolic links pointing into other places in the cache as well as into the workspace of our little example.

Note also that you can see the full compilation/linkage commands in the output!

## Demonstrate Redaction and Visibility

Let's check one of the special moves of bazel - redaction. Bazel claims to produce fast and correct builds. Fast in this case probably refers to the well defined dependency model, which makes rebuilds unnecessary most of the time. Now what does correct mean? Maybe we could use the word "reproducible" instead? What bazel does is it takes control of all of your projects dependencies and encodes them (you can think of it as if every product/action/system dependency is protected by hashes).

We did a very simple demonstration of this in our bazel showroom. Check it out like this:

$ git checkout 7ed34cb
Now have a look at main.cc

#include <iostream>
 
//#include "pilib.h"
 
int main(int argc, char** argv)
{
    std::cout << "Today: " << __DATE__ << std::endl;
    std::cout << "Time:  " << __TIME__ << std::endl;
    std::cout << "Hello Ma(i)n" << std::endl;
    return 0;
}
Let's see what happens when we run this code:

$ bazel run //main
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
Seems like bazel replaced these common macros with a fixed string, snap! If you still wonder why think of a large source base, where some source files use these macros to encode the data into the binaries. These files will have to be built every time you hit build and two objects will never be the same - what a bummer for caching! You might think that you sometimes want these things to be placed in your code. Bazel allows that with so called linkstamps, but that is a topic for another time.

Since we're at it, let's try another feature of the visibility setting: It provides us with a way to allow the consumption only by specific targets or packages. You can find an example in pilib/BUILD:

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
In this example we only allow the package //main to consume //pilib:pi.

Consuming External Dependencies and Testing
We use Google Test for testing C++ code here at work. So let's see how we can consume Google Test and add it to our workspace. The easiest way for you to it is this line:

$ git checkout d5dfa5d
Let's check what happened behind the surface. First, the external repository was added to the WORKSPACE file:

new_http_archive(
    name = "googletest",
    url = "https://github.com/google/googletest/archive/release-1.8.0.zip",
    sha256 = "f3ed3b58511efd272eb074a3a6d6fb79d7c2e6a0e374323d1e6bcbcc1ef141bf",
    build_file = "third_party/BUILD.gtest",
)
The rule new_http_archive is a so-called "WORKSPACE rule", i.e. rules that can be used in WORKSPACE files (or files sourced from WORKSPACE). Note that there exists both http_archive as well as new_http_archive. The "new" prefix simply tells bazel that it shall not look for a BUILD file in the external dependency, but a custom build file will be provided, which reads in our case as follows.

cc_library(
    name = "gtest",
    srcs = glob(
        include = [
            "googletest-release-1.8.0/googletest/src/*.cc",
            "googletest-release-1.8.0/googletest/src/*.h",
            "googletest-release-1.8.0/googletest/include/gtest/**/*.h"],
        exclude = [
            "googletest-release-1.8.0/googletest/src/gtest-all.cc",
            "googletest-release-1.8.0/googletest/src/gtest-main.cc"],
    ),
    hdrs = glob(
        ["googletest-release-1.8.0/googletest/include/gtest/gtest.h"],
    ),
    copts = ["-Iexternal/googletest/googletest-release-1.8.0/googletest"],
    includes = ["googletest-release-1.8.0/googletest/include",
             ],
    visibility = ["//visibility:public"],
    linkstatic = 1,
)
This should look somewhat familiar to you now. Note the usage of the include and exclude parameters for the glob function and the required copts parameter, which will be used when Google Test is built.

In the checked out revision, we use Google Test for an acceptance test in //main.

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
As you can see, a cc_test was added with two dependencies, one of which is Google Test. The dependency is composed of multiple indicators:

The "@" indicates an external package
The string that follows until the "//" refers to the name of the new_http_archive
After that, the target label is given as usual (:gtest)
The size indicates how "heavy" the test is. The Bazel documentation has a table that maps the size string to RAM, CPU codes, I/O fraction and default timeout. 

Platform Specific Configurations
Bazel has built in support for platform specifics configurations. It's a little special so we need to pull a bigger change set as you might think. As usual, you get the changes in O(1):

$ git checkout 26e41ed
The first notable change can be found in common/BUILD, where we define config settings based on bazel's provided platform values:

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
Note also here the usage of the visibility parameter. The label "k8" is bazel's way of identifying the x86_64 platform. All we did with these declarations is telling bazel that we want to switch declarations based on these two settings somewhere in our BUILD files.

We apply these configurations in the file platformlib/BUILD using the function select:

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
We select a specific source file based on the configuration settings define in common/BUILD. Note that we have convenience functions in Werkstatt for our supported platforms, which are (maybe) easier to use.

Let's run the code and check what platform you work on! I've got the following (including the bonus information number pi):

$ bazel run //main
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
Platform-specific targets
Sometimes you want to declare a rule to be built/run on only one platform, and by default Bazel assumes all targets are available for the current target platform. Use restricted_to to define on which platform a target should be built or run.

cc_binary(
    name = "platform.specific",
    restricted_to = "//:arm" # Select :arm for ARM aarch64 targets, and :k8 for x86_64 specific targets
)
Just for fun we removed the include directives from the cc_library calls just to see how it will look like - this seems to be the recommended way of how things work inside Google and it's for sure the way with the least ambiguities (smile) Note that this is not common practice at AID.

You can checkout the source using the following command:

$ git checkout 4664828

Test and Run Working Directories
Now let's examine in which directory our tests and runs are actually executed. We of course prepared something for you here as well, check it out like this:

$ git checkout 9b8b125
Let's have a quick look at the file main/test.cc:

#include <gtest/gtest.h>
#include <unistd.h>
 
TEST(main, get_cwd)
{
    char cwd[1024];
 
    ASSERT_NE(nullptr, getcwd(cwd, sizeof(cwd)));
    std::cout << "Current Working Directory" << std::endl;
    std::cout << cwd << std::endl;
}
You can see that we get the current working directory with getcwd and print it to the console.

Bazel Test Working Directories
Let's run our simple test and see what it will print!

$ bazel test --test_output=all //...
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
From here you have a perfect view on the efforts that bazel puts into getting your executions run in a sandbox. Let's try to look what's inside the directory:

$ ls /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/bazel-sandbox/1599912547847164232/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/test.runfiles/__main__
ls: /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/bazel-sandbox/1599912547847164232/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/test.runfiles/__main__: No such file or directory
Hm, bazel cleaned it up already. Let's inject some code that tells us what the contents of our working directory are during runtime. To get the contents I simply put the following line to the end of TEST(main, get_cwd):

system("ls -lR > /tmp/bazel_test_lsR.txt");
Then run the tests again using:

$ bazel test //...
And let's check what we wrote into the temp file. Here is what I've got:



$ cat /tmp/bazel_test_lsR.txt
total 0
drwxr-xr-x  3 kuze  wheel  96 May 28 05:39 main
 
./main:
total 0
lrwxr-xr-x  1 kuze  wheel  120 May 28 05:39 test -> /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/test
That means that we have one directory main that contains one binary test (which is in turn a symbolic link). Pretty straight forward and it really contains all we need to run the test. You can even try to run the executable directly now, it should still be there and it will print out your current working directory.

It's a good time to delete the temp file now before running the next step.

Let's change the system call at the end of TEST(main, get_cwd) to check where tests are run when tests are run multiple times. This is how it looks like in my editor:

system("pwd > /tmp/bazel_test_lsR_$$.txt");
Note the $$ at the end of the temp file - it makes sure that we don't overwrite the same file over and over again, but use the process' ID as a distinction.

Now let's check what happens, when we run 

$ bazel test --runs_per_test=3 //...
We now should see three output files. Let's show what they contain:

$ grep ""  /tmp/bazel_test_lsR_*
/tmp/bazel_test_lsR_25353.txt:/private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/bazel-sandbox/3585304366782971843/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/test.runfiles/__main__
/tmp/bazel_test_lsR_25354.txt:/private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/bazel-sandbox/1798063428567112278/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/test.runfiles/__main__
/tmp/bazel_test_lsR_25355.txt:/private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/bazel-sandbox/4982931142553821730/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/test.runfiles/__main__
You might have to scroll to the right to see that they are all executed in a different directory. Their content is all the same just as with the single test shown above, i.e. one directory main containing a link to the binary test.

Bazel Run Working Directory
Let's add the code for getting the current working directory to the main function so that it looks like the following:

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
When I ran the binary I got the following output

$ bazel run //main
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
In the working directory, I find another symbolic link:

$ ls -l /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/main.runfiles/__main__/main/main
lrwxr-xr-x  1 kuze  wheel  120 May 25 16:26 /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/main.runfiles/__main__/main/main -> /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/main
The target of that link is the actual executable:

$ file /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/main
/private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/main/main: Mach-O 64-bit executable x86_64
After finishing your investigations, don't forget to undo all your changes using the following command before proceeding with this walk through:

$ git reset --hard HEAD

Using Data Dependencies
Sometimes we depend on data in our tests or binaries and bazel needs to know about them as well, of course. Otherwise they won't show up when running in the sandbox.

Let's check out what we have prepared for you

$ git checkout 7165421
We added a directory called datatest that contains a file named tag, which in turn contains a magic number. Now let's assume we want to check our code against the contents of that file. Let's have a look at datatest/BUILD:

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
Note the use of the filegroup rule for declaring the data to bazel. We consume that data file in our test using the data keyword, very similar as we did with the deps keyword.

Now where does bazel put that file to? I am sure you now have the means to find that out, but let us introduce another common practice to do that. We can ask bazel to not actual run the binary but put everything it would have done into a script using the following command:

$ bazel build //... # might be required before calling run below (probably a bug)
$ bazel run --script_path=run.sh //datatest
This is what run.sh contains on my machine:

#!/bin/sh
cd /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/datatest/datatest.runfiles/__main__ && \
  exec /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/datatest/datatest "$@"
The structure of the working directory seem familiar, doesn't it? Let's check the contents of the directory:

/private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/datatest/datatest.runfiles/__main__
└── datatest
    ├── datatest -> /private/var/tmp/_bazel_kuze/7c16ad40d245d149d39e82eda2a03870/execroot/__main__/bazel-out/darwin-fastbuild/bin/datatest/datatest
    └── tag -> /Users/kuze/Developer/bazel_showroom/datatest/tag
We can see that it contains a directory datatest that in turn contains a link to the binary called datatest and a link to the file tag. Feel free to remove the data dependency from datatest and run it again, the file should have been vanished.

Transitive Data Dependencies
There is actually nothing specific to transitive data dependencies, since they work exactly like other transitive dependencies, but we made an example anyways, because it's so much fun.

Check out the sources like this:

$ git checkout c119f1b
You will see that the datalib now exposes an interface to fetch it's data called data lib::consume_data(). If you have a look into the implementation of consume_data() you will see that it's blatantly opening a file (exposing a runtime dependency to the consuming application, how ugly!) and return the contents of that data file.

Let's have a look at main/main.cc, which consumes the data and prints it's contents:

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
Now, if the data file wasn't found, we'd expect a runtime_error exception being thrown. Feel free to deactivate the data dependency and see what will happen.

C++ Library Header Configuration
Bazel has some parameters that control how consumers of a cc_library shall include the library headers. The parameters allow you to unlink the include path from the actual file system location for the headers. You can only use the settings once per cc_library.

The example we walk through here is available with:

$ git checkout 14d65ba
For system and external libraries, it makes sense to use the includes parameter, which adds the specified directory available to consumers using the -isystem compiler flag. Note that this flag suppresses all warnings from these headers, that includes templates declared in those headers.

For all other libraries (especially libraries developed in house), we specify the include paths using the following parameters:

<none>	If none of the parameters of this table is used the headers will be available relative to the BUILD file where the target is specified. If you look at the directory structure for pilib for example, the header would have to be consumed using
#include "include/pilib.h"
That is of course an ugly example, but it explains the result well.
strip_include_prefix	The specified string will be stripped from the required include statement. That means that if we use
strip_include_prefix = "include",
in pilib, the header would be available using
#include "pilib.h" 
include_prefix	
The specified string will be added to the required include statement. If we use
include_prefix = "pilib",
in addition to the strip_include_prefix in pilib, the header will be available using
#include "pilib/pilib.h".
If  we omit the strip_include_prefix option, the header will be available using
#include "pilib/include/pilib.h". 

