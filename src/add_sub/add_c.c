#include "add_c.h"
#include "public.h"

int add(int p1, int p2)
{
    return p1 + p2;
}

int judge(int p1, int p2)
{
    if (p1 < p2)
    {
        return LESS;
    }
    else if (p1 > p2)
    {
        return GREATER;
    }
    else
    {
        return EQUAL;
    } 
}