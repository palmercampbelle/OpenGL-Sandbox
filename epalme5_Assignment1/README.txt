README Project 1
CS425 FA2018
Etienne Palmer-Campbell epalme5
Sept 25th 2018
*******************************************************************

---SLIDE 1:
I created a function for drawing lines called drawLine() that uses 
the midpoint algorithm for scan conversion. The calculation assumes that
the slope of the line is between 0 and 1, so there is a variety of special
cases designed to transform the line temporarily for scan conversion.

Then I created a function for drawing character strings onto the screen, 
but only partially implemented the characters it could draw to conserve time.
This used my drawLine function.

---SLIDE 2:
I created a drawPolygon, drawCircle, and drawEllipse function in order to 
draw the robot.

-drawPolygon : uses drawLine to connect a series of points, forming a polygon
-drawCircle : uses the midpoint algorithm for scan converting 1/8 of a circle,
and then copies that portion 8 times to complete the circle.
-drawEllipse : uses the midpoint algorithm for scan converting 2 symmetrical
regions of an ellipse.

---SLIDE 3:

I created a fillPolygon, fillCircle, fillEllipse function to draw filled in
shapes for the robot to have color

-fillPolygon : uses edge coherence and a scan-line algorithm to fill any polygon
with a specified color, using a list of sequential adjacent vertices
-fillEllipse : is largely identical to drawEllipse, only instead of plotting 
four points it instead uses those 4 points to draw two scan lines. So by proxy 
it uses midpoint algorithm for scan converting ellipse.
-fillCircle : is just a helper function that uses fillEllipse to draw a circle,
it does not employ any more efficient algorithm of its own or take advantage
of any special case optimizations.

---Utility code
-I made several data structures for the creation and use of Edge Tables and Active Edge Lists,
in order to make it easier and more readable to use edge coherence.
	- Edge : holds data regarding active edges
	- EdgeList : holds list of active edges, and sorts them as they are added ( O(n) )
	- EdgeTable : holds directory of edges corresponding to their ymin
-The Color structures are for convenience.