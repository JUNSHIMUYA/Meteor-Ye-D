
#include "SelChild.h"


myType SelCh(myType& R, myType& L , MSSType& B)
{
    myType Res;
    MSSType res(make_pair(0, 0));
    myType c(R - L);
    MSS_funcSMUTL(c, B, res);
   
    if(partyNum == PARTY_A) 
        Res = L + res.first - res.second; 
    if(partyNum == PARTY_B) 
        Res = 0  - res.second; 

    return Res;
}