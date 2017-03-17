#pragma once

#include "PrefixHeader.pch"

class Instruction {
    friend class GameStateMachine;
    
public:
    Instruction()
    {
        //  Empty implementation
    }
    
    virtual ~Instruction()
    {
        //  Empty implementation
    }
};
