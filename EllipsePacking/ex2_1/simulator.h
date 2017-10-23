#pragma once
#include "polygon.h"
//#include "circle.h"
#include "ellipse.h"

class Simulator
{
public:
	float				width;
	Polygon				*object;
	//vector<Circle*>		particles;
	vector<Ellipse*>	particles;
	float				time;
	bool				finish;

public:
	Simulator(void);
	Simulator( Polygon* new_object );
	~Simulator(void);
	void Start();
	void Update();
	bool Jammed();
	void Paint( QPainter &paint );
};