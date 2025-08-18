#include "add_c.h"
#include "sub_c.h"
#include "public.h"


int main(void)
{
    int a = 1, b = 2;
    switch(judge(a,add(a,b)))
    {
        case GREATER:
            break;
        case LESS:
            break;
        case EQUAL:
            break;
        default:
            break;
    }
    return 0;
}