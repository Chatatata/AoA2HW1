#pragma once

#include "PrefixHeader.pch"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Brick.hpp"

class FileParser {
public:
    const String fileName;
    
    FileParser(const String fileName)
    : fileName(fileName)
    {
        //  Empty implementation
    }
    
    std::vector<Brick> parse() const noexcept
    {
        std::ifstream inputFileStream(fileName);
        String line;
        std::vector<Brick> bricks;
        
        if (inputFileStream.is_open()) {
            while (std::getline(inputFileStream, line)) {
                std::istringstream iss(line);
                
                UInt64 xParam;
                UInt64 yParam;
                UInt64 size;
                Char8 directionLetter;
                
                iss >> yParam >> xParam >> size >> directionLetter;
                
                if (directionLetter == 'h') {
                    Point position(xParam, yParam);
                    
                    Brick brick(position,
                                size,
                                directionLetter == 'h' ? Brick::Direction::Horizontal : Brick::Direction::Vertical,
                                bricks.size() == 0);
                    
                    bricks.push_back(brick);
                } else {
                    Point position(xParam, yParam - (size - 1));
                    
                    Brick brick(position,
                                size,
                                directionLetter == 'h' ? Brick::Direction::Horizontal : Brick::Direction::Vertical,
                                bricks.size() == 0);
                    
                    bricks.push_back(brick);
                }
            }
            
            return bricks;
        } else {
            throw std::runtime_error("File could not be opened");
        }
    }
};
