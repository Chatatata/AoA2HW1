#pragma once

#include "PrefixHeader.pch"

#include <vector>
#include <queue>
#include <deque>

#include "MovementInstruction.hpp"

class InstructionNotAvailable : Exception {
    char const *what() const throw () override
    {
        return "State machine has no remaining instruction.";
    }
};

class InvalidInstruction : Exception {
    char const *what() const throw () override
    {
        return "Brick identifier given in instruction does not represent any object in current state.";
    }
};

class GameStateMachine {
public:
    static const UInt64 MapSize;
    static const Point ExitLocation;
    
    GameStateMachine(std::vector<Brick> initialState, std::vector<MovementInstruction> instructions = std::vector<MovementInstruction>())
    : currentState(initialState), awaiting(std::queue<MovementInstruction>()), done(), halted(false)
    {
        for (auto &eachInstruction : instructions) {
            awaiting.push(eachInstruction);
        }
    }
    
    void addInstruction(MovementInstruction instruction) noexcept
    {
        awaiting.push(instruction);
    }
    
    void runNextInstruction()
    {
        if (!awaiting.empty()) {
            auto &instruction = awaiting.front();
            
            runInstruction(instruction);
            
            done.push_back(instruction);
            awaiting.pop();
        } else {
            throw InstructionNotAvailable();
        }
    }
    
    void runRemainingInstructions()
    {
        try {
            while (true) {
                runNextInstruction();
            }
        } catch (InstructionNotAvailable &ina) {
            return;
        }
    }
    
    Boolean isDetachable() noexcept
    {
        auto &targetBrick = getTargetBrick();
        
        //  Draw current battle map
        Boolean battleMap[MapSize][MapSize];
        
        for (int a = 0; a < (int)MapSize; ++a) {
            for (int b = 0; b < (int)MapSize; ++b) {
                battleMap[b][a] = false;
            }
        }
        
        for (auto &eachBrick : currentState) {
            for (UInt64 i = 0; i < eachBrick.size; ++i) {
                if (eachBrick.direction == Brick::Direction::Horizontal) {
                    battleMap[eachBrick.position.y][eachBrick.position.x + i] = true;
                } else {
                    battleMap[eachBrick.position.y + i][eachBrick.position.x] = true;
                }
            }
        }
        
        //  Find out if action is doable
        for (UInt64 i = (targetBrick.position.x + targetBrick.size); i < MapSize; ++i) {
            if (battleMap[targetBrick.position.y][i]) {
                return false;
            }
        }
        
        return true;
    }
    
    Boolean isInstructionPossible(const MovementInstruction &instruction)
    {
        auto &brick = getBrickWithIdentifier(instruction.getBrickId());
        
        //  Check boundaries
        if (instruction.getDirection() == MovementInstruction::Direction::Positive) {
            if ((brick.direction == Brick::Direction::Horizontal && brick.position.x >= MapSize - 2) ||
                (brick.direction == Brick::Direction::Vertical && brick.position.y >= MapSize - 2)) {
                return false;
            }
        } else {
            if ((brick.direction == Brick::Direction::Horizontal && brick.position.x <= 1) ||
                (brick.direction == Brick::Direction::Vertical && brick.position.y <= 1)) {
                return false;
            }
        }
        
        //  Draw current battle map
        Boolean battleMap[MapSize][MapSize];
        
        for (int a = 0; a < (int)MapSize; ++a) {
            for (int b = 0; b < (int)MapSize; ++b) {
                battleMap[b][a] = false;
            }
        }
        
        for (auto &eachBrick : currentState) {
            for (UInt64 i = 0; i < eachBrick.size; ++i) {
                if (eachBrick.direction == Brick::Direction::Horizontal) {
                    battleMap[eachBrick.position.y][eachBrick.position.x + i] = true;
                } else {
                    battleMap[eachBrick.position.y + i][eachBrick.position.x] = true;
                }
            }
        }
        
        //  Find out if action is doable
        if (instruction.getDirection() == MovementInstruction::Direction::Positive) {
            if (brick.direction == Brick::Direction::Horizontal) {
                return !battleMap[brick.position.y][brick.position.x + brick.size];
            } else {
                return !battleMap[brick.position.y + brick.size][brick.position.x];
            }
        } else {
            if (brick.direction == Brick::Direction::Horizontal) {
                return !battleMap[brick.position.y][brick.position.x - 1];
            } else {
                return !battleMap[brick.position.y - 1][brick.position.x];
            }
        }
    }
    
