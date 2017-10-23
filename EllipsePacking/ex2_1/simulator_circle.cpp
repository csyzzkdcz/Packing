#include "simulator_circle.h"
#include <cmath>

Simulator_Circle::Simulator_Circle(void)
{
}

Simulator_Circle::Simulator_Circle( Polygon* new_object )
{
	object = new_object;
}

Simulator_Circle::~Simulator_Circle(void)
{
}

void Simulator_Circle::Start()
{
	Circle* new_particle;
	//Ellipse* new_particle;
	int i;
	for( i=0; i<PARTICLE_NUM; i++ )
	{
		new_particle = new Circle();
		//new_particle = new Ellipse();
		new_particle->Generate(object);
		particles.push_back(new_particle);
	}
	time = 0;
	finish = false;
}

void Simulator_Circle::Update()
{
	int i;
	if( Jammed() )
		finish = true;
	else if(!finish)
	if(!finish)
	{
		time += UNIT_INTERVAL;
		for( i=0; i<PARTICLE_NUM; i++ )
			particles[i]->Move();
	}
}

bool Simulator_Circle::Jammed()
{
	int i,j,n,jammed;
	n = object->points.size();
	jammed = 0;
	for( i=0; i<PARTICLE_NUM; i++ )
		particles[i]->counter = 0;
	for( i=0; i<PARTICLE_NUM; i++ )
	{
		for( j=i+1; j<PARTICLE_NUM; j++ )
		{
			/*if(particles[i]->Is_In(particles[j]))
			{
				particles[i]->Generate(object, 1);
				break;
			}*/
			particles[i]->Detect(particles[j]);
		}
		for( j=0; j<n; j++ )
			particles[i]->Detect(object->points[j],object->points[(j+1)%n],1);
		if( particles[i]->counter >= JAMMED_TIME )
			jammed++;
	}
	return ( jammed==PARTICLE_NUM );
}

void Simulator_Circle::Paint( QPainter &paint )
{
	int i;
	for( i=0; i<particles.size(); i++ )
		particles[i]->Draw(paint);
}