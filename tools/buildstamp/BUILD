load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

exports_files([
    "generate_version_built_cc.sh",
])

# genrule for volatile data - it will invalidate actions depending on it
genrule(
    name = "generate_stable_version_built_cc",
    outs = ["build_info_stable.h"],
    cmd = "$(location :generate_version_built_cc.sh) bazel-out/stable-status.txt >> $@",
    stamp = 1,
    tools = [":generate_version_built_cc.sh"],
    visibility = ["//visibility:private"],
)

# build info for when a component was BUILT
# get the definitions by running `tools/buildstamp/generate_version_built_cc.sh bazel-out/stable-status.txt`
cc_library(
    name = "build_info_built",
    srcs = [
        "build_info_built.cc",
        "build_info_stable.h",
    ],
    hdrs = ["build_info_built.h"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "build_info_built_test",
    size = "small",
    srcs = ["build_info_built_test.cc"],
    stamp = 1,
    deps = [
        ":build_info_built",
        "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "print_build_info_built",
    srcs = ["print_build_info_built.cc"],
    stamp = 1,
    deps = [
        ":build_info_built",
    ],
)

cc_library(
    name = "build_info_last_change",
    hdrs = ["build_info_last_change.h"],
    linkstamp = "build_info_last_change.cc",
    visibility = ["//visibility:public"],
    alwayslink = 1,
)

cc_test(
    name = "build_info_last_change_test",
    size = "small",
    srcs = ["build_info_last_change_test.cc"],
    stamp = 1,
    deps = [
        ":build_info_last_change",
        "@googletest//:gtest_main",
    ],
)

cc_binary(
    name = "print_build_info_last_change",
    srcs = ["print_build_info_last_change.cc"],
    stamp = 1,
    deps = [
        ":build_info_last_change",
    ],
)
