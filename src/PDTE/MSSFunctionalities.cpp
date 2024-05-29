
#pragma once
#include "MSSFunctionalities.h"
#include "../Precompute.h"
#include <thread>


using namespace std;
extern Precompute PrecomputeObject;
extern string SECURITY_TYPE;

/******************************** Functionalities 2PC ********************************/
 // Share Truncation, truncate shares of a by power (in place) (power is logarithmic)

void funcReconstructBit(const ASSVectorSmallType &a, vector<smallType> &b, size_t size, string str, bool print)
{
	log_print("Reconst: ASSSmallType (bits), smallType (bit)");


		vector<smallType> a_next(size), a_prev(size);
		for (int i = 0; i < size; ++i)
		{
			a_prev[i] = 0;
			a_next[i] = a[i];
			b[i] = a[i];
			
		}

		thread *threads = new thread[2];
		threads[0] = thread(sendVector<smallType>, ref(a_next), nextParty(partyNum), size);
		threads[1] = thread(receiveVector<smallType>, ref(a_prev), nextParty(partyNum), size);
		for (int i = 0; i < 2; i++)
			threads[i].join();
		delete[] threads;

		for (int i = 0; i < size; ++i)
			b[i] = b[i] ^ a_prev[i];

		if (print)
		{
			std::cout << str << ": \t\t";
			for (int i = 0; i < size; ++i)
				cout << (int)(b[i]) << " "; 
			std::cout << std::endl;
		}
	
}

void funcReconstruct(const ASSVectorSmallType &a, vector<smallType> &b, size_t size, string str, bool print)
{
	log_print("Reconst: ASSSmallType, smallType");


		vector<smallType> a_next(size), a_prev(size);
		for (int i = 0; i < size; ++i)
		{
			a_prev[i] = 0;
			a_next[i] = a[i];
			b[i] = a[i];
			//b[i] = additionModPrime[b[i]][a[i].second];
		}

		thread *threads = new thread[2];

		threads[0] = thread(sendVector<smallType>, ref(a_next), nextParty(partyNum), size);
		threads[1] = thread(receiveVector<smallType>, ref(a_prev), nextParty(partyNum), size);

		for (int i = 0; i < 2; i++)
			threads[i].join();

		delete[] threads;

		for (int i = 0; i < size; ++i)
			b[i] = additionModPrime[b[i]][a_prev[i]];

		if (print)
		{
			std::cout << str << ": \t\t";
			for (int i = 0; i < size; ++i)
				cout << (int)(b[i]) << " "; 
			std::cout << std::endl;
		}
	
}

void funcReconstruct(const ASSVectorMyType &a, vector<myType> &b, size_t size, string str, bool print)
{
	log_print("Reconst: ASSMyType, myType");
	assert(a.size() == size && "a.size mismatch for reconstruct function");

	vector<myType> a_next(size), a_prev(size, 0);
	for (int i = 0; i < size; ++i)
	{
		a_next[i] = a[i];
		b[i] = a[i];
	}

		thread *threads = new thread[2];

		threads[0] = thread(sendVector<myType>, ref(a_next), nextParty(partyNum), size);
		threads[1] = thread(receiveVector<myType>, ref(a_prev), nextParty(partyNum), size);

		for (int i = 0; i < 2; i++)
			threads[i].join();

		delete[] threads;

		for (int i = 0; i < size; ++i)
			b[i] = b[i] + a_prev[i];

		if (print)
		{
			std::cout << str << ": \t\t";
			for (int i = 0; i < size; ++i)
				print_linear(b[i], "SIGNED");
			std::cout << std::endl;
		}
	
}


/******************************** Functionalities MPC ********************************/

