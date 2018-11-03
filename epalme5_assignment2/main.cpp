/**
 * Created by Etienne Palmer-Campbell Oct 2018
 **/

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Switch between using my own code and GLUT library functions
// #define USE_GLUT_TRANSFORMATIONS
#ifdef USE_GLUT_TRANSFORMATIONS
#ifndef CLIPPING_OFF
#define CLIPPING_OFF
#endif
#endif 

#define CLIPPING_OFF

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>

#include "model.h"
#include "matrix.h"
#include "transform.h"
#include "vertex.h"
#include "database.h"

using namespace std;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~								GLOBALS									~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// the edges of our viewing window
float RIGHT;
float LEFT;
float TOP;
float BOTTOM;

constexpr float BACK = 5000;	// distance from PRP to back clipping plane
constexpr float FRONT = 200;	// distance from PRP to front clipping plane
constexpr float MODEL_SCALE_FACTOR = 2.5f;	// used for tweaking the size of models to match the viewing volume
constexpr float CAMERA_SPEED = 6 * MODEL_SCALE_FACTOR;

int gStartTime;
float gTheta = 0;	// used to rotate models over time

Vertex4f PRP( 0, MODEL_SCALE_FACTOR * 250, BACK * 0.9f, 1 );	// the starting 'eye point' of the camera (in Viewing Reference Coordinates)
Vertex4f VRP( 0, 0, 0, 1 );		// viewing reference point (in World Coordinates)
Vertex4f VuP( 0, 1, 0, 0 );		// View up vector

Matrix4f Nper   = Identity();	// the normalization matrix for our perspective viewing volume
Matrix4f Mvv3dv = Identity();	// scales, translates, and projects into 2D device coordinates
Matrix4f Mfru   = Identity();
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~							CLIPPING UTILITY							~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

typedef char ClipCode;
enum ClipCodeType : char
{
	cc_INSIDE   = 0,
	cc_ABOVE  = (1 << 0),
	cc_BELOW  = (1 << 1),
	cc_RIGHT  = (1 << 2),
	cc_LEFT   = (1 << 3),
	cc_BEHIND = (1 << 4),
	cc_FRONT  = (1 << 5),
};

// generate outcode for the Cohen-Sutherland Line Clipping algorithm
ClipCode GenerateClipCode( const Vertex4f& P )
{
	ClipCode code = cc_INSIDE;

	if ( P.X() < -P.W() )
	{
		code |= cc_LEFT;
	}
	else if ( P.X() > P.W() )
	{
		code |= cc_RIGHT;
	}
	if ( P.Y() < -P.W() )
	{
		code |= cc_BELOW;
	} 
	else if ( P.Y() > P.W() )
	{
		code |= cc_ABOVE;
	}
	if ( P.Z() < -P.W() )
	{
		code |= cc_BEHIND;
	}
	else if ( P.Z() > 0 )
	{
		code |= cc_FRONT;
	}

	return code;
}

