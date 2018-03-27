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
