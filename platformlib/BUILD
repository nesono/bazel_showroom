cc_library(
    name = "platform",
    srcs = select({
        "//common:osx": ["platform_macos.cc"],
        "//common:linux": ["platform_linux.cc"],
        "//conditions:default": ["platform_unknown.cc"],
        }),
    hdrs = ["platform.h"],
    includes = ["."],
    visibility = ["//visibility:public"],
)

