
#ifndef PRECOMPUTE_H
#define PRECOMPUTE_H

#pragma once
#include "globals.h"


class Precompute
{
private:
	void initialize();

public:
	Precompute();
	~Precompute();

	void getDividedShares(ASSVectorMyType &r, ASSVectorMyType &rPrime, int d, size_t size);
	void getZeroShares(ASSVectorMyType &z, size_t size);
	void getRandomBitShares(ASSVectorSmallType &a, size_t size);
	void getSelectorBitShares(ASSVectorSmallType &c, ASSVectorMyType &m_c, size_t size);
	void getShareConvertObjects(ASSVectorMyType &r, ASSVectorSmallType &shares_r, ASSVectorSmallType &alpha, size_t size);
	//void getTriplets(RSSVectorMyType &a, RSSVectorMyType &b, RSSVectorMyType &c, 
	//				size_t rows, size_t common_dim, size_t columns);
	//void getTriplets(RSSVectorMyType &a, RSSVectorMyType &b, RSSVectorMyType &c, size_t size);
	//void getTriplets(RSSVectorSmallType &a, RSSVectorSmallType &b, RSSVectorSmallType &c, size_t size);

};


#endif