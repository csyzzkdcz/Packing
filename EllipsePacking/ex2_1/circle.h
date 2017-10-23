#pragma once
#include <qpainter.h>
using namespace std;
#include "polygon.h"
#include "reference.h"

class Circle
{
public:
	QPointF center;
	float radius;
	QPointF velocity;
	float ratio;
	int counter;

public:
	Circle (void) {};
	~Circle (void) {};

public:
	void Draw( QPainter &paint )
	{
		paint.setPen(Qt::red);
		paint.setBrush(Qt::lightGray);
		paint.drawEllipse( center,radius,radius );
	}

	void Generate( Polygon* object )
	{
		int x,y,width,height;
		object->CalInterior();
		width = object->max_x - object->min_x;
		height = object->max_y - object->min_y;
		do{
			x = rand()%width;
			y = rand()%height;
		}while(!object->is_inside_[x][y]);
		center.setX(object->min_x+x);
		center.setY(object->min_y+y);
		radius = 0;
		do{
			x = rand()%(2*MAX_VELOCITY) - MAX_VELOCITY;
			y = rand()%(2*MAX_VELOCITY) - MAX_VELOCITY;
		}while( x*x+y*y > MAX_VELOCITY*MAX_VELOCITY );
		velocity.setX(x);
		velocity.setY(y);
		ratio = LEAST_RATIO + 0.2 * ( rand()%(5*(MAX_RATIO-LEAST_RATIO)) );
		//ratio = 10;
	}

	void Move()
	{
		center.setX( center.rx() + velocity.rx() * UNIT_INTERVAL );
		center.setY( center.ry() + velocity.ry() * UNIT_INTERVAL );
		if( counter <= JAMMED_TIME )
			radius += ratio * UNIT_INTERVAL;
	}

	bool Detect( Circle* P )
	{
		float dx,dy,d,k,s;
		float m1,m2;				// ball mass
		float vx,vy,vn,vt,ve;		// relative velocity
		float ux,uy;				// total velocity
		int sgn;
		dx = P->center.rx() - center.rx();
		dy = P->center.ry() - center.ry();
		d = P->radius + radius;
		ux = (velocity.rx() + P->velocity.rx()) / 2;
		uy = (velocity.ry() + P->velocity.ry()) / 2;
		m1 = radius*radius;
		m2 = P->radius*P->radius;
		if( dx==0 )
		{
			sgn = P->center.ry()<center.ry() ? -1 : 1;
			vn = sgn * (velocity.ry() - P->velocity.ry());
			vt = velocity.rx() - P->velocity.rx();
			ve = (ratio+P->ratio);
			vx = vt;
			vy = -sgn * ( DECAY*(vn+ve)+ve );
		}
		else
		{
			k = (float)dy / dx;
			sgn = P->center.rx()<center.rx() ? -1 : 1;
			vx = velocity.rx() - P->velocity.rx();
			vy = velocity.ry() - P->velocity.ry(); 
			vn = sgn * (vx+k*vy) / sqrt(k*k+1);
			vt = (-k*vx+vy) / sqrt(k*k+1);
			ve = (ratio+P->ratio);
			vx = -( sgn * ( DECAY*(vn+ve)+ve ) + k*vt ) / sqrt(k*k+1);
			vy = ( -k*sgn * ( DECAY*(vn+ve)+ve ) + vt ) / sqrt(k*k+1);
		}

		s = d - sqrt(dx*dx+dy*dy);
		if( s>=EPSILON )
		{
			if( radius>P->radius )
				radius -= s;
			else
				P->radius -= s;
		}
		if( s>=-EPSILON )
		{
			counter++;
			P->counter++;
		}
		if( s>0 && vn+ve>0 )
		{
			k = m2/(m1+m2);
			velocity.setX( ux + k*vx );
			velocity.setY( uy + k*vy );
			P->velocity.setX( ux - (1-k)*vx );
			P->velocity.setY( uy - (1-k)*vy );
			return true;
		}
		else
			return false;
	}

