int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

// Returns min if value < min, max if value > max, and otherwise the value itself.
int clamp(int value, int min_, int max_)
{
    return min(max(value, min_), max_);
}
