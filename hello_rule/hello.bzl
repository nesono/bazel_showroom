_MAIN_TEMPLATE = "//hello_rule:main.cc"

_IMPL_TEMPLATE = "//hello_rule:lib.cc"
_HDR_TEMPLATE = "//hello_rule:lib.h"


def _main_impl(ctx):
    ctx.actions.expand_template(
        template = ctx.file._template,
        output = ctx.outputs.cpp_output,
        substitutions = {
            "{GREETEE}": ctx.attr.greetee,
        },
    )

hello_main_cpp = rule(
    implementation = _main_impl,
    attrs = {
        "greetee": attr.string(default="World"),
        "_template" : attr.label(
            default = Label(_MAIN_TEMPLATE),
            allow_single_file = True,
        ),
    },
    outputs = { "cpp_output": "main/%{greetee}.cpp" }, # use subdir main to avoid collisions
)

def hello_cc_binary(name, **kwargs):
    print("hello_cc_binary called from {}".format(native.package_name()))
    hello_main_cpp(
        name = "{}.main_template".format(name),
        greetee = name,
    )
    native.cc_binary(
        name = "{}".format(name),
        srcs = [
            ":{}.main_template".format(name),
        ],
        visibility = ["//visibility:public"],
    )



def _lib_impl(ctx):
    ctx.actions.expand_template(
        template = ctx.file._template_cpp,
        output = ctx.outputs.cpp_output,
        substitutions = {
            "{GREETEE}": ctx.attr.greetee,
        },
    )
    ctx.actions.expand_template(
        template = ctx.file._library_h,
        output = ctx.outputs.h_output,
        substitutions = {
            "{GREETEE}": ctx.attr.greetee,
        },
    )

hello_library_cpp = rule(
    implementation = _lib_impl,
    attrs = {
        "greetee": attr.string(default="World"),
        "_template_cpp" : attr.label(
            default = Label(_IMPL_TEMPLATE),
            allow_single_file = True,
        ),
        "_library_h" : attr.label(
            default = Label(_HDR_TEMPLATE),
            allow_single_file = True,
        ),
    },
    outputs = {
       "cpp_output": "%{greetee}.cpp",
       "h_output": "%{greetee}.h",
    },
)

def hello_cc_library(name, **kwargs):
    hello_library_cpp(
        name = "{}.library_template".format(name),
        greetee = name,
        visibility = ["//visibility:public"],
    )
    native.cc_library(
        name = name,
        srcs = [
            ":{}.library_template".format(name),
        ],
        visibility = ["//visibility:public"],
    )
