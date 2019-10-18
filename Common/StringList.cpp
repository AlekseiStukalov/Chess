#include "stdafx.h"
#include "StringList.h"
#include <algorithm>

stringVector::stringVector()
{
}

stringVector::~stringVector()
{
}

size_t stringVector::find_no_case(std::string item, size_t startPos)
{
    std::transform(item.begin(), item.end(), item.begin(), ::toupper);
    size_t pos = -1;
    for (size_t i = startPos; i < this->size(); i++)
    {
        std::string buffer(this->at(i));
        std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::toupper);
        if (buffer == item)
        {
            pos = i;
            break;
        }
    }
    return pos;
}
