NumberProvider = provider(fields=["number"])

def _impl(ctx):
    accu = ctx.attr.number
    for dep in ctx.attr.deps:
        accu += dep[NumberProvider].number
    print("Label {} in package {} has sum: {}".format(ctx.label.name, ctx.label.package, accu))
    return [NumberProvider(number=accu)]

sum = rule(
    implementation = _impl,
    attrs = {
        "number": attr.int(default = 1),
        "deps": attr.label_list(), # optional provider
        # "deps": attr.label_list(providers = [NumberProvider])), # mandatory provider
    },
)

