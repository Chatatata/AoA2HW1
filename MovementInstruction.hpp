#pragma once

#include "Instruction.hpp"
#include "Brick.hpp"

class MovementInstruction : Instruction {
public:
    enum class Direction {
        Positive,
        Negative
    };
    
#pragma mark - Constructors
    designated MovementInstruction(UInt64 brickId, Direction direction)
    : Instruction(), brickId(brickId), direction(direction)
    {
        //  Empty implementation
    }
    
    convenience MovementInstruction()
    : Instruction(), brickId(0), direction(Direction::Positive)
    {
        //  Empty implementation
    }
    
    UInt64 getBrickId() const noexcept
    {
        return brickId;
    }
    
    Direction getDirection() const noexcept
    {
        return direction;
    }
    
    friend std::ostream &operator << (std::ostream &outputStream, const MovementInstruction& mi) noexcept;
    
private:
    UInt64 brickId;
    Direction direction;
};

std::ostream &operator << (std::ostream &outputStream, const MovementInstruction& mi) noexcept
{
    outputStream << "Brick #";
    outputStream << mi.brickId;
    outputStream << ", ";
    outputStream << (mi.direction == MovementInstruction::Direction::Positive ? "Positive":"Negative");
    
    return outputStream;
}
