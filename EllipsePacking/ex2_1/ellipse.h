#pragma once
#include <qpainter.h>
#include "polygon.h"
#include "reference.h"
class Ellipse
{
public:
	QPointF center;
	QPointF velocity;
	float long_axis;
	float short_axis;
	int counter;
	float ratio_long;
	float ratio_short;

public:
	Ellipse(void){};
	~Ellipse(void){};
public:
	void Generate(Polygon *object, int flag);
	void Draw( QPainter &paint );
	void Move();
	double Distance(Ellipse *E, double &slope, QPointF &point);	//find the nearest distance between two ellipse
	double Distance();				//find the neasst distance from the origin to this ellipse
	double intersection(double x0, double y0, double a, double b, double k, QPointF &point);
	// find the intersection points of (x-x0)^2/a^2 + (y-y0)^2/b^2=1 and y=kx return the min-norm of these intersections.
	bool Detect(Ellipse *E);		//detect whether twe ellipses are intersection
	bool Detect(QPointF start, QPointF end);	//dectect whether this ellipse intersect with the boundary line
	bool Is_In(Ellipse *E);			//detect the center of this ellipse is in the given ellipse
};