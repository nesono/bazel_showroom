cc_binary (
	name = "main",
	srcs = ["main.cc"],
	deps = ["//pilib:pi"],
	linkstatic = 1,
)

cc_test (
	name = "pi_acceptance",
	srcs = ["test.cc"],
	linkstatic = 1,
	deps = [
		"@googletest//:gtest",
		"//pilib:pi"],
	size = "small",
)
