cc_library(
    name = "pi",
    srcs = glob(
        [ "src/*.cc",
          "src/*.h" ],
    ),
    hdrs = glob(
        ["include/*.h"],
    ),
    copts = ["-Ipilib/include",],
    strip_include_prefix = "include",
    include_prefix = 'pilib',
    visibility = ["//main:__pkg__"],
    deps = [
        "//tools/buildstamp:build_info_last_change",
    ],
)

