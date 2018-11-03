/**
 * Created by Etienne Palmer-Campbell Oct 2018
 **/
#pragma once

#include <vector>
using std::vector;

struct Vertex4f
{
	vector<float> mV;

	Vertex4f();
	Vertex4f( float x, float y, float z, float w = 1 );

	void ConvertW( float iW = 1 );

	float Mag() const;
	void Normalize();
	Vertex4f Cross( const Vertex4f& B ) const;

	Vertex4f operator* ( const Vertex4f& B ) const;
	float& operator[]( int i );
	const float& operator[] ( int i ) const;

	float& X() { return mV[0]; }
	float& Y() { return mV[1]; }
	float& Z() { return mV[2]; }
	float& W() { return mV[3]; }
	const float& X() const { return mV[0]; }
	const float& Y() const { return mV[1]; }
	const float& Z() const { return mV[2]; }
	const float& W() const { return mV[3]; }

};
