
#pragma once
#include "../tools.h"
#include "../connect.h"
#include "../globals.h"
using namespace std;

extern void start_time();
extern void start_communication();
extern void end_time(string str);
extern void end_communication(string str);


void funcReconstructBit(const ASSVectorSmallType &a, vector<smallType> &b, size_t size, string str, bool print);
void funcReconstruct(const ASSVectorMyType &a, vector<myType> &b, size_t size, string str, bool print);
void funcReconstruct(const ASSVectorSmallType &a, vector<smallType> &b, size_t size, string str, bool print);

void MSS_funcMatMul(const MSSVectorType &a, const ASSVectorMyType &b, MSSVectorType &c, 
					size_t rows, size_t common_dim, size_t columns,
				 	size_t transpose_a, size_t transpose_b, size_t truncation);
void MSS_funcMUTL(const MSSVectorType &a, const ASSVectorMyType &b, MSSVectorType &c,size_t size, bool truncation, size_t precision);
void MSS_funcSMUTL(const myType &a, const MSSType &b, MSSType &c);
void MSS_funcComp(const MSSType &a, const myType &b, MSSType &res , size_t size);


void aggregateCommunication();


//Test MSS

void testMSSMUTL(size_t size, size_t iter);
void testMSSMatMul(size_t rows, size_t common_dim, size_t columns, size_t iter);
void testComp(size_t size, size_t iter);
