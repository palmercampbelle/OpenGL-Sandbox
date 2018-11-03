/*
 * File:   epalme5_assignment1.cpp
 * Author: Etienne Palmer-Campbell | epalme5
 *
 * Date: Sept 20th, 2018
 *
 * Operating System: Windows 10 64-bit
 * Compiler: Visual Studio 2017
 * Platform: x86
 *
 * Press key 'a' to display previous, and 'd' to display next
 */



#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL\glut.h>
#endif


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <list>
using namespace std;

/******************************************************************/
// Utility code; structs & globals

struct Vertex2i
{
	int X;
	int Y;
};

struct Color3f
{
	float R;
	float G;
	float B;

	void Set() const
	{
		glColor3f( R, G, B );
	}
};

namespace ColorPalette
{
	const Color3f LightBlue{0.4f, 0.6f, 1.f};
	const Color3f Red{0.9f, 0.2f, 0.2f};
	const Color3f Green{0.2f, 0.8f, 0.2f};
	const Color3f Yellow{0.9f, 0.9f, 0.1f};
	const Color3f Orange{0.9f, 0.6f, 0.1f};
	const Color3f Black{0.f, 0.f, 0.f};
	const Color3f White{1.f, 1.f, 1.f};
}

// used to create Edge Table and Active Edge List
struct Edge
{
	int ymax;
	float xmin;
	float invM;
};
struct EdgeList : list<Edge>
{
	void addEdge( const Edge& E)
	{
		for( auto it = this->begin(); it != this->end(); it++)
		{
			if(it->xmin > E.xmin)
			{
				this->insert( it, E );
				return;
			}
		}
		this->push_back( E );
	}
};
typedef vector<EdgeList> EdgeTable;


// tracks which slide to display
int currentSlide = 1;

constexpr int windowHeight = 800;
constexpr int windowWidth = 800;

/******************************************************************/

/* output code by Mark Kilgard */
void output( int x, int y )
{
// 	int len, i;
	glRasterPos2f( x, y );
}

void Keyboard( unsigned char key, int x, int y )
{
	//cout<<key<<endl;
	switch ( key )
	{
	case 27:       //ESCAPE key
		exit( 0 );
		break;

	case 'd':      // Next slide
		if ( currentSlide < 3 )
		{
			currentSlide++;
		}
		break;

	case 'a':      // Previous slide
		if ( currentSlide > 1 )
		{
			currentSlide--;
		}
		break;
	}
}


/******************************************************************/
// DRAWING FUNCTIONS
/******************************************************************/

// uses the midpoint line algorithm to scan convert a line between two points
void drawLine( const Vertex2i& A, const Vertex2i& B )
{
	Vertex2i p0 = A;
	Vertex2i p1 = B;
	bool reflect = abs( B.Y - A.Y ) > abs( B.X - A.X );

	// helper lambda that plots a single point, corrected for reflection if needed
	auto drawPoint = [&reflect]( const int& rX, const int& rY )
	{
		if ( reflect )
		{
			glVertex2f( rY, rX );
		}
		else
		{
			glVertex2f( rX, rY );
		}
	};

	// if |slope| > 1 then swap X/Y
	if ( reflect )
	{
		p0 = { A.Y, A.X };
		p1 = { B.Y, B.X };
	}

	// correct for out of order points - first point must have smaller X
	if ( p0.X > p1.X )
	{
		Vertex2i t = p0;
		p0 = p1;
		p1 = t;
	}

	int dy = p1.Y - p0.Y;
	int dx = p1.X - p0.X;
	int yIncr = 1;
	int y = p0.Y;
	// correct for negative slope
	if ( dy < 0 )
	{
		yIncr = -1;
		dy = -dy;
	}
	int d = 2 * dy - dx;	// decision variable
	int dE = 2 * dy;		// delta East
	int dNE = 2 * (dy - dx);// delta NorthEast

	// now we may assume 0 <= m <= 1 and begin midpoint algorithm
	// simple case : horizontal line
	if ( dy == 0 )
	{
		for ( int x = p0.X; x <= p1.X; x++ )
		{
			drawPoint( x, y );
		}
		return;
	}
	// simple case : m = 1
	if ( dy == dx )
	{
		for ( int x = p0.X; x <= p1.X; x++ )
		{
			drawPoint( x, y );
			y += yIncr;
		}
		return;
	}

	// midpoint algorithm: plot line
	for ( int x = p0.X; x <= p1.X; x++ )
	{
		drawPoint( x, y );
		if(d <= 0)
		{
			d += dE;
		}
		else
		{
			d += dNE;
			y += yIncr;
		}
	}
}

