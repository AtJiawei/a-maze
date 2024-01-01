
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

cc_binary(
    name = "maze",
    srcs = ["maze.c", "lib.c", "lib.h"],
    linkopts = select({
        "@platforms//os:osx": ["-lncurses"],
        "//conditions:default": ["-lncursesw"],
    }),
    defines = select({
        "@platforms//os:linux": ["_XOPEN_SOURCE"],
        "//conditions:default": [],
    }),
)
