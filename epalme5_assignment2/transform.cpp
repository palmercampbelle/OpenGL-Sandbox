/**
 * Created by Etienne Palmer-Campbell Oct 2018
 **/

#include "transform.h"
#include <cmath>


static Matrix4f Mview;	// View Orientation Matrix	(gluLookAt)
static Matrix4f Mproj;	// View Mapping Matrix		(glFrustum)

Matrix4f Identity()
{
	return Matrix4f(
		{
			1,	0,	0,	0,
			0,	1,	0,	0,
			0,	0,	1,	0,
			0,	0,	0,	1
		} );
}

Matrix4f Translate( float dX, float dY, float dZ )
{
	return Matrix4f(
		{
			1,	0,	0,	dX,
			0,	1,	0,	dY,
			0,	0,	1,	dZ,
			0,	0,	0,	1
		} );
}

Matrix4f Scale( float S )
{
	return Scale( S, S, S );
}

Matrix4f Scale( float sX, float sY, float sZ )
{
	return Matrix4f(
		{
			sX,	0,	0,	0,
			0,	sY,	0,	0,
			0,	0,	sZ,	0,
			0,	0,	0,	1
		} );
}

Matrix4f RotateX( float theta )
{
	float cosT = cosf( theta );
	float sinT = sinf( theta );
	return Matrix4f(
		{
		1,	0,		0,		0,
		0,	cosT,	-sinT,	0,
		0,	sinT,	cosT,	0,
		0,	0,		0,		1
		} );
}

Matrix4f RotateY( float theta )
{
	float cosT = cosf( theta );
	float sinT = sinf( theta );
	return Matrix4f(
		{
		cosT,	0,	sinT,	0,
		0,		1,	0,		0,
		-sinT,	0,	cosT,	0,
		0,		0,	0,		1
		} );
}

Matrix4f RotateZ( float theta )
{
	float cosT = cosf( theta );
	float sinT = sinf( theta );
	return Matrix4f(
		{
		cosT,	-sinT,	0,	0,
		sinT,	cosT,	0,	0,
		0,		0,		1,	0,
		0,		0,		0,	1
		} );
}

Matrix4f PerProj( float d )
{
	return Matrix4f(
		{
			1,	0,	0,	0,
			0,	1,	0,	0,
			0,	0,	1,	0,
			0,	0, 1 / d,	0
		} );
}

Matrix4f OrtProj()
{
	return Matrix4f(
		{
			1,	0,	0,	0,
			0,	1,	0,	0,
			0,	0,	0,	0,
			0,	0,	0,	1
		} );
}

/**
 * matrix for orthographic parallel projection
 */
Matrix4f Ortho( float left, float right, float bottom, float top, float near, float far )
{
	float tX = (right + left) / (right - left);
	float tY = (top + bottom) / (top - bottom);
	float tZ = (far + near) / (far - near);
	float x = 2 / (right - left);
	float y = 2 / (top - bottom);
	float z = -2 / (far - near);
	return Matrix4f(
		{
			x,	0,	0,	tX,
			0,	y,	0,	tY,
			0,	0,	z,	tZ,
			0,	0,	0,	1
		} );
}

/**
 * construct a matrix that normalizes and rotates the Viewing Volume such that
 * its components u/v/w are unit vectors lying on the x/y/z axes respectively
 *
 * Takes PRP, CW, and VuP
 */
Matrix4f RotateVRC( float prpX, float prpY, float prpZ,			// PRP
					float centerX, float centerY, float centerZ,	// Center of Window
					float upX, float upY, float upZ )			// VuP
{
	Vertex4f Ry( upX, upY, upZ, 0 );	// VuP
	Vertex4f Rz( prpX - centerX, prpY - centerY, prpZ - centerZ, 0 );	// Viewing Plane Normal (VPN)
	Rz.Normalize();

	Vertex4f Rx = Ry.Cross( Rz );
	Rx.Normalize();

	Ry = Rz.Cross( Rx );
	Ry.Normalize();

	return Matrix4f(
		{
			Rx[0],	Rx[1],	Rx[2],	0,
			Ry[0],	Ry[1],	Ry[2],	0,
			Rz[0],	Rz[1],	Rz[2],	0,
			0,		0,		0,		1
		} );
}

Matrix4f Shear( float viewCenterX, float viewCenterY, float viewCenterZ,
				float prpX, float prpY, float prpZ )
{
	float dopX = viewCenterX - prpX;
	float dopY = viewCenterY - prpY;
	float dopZ = viewCenterZ - prpZ;

	float shX = -(dopX / dopZ);
	float shY = -(dopY / dopZ);

	return Matrix4f(
		{
			1,	0,	shX,	0,
			0,	1,	shY,	0,
			0,	0,	1,		0,
			0,	0,	0,		1
		} );
}

/**
 * convert from the canonical perspective viewing volume into parallel
 */
Matrix4f ConvertPerToParVV( float zMin )
{
	return Matrix4f(
		{
			1,	0,			0,				0,
			0,	1,			0,				0,
			0,	0,	1 / (1 + zMin),	-zMin / (1 + zMin),
			0,	0,			-1,				0
		} );
}

// mimics glFrustum
Matrix4f Frustum( float left, float right, float bottom, float top, float near, float far )
{
	float A = (right + left) / (right - left);
	float B = (top + bottom) / (top - bottom);
	float C = -(far + near) / (far - near);
	float D = -(2 * far*near) / (far - near);
	float sX = (2 * near) / (right - left);
	float sY = (2 * near) / (top - bottom);
	return Matrix4f(
		{
			sX,	0,	A,	0,
			0,	sY,	B,	0,
			0,	0,	C,	D,
			0,	0,	-1,	0
		} );
}