// this overload of drawLine() will connect a sequence of vertices with lines
template<typename... Rest>
void drawLine( const Vertex2i& A, const Vertex2i& B, const Rest&... rest )
{
	drawLine( A, B );
	drawLine( B, rest... );
}

// Connect a sequence of vertices using drawLine() and close the end to form a polygon
void drawPolygon( const vector<Vertex2i>& vertices )
{
	if ( vertices.size() < 3 ) return;

	// connect each vertex in sequence
	for ( unsigned int i = 1; i < vertices.size(); i++ )
	{
		drawLine( vertices[i - 1], vertices[i] );
	}
	// connect start to end
	if ( vertices.size() > 2 )
	{
		drawLine( vertices[0], vertices[vertices.size() - 1] );
	}
}

// write a string to screen (incomplete alphabet)
// Letters will be mono spaced to 100x200
void drawString( const char* cstr, Vertex2i& cursorPos )
{
	// This helper lambda will use drawLine() to create letters of choice at target location
	// Currently only supports small number of characters
	auto drawChar = [&cursorPos]( char letter )
	{
		const int x = cursorPos.X;
		const int y = cursorPos.Y;
		switch ( letter )
		{
		case 'E':
			drawLine( {x, y}, {x, y + 200} );
			drawLine( {x, y}, {x + 100, y} );
			drawLine( {x, y + 100}, {x + 100, y + 100} );
			drawLine( {x, y + 200}, {x + 100, y + 200} );
			break;
		case 'I':
			drawLine( {x, y}, {x + 100, y} );
			drawLine( {x + 50, y}, {x + 50, y + 200} );
			drawLine( {x, y + 200}, {x + 100, y + 200} );
			break;
		case 'N':
			drawLine( {x, y}, {x, y + 200} );
			drawLine( {x, y + 200}, {x + 100, y} );
			drawLine( {x + 100, y}, {x + 100, y + 200} );
			break;
		case 'T':
			drawLine( {x + 50, y}, {x + 50, y + 200} );
			drawLine( {x, y + 200}, {x + 100, y + 200} );
			break;
		default:
			return;
		}

		cursorPos.X += 110;
	};

	// draw each char of string
	int size = strlen( cstr );
	for ( int i = 0; i < size; i++ )
	{
		drawChar( cstr[i] );
	}
}

// uses midpoint algorithm for scan converting circle
void drawCircle( const Vertex2i& center, int radius)
{
	// helper lambda for drawing the 8-way symmetrical points of circle 
	auto drawCirclePoints = [&center]( int x, int y )
	{
		glVertex2f( center.X - x, center.Y - y );
		glVertex2f( center.X - x, center.Y + y );
		glVertex2f( center.X + x, center.Y - y );
		glVertex2f( center.X + x, center.Y + y );
		glVertex2f( center.X - y, center.Y - x );
		glVertex2f( center.X - y, center.Y + x );
		glVertex2f( center.X + y, center.Y - x );
		glVertex2f( center.X + y, center.Y + x );
	};

	int x = 0;
	int y = radius;
	float d = (5.f / 4.f) - radius;
	drawCirclePoints( x, y );

	// scan convert 1/8 of the circle, and replicate it 8 times
	while( y > x )
	{
		if (d < 0 )
		{// go East
			d += (2.f * x) + 3.f;
		}
		else
		{// go SouthEast
			d += (2.f * (x - y)) + 5.f;
			y--;
		}
		x++;
		drawCirclePoints( x, y );
	}
}

