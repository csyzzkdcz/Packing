#include"ellipse.h"
#include"circle.h"
void Ellipse::Draw(QPainter &paint)
{
	paint.setPen(Qt::red);
	paint.setBrush(Qt::lightGray);
	paint.drawEllipse( center,short_axis,long_axis );
}

void Ellipse::Generate( Polygon* object, int flag)
{
	int x,y,width,height;
	object->CalInterior();
	width = object->max_x - object->min_x;
	height = object->max_y - object->min_y;
	do{
		x = rand()%width;
		y = rand()%height;
	}while(!object->is_inside_[x][y]);
	//x=100+flag;
	//y=10;
	/*x=100;
	y=100+10*flag;*/
	center.setX(object->min_x+x);
	center.setY(object->min_y+y);
	do{
		x = rand()%(2*MAX_VELOCITY) - MAX_VELOCITY;
		y = rand()%(2*MAX_VELOCITY) - MAX_VELOCITY;
	}while( x*x+y*y > MAX_VELOCITY*MAX_VELOCITY );
	velocity.setX(x);
	velocity.setY(y);
	ratio_short = LEAST_RATIO + 0.2 * ( rand()%(5*(MAX_RATIO-LEAST_RATIO)) );
	ratio_long = ratio_short * (MIN_LONG_TO_SHORT + rand()%(MAX_LONG_TO_SHORT-MIN_LONG_TO_SHORT));
	short_axis = MIN_SHORT_AXIS;
	long_axis = ratio_long / ratio_short * short_axis;
	/*long_axis = 100;
	short_axis = ratio_short / ratio_long * long_axis;*/
}

void Ellipse::Move()
{
	center.setX( center.rx() + velocity.rx() * UNIT_INTERVAL );
	center.setY( center.ry() + velocity.ry() * UNIT_INTERVAL );
	if( counter <= JAMMED_TIME )
	{
		long_axis += ratio_long * UNIT_INTERVAL;
		short_axis += ratio_short * UNIT_INTERVAL;
	}
}

double Ellipse::intersection(double x0, double y0, double a, double b, double k, QPointF &point)
{
	double dist1,dist2;
	double dist;
	double x1,x2,y1,y2;
	if(k==INFINITE)
	{
		x1 = 0;
		x2 = 0;
		y1 = y0 + b * sqrt(1 - pow(x0/a , 2.0));
		y2 = y0 - b * sqrt(1 - pow(x0/a , 2.0));
	}
	else
	{
		double delta = pow( b*b*x0+a*a*k*y0 ,2.0) - (b*b*x0*x0 + a*a*y0*y0 - a*a*b*b)* (b*b+a*a*k*k);
		if(abs(delta) < PRICISE_TOL)
			delta = 0;
		x1 = ( ( b*b*x0 + a*a*k*y0 ) + sqrt( delta ) )/(b*b + a*a*k*k);
		x2 = ( ( b*b*x0 + a*a*k*y0 ) - sqrt( delta ) )/(b*b + a*a*k*k);
		y1 = k*x1;
		y2 = k*x2;
	}
	dist1 = sqrt(x1*x1 + y1*y1);
	dist2 = sqrt(x2*x2 + y2*y2);
	if(dist1>dist2)
	{
		point.setX(x2);
		point.setY(y2);
		dist=dist2;
	}
	else
	{
		point.setX(x1);
		point.setY(y1);
		dist=dist1;
	}
	return dist ; 
}

bool Ellipse::Is_In(Ellipse *E)
{
	double temp = pow( (center.rx() - E->center.rx())/E->long_axis, 2.0 ) + pow( (center.ry() - E->center.ry())/E->short_axis , 2.0);
	if(temp - 1 < -PRICISE_TOL)
		return true;
	else
		return false;
}

