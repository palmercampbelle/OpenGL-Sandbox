/**
 * Created by Etienne Palmer-Campbell Oct 2018
 **/
#pragma once

#include "vertex.h"
#include <vector>
using std::vector;

class Model
{
	vector<Vertex4f> mVertexList;
	vector<vector<int>> mFaceList;

public:
	// constructors
	Model();
	Model( const vector<Vertex4f>& vertices );
	Model( const vector<Vertex4f>& vertices, const vector<vector<int>>& faces );

	void addVertex( const Vertex4f& vertex );
	void addFace( const vector<int>& faceIndices );

	vector<vector<Vertex4f>> getFaceVertices() const;
};