// use Cohen-Sutherland Line-Clipping algorithm on Homogeneous 4D points
void ClipAndDrawLine( Vertex4f P1, Vertex4f P2 )
{
	ClipCode code1 = GenerateClipCode( P1 );
	ClipCode code2 = GenerateClipCode( P2 );
	bool accept = false;
	bool done = false;

#ifdef CLIPPING_OFF
	code1 = cc_INSIDE;
	code2 = cc_INSIDE;
#endif

	do // while ( done == false );
	{
		if ( !(code1|code2) )	// both points inside window, no clip
		{
			accept = true;
			done = true;
		}
		else if ( code1 & code2 )	// both points share a clip region, clip entire line
		{
			done = true;
		}
		else	// the line may intersect with window, calculate partial clip
		{
			Vertex4f Pi;	// intersection point

			ClipCode outCode = code1 ? code1 : code2;	// choose a clip code from outside the window

			float t;
			if ( outCode & cc_ABOVE )
			{
				Pi.Y() = P1.W();
				t = (Pi.Y() - P1.Y()) / (P2.Y() - P1.Y());
				Pi.X() = P1.X() + t * (P2.X() - P1.X());
				Pi.Z() = P1.Z() + t * (P2.Z() - P1.Z());
				Pi.W() = P1.W() + t * (P2.W() - P1.W());
			}
			else if ( outCode & cc_BELOW )
			{
				Pi.Y() = -P1.W();
				t = (Pi.Y() - P1.Y()) / (P2.Y() - P1.Y());
				Pi.X() = P1.X() + t * (P2.X() - P1.X());
				Pi.Z() = P1.Z() + t * (P2.Z() - P1.Z());
				Pi.W() = P1.W() + t * (P2.W() - P1.W());
			}
			else if ( outCode & cc_RIGHT )
			{
				Pi.X() = P1.W();
				t = (Pi.X() - P1.X()) / (P2.X() - P1.X());
				Pi.Y() = P1.Y() + t * (P2.Y() - P1.Y());
				Pi.Z() = P1.Z() + t * (P2.Z() - P1.Z());
				Pi.W() = P1.W();
			}
			else if ( outCode & cc_LEFT )
			{
				Pi.X() = -P1.W();
				t = (Pi.X() - P1.X()) / (P2.X() - P1.X());
				Pi.Y() = P1.Y() + t * (P2.Y() - P1.Y());
				Pi.Z() = P1.Z() + t * (P2.Z() - P1.Z());
				Pi.W() = P1.W();
			}
			else if ( outCode & cc_FRONT )
			{
				Pi.Z() = 0;
				t = (Pi.Z() - P1.Z()) / (P2.Z() - P1.Z());
				Pi.Y() = P1.Y() + t * (P2.Y() - P1.Y());
				Pi.X() = P1.X() + t * (P2.X() - P1.X());
				Pi.W() = P1.W();
			}
			else if ( outCode & cc_BEHIND )
			{
				Pi.Z() = -P1.W();
				t = (Pi.Z() - P1.Z()) / (P2.Z() - P1.Z());
				Pi.Y() = P1.Y() + t * (P2.Y() - P1.Y());
				Pi.X() = P1.X() + t * (P2.X() - P1.X());
				Pi.W() = P1.W();
			}

			if ( outCode == code1 )
			{
				P1 = Pi;
				code1 = GenerateClipCode( P1 );
			}
			else
			{
				P2 = Pi;
				code2 = GenerateClipCode( P2 );
			}
		}
	} while ( done == false );

	if ( accept )
	{
		// transform into 2D device coordinates and draw
		P1 = Mvv3dv * P1;
		P2 = Mvv3dv * P2;
		P1.ConvertW();
		P2.ConvertW();
		glVertex3f( P1.X(), P1.Y(), P1.Z() );
		glVertex3f( P2.X(), P2.Y(), P2.Z() );
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~							DISPLAY UTILITY								~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void updateCamera()
{
#ifdef USE_GLUT_TRANSFORMATIONS
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( PRP.X(), PRP.Y(), PRP.Z(),
			   VRP.X(), VRP.Y(), VRP.Z(),	/* always looking middle of horizon */
			   VuP.X(), VuP.Y(), VuP.Z() );
#else	// USE_GLUT_TRANSFORMATIONS

	/**
	 * These transformation matrices combine to form the normalization of the perspective
	 * projection of the viewing volume
	 */
	const Matrix4f Tvrp = Translate( -VRP.X(), -VRP.Y(), -VRP.Z() );
	const Matrix4f R    = RotateVRC( PRP.X(), PRP.Y(), PRP.Z(),
								     VRP.X(), VRP.Y(), VRP.Z(),	/* always looking middle of horizon */
								     VuP.X(), VuP.Y(), VuP.Z() );
	const Matrix4f Tprp = Translate( -PRP.X(), -PRP.Y(), -PRP.Z() );


	Vertex4f centerVW;
	centerVW.X() = (RIGHT + LEFT) / 2;
	centerVW.Y() = (TOP + BOTTOM) / 2;
	centerVW.Z() = 0;
	centerVW = Tprp * centerVW;	// the PRP is a constant distance from the view plane
	const Matrix4f Sh = Shear( centerVW.X(), centerVW.Y(), centerVW.Z(),
							   PRP.X(), PRP.Y(), PRP.Z() );


	// determine Scale factors
	float sX = (2 * FRONT) / ((RIGHT - LEFT) * BACK);
	float sY = (2 * FRONT) / ((TOP - BOTTOM) * BACK);
	float sZ = 1 / BACK;
	const Matrix4f Sper = Scale( sX, sY, sZ );

	// convert from Perspective to Parallel VV
	float zMin = (FRONT / BACK);
	const Matrix4f Mc = ConvertPerToParVV( zMin );

	// update Normalization matrix for Parallel Canonical Viewing Volume
	Nper = Matrix4f::DotProduct( Mc, Sper, Sh, Tprp, R, Tvrp );
// 	Nper = Matrix4f::DotProduct( Mfru, Tprp, R, Tvrp );

#endif // USE_GLUT_TRANSFORMATIONS
}

void drawFaces( Matrix4f M, vector<vector<Vertex4f>> faceVerts )
{
	for ( vector<Vertex4f>& face : faceVerts )
	{
		for ( Vertex4f& P : face )
		{
			// apply model transformations
			P = (M * P);
			P.ConvertW();

			// transform into homogeneous 4D canonical viewing volume
			P = Nper * P;
		}

		glBegin( GL_LINES );
		for ( size_t i = 0; i < face.size(); ++i )
		{
			const Vertex4f& P1 = face[i];
			const Vertex4f& P2 = ((i + 1) == face.size()) ? face[0] : face[i+1];
			// transform and project onto device coordinates
			ClipAndDrawLine( P1, P2 );
		}
		glEnd();
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~						MAIN GLUT FUNCTIONS								~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// accept user input
void Keyboard( unsigned char key, int x, int y )
{
	switch ( key )
	{
	case 27:	// ESCAPE key; exits program
		exit( 0 );
		break;
	case 'w':	// fly up
		PRP.Y() += CAMERA_SPEED;
		break;
	case 's':	// fly down
		PRP.Y() -= CAMERA_SPEED;
		break;
	case 'a':	// strafe left
		PRP.X() -= CAMERA_SPEED;
		break;
	case 'd':	// strafe right
		PRP.X() += CAMERA_SPEED;
		break;
	case 'f':	// fly forwards/zoom in
		PRP.Z() -= CAMERA_SPEED;
		break;
	case 'v':	// fly backwards/zoom out
		PRP.Z() += CAMERA_SPEED;
		break;
	}
}

// initialize GLOBALS and time
void init()
{
	gStartTime = glutGet( GLUT_ELAPSED_TIME );

	initModels();
}

void display()
{
	glClear( GL_COLOR_BUFFER_BIT );
	glColor3f( 0, 0, 0 );

	// update the viewing volume transformations
	updateCamera();

	const Matrix4f S_model = Scale( MODEL_SCALE_FACTOR );
	Matrix4f M_model = Identity();

	// draw rotating pyramid model
	M_model = Matrix4f::DotProduct( S_model,
									Translate( 0, 100, BACK * 0.2f ),
									RotateY( gTheta ) );
	drawFaces( M_model, gPyramid->getFaceVertices() );

	// draw road
	M_model = Matrix4f::DotProduct( S_model,
									Translate( 0, 0, BACK * 0.2f ) );
	drawFaces( M_model, gRoad->getFaceVertices() );

	// horizon line and back wall
	M_model = S_model;
	drawFaces( M_model, gBackWall->getFaceVertices() );

	glutSwapBuffers();
}

void reshape( int w, int h )
{
	// update the edges of our viewing window
	RIGHT = (w * 0.5);
	LEFT = (-w * 0.5);
	TOP = (h * 0.5);
	BOTTOM = (-h * 0.5);

	glViewport( 0, 0, (GLsizei)w, (GLsizei)h );

#ifdef USE_GLUT_TRANSFORMATIONS
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum( (GLdouble)LEFT, (GLdouble)RIGHT, (GLdouble)BOTTOM, (GLdouble)TOP, (GLdouble)FRONT, (GLdouble)BACK );
#else
	Mfru = Frustum( LEFT, RIGHT, BOTTOM, TOP, FRONT, BACK );

	// Translate and scale back to device coord
	const Matrix4f T1 = Translate( 1, 1, 1 );
	const Matrix4f S = Scale( (RIGHT - LEFT) / 2, (TOP - BOTTOM) / 2, 0 );
	const Matrix4f T2 = Translate( LEFT, BOTTOM, 0 );
	const Matrix4f Mort = Ortho( LEFT, RIGHT, BOTTOM, TOP, 0, 1 );
	
	Mvv3dv = Matrix4f::DotProduct( Mort, T2, S, T1 );

#endif // USE_GLUT_TRANSFORMATIONS
}

// update the angle of rotation
void idle()
{
	int currentTime = glutGet( GLUT_ELAPSED_TIME );

	int dt = currentTime - gStartTime;
	gStartTime = currentTime;

	gTheta += dt * -0.0021;

	glutPostRedisplay();
}

int main( int argc, char** argv )
{
// initialize glut & display window
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "CS425 - epalme5 Assignment 2" );

	glClearColor( 1, 1, 1, 1 );

	init();

	/* Receive keyboard inputs */
	glutKeyboardFunc( Keyboard );

	// display function
	glutDisplayFunc( display );

	// idle function
	glutIdleFunc( idle );

	// reshape function
	glutReshapeFunc( reshape );

	// loop
	glutMainLoop();
	return 0;
}