// uses midpoint algorithm to scan convert points of ellipse
void drawEllipse( const Vertex2i& center, int a/*width radius*/, int b/*height radius*/)
{
	// helper lambda to write 4-way symmetrical points of ellipse
	auto drawEllipsePoints = [&center]( int x, int y )
	{
		glVertex2f( center.X + x, center.Y + y );
		glVertex2f( center.X - x, center.Y + y );
		glVertex2f( center.X + x, center.Y - y );
		glVertex2f( center.X - x, center.Y - y );
	};

	int a2 = a * a;
	int b2 = b * b;
	int x = 0;
	int y = b;

	// decision variable for first region of ellipse
	float d1 = b2 - (a2 * b) + (0.25f * b);

	drawEllipsePoints( x, y );

	// draw first region
	while ( (a2 * (y - .5f)) > (b2 * (x + 1)) )
	{
		if( d1 < 0)
		{// go east
			d1 += b2 * ((2 * x) + 3);
		}
		else
		{// go south east
			d1 += (b2 * ((2 * x) + 3)) + (a2 * ((-2 * y) + 2));
			y--;
		}
		x++;
		drawEllipsePoints( x, y );
	}

	// second decision variable
	float d2 = (b2 * ((x + .5f) * (x + .5f))) + (a2 * ((y - 1)*(y - 1))) - (a2 * b2);
	// draw second region
	while( y > 0)
	{
		if (d2 < 0)
		{// go south east
			d2 += (b2 * ((2 * x) + 2)) + (a2 * ((-2 * y) + 3));
			x++;
		}
		else
		{// go east
			d2 += a2 * ((-2 * y) + 3);
		}
		y--;
		drawEllipsePoints( x, y );
	}
}

/******************************************************************/
// FILLING FUNCTIONS //
/******************************************************************/

// uses midpoint algorithm to scan convert points of ellipse, then fills with scan lines
void fillEllipse( const Vertex2i& center, int a/*width radius*/, int b/*height radius*/, const Color3f& color )
{
	color.Set();
	// helper lambda to fill scan lines of symmetrical halves of ellipse
	auto drawEllipseScanLines = [&center]( int x, int y )
	{
		drawLine( Vertex2i{center.X - x, center.Y + y},
				  Vertex2i{center.X + x, center.Y + y} );
		drawLine( Vertex2i{center.X - x, center.Y - y},
				  Vertex2i{center.X + x, center.Y - y} );
	};

	int a2 = a * a;
	int b2 = b * b;
	int x = 0;
	int y = b;
	float d1 = b2 - (a2 * b) + (0.25f * b);	// decision variable for first region of ellipse
	drawEllipseScanLines( x, y );

	// draw first region
	while ( (a2 * (y - .5f)) > (b2 * (x + 1)) )
	{
		if ( d1 < 0 )
		{// go east
			d1 += b2 * ((2 * x) + 3);
		}
		else
		{// go south east
			d1 += (b2 * ((2 * x) + 3)) + (a2 * ((-2 * y) + 2));
			y--;
		}
		x++;
		drawEllipseScanLines( x, y );
	}

	// second decision variable
	float d2 = (b2 * ((x + .5f) * (x + .5f))) + (a2 * ((y - 1)*(y - 1))) - (a2 * b2);
	// draw second region
	while ( y > 0 )
	{
		if ( d2 < 0 )
		{// go south east
			d2 += (b2 * ((2 * x) + 2)) + (a2 * ((-2 * y) + 3));
			x++;
		}
		else
		{// go east
			d2 += a2 * ((-2 * y) + 3);
		}
		y--;
		drawEllipseScanLines( x, y );
	}

	ColorPalette::White.Set();
}

// uses fillEllipse() to fill a circle
void fillCircle( const Vertex2i& center, int radius, const Color3f& color)
{
	fillEllipse( center, radius, radius, color );
}

