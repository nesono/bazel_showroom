cc_test(
    name = "datatest",
    srcs = ["datatest.cc"],
    deps = [
        "@googletest//:gtest",
        ":datalib",
    ],
    size = "small",
    copts = select({
        "//common:linux": ["-pthread"],
        "//conditions:default": None,
    }),
    linkopts = select({
        "//common:linux": ["-lpthread"],
        "//conditions:default": None,
    }),
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