double Ellipse::Distance(Ellipse *E, double &slope,QPointF &point)
{
	double dist=0, dist_1=0, slope_1, delta_k;
	double center_x_after = (E->center.rx() - center.rx() )/ short_axis;
	double center_y_after = (E->center.ry() - center.ry() )/ long_axis;
	double long_axis_after = E->long_axis/long_axis;
	double short_axis_after = E->short_axis/short_axis;
	double above_point = center_y_after + long_axis_after;
	double down_point = center_y_after - long_axis_after;
	double k2,k1,p1,p2,k2_delta;
	double dist_k1, dist_k2, dist_p1, dist_p2,dist_k2_delta;
	if(abs(center_x_after)<= EPSILON / long_axis)
	{
		slope = INFINITE;
		double d1 = abs(above_point);
		double d2 = abs(down_point);
		if(d1 < d2)
		{
			point.setX(center_x_after);
			point.setY(above_point);
			return d1;
		}
		else
		{
			point.setX(center_x_after);
			point.setY(down_point);
			return d2;
		}
	}
	else
	{
		double tempt_delta, A, B, C;
		A = short_axis_after * short_axis_after - center_x_after * center_x_after;
		B = 2 * center_x_after * center_y_after;
		C = long_axis_after * long_axis_after - center_y_after * center_y_after;
		tempt_delta = B*B - 4*A*C;
		k1 = (-B + sqrt(tempt_delta))/(2*A);
		k2 = (-B - sqrt(tempt_delta))/(2*A);
		if(k1 < k2)
		{
			double temp=k1;
			k1=k2;
			k2=temp;
		}
		if(k2 > 0 || k1 < 0 || down_point * above_point < 0)
		{
			p2 = k1 - DIVIDE_RATIO * (k1 - k2);
			p1 = k2 + DIVIDE_RATIO * (k1 - k2);
		
			dist_k1 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, k1, point);
			dist_k2 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, k2, point);
			dist_p1 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, p1, point);
			dist_p2 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, p2, point);
			for(;abs(dist_k1-dist_k2) > EPSILON_SLOPE || abs(k2-k1) > INTERVAL_TOL;)
			{
				if(dist_k2 >= dist_p2 && dist_p2 > dist_p1 || dist_p2 >= dist_k2 && dist_k2 >= dist_p1 || dist_p2 >= dist_p1 && dist_p1 >= dist_k2)
					k2=p2;
				else if(dist_p1 >= dist_k2 && dist_k2 >= dist_p2 || dist_k2 >= dist_p1 && dist_p1 >= dist_p2)
					k1=p1;
				else
				{
					/*delta_k = (k1-k2)/SLOPE_NUMBER > DELTA_K ? DELTA_K : (k1-k2)/SLOPE_NUMBER;
					k2_delta = k2 + delta_k;
					dist_k2_delta = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, k2_delta, point);
					if(dist_k2_delta > dist_k2)
						k2=p2;
					else
						k1=p2;*/
					k1=p2;
				}
				p2 = k1 - DIVIDE_RATIO * (k1 - k2);
				p1 = k2 + DIVIDE_RATIO * (k1 - k2);
				dist_k1 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, k1, point);
				dist_k2 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, k2, point);
				dist_p1 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, p1, point);
				dist_p2 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, p2, point);

			}
			dist = (dist_k1+dist_k2)/2;
			slope=(k1+k2)/2;
		}
		else
		{
			double temp_1 = k1;
			double temp_2 = k2;
			//[-INF, k2]
			k1 = temp_2;
			k2 = -INFINITE;
			p2 = k1 - DIVIDE_RATIO * (k1 - k2);
			p1 = k2 + DIVIDE_RATIO * (k1 - k2);
			dist_k1 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, k1, point);
			dist_k2 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, k2, point);
			dist_p1 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, p1, point);
			dist_p2 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, p2, point);
			for(;abs(dist_k1-dist_k2) > EPSILON_SLOPE || abs(k2-k1) > INTERVAL_TOL;)
			{
				if(dist_k2 >= dist_p2 && dist_p2 > dist_p1 || dist_p2 >= dist_k2 && dist_k2 >= dist_p1)
					k2=p2;
				else if(dist_p1 >= dist_k2 && dist_k2 >= dist_p2 || dist_k2 >= dist_p1 && dist_p1 >= dist_p2)
					k1=p1;
				else
				{
					delta_k = (k1-k2)/SLOPE_NUMBER > DELTA_K ? DELTA_K : (k1-k2)/SLOPE_NUMBER;
					k2_delta = k2 + delta_k;
					dist_k2_delta = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, k2_delta, point);
					if(dist_k2_delta > dist_k2)
						k1=p2;
					else
						k2=p1;
				}
				p2 = k1 - DIVIDE_RATIO * (k1 - k2);
				p1 = k2 + DIVIDE_RATIO * (k1 - k2);
				dist_k1 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, k1, point);
				dist_k2 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, k2, point);
				dist_p1 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, p1, point);
				dist_p2 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, p2, point);

			}
			dist = (dist_k1+dist_k2)/2;
			slope=(k1+k2)/2;

			//[k1, INF]
			k2 = temp_1;
			k1 = INFINITE;
			p2 = k1 - DIVIDE_RATIO * (k1 - k2);
			p1 = k2 + DIVIDE_RATIO * (k1 - k2);
			dist_k1 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, k1, point);
			dist_k2 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, k2, point);
			dist_p1 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, p1, point);
			dist_p2 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, p2, point);
			for(;abs(dist_k1-dist_k2) > EPSILON_SLOPE || abs(k2-k1) > INTERVAL_TOL;)
			{
				if(dist_k2 >= dist_p2 && dist_p2 > dist_p1 || dist_p2 >= dist_k2 && dist_k2 >= dist_p1)
					k2=p2;
				else if(dist_p1 >= dist_k2 && dist_k2 >= dist_p2 || dist_k2 >= dist_p1 && dist_p1 >= dist_p2)
					k1=p1;
				else
				{
					delta_k = (k1-k2)/SLOPE_NUMBER > DELTA_K ? DELTA_K : (k1-k2)/SLOPE_NUMBER;
					k2_delta = k2 + delta_k;
					dist_k2_delta = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, k2_delta, point);
					if(dist_k2_delta < dist_k2)
						k1=p2;
					else
						k2=p1;
				}
				p2 = k1 - DIVIDE_RATIO * (k1 - k2);
				p1 = k2 + DIVIDE_RATIO * (k1 - k2);
				dist_k1 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, k1, point);
				dist_k2 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, k2, point);
				dist_p1 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, p1, point);
				dist_p2 = intersection(center_x_after, center_y_after, short_axis_after, long_axis_after, p2, point);

			}
			dist_1 = (dist_k1+dist_k2)/2;
			slope_1 =(k1+k2)/2;

			if(dist_1 < dist)
			{
				dist = dist_1;
				slope = slope_1;
			}
		}
		return dist;
	}
}

