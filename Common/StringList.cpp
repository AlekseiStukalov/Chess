#include "stdafx.h"
#include "StringList.h"


stringVector::stringVector()
{
}

stringVector::~stringVector()
{
}

size_t stringVector::find_no_case(std::string item, size_t startPos)
{
    size_t pos = -1;
    for (size_t i = startPos; i < this->size(); i++)
    {
        if (this->at(i) == item)
        {
            pos = i;
            break;
        }
    }
    return pos;
}
