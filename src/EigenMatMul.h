
#ifndef EIGEN_H
#define EIGEN_H

#pragma once
#include "globals.h"
#include <Eigen/Dense>
using namespace Eigen;
using eig_mat = Matrix<myType, Dynamic, Dynamic>;

class eigenMatrix {
public: 
	std::array<eig_mat, 2> m_share;
	size_t rows;
	size_t columns;
	
	eigenMatrix(size_t _rows, size_t _columns)
	: m_share({eig_mat(_rows, _columns), eig_mat(_rows, _columns)}),
	  rows(_rows),
	  columns(_columns){}

	eigenMatrix operator*(const eigenMatrix &B)
	{
		assert(this->columns == B.rows && "Dimension mismatch");

		eigenMatrix ret(this->rows, B.columns);
		ret.m_share[0] = eig_mat::Zero(this->rows, B.columns);

		eigenMatrix ret1(this->rows, B.columns);
		ret1.m_share[0] = eig_mat::Zero(B.rows, B.columns);

		ret.m_share[1] = this->m_share[0] * B.m_share[0] - ret.m_share[0] + ret.m_share[0] ;
		
		
		
		if(partyNum == PARTY_B){
			
		ret.m_share[1] = this->m_share[0] * ret1.m_share[0] - ret.m_share[0] + ret.m_share[0] ;
			
		}		

		return ret;
	}
};


#endif