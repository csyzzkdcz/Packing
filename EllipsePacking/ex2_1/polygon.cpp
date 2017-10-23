#include "polygon.h"
#include <cmath>
#define POINT_SIZE 3
#define INFINITY 5000

Polygon::Polygon()
{
	finish = false;
	selected = INFINITY;
	min_x = min_y = INFINITY;
	max_x = max_y = -1;
}

Polygon::~Polygon()
{
}

void Polygon::AddPoint( QPoint &point )
{
	QPointF new_point;
	new_point.setX(point.rx());
	new_point.setY(point.ry());
	points.push_back(new_point);
}

void Polygon::ChangeLastPoint( QPoint &point )
{
	QPointF new_point;
	points.pop_back();
	new_point.setX(point.rx());
	new_point.setY(point.ry());
	points.push_back(new_point);
}

void Polygon::SelectPoint( QPoint &point )
{
	int i;
	float distance;
	for( i=0; i<points.size(); i++ )
	{
		distance = sqrt(pow(float(point.rx()-points[i].rx()),2)+pow(float(point.ry()-points[i].ry()),2));
		if( distance<3*POINT_SIZE )
			selected = i;
	}
}

void Polygon::ChangeSelectedPoint( QPoint &point )
{
	if( selected<INFINITY )
	{
		points[selected].setX(point.rx());
		points[selected].setY(point.ry());
	}
	CalInterior();
}

void Polygon::Draw( QPainter &paint, bool solid )
{
	int i,x,y;
	if( finish && solid )      
	{
		paint.setPen( Qt::yellow );
		for( x=min_x; x<=max_x; x++ )
			for( y=min_y; y<=max_y; y++ )
				if( is_inside_[x-min_x][y-min_y] )
					paint.drawPoint(x,y);
	}
	paint.setPen( QPen(Qt::blue,3) );
	for( i=1;i<points.size();i++ )
		paint.drawLine( points[i-1], points[i] );
	if( finish ) 
		paint.drawLine( points.back(), points.front() );
	paint.setPen( QPen(Qt::red,8) );
	for( i=0;i<points.size();i++ )
		paint.drawPoint( points[i] );
}

void Polygon::CalSize()
{
	for( int i=0; i<points.size(); i++ )
	{
		if( points[i].rx()<min_x )
			min_x = points[i].rx();
		if( points[i].rx()>max_x )
			max_x = points[i].rx();
		if( points[i].ry()<min_y )
			min_y = points[i].ry();
		if( points[i].ry()>max_y )
			max_y = points[i].ry();
	}
}

void Polygon::ScanLine( int y )
{
	int i,j,n,x;
	float s;
	vector<int> intersections;
	bool status;
	n = points.size();
	for( i=0; i<n; i++ )
		if( points[i].ry() < y )
		{
			if( points[(i+1)%n].ry() > y )
			{
				s = (points[(i+1)%n].ry()-y) / (float)(points[(i+1)%n].ry()-points[i].ry());
				x = s*points[i].rx() + (1-s)*points[(i+1)%n].rx();
				intersections.push_back(x);
			}
			else
			{
				j=1;
				while( points[(i+j)%n].ry() == y )
					j++;
				if( points[(i+j)%n].ry() > y )
					intersections.push_back(points[(i+j-1)%n].rx());
			}
		}
		else if( points[i].ry() > y )
		{
			if( points[(i+1)%n].ry() < y )
			{
				s = (points[(i+1)%n].ry()-y) / (float)(points[(i+1)%n].ry()-points[i].ry());
				x = s*points[i].rx() + (1-s)*points[(i+1)%n].rx();
				intersections.push_back(x);
			}
			else
			{
				j=1;
				while( points[(i+j)%n].ry() == y )
					j++;
				if( points[(i+j)%n].ry() < y )
					intersections.push_back(points[(i+j-1)%n].rx());
			}
		}

	sort(intersections.begin(),intersections.end());
	n = intersections.size();
	j = min_x-1;
	status = false;
	for( i=0; i<n; i++ )
	{
		for( x=j+1; x<intersections[i]; x++ )
			is_inside_[x-min_x][y-min_y] = status;
		is_inside_[x-min_x][y-min_y] = false;
		j = intersections[i];
		status = !status;
	}
	for( x=j+1; x<=max_x; x++ )
		is_inside_[x-min_x][y-min_y] = status;
}

void Polygon::CalInterior()
{
	int width,height,i,j;
	CalSize();
	width = max_x - min_x + 1;
	height = max_y - min_y + 1;

	// malloc
	is_inside_ = ( bool** )malloc( width * sizeof(bool*) );
	for( i=0; i<width; i++ )
		is_inside_[i] = ( bool* )malloc( height * sizeof(bool) );

	// scanline
	for( j=0; j<height; j++ )
		ScanLine(min_y+j);
}
