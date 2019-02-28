load('//hello_rule:hello.bzl', 'hello_cc_binary', 'hello_cc_library')

hello_cc_binary(
    name = "world",
)

hello_cc_library(
    name = "mathilda",
    greetee = "Mathilda",
)

cc_binary(
    name = "hello_mathilda",
    srcs = [
        "mathilda.cc",
    ],
    deps = [
        ":mathilda",
    ]
)
