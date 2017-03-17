#pragma once

#include "GameStateMachine.hpp"
#include "MovementInstructionFactory.hpp"

class ExpansionNotAvailable : Exception {
    char const *what() const throw () override
    {
        return "Expansion not available.";
    }
};

class GameStateMachineManager {
public:
    std::vector<Brick> initialState;
    MovementInstructionFactory *factory = nullptr;
    std::vector<GameStateMachine> machineHistory;
    
    GameStateMachineManager(std::vector<Brick> initialState)
    : initialState(initialState), factory(new MovementInstructionFactory()), machineHistory(std::vector<GameStateMachine>())
    {
        //  Empty implementation
    }
    
    virtual ~GameStateMachineManager() final
    {
        delete factory;
    }
    
    inline GameStateMachine performBFSIndexing()
    {
        GameStateMachine initialMachine(initialState);
        
        return performBFSIndexing(initialMachine);
    }
    
    inline GameStateMachine performDFSIndexing()
    {
        GameStateMachine initialMachine(initialState);
        
        return performDFSIndexing(initialMachine);
    }
    
protected:
    inline GameStateMachine performDFSIndexing(GameStateMachine &currentState)
    {
        auto possibleInstructions =
        factory->generatePossibleInstructionsForBlockRange(currentState,
                                                           1,
                                                           initialState.size());
        
        for (auto &eachInstruction : possibleInstructions) {
            GameStateMachine middleMachine(currentState);
            
            middleMachine.addInstruction(eachInstruction);
            
            middleMachine.runRemainingInstructions();
            
            if (didPassStateMachine(middleMachine)) {
                continue;
            } else if (middleMachine.isDetachable()) {
                return middleMachine;
            } else {
                machineHistory.push_back(middleMachine);
                
                try {
                    return performDFSIndexing(middleMachine);
                } catch (ExpansionNotAvailable &ean) {
                    continue;
                }
            }
        }
        
        throw ExpansionNotAvailable();
    }
    
    inline GameStateMachine performBFSIndexing(GameStateMachine &currentState)
    {
        auto possibleInstructions =
        factory->generatePossibleInstructionsForBlockRange(currentState,
                                                           1,
                                                           initialState.size());
        
        std::vector<GameStateMachine> machineStore;
        
        for (auto &eachInstruction : possibleInstructions) {
            GameStateMachine middleMachine(currentState);
            
            middleMachine.addInstruction(eachInstruction);
            
            middleMachine.runRemainingInstructions();
            
            if (didPassStateMachine(middleMachine)) {
                continue;
            } else if (middleMachine.isDetachable()) {
                return middleMachine;
            } else {
                machineHistory.push_back(middleMachine);
                machineStore.push_back(middleMachine);
            }
        }
        
        for (auto &eachMachine : machineStore) {
            try {
                return performBFSIndexing(eachMachine);
            } catch (ExpansionNotAvailable &ena) {
                continue;
            }
        }
        
        throw ExpansionNotAvailable();
    }

private:
    inline Boolean didPassStateMachine(GameStateMachine &state)
    {
        for (auto &eachMachine : machineHistory) {
            if (eachMachine.hasEquivalency(state)) {
                return true;
            }
        }
        
        return false;
    }
};
