bool Ellipse::Detect(Ellipse *E)
{
	double m;
	double vn,vt,ve,vx,vy,ux,uy;
	double d_x, d_y;
	double s;
	int sgn;
	QPointF intersection_point;
	double dist,dist_1,k_slope,ratio_after;
	dist = Distance(E, k_slope,intersection_point);
	m = E->long_axis * E->short_axis / (long_axis * short_axis);
	ux = (1.0/(1.0 + m) * velocity.rx() + m/(1.0 + m) * E->velocity.rx())/short_axis;
	uy = (1.0/(1.0 + m) * velocity.ry() + m/(1.0 + m) * E->velocity.ry())/long_axis;
	ratio_after =E-> ratio_long / long_axis > E->ratio_short / short_axis ? E-> ratio_long / long_axis : E->ratio_short / short_axis;
	if(k_slope==INFINITE)
	{
		sgn = E->center.ry()<center.ry() ? -1 : 1;
		vn = sgn * (velocity.ry() - E->velocity.ry())/long_axis;
		vt = (velocity.rx() - E->velocity.rx())/short_axis;
		ve = (ratio_long+E->ratio_long)/long_axis;
		vx = vt;
		vy = -sgn * ( DECAY*(vn+ve)+ve );
	}
	else
	{
		sgn = E->center.rx()<center.rx() ? -1 : 1;
		vx = (velocity.rx() - E->velocity.rx())/short_axis;
		vy = (velocity.ry() - E->velocity.ry())/long_axis; 
		vn = sgn * (vx+k_slope*vy) / sqrt(k_slope*k_slope+1);
		vt = (-k_slope*vx+vy) / sqrt(k_slope*k_slope+1);
		ve = (ratio_long/long_axis + ratio_after);
		vx = -( sgn * ( DECAY*(vn+ve)+ve ) + k_slope*vt ) / sqrt(k_slope*k_slope+1);
		vy = ( -k_slope*sgn * ( DECAY*(vn+ve)+ve ) + vt ) / sqrt(k_slope*k_slope+1);
	}
	s=1-dist;
	if(k_slope != INFINITE)
	{
		d_y = (s * abs(k_slope) / sqrt(1 + k_slope * k_slope)) * long_axis;
		d_x = (s * 1 / sqrt(1 + k_slope * k_slope)) * short_axis;
	}
	else
	{
		d_x = 0;
		d_y = s * long_axis;
	}
	//if(EPSILON >= sqrt(d_x * d_x + d_y * d_y ) )	//collision happens
	if(s>=-EPSILON)
	{
		counter++;
		E->counter++;
	}
	if( s>0 && vn+ve>0 )
	{
		double k = m/(1 + m);
		velocity.setX( (ux + k*vx) * short_axis );
		velocity.setY( (uy + k*vy) * long_axis );
		E->velocity.setX( (ux - (1-k)*vx) * short_axis );
		E->velocity.setY( (uy - (1-k)*vy) * long_axis );

		//if(s> EPSILON / long_axis)
		if(sqrt(d_x * d_x + d_y * d_y ) > EPSILON)
		{
			double d = sqrt( d_x*d_x / (E->short_axis * E->short_axis)+d_y*d_y / (E->long_axis * E->long_axis));
			//if (m < 1 && E->short_axis * (1-d) >= MIN_SHORT_AXIS_COLLISION || m > 1 && short_axis * dist < MIN_SHORT_AXIS_COLLISION )
			if(m > 1)
			{
				E->long_axis = E->long_axis * (1-d);
				E->short_axis = E->short_axis * (1-d);
			}

			else
			{
				long_axis = long_axis * dist;
				short_axis = short_axis *dist;
			}
		}
		return true;
	}
	else
		return false;
}

bool Ellipse::Detect(QPointF start, QPointF end)
{
	Circle *P;
	P=new Circle;
	P->radius=1;
	P->center.setX(0);
	P->center.setY(0);
	P->velocity.setX(velocity.rx()/short_axis);
	P->velocity.setY(velocity.ry()/long_axis);
	P->counter=counter;
	P->ratio = ratio_long/long_axis;
	start.setX((start.rx()-center.rx())/short_axis);
	start.setY((start.ry()-center.ry())/long_axis);
	end.setX((end.rx()-center.rx())/short_axis);
	end.setY((end.ry()-center.ry())/long_axis);
	bool bv=P->Detect(start, end , long_axis);
	if(bv)
	{
		counter=P->counter;
		center.setX(short_axis * P->center.rx() + center.rx());
		center.setY(long_axis * P->center.ry() + center.ry());
		velocity.setX(short_axis * P->velocity.rx());
		velocity.setY(long_axis * P->velocity.ry());
		long_axis = long_axis * P->radius;
		short_axis = short_axis * P->radius;
	}
	return bv;
}


