#pragma once
// ^ only include this file once when compiling the program. Otherwise we might end up with multiple declarations with the same name, which the compiler does not know what to do with.

int max(int a, int b);
int min(int a, int b);

// Returns min if value < min, max if value > max, and otherwise the value itself.
int clamp(int value, int min_, int max_);
