#include "Utility.h"


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
