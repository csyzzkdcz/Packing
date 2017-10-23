#include "viewwidget.h"

ViewWidget::ViewWidget( QWidget *parent )
{
	ui.setupUi(this);
	object_ = NULL;
	carving_ = NULL;
	carving_circle = NULL;
	draw_status_ = false;
	is_drawing_ = false;
	is_carving_ = false;
	is_carving_circle = false;
	chose = 0;
}

ViewWidget::~ViewWidget()
{
	delete object_;
	delete carving_;
	delete carving_circle;
}

void ViewWidget::mousePressEvent( QMouseEvent *event )
{
	if( object_ == NULL )
		return;
	if( draw_status_ && Qt::LeftButton == event->button() )
	{
		if( object_->points.size()==0 )
		{
			is_drawing_ = true;
			object_->AddPoint(event->pos());
		}
		if( is_drawing_ )
			object_->AddPoint(event->pos());
		else
			object_->SelectPoint(event->pos());
	}
	if( draw_status_ && Qt::RightButton == event->button() )
	{
		object_->finish = true;
		is_drawing_ = false;
	}
}

void ViewWidget::mouseMoveEvent( QMouseEvent *event )
{
	if( object_ == NULL )
		return;
	if( is_drawing_ )
	{
		setMouseTracking(true);
		object_->ChangeLastPoint(event->pos());
	}
	else
	{
		setMouseTracking(false);
		object_->ChangeSelectedPoint(event->pos());
	}
}

void ViewWidget::mouseReleaseEvent( QMouseEvent *event )
{
	if( object_!=NULL && !is_drawing_ )
		object_->CalInterior();
}

void ViewWidget::paintEvent( QPaintEvent * )
{
	QPainter painter(this);
	if( object_!=NULL )
	{
		if(chose == 1)
			object_->Draw( painter, !is_carving_ || carving_->finish);
		else
			object_->Draw( painter, !is_carving_circle || carving_circle->finish);
	}
	if( is_carving_ )
	{
		if( !carving_->finish )
			carving_->Update();
		carving_->Paint(painter);
	}
	if( is_carving_circle )
	{
		if( !carving_circle->finish )
			carving_circle->Update();
		carving_circle->Paint(painter);
	}
	update();
}

void ViewWidget::SetDrawStatus()
{
	if( object_!=NULL )
		delete object_;
	object_ = new Polygon;
	draw_status_ = true;                       // set draw status
}

void ViewWidget::DeleteFigure()
{
	if( object_!=NULL )
	{
		delete object_;
		object_ = NULL;
	}
	if( carving_!=NULL )
	{
		delete carving_;
		carving_ = NULL;
	}
	if( carving_circle != NULL)
	{
		delete carving_circle;
		carving_circle = NULL;
	}

	is_carving_ = false;
	is_carving_circle = false;
}

void ViewWidget::ComputeCarving()
{
	if( carving_!=NULL )
	{
		delete carving_;

	}
	if( carving_circle!=NULL )
	{
		delete carving_circle;
		carving_circle = NULL;
		is_carving_circle = false;
	}
	if( object_ == NULL )
		return;
	carving_ = new Simulator(object_);
	carving_->Start();
	is_carving_ = true;
	chose = 1;
}
void ViewWidget::ComputeCarving_circle()
{
	if( carving_!=NULL )
	{
		delete carving_;
		carving_ = NULL;
		is_carving_ = false;
	}
	if( carving_circle!=NULL )
	{
		delete carving_circle;
	}
	if( object_ == NULL )
		return;
	carving_circle = new Simulator_Circle(object_);
	carving_circle->Start();
	is_carving_circle = true;
	chose = 0;
}