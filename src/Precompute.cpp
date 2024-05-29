
#pragma once
#include "Precompute.h"

Precompute::Precompute(){initialize();}
Precompute::~Precompute(){}
void Precompute::initialize(){}

// Currently, r = 3 and rPrime = 3 * 2^d
// TODO: ReLU produces a bug with this. Why? funcRELU does not even call getDividedShares()
void Precompute::getDividedShares(ASSVectorMyType &r, ASSVectorMyType &rPrime, int d, size_t size)
{
	assert(r.size() == size && "r.size is incorrect");
	assert(rPrime.size() == size && "rPrime.size is incorrect");

	for (int i = 0; i < size; ++i)
	{
		r[i] = 0;
		
		rPrime[i] = 0;
		
		// r[i].first = 1;
		// r[i].second = 1;
		// rPrime[i].first = d;
		// rPrime[i].second = d;		
	}
}

void Precompute::getZeroShares(ASSVectorMyType &z, size_t size)
{
	assert(z.size() == size && "r.size is incorrect");

	for (int i = 0; i < size; ++i)
	{
		z[i] = 0;
		
		// r[i].first = 1;
		// r[i].second = 1;
		// rPrime[i].first = d;
		// rPrime[i].second = d;		
	}
}

void Precompute::getRandomBitShares(ASSVectorSmallType &a, size_t size)
{
	assert(a.size() == size && "size mismatch for getRandomBitShares");
	for(auto &it : a)
		it = 0;
}


//m_0 is random shares of 0, m_1 is random shares of 1 in RSSMyType. 
//This function generates random bits c and corresponding RSSMyType values m_c
void Precompute::getSelectorBitShares(ASSVectorSmallType &c, ASSVectorMyType &m_c, size_t size)
{
	assert(c.size() == size && "size mismatch for getSelectorBitShares");
	assert(m_c.size() == size && "size mismatch for getSelectorBitShares");
	for(auto &it : c)
		it = 0;

	for(auto &it : m_c)
		it = 0;
}

//Shares of random r, shares of bits of that, and shares of wrap3 of that.
void Precompute::getShareConvertObjects(ASSVectorMyType &r, ASSVectorSmallType &shares_r, 
										ASSVectorSmallType &alpha, size_t size)
{
	assert(shares_r.size() == size*BIT_SIZE && "getShareConvertObjects size mismatch");
	for(auto &it : r)
		it = 0;

	for(auto &it : shares_r)
		it = 0;

	for(auto &it : alpha)
		it = 0;
}
