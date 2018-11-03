/**
 * Created by Etienne Palmer-Campbell Oct 2018
 **/
#pragma once

#include "vertex.h"
#include <vector>
using std::vector;

class Matrix4f
{
	vector<float> mMatrix;

public:
	Matrix4f();
	Matrix4f( const Matrix4f& other );
	Matrix4f( const vector<float>& elems );

	Matrix4f operator* ( const Matrix4f& other ) const;
	Vertex4f operator* ( const Vertex4f& other ) const;
	float& operator() ( int i, int j );
	const float& operator() ( int i, int j ) const;

	template<typename... Rest>
	static Matrix4f DotProduct( const Matrix4f& A, const Matrix4f& B, const Rest&... rest ) { return A * DotProduct( B, rest... ); }
	static Matrix4f DotProduct( const Matrix4f& A, const Matrix4f& B ) { return A * B; }
};
