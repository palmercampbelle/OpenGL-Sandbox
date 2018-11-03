/**
 * Created by Etienne Palmer-Campbell Oct 2018
 **/


#include "matrix.h"
#include "transform.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~								Matrix4f								~~// 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

Matrix4f::Matrix4f()
{
	mMatrix.resize( 16, 0 );
}

Matrix4f::Matrix4f( const Matrix4f& other )
	: mMatrix( other.mMatrix )
{
}

Matrix4f::Matrix4f( const vector<float>& elems )
	: mMatrix( elems )
{
}

// naive matrix product algorithm
Matrix4f Matrix4f::operator*( const Matrix4f& B ) const
{
	Matrix4f C;
	const Matrix4f& A = *this;

	for ( int i = 0; i < 4; ++i )
	{
		for ( int j = 0; j < 4; ++j )
		{
			for ( int k = 0; k < 4; ++k )
			{
				C( i, j ) += A( i, k ) * B( k, j );
			}
		}
	}

	return C;
}

Vertex4f Matrix4f::operator*( const Vertex4f& other ) const
{
	const Matrix4f& M = *this;
	Vertex4f P;
	for ( size_t i = 0; i < 4; ++i )
	{
		P.mV[i] = 0;
		for ( size_t j = 0; j < 4; ++j )
		{
			P.mV[i] += other.mV[j] * M( i, j );
		}
	}
	return P;
}

// 2D indexing of individual matrix elements
float& Matrix4f::operator()( int i, int j )
{
	return mMatrix[(i * 4) + j];
}
const float& Matrix4f::operator()( int i, int j ) const
{
	return mMatrix[(i * 4) + j];
}


