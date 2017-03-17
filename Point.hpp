#pragma once

#include "PrefixHeader.pch"

struct Point {
    UInt64 x;
    UInt64 y;
    
    Point(const UInt64 x, const UInt64 y)
    : x(x), y(y)
    {
        //  Empty implementation
    }
    
    Boolean operator != (const Point &other) const noexcept
    {
        return !operator == (other);
    }
    
    Boolean operator == (const Point &other) const noexcept
    {
        return x == other.x && y == other.y;
    }
};
