
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

cc_library(
    name = "lib",
    srcs = ["lib.c"],
    hdrs = ["lib.h"],
)

cc_binary(
    name = "maze",
    srcs = ["maze.c"],
    deps = ["lib"],
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
    name = "test",
    deps = ["lib"],
    srcs = ["test.c"],
)
