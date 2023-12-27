#include "Utility.h"

#ifndef cppCompat_h
int stringToInt(String s)
{
    return s.toInt();
}
#endif

int utility::dx(int dir)
{
    switch (dir)
    {
        case 0:
            return -1;
        case 2:
            return 1;
        default:
            return 0;
    }
}

int utility::dy(int dir)
{
    switch (dir)
    {
    case 1:
        return 1;
    case 3:
        return -1;
    default:
        return 0;
    }
}
