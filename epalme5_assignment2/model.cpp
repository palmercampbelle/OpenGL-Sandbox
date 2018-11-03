/**
 * Created by Etienne Palmer-Campbell Oct 2018
 **/
#include "model.h"

Model::Model()
	: Model( {}, {} )
{
}

// constructor taking a vertex list
Model::Model( const vector<Vertex4f>& vertices )
	: Model( vertices, {} )
{
}

// constructor taking a vertex list
//    and an array of face vertex indices
Model::Model( const vector<Vertex4f>& vertices, const vector<vector<int>>& faces )
	: mVertexList( vertices )
{
	for ( const vector<int>& face : faces )
	{
		addFace( face );
	}
}


void Model::addVertex( const Vertex4f& vertex )
{
	mVertexList.push_back( vertex );
}

void Model::addFace( const vector<int>& faceIndices )
{
	mFaceList.push_back( faceIndices );
}

vector<vector<Vertex4f>> Model::getFaceVertices() const
{
	vector<vector<Vertex4f>> faceVertices;

	faceVertices.resize( mFaceList.size() );
	for ( size_t i = 0; i < mFaceList.size(); ++i )
	{
		faceVertices[i].reserve( mFaceList[i].size() );
		for ( size_t f : mFaceList[i] )
		{
			faceVertices[i].push_back( mVertexList[f - 1] );
		}
	}

	return faceVertices;
}