    Boolean hasEquivalency(const GameStateMachine &other) const noexcept
    {
        if (currentState.size() != other.currentState.size()) {
            return false;
        }
        
        for (UInt64 i = 0; i < currentState.size(); ++i) {
            if (currentState[i] != other.currentState[i]) {
                return false;
            }
        }
        
        return true;
    }
    
    MovementInstruction getNextInstruction() const noexcept
    {
        return awaiting.front();
    }
    
    Boolean isHalted() const noexcept
    {
        return halted;
    }
    
    std::ostream &battlemap(std::ostream &outputStream = std::cout)
    {
        UInt64 battleMap[MapSize][MapSize];
        
        for (int a = 0; a < (int)MapSize; ++a) {
            for (int b = 0; b < (int)MapSize; ++b) {
                battleMap[b][a] = 0;
            }
        }
        
        for (auto &eachBrick : currentState) {
            for (UInt64 i = 0; i < eachBrick.size; ++i) {
                if (eachBrick.direction == Brick::Direction::Horizontal) {
                    battleMap[eachBrick.position.y][eachBrick.position.x + i] = eachBrick.id;
                } else {
                    battleMap[eachBrick.position.y + i][eachBrick.position.x] = eachBrick.id;
                }
            }
        }
        
        for (int a = 0; a < (int)MapSize + 1; ++a) {
            if (a == 0 || a == (int)(MapSize)) {
                for (int b = 0; b < (int)MapSize; ++b) {
                    outputStream << "==";
                }
            } else {
                for (int b = 0; b < (int)MapSize; ++b) {
                    if (battleMap[a][b]) {
                        outputStream << battleMap[a][b];
                    } else {
                        outputStream << " ";
                    }
                    
                    outputStream << " ";
                }
            }
            
            outputStream << std::endl;
        }
        
        outputStream << std::endl << std::endl;
        
        return outputStream;
    }
    
    const std::vector<MovementInstruction> &getDone() const noexcept
    {
        return done;
    }
    
    const std::queue<MovementInstruction> &getAwaiting() const noexcept
    {
        return awaiting;
    }
    
    const std::vector<Brick> &getCurrentState() const noexcept
    {
        return currentState;
    }
    
protected:
    std::vector<Brick> currentState;
    std::queue<MovementInstruction> awaiting;
    std::vector<MovementInstruction> done;
    Boolean halted = false;
    
    void runInstruction(const MovementInstruction &instruction)
    {
        if (!halted) {
            if (isInstructionPossible(instruction)) {
                performInstruction(instruction);
            } else {
                halted = true;
                
                throw InvalidInstruction();
            }
        } else {
            throw InvalidInstruction();
        }
    }
    
    Brick &getBrickWithIdentifier(const UInt64 id)
    {
        for (auto &eachBrick : currentState) {
            if (eachBrick.id == id) {
                return eachBrick;
            }
        }
        
        throw std::runtime_error("brick not found");
    }
    
    Brick &getTargetBrick()
    {
        for (auto &eachBrick : currentState) {
            if (eachBrick.isTarget) {
                return eachBrick;
            }
        }
        
        throw std::runtime_error("brick not found");
    }
    
    void performInstruction(const MovementInstruction &instruction)
    {
        auto &brick = getBrickWithIdentifier(instruction.getBrickId());
        
        if (instruction.getDirection() == MovementInstruction::Direction::Positive) {
            if (brick.direction == Brick::Direction::Horizontal) {
                brick.position.x += 1;
            } else {
                brick.position.y += 1;
            }
        } else {
            if (brick.direction == Brick::Direction::Horizontal) {
                brick.position.x -= 1;
            } else {
                brick.position.y -= 1;
            }
        }
    }
};





































