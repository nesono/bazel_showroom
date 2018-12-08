cc_test(
    name = "build_info_test",
    srcs = ["build_info_test.cc"],
    deps = [
        ":build_info",
        "@googletest//:gtest",
    ],
    stamp = True,
    size = "small",
)

cc_library(
    name = "build_info",
    linkstamp = "build_info.cc",
    hdrs = ["build_info.h"],
    alwayslink = 1,
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "print_build_info",
    srcs = ["print_build_info.cc"],
    stamp = True,
    deps = [
        ":build_info",
    ],
)

