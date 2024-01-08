
```
bazel run test --compilation_mode=dbg --run_under=lldb
```

Here is how to write github flavoured markdown:
https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax


`run`: runs the program. lldb will stop the program when assert fails or a segementation fault happens.

If we want the program to pause at a chosen point, we use breakpoint.how? 
`b maze.c:134`

