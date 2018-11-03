/**
 * Created by Etienne Palmer-Campbell Oct 2018
 **/

#include "vertex.h"
#include <cmath>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~								Vertex4f								~~// 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

Vertex4f::Vertex4f( float x, float y, float z, float w )
	: mV( {x,y,z,w} )
{
}

Vertex4f::Vertex4f()
	: Vertex4f( 0, 0, 0 )
{
}

void Vertex4f::ConvertW( float iW )
{
	for ( size_t i = 0; i < 3; i++ )
	{
		mV[i] = (mV[i] / mV[3]) * iW;
	}
	mV[3] = iW;
}

float& Vertex4f::operator[]( int i )
{
	return mV[i];
}

const float& Vertex4f::operator[]( int i ) const
{
	return mV[i];
}

float Vertex4f::Mag() const
{
	if ( mV[3] != 0.f )
	{// not a vector, can't perform
		return 0.f;
	}
	float x2 = (mV[0] * mV[0]);
	float y2 = (mV[1] * mV[1]);
	float z2 = (mV[2] * mV[2]);
	return std::sqrtf( x2 + y2 + z2 );
}

void Vertex4f::Normalize()
{
	if ( float mag = Mag() )
	{
		mV[0] /= mag;
		mV[1] /= mag;
		mV[2] /= mag;
	}
}

Vertex4f Vertex4f::Cross( const Vertex4f& B ) const
{
	Vertex4f C( 0, 0, 0, 0 );
	if ( W() != 0 )
	{// not a vector, can't perform
		return C;
	}
	const Vertex4f& A = *this;
	// return result of A x B
	C.X() = (A.Y() * B.Z()) - (A.Z() * B.Y());
	C.Y() = (A.Z() * B.X()) - (A.X() * B.Z());
	C.Z() = (A.X() * B.Y()) - (A.Y() * B.X());
	return C;
}

Vertex4f Vertex4f::operator*( const Vertex4f& B ) const
{
	return this->Cross( B );
}