void MSS_funcMUTL(const MSSVectorType &a, const ASSVectorMyType &b, MSSVectorType &c,size_t size, bool truncation, size_t precision)
{
	
	log_print("MSS Function for MUTL");
	assert(a.size() == size && "a size is incorrect");
	assert(b.size() == size && "b size is incorrect");
	assert(c.size() == size && "c size is incorrect");

	vector<myType> Delta(size, 0), temp4(size, 0);

	ASSVectorMyType temp3(size);
	
	for (int i = 0; i < size; i++){
		if(partyNum == PARTY_A){ 
			temp3[i] = a[i].first * b[i] + 0 - 0;
			//cout<<temp3[i]<<endl;
		}
		else if(partyNum == PARTY_B){
			temp3[i] = a[i].first * 0 + 0 - 0;
			//cout<<temp3[i]<<endl;
		}
		temp4[i] = temp3[i];
	}
	//funcReconstruct(temp3, Delta, size, "Delta Reconst", false);

	
	thread *threads = new thread[2];

	threads[0] = thread(sendVector<myType>, ref(temp4), nextParty(partyNum), size);
	
	threads[1] = thread(receiveVector<myType>, ref(Delta), nextParty(partyNum), size);
	

	for (int i = 0; i < 2; i++)
		threads[i].join();
	delete[] threads; 

	for(int i = 0; i < size; i++){
		Delta[i] = Delta[i] + temp3[i];
	}
	
	if(truncation)
		dividePlain(Delta, (1<<truncation));

	
	for (int i = 0; i < size; ++i)
	{
		
		c[i] = make_pair(Delta[i], 0);
	}
}

void MSS_funcSMUTL(const myType &a, const MSSType &b, MSSType &c)
{

	vector<myType> Delta(1, 0), temp4(1, 0);
	myType temp3;
	
	if(partyNum == PARTY_A){
		temp3 = a * b.first + 0 - 0;
		//cout<<temp3[i]<<endl;
	}
	else if(partyNum == PARTY_B){
		temp3 = 0 * b.first + 0 - 0;
		//cout<<temp3[i]<<endl;
	}
	temp4[0] = temp3;
	
	//funcReconstruct(temp3, Delta, size, "Delta Reconst", false);

	thread *threads = new thread[2];
	threads[0] = thread(sendVector<myType>, ref(temp4), nextParty(partyNum), 1);
	threads[1] = thread(receiveVector<myType>, ref(Delta), nextParty(partyNum), 1);
	
	for (int i = 0; i < 2; i++)
		threads[i].join();
	delete[] threads; 

	Delta[0] = Delta[0] + temp3;
	c = make_pair(Delta[0], 0);
}



void MSS_funcMatMul(const MSSVectorType &a, const ASSVectorMyType &b, MSSVectorType &c, 
					size_t rows, size_t common_dim, size_t columns,
				 	size_t transpose_a, size_t transpose_b, size_t truncation)
{
	log_print("MSS_funcMatMul");
	cout << "WE ARE USING MSS MATMUL!" << endl;
	assert(a.size() == rows*common_dim && "Matrix a incorrect for Mat-Mul");
	assert(b.size() == common_dim*columns && "Matrix b incorrect for Mat-Mul");
	assert(c.size() == rows*columns && "Matrix c incorrect for Mat-Mul");

	size_t final_size = rows*columns;
	ASSVectorMyType temp3(final_size);
	vector<myType> Delta(final_size, 0);

	matrixMultMSS(a, b, temp3, rows, common_dim, columns, transpose_a, transpose_b);
	
	funcReconstruct(temp3, Delta, final_size, "Delta Reconst", false);
	

	if (partyNum == PARTY_A)
	{
		for (int i = 0; i < final_size; ++i)
		{
			c[i] = make_pair(Delta[i], 0);
		}
	}
	
	if (partyNum == PARTY_B)
	{
		for (int i = 0; i < final_size; ++i)
		{
			c[i] = make_pair(Delta[i], 0);
		}
	}

	
}

