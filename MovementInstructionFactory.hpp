#pragma once

#include "PrefixHeader.pch"

#include <vector>

#include "GameStateMachine.hpp"

class MovementInstructionFactory {
public:
    std::vector<MovementInstruction> generatePossibleInstructionsForBlockRange(GameStateMachine &gsm, const UInt64 startIndex, const UInt64 endIndex)
    {
        std::vector<MovementInstruction> instructions;
        
        for (UInt64 i = startIndex; i <= endIndex; ++i) {
            if (gsm.isInstructionPossible(MovementInstruction(i, MovementInstruction::Direction::Positive))) {
                instructions.push_back(MovementInstruction(i, MovementInstruction::Direction::Positive));
            }
            
            if (gsm.isInstructionPossible(MovementInstruction(i, MovementInstruction::Direction::Negative))) {
                instructions.push_back(MovementInstruction(i, MovementInstruction::Direction::Negative));
            }
        }
        
        return instructions;
    }
};
