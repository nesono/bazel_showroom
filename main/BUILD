load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_test")

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    linkstatic = 1,
    deps = [
        "//datalib",
        "//pilib:pi",
        "//platformlib",
    ],
)

cc_test(
    name = "test",
    size = "small",
    srcs = ["test.cc"],
    copts = select({
        "//common:linux": ["-pthread"],
        "//conditions:default": None,
    }),
    linkopts = select({
        "//common:linux": ["-lpthread"],
        "//conditions:default": None,
    }),
    linkstatic = 1,
    deps = ["@googletest//:gtest_main"],
)

cc_test(
    name = "pi_acceptance",
    size = "small",
    srcs = ["pi_at.cc"],
    copts = select({
        "//common:linux": ["-pthread"],
        "//conditions:default": None,
    }),
    linkopts = select({
        "//common:linux": ["-lpthread"],
        "//conditions:default": None,
    }),
    linkstatic = 1,
    deps = [
        "//pilib:pi",
        "@googletest//:gtest_main",
    ],
)
