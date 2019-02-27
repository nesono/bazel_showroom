load("//rule_sum:sum.bzl", "sum")

sum(
    name = "target_1",
)

sum(
    name = "another_target_1",
)

sum(
    name = "target_2",
    deps = [":target_1"],
)

sum(
    name = "target_3",
    deps = [":target_2"],
)

sum(
    name = "target_5",
    deps = [
        ":another_target_1",
        ":target_3",
    ],
)