// use scan line algorithm and edge coherence to fill polygon from its sequential vertices
void fillPolygon( const vector<Vertex2i>& vertices, const Color3f& color )
{
	if ( vertices.size() < 3 ) return;
	color.Set();

	EdgeTable ET;
	EdgeList AEL;

	// find the highest and lowest point on the polygon
	int yMin = vertices[0].Y;
	int yMax = vertices[0].Y;
	for ( unsigned int i = 1; i < vertices.size(); i++ )
	{
		yMax = vertices[i].Y > yMax ? vertices[i].Y : yMax;
		yMin = vertices[i].Y < yMin ? vertices[i].Y : yMin;
	}

	ET.resize( yMax - yMin );
	// fill the edge table
	for ( unsigned int j = 0; j < vertices.size(); j++ )
	{
		const Vertex2i& A = vertices[j];
		const Vertex2i& B = (j == 0) ? vertices[vertices.size() - 1] : vertices[j - 1];
		const Vertex2i& C = (j == (vertices.size() - 1)) ? vertices[0] : vertices[j + 1];

		// check see if A is a ymin and add it to edge table if so
		auto addEdgeIfYmin = [&ET, yMin](const Vertex2i& A, const Vertex2i& B)
		{
			if ( A.Y < B.Y )
			{
				float invM = (static_cast<float>(B.X) - A.X) / (static_cast<float>(B.Y) - A.Y);
				ET[A.Y - yMin].addEdge( Edge{B.Y, static_cast<float>(A.X), invM} );
			}
		};

		addEdgeIfYmin( A, B );
		addEdgeIfYmin( A, C );
	}

	// draw scan lines from bottom of polygon to the top to fill
	for( int y = yMin; y < yMax; y++)
	{
		// add any new edges from edge table
		for(const Edge& E : ET[y - yMin])
		{
			AEL.addEdge( E );
		}
		// run through all the active edge pairs and draw scan lines
		for ( auto it = AEL.begin(); it != AEL.end() && next( it ) != AEL.end(); ++it )
		{
			// find first unexpired edge
			while ( it->ymax == y && (it != AEL.end() && next( it ) != AEL.end()) )
			{
				it = AEL.erase( it );
			}
			if ( it == AEL.end() || next( it ) == AEL.end() ) break;
			Edge& e1 = *it;
			++it;
			// find second unexpired edge
			while ( it->ymax == y && it != AEL.end() )
			{
				it = AEL.erase( it );
			}
			if ( it == AEL.end() ) break;
			Edge& e2 = *it;

			// draw scan line between the two active edges
			int x1 = floorf(e1.xmin);
			int x2 = floorf(e2.xmin);
			while(x1 < x2)
			{
				glVertex2f( x1++, y );
			}

			// increment the x values by inverted slope; for the next scanline
			e1.xmin += e1.invM;
			e2.xmin += e2.invM;
		}
	}

	ColorPalette::White.Set();
}


/******************************************************************/