void MSS_funcComp(const MSSType &a, const myType &b,  MSSType &res, size_t size)
{
	//P_A 1
	// myType r = aes_indep->get64Bits();
	myType r = 1 % MSS_BOUND ;
	// cout <<r<< MSS_BOUND <<endl;
	vector<uint64_t>ri(bitdecom(r , 64)); 
	//for(int i = 0; i < 64; i++) cout<<ri[i]<<endl;
	bool delta_A = 0;
	bool delta_B = 0;//default 0; 
	bool s = 1 - 2 * delta_A; //s = 1
	vector<bool>yi_A(64, 0);
	bool e_A = 1;             //1^delta_A
	int maskri = 1, R = 0;

	vector<bool>b_A; //
	bool t;
	for(int i = 0; i < 64; i++)
	{
		t = (s + ri[i])%2;
		b_A.push_back(t);
	}
	//cout<<"ok"<<endl;
	//for(int i = 0; i < 64; i++) cout<<"b_A:"<<b_A[i]<<endl;

	MSSType c;
	vector<myType> sndz(size, 0), revzi(size*64, 0);
	//local compute the [[c]] = [[a]] - [[b]]
	thread *threads = new thread[3];
	//thread threads;
	bool finalres;

	if(partyNum == 1) //A
	{

		finalres = ((r >> 40 )%2)^e_A^1; // 0
		
		//cout <<"comp finalres:  " << finalres <<endl;

		vector<uint64_t>b_B(64, 0);
		threads[0] = thread(receiveVector<myType>, ref(revzi), nextParty(partyNum), size*64);
		threads[0].join();
		//for(int i = 0;i < 64 ;i++) cout<<"revzi: "<<revzi[i]<<endl;
		for(int i = 0; i < 64; i++){
			b_B[i] = (1 * ( 3 * ( ( 1 - 2 * ri[i] ) * revzi[i] + ri[i] ) - revzi[i] ) - 0) % 2;
		}
		
		threads[1] = thread(sendVector<myType>, ref(b_B), nextParty(partyNum), size*64);
		threads[1].join();

		vector<myType>Delta_b(1, 0);
		threads[2] = thread(receiveVector<myType>, ref(Delta_b), nextParty(partyNum), 1);
		threads[2].join();

		MSSType fres(make_pair(Delta_b[0], finalres));
		res = fres;
		//for(int i = 0;i < 64 ;i++) cout<<"b_B: "<<b_B[i]<<endl;
		
	}

	if(partyNum == 0) //B
	{
		vector<uint64_t>sndzi;
		vector<uint64_t>recb_B(64, 0);
		
		// c.first = a.first - b.first;
		// c.second = a.second - b.second;
		// sndz[0] = c.first - c.second - (0 + r) + MSS_BOUND;
		sndz[0] = a.first - a.second - b - r + MSS_BOUND;
		//cout<<"C:"<<sndz[0]<<endl;
		//cout<<"sndz: "<<sndz[i]<<endl;
		vector<uint64_t>sndzi_t(bitdecom(sndz[0], 64));
		for(int i = 0;i < 64 ;i++) sndzi_t[i] = sndzi_t[i] - 0; //
		sndzi.insert(sndzi.begin(), sndzi_t.begin(), sndzi_t.end() ); //bitdec
		//for(int i = 0;i < 64 ;i++) cout<<"sndzi: "<<sndzi[i]<<endl;
			
		threads[0] = thread(sendVector<myType>, ref(sndzi), nextParty(partyNum), size*64);
		threads[0].join();

		threads[1] = thread(receiveVector<myType>, ref(recb_B), nextParty(partyNum), size*64);
		threads[1].join();

		//for(int i = 0;i < 64 ;i++) cout <<"recb_B:" << recb_B[i] <<endl;
		for(int i = 0;i < 64 ;i++)
		{
			if(recb_B[i]^b_A[i] == 0) {
				delta_B = 1;
				break;
			}
		
		}
		finalres = ((sndz[0] >> 40 )%2)^delta_B;
		//br = aes_indep->get64Bits();
		uint64_t br = 0; 
		vector<myType>Delta_b(1, finalres ^ br);
		
		MSSType fres(make_pair(Delta_b[0], br));
		
		threads[2] = thread(sendVector<myType>, ref(Delta_b), nextParty(partyNum), 1);
		threads[2].join();

		 
		res = fres;
		///cout <<"comp finalres:  " << finalres <<endl;


	}
	delete [] threads;

}

//******************************** Test *********************************************

void testMSSMUTL(size_t size, size_t iter)
{

	MSSVectorType a(size, make_pair(5, 0) );
	ASSVectorMyType b(size, 2);
	MSSVectorType c(size);
	
	for (int runs = 0; runs < iter; ++runs)
		MSS_funcMUTL(a, b, c, size, false, FLOAT_PRECISION);
}


void testMSSMatMul(size_t rows, size_t common_dim, size_t columns, size_t iter)
{

	MSSVectorType a(rows*common_dim, make_pair(2, 0));
	ASSVectorMyType b(common_dim*columns, 2);
	MSSVectorType c(rows*columns);

	for (int runs = 0; runs < iter; ++runs)
		MSS_funcMatMul(a, b, c, rows, common_dim, columns, 0, 0, FLOAT_PRECISION);
	// cout<<"1:"<<c[0].first<<c[0].second<<endl;
}


void testComp(size_t size, size_t iter)
{
	MSSType a(make_pair(100000000, 0));
	myType b(2450);
	MSSType res;

	for (int runs = 0; runs < iter; ++runs)
		MSS_funcComp(a, b, res, size);
}

