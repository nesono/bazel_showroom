# Bazel Custom Code Producing Rule

This directory contains custom rules that create code (i.e. c-binary or
c-library) that can be further consumed in bazel targets.

Note: The bazel query command provides a useful output format, which shows the
intermediate BUILD file after macro expansion. For example:

```bash
$ bazel query --output=build //car/perception/object_fusion/tools/replay/...
```

The query produces for example the following output.

```python
# /home/issingjo/Developer/bazel_showroom/hello_rule/BUILD:3:1
cc_binary(
  name = "world",
  visibility = ["//visibility:public"],
  generator_name = "world",
  generator_function = "hello_cc_binary",
  generator_location = "hello_rule/BUILD:3",
  srcs = ["//hello_rule:world.main_template"],
)

# /home/issingjo/Developer/bazel_showroom/hello_rule/BUILD:3:1
hello_main_cpp(
  name = "world.main_template",
  generator_name = "world",
  generator_function = "hello_cc_binary",
  generator_location = "hello_rule/BUILD:3",
  greetee = "world",
)

# /home/issingjo/Developer/bazel_showroom/hello_rule/BUILD:12:1
cc_binary(
  name = "hello_mathilda",
  deps = ["//hello_rule:mathilda"],
  srcs = ["//hello_rule:mathilda.cc"],
)

# /home/issingjo/Developer/bazel_showroom/hello_rule/BUILD:7:1
cc_library(
  name = "mathilda",
  visibility = ["//visibility:public"],
  generator_name = "mathilda",
  generator_function = "hello_cc_library",
  generator_location = "hello_rule/BUILD:7",
  srcs = ["//hello_rule:mathilda.library_template"],
)

# /home/issingjo/Developer/bazel_showroom/hello_rule/BUILD:7:1
hello_library_cpp(
  name = "mathilda.library_template",
  visibility = ["//visibility:public"],
  generator_name = "mathilda",
  generator_function = "hello_cc_library",
  generator_location = "hello_rule/BUILD:7",
  greetee = "mathilda",
)
```
