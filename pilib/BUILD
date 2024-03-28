load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "pi",
    srcs = glob(
        [
            "src/*.cc",
            "src/*.h",
        ],
    ),
    hdrs = glob(
        ["include/*.h"],
    ),
    copts = ["-Ipilib/include"],
    include_prefix = "pilib",
    strip_include_prefix = "include",
    visibility = ["//main:__pkg__"],
    deps = [
        "//tools/buildstamp:build_info_last_change",
    ],
)
