#pragma once

#include "../Common/Includes.h"

class stringVector : public std::vector<std::string>
{
public:
    stringVector();
    ~stringVector();

    size_t find(std::string item, size_t startPos = 0);
};