void display( void )
{
/* clear the screen to the clear color */
	glClear( GL_COLOR_BUFFER_BIT );


	// Draw slide 1 : My name ("ETIENNE")
	if ( currentSlide == 1 )
	{
		ColorPalette::Green.Set();
		glBegin( GL_POINTS );
		Vertex2i cursorPos{15, 500};
		drawString( "ETIENNE", cursorPos );

		glEnd();
		ColorPalette::White.Set();
		output( 200, 150 );// "ETIENNE"
	}

	// Draw slide 2 : Robot
	if ( currentSlide == 2 )
	{
		ColorPalette::White.Set();
		glBegin( GL_POINTS );
		// head
		drawPolygon( {Vertex2i{200, 100},
					 Vertex2i{200, 600},
					 Vertex2i{600, 600},
					 Vertex2i{600, 100}} );
		// left ear
		drawPolygon( {Vertex2i{200, 350},
					 Vertex2i{200, 500},
					 Vertex2i{150, 500},
					 Vertex2i{150, 350}} );
		// right ear
		drawPolygon( {Vertex2i{600, 350},
					 Vertex2i{600, 500},
					 Vertex2i{650, 500},
					 Vertex2i{650, 350}} );
		// nose
		drawPolygon( {Vertex2i{400, 400},
					 Vertex2i{350, 300},
					 Vertex2i{450, 300}} );
		// left horn
		drawPolygon( {Vertex2i{300, 600},
					 Vertex2i{200, 600},
					 Vertex2i{250, 700}} );
		// right horn
		drawPolygon( {Vertex2i{500, 600},
					 Vertex2i{600, 600},
					 Vertex2i{550, 700}} );
		// left eye
		drawCircle( {300, 475}, 40 );
		// right eye
		drawCircle( {500, 475}, 40 );
		// mouth
		drawEllipse( {400, 200}, 75, 25 );

		glEnd();
		ColorPalette::White.Set();
		output( 200, 150 );// Robot
	}

	// Draw slide 3 : Colorful robot
	if ( currentSlide == 3 )
	{
		glBegin( GL_POINTS );
		// head
		fillPolygon( {Vertex2i{200, 100},
					 Vertex2i{200, 600},
					 Vertex2i{600, 600},
					 Vertex2i{600, 100}},
					 ColorPalette::LightBlue );
		// right ear
		fillPolygon( {Vertex2i{600, 350},
					 Vertex2i{600, 500},
					 Vertex2i{650, 500},
					 Vertex2i{650, 350}},
					 ColorPalette::Green );
		// left ear
		fillPolygon( {Vertex2i{200, 350},
					 Vertex2i{200, 500},
					 Vertex2i{150, 500},
					 Vertex2i{150, 350}},
					 ColorPalette::Green );
		// nose
		fillPolygon( {Vertex2i{400, 400},
					 Vertex2i{350, 300},
					 Vertex2i{450, 300}},
					 ColorPalette::Orange );
		// right horn
		fillPolygon( {Vertex2i{500, 600},
					 Vertex2i{600, 600},
					 Vertex2i{550, 700}},
					 ColorPalette::Red );
		// left horn
		fillPolygon( {Vertex2i{300, 600},
					 Vertex2i{200, 600},
					 Vertex2i{250, 700}},
					 ColorPalette::Red );
		// left eye
		fillCircle( {300, 475}, 40, ColorPalette::Black );
		// right eye
		fillCircle( {500, 475}, 40, ColorPalette::Black );
		// mouth
		fillEllipse( {400, 200}, 75, 25, ColorPalette::Yellow );

		glEnd();
		ColorPalette::White.Set();
		output( 200, 150 );// colorful robot
	}

	/* swap buffers */
	glutSwapBuffers();
}

void reshape( int w, int h )
{
/* set the viewport */
	glViewport( 0, 0, (GLsizei)w, (GLsizei)h );  // lower-left corner

	/* Matrix for projection transformation */
	glMatrixMode( GL_PROJECTION );

	/* replaces the current matrix with the identity matrix */
	glLoadIdentity();

	/* Define a 2d orthographic projection matrix */
	gluOrtho2D( 0.0, (GLdouble)w, 0.0, (GLdouble)h );  // left, right, bottom, top
}

/*******************************************************************/

int main( int argc, char** argv )
{

/* deal with any GLUT command Line options */
	glutInit( &argc, argv );

	/* create an output window */
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( windowWidth, windowHeight );

	/* set the name of the window and try to create it */
	glutCreateWindow( "CS 425 - Assignment 1" );

	/* specify clear values for the color buffers */
	glClearColor( 0.0, 0.0, 0.0, 1.0 );

	/* Receive keyboard inputs */
	glutKeyboardFunc( Keyboard );

	/* assign the display function */
	glutDisplayFunc( display );

	/* assign the idle function */
	glutIdleFunc( display );

	/* sets the reshape callback for the current window */
	glutReshapeFunc( reshape );

	/* enters the GLUT event processing loop */
	glutMainLoop();

	return (EXIT_SUCCESS);
}