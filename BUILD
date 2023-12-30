
cc_binary(
    name = "ncurses",
    srcs = ["ncurses.c"],
    linkopts = select({
        "@platforms//os:osx": ["-lncurses"],
        "//conditions:default": ["-lncursesw"],
    }),
    defines = select({
        "@platforms//os:linux": ["_XOPEN_SOURCE"],
        "//conditions:default": [],
    }),
)
