/**
 * Created by Etienne Palmer-Campbell Oct 2018
 **/
#pragma once

#include "matrix.h"
#include <vector>
using std::vector;

// Identity Matrix
Matrix4f Identity();

// generic transformations
Matrix4f Translate( float dX, float dY, float dZ );
Matrix4f Scale( float sX, float sY, float sZ );
Matrix4f Scale( float S );
Matrix4f RotateX( float theta );
Matrix4f RotateY( float theta );
Matrix4f RotateZ( float theta );

// viewing transformations
Matrix4f PerProj( float d );
Matrix4f OrtProj();
Matrix4f Ortho( float left, float right, float bottom, float top, float near, float far );
Matrix4f RotateVRC( float prpX, float prpY, float prpZ,			// PRP
					float centerX, float centerY, float centerZ,	// Center of Window
					float upX, float upY, float upZ );			// VuP
Matrix4f Shear( float viewCenterX, float viewCenterY, float viewCenterZ,
				float prpX, float prpY, float prpZ );
Matrix4f ConvertPerToParVV( float zMin );
Matrix4f Frustum( float left, float right, float bottom, float top, float near, float far );



