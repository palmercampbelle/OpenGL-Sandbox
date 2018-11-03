/**
 * Created by Etienne Palmer-Campbell Oct 2018
 **/

#include "database.h"

unique_ptr<Model> gPyramid;
unique_ptr<Model> gRoad;
unique_ptr<Model> gBackWall;

void initModels()
{

	vector<Vertex4f> backWallVertices = {
		Vertex4f( -10000, 0, 0 ),	// vertex 1
		Vertex4f(  10000, 0, 0 ),	// vertex 2
		Vertex4f( -2500, 0, 0 ),	// vertex 3
		Vertex4f( -2500, 2000, 0 ),	// vertex 4
		Vertex4f(  2500, 2000, 0 ),	// vertex 5
		Vertex4f(  2500, 0, 0 ),	// vertex 6
		Vertex4f( -1500, 0, 0 ),	// vertex 7
		Vertex4f( -1500, 1250, 0 ),	// vertex 8
		Vertex4f(  1500, 1250, 0 ),	// vertex 9
		Vertex4f(  1500, 0, 0 ),		// vertex 10
		Vertex4f( -100, 1100, 0 ),	// vertex 11
		Vertex4f( -100, 900, 0 ),	// vertex 12
		Vertex4f( 100, 1100, 0 ),	// vertex 13
		Vertex4f( 100, 900, 0 ),	// vertex 14
		Vertex4f( -50, 950, 0 ),	// vertex 15
		Vertex4f( 50, 950, 0 ),		// vertex 16
		Vertex4f( 0, 1025, 0 ),		// vertex 17
	};
	vector<vector<int>> backWallFaces = {
		{1,2},		// horizon line
		{3,4,5,6},	// outer wall
		{7,8,9,10},	// inner wall
		{11,12,14,13},// painting frame
		{15,16,17}// triangle painting
	};

	vector<Vertex4f> roadVertices = {
		Vertex4f( 100, 0, 0 ),		// vertex 1
		Vertex4f( -100, 0, 0 ),		// vertex 2
		Vertex4f( -100, 0, 1050 ),	// vertex 3
		Vertex4f( 100, 0, 1050 ),	// vertex 4
		Vertex4f( 0, 0, 0 ),		// vertex 5
		Vertex4f( 0, 0, 30 ),		// vertex 6
		Vertex4f( 0, 0, 60 ),		// vertex 7
		Vertex4f( 0, 0, 90 ),		// vertex 8
		Vertex4f( 0, 0, 120 ),		// vertex 9
		Vertex4f( 0, 0, 150 ),		// vertex 10
		Vertex4f( 0, 0, 180 ),		// vertex 11
		Vertex4f( 0, 0, 210 ),		// vertex 12
		Vertex4f( 0, 0, 240 ),		// vertex 13
		Vertex4f( 0, 0, 270 ),		// vertex 14
		Vertex4f( 0, 0, 300 ),		// vertex 15
		Vertex4f( 0, 0, 330 ),		// vertex 16
		Vertex4f( 0, 0, 360 ),		// vertex 17
		Vertex4f( 0, 0, 390 ),		// vertex 18
		Vertex4f( 0, 0, 420 ),		// vertex 19
		Vertex4f( 0, 0, 450 ),		// vertex 20
		Vertex4f( 0, 0, 480 ),		// vertex 21
		Vertex4f( 0, 0, 510 ),		// vertex 22
		Vertex4f( 0, 0, 540 ),		// vertex 23
		Vertex4f( 0, 0, 570 ),		// vertex 24
		Vertex4f( 0, 0, 600 ),		// vertex 25
		Vertex4f( 0, 0, 630 ),		// vertex 26
		Vertex4f( 0, 0, 660 ),		// vertex 27
		Vertex4f( 0, 0, 690 ),		// vertex 28
		Vertex4f( 0, 0, 720 ),		// vertex 29
		Vertex4f( 0, 0, 750 ),		// vertex 30
		Vertex4f( 0, 0, 780 ),		// vertex 31
		Vertex4f( 0, 0, 810 ),		// vertex 32
		Vertex4f( 0, 0, 840 ),		// vertex 33
		Vertex4f( 0, 0, 870 ),		// vertex 34
		Vertex4f( 0, 0, 900 ),		// vertex 35
		Vertex4f( 0, 0, 930 ),		// vertex 36
		Vertex4f( 0, 0, 960 ),		// vertex 37
		Vertex4f( 0, 0, 990 ),		// vertex 38
		Vertex4f( 0, 0, 1020 ),		// vertex 39
		Vertex4f( 0, 0, 1050 ),		// vertex 40
	};
	vector<vector<int>> roadFaces = {
		{ 1,2,3,4 },	// the face of the road
		{ 5, 6 },		// the dashes
		{ 7, 8 },
		{ 9, 10 },
		{ 11, 12 },
		{ 13, 14 },
		{ 15, 16 },
		{ 17, 18 },
		{ 19, 20 },
		{ 21, 22 },
		{ 23, 24 },
		{ 25, 26 },
		{ 27, 28 },
		{ 29, 30 },
		{ 31, 32 },
		{ 33, 34 },
		{ 35, 36 },
		{ 37, 38 },
		{ 39, 40 },
	};


	// x, y, and z coordinates of each vertex in the model geometry
	vector<Vertex4f> pyramidVertices = {
		Vertex4f( 0, 40, 0 ),		// vertex 1
		Vertex4f( 0, 0, -20 ),	// vertex 2
		Vertex4f( 40, 0, -20 ),	// vertex 3
		Vertex4f( 40, 30, 0 ),	// vertex 4
		Vertex4f( 20, 50, -20 )	// vertex 5
	};

	// faces are a collection of consecutive vertices (here, references to the index in the vertex list)
	vector<vector<int>> pyramidFaces = {
		{ 1, 2, 3, 4 }, // square 1-2-3-4
		{ 1, 2, 5 },    // triangle 1-2-5
		{ 2, 3, 5 },    // triangle 2-3-5
		{ 3, 4, 5 },    // triangle 3-4-5
		{ 4, 1, 5 }     // triangle 4-1-5
	};

	// initialize the models
	gPyramid = make_unique<Model>( pyramidVertices, pyramidFaces );
	gRoad = make_unique<Model>( roadVertices, roadFaces );
	gBackWall = make_unique<Model>( backWallVertices, backWallFaces );
}