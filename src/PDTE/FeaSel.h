

#ifndef FEASEL_H
#define FEASEL_H

#include "../globals.h"
#include <vector>
#include "../tools.h"
#include "MSSFunctionalities.h"
    
MSSVectorType FeaSel(MSSVectorType& S1, ASSVectorMyType& S2, size_t rows, size_t common_dim, size_t columns);

#endif