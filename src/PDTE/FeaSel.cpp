#include "FeaSel.h"

MSSVectorType FeaSel(MSSVectorType& S1, ASSVectorMyType& S2, size_t rows, size_t common_dim, size_t columns)
{
    size_t final_size = rows*columns;
	ASSVectorMyType temp3(final_size);
	vector<myType> Delta(final_size, 0);
    MSSVectorType Res(rows*columns);

	matrixMultMSS(S1, S2, temp3, rows, common_dim, columns, 0, 0);
	
	funcReconstruct(temp3, Delta, final_size, "Delta Reconst", false);

	if (partyNum == PARTY_A)
	{
		for (int i = 0; i < final_size; ++i)
		{
			Res[i] = make_pair(Delta[i], 0);
		}
	}
	
	if (partyNum == PARTY_B)
	{
		for (int i = 0; i < final_size; ++i)
		{
			Res[i] = make_pair(Delta[i], 0);
		}
	}
    return Res;
}