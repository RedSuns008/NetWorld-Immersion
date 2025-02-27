int clamp(int x, int minX, int maxX)
{
    return max(min(maxX, x), minX);
}