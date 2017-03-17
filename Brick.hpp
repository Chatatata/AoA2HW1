#pragma once

#include "PrefixHeader.pch"

#include "Point.hpp"

class InvalidBrickSizeException : Exception {
    
};

struct Brick {
    enum class Direction {
        Horizontal,
        Vertical
    };
    
    static UInt64 IdentifierSequence;
    
    UInt64 id;
    Point position;
    UInt64 size;
    Direction direction;
    Boolean isTarget;
    
    designated Brick(const Point position, const UInt64 size, const Direction direction, const Boolean isTarget = false)
    : position(position), size(size), direction(direction), isTarget(isTarget), id(IdentifierSequence++)
    {
        if (size <= 1) {
            throw InvalidBrickSizeException();
        }
    }
    
    Boolean operator != (const Brick &other) const noexcept
    {
        return !operator == (other);
    }
    
    Boolean operator == (const Brick &other) const noexcept
    {
        return
            id == other.id &&
            position == other.position &&
            size == other.size &&
            direction == other.direction &&
            isTarget == other.isTarget;
    }
};