	bool Detect( QPointF start, QPointF end , double max_ratio)
	{
		float k,b,t,d,s;
		int sgn;
		float vn,vt,vx,vy;
		if( start.rx()==end.rx() )
		{
			d = abs(center.rx()-start.rx());
			sgn = center.rx()<start.rx() ? -1 : 1;
			vn = -sgn*velocity.rx();
			vt = velocity.ry();
			vx = sgn * ( DECAY*(vn+ratio)+ratio );
			vy = vt;
		}
		else
		{
			k = (end.ry()-start.ry()) / (end.rx()-start.rx());
			b = center.ry() - ( start.ry()+k*(center.rx()-start.rx()) );
			t = ( (center.rx()-start.rx())*(end.rx()-start.rx()) + (center.ry()-start.ry())*(end.ry()-start.ry()) ) / ( (end.rx()-start.rx())*(end.rx()-start.rx()) + (end.ry()-start.ry())*(end.ry()-start.ry()) );
			if( t<0 && center.rx()!=start.rx() )
			{
				k = (center.ry()-start.ry()) / (center.rx()-start.rx());
				d = sqrt( (center.rx()-start.rx()) * (center.rx()-start.rx()) + (center.ry()-start.ry()) * (center.ry()-start.ry()) );
				sgn = center.rx()<start.rx() ? -1 : 1;
				vn = -sgn * ( velocity.rx() + k*velocity.ry() ) / sqrt(k*k+1);
				vt = ( -k*velocity.rx() + velocity.ry() ) / sqrt(k*k+1);
				vx = ( sgn * ( DECAY*(vn+ratio)+ratio ) - k*vt ) / sqrt(k*k+1);
				vy = ( k*sgn * ( DECAY*(vn+ratio)+ratio ) + vt ) / sqrt(k*k+1);
			}
			else if( t>1 && center.rx()!=end.rx() )
			{
				k = (center.ry()-end.ry()) / (center.rx()-end.rx());
				d = sqrt( (center.rx()-end.rx()) * (center.rx()-end.rx()) + (center.ry()-end.ry()) * (center.ry()-end.ry()) );
				sgn = center.rx()<end.rx() ? -1 : 1;
				vn = -sgn * ( velocity.rx() + k*velocity.ry() ) / sqrt(k*k+1);
				vt = ( -k*velocity.rx() + velocity.ry() ) / sqrt(k*k+1);
				vx = ( sgn * ( DECAY*(vn+ratio)+ratio ) - k*vt ) / sqrt(k*k+1);
				vy = ( k*sgn * ( DECAY*(vn+ratio)+ratio ) + vt ) / sqrt(k*k+1);
			}
			else
			{
				d = abs(b) / sqrt(k*k+1);
				sgn = b<0 ? -1 : 1;
				vn = sgn * ( k*velocity.rx() - velocity.ry() ) / sqrt(k*k+1);
				vt = ( velocity.rx() + k*velocity.ry() ) / sqrt(k*k+1);
				vx = ( -k*sgn * ( DECAY*(vn+ratio)+ratio ) + vt ) / sqrt(k*k+1);
				vy = ( sgn * ( DECAY*(vn+ratio)+ratio ) + k*vt ) / sqrt(k*k+1);
			}
		}

		s = radius - d;
		if( s>=EPSILON*1.0 / max_ratio)
		{
			radius -= s;
		}

		if( s>=-EPSILON*1.0 / max_ratio )
			counter++;
		if( s>0 && vn+ratio>0 )
		{
			//velocity.setX(vx);
			//velocity.setY(vy);
			double vec_=sqrt(vx*vx+vy*vy);
			if(vec_>MAX_MOVING_VEC / max_ratio)
			{
				velocity.setX(radius*vx/vec_);
				velocity.setY(radius*vy/vec_);
			}
			else
			{
				velocity.setX(vx);
				velocity.setY(vy);

			}
			//counter++;
			return true;
		}
		else
			return false;
	}
};