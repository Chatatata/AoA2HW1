#include <iostream>
#include <fstream>
#include <cassert>
#include <ratio>
#include <thread>
#include <chrono>
#include <iomanip>
#include <ctime>

#include "FileParser.hpp"
#include "GameStateMachineManager.hpp"
#include "MovementInstructionFactory.hpp"


#pragma mark - Static values instantiation
      UInt64 Brick::IdentifierSequence = 1;
const UInt64 GameStateMachine::MapSize = 8;
const Point GameStateMachine::ExitLocation = Point(3,6);

inline void writeResult(const std::string fileName,
                        GameStateMachine &machine,
                        const UInt64 duration) noexcept {
    std::ofstream outputFileStream(fileName);
    
    if (!outputFileStream.is_open()) {
        throw std::runtime_error("Output file could not be opened");
    }
    
    outputFileStream << "OUTLINE" << std::endl;
    
    outputFileStream << "Overall running time of algorithm: " << duration << " ns" << std::endl;
    outputFileStream << "Benchmarking interface: std::chrono::high_resolution_clock" << std::endl;
    
    //  Write the result to the file
    auto completedInstructions = machine.getDone();
    
    outputFileStream << "Number of instructions executed by state machine: " << completedInstructions.size() << std::endl;
    outputFileStream << "Halting state: " << (machine.isHalted() ? "halted":"not halted") << std::endl;
    outputFileStream << "Detachablity: " << (machine.isDetachable() ? "detachable":"non-detachable") << std::endl << std::endl;
    outputFileStream << "Last state of finite state machine: " << std::endl;
    
    machine.battlemap(outputFileStream);
    
    outputFileStream << "State machine instruction traceback:" << std::endl;
    for (UInt64 i = 0; i < completedInstructions.size(); ++i) {
        outputFileStream << "\t$" << i << "\t" << completedInstructions[i] << std::endl;
    }
}

auto main(Int32 argc, const Char8 * argv[]) -> Int32 {
    if (argc != 4) {
        std::cerr << "usage: puzzle [bfs | dfs] input_file output_file" << std::endl;
    
        return 9;
    }
    
    //  Instantiate file parser with file name
    FileParser parser(argv[2]);
    
    auto bricks = parser.parse();
    
    GameStateMachineManager manager(bricks);
    
    if (std::string(argv[1]) == "bfs") {
        auto start = std::chrono::high_resolution_clock::now();
        auto r = manager.performBFSIndexing();
        auto end = std::chrono::high_resolution_clock::now();
        
        auto result = static_cast<UInt64>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
        
        std::cout << result;
        
        writeResult(argv[3], r, result);
    } else {
        auto start = std::chrono::high_resolution_clock::now();
        auto r = manager.performDFSIndexing();
        auto end = std::chrono::high_resolution_clock::now();
        
        auto result = static_cast<UInt64>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
        
        std::cout << result;
        
        writeResult(argv[3], r, result);
    }
    
    return 0;
}
