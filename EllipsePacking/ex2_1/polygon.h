#pragma once
#include <qpainter.h>
#include <vector>
using namespace std;

class Polygon
{
public:
	vector<QPointF>		points;                   // vertexs
	bool		finish;
	int			selected;
	int			min_x,max_x,min_y,max_y;
	bool**		is_inside_;

public:
	Polygon(void);
	~Polygon(void);

public:	
	// basic operations
	void AddPoint( QPoint &point );    // add new vertex
	void ChangeLastPoint( QPoint &point );      // change the last point
	void SelectPoint( QPoint &point );		 // select point
	void ChangeSelectedPoint( QPoint &point );      // change the seltected point
	void Draw( QPainter &paint, bool solid );       // draw polygon on screen
	void CalSize();						// calculate size
	// calculate interior
	void ScanLine( int y );
	void CalInterior();
};
