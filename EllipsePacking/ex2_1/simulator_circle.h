#pragma once
#include "polygon.h"
#include "circle.h"
#include "ellipse.h"

class Simulator_Circle
{
public:
	float				width;
	Polygon				*object;
	vector<Circle*>		particles;
	//vector<Ellipse*>	particles;
	float				time;
	bool				finish;

public:
	Simulator_Circle(void);
	Simulator_Circle( Polygon* new_object );
	~Simulator_Circle(void);
	void Start();
	void Update();
	bool Jammed();
	void Paint( QPainter &paint );
};