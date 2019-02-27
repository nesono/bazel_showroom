cc_binary (
    name = "main",
    srcs = ["main.cc"],
    deps = [
        "//pilib:pi",
        "//platformlib",
        "//datalib",
        "//tools/buildstamp:build_info",
    ],
    linkstatic = 1,
)

cc_test (
    name = "test",
    srcs = ["test.cc"],
    linkstatic = 1,
    deps = ["@googletest//:gtest_main"],
    size = "small",
    copts = select({
        "//common:linux": ["-pthread"],
        "//conditions:default": None,
    }),
    linkopts = select({
        "//common:linux": ["-lpthread"],
        "//conditions:default": None,
    })
)

cc_test (
    name = "pi_acceptance",
    srcs = ["pi_at.cc"],
    linkstatic = 1,
    deps = [
        "@googletest//:gtest_main",
        "//pilib:pi"],
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
