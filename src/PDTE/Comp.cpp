
#include "Comp.h"

MSSType Comp(MSSType S1, myType S2)
{
    MSSType res(make_pair(0, 0));
    MSS_funcComp(S1, S2, res, 1);
    return res;
}