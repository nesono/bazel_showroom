cc_binary (
    name = "main",
    srcs = ["main.cc"],
    deps = [
        "//pilib:pi",
        "//platformlib",
    ],
    linkstatic = 1,
)

cc_test (
    name = "test",
    srcs = ["test.cc"],
    linkstatic = 1,
    deps = ["@googletest//:gtest"],
    size = "small",
)

cc_test (
    name = "pi_acceptance",
    srcs = ["pi_at.cc"],
    linkstatic = 1,
    deps = [
        "@googletest//:gtest",
        "//pilib:pi"],
    size = "small",
)
