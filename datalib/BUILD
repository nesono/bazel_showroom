load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_test(
    name = "datatest",
    size = "small",
    srcs = ["datatest.cc"],
    copts = select({
        "//common:linux": ["-pthread"],
        "//conditions:default": None,
    }),
    linkopts = select({
        "//common:linux": ["-lpthread"],
        "//conditions:default": None,
    }),
    deps = [
        ":datalib",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "datalib",
    srcs = ["datalib.cc"],
    hdrs = ["datalib.h"],
    data = [":tagfile"],
    visibility = ["//main:__pkg__"],
)

filegroup(
    name = "tagfile",
    srcs = ["tag"],
)
