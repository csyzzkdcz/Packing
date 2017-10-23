#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <qevent.h>
#include <qpainter.h>
#include "ui_viewwidget.h"
#include "simulator.h"
#include "simulator_circle.h"

class ViewWidget : public QWidget
{
	Q_OBJECT

private:
	bool		draw_status_;
	bool		is_drawing_;
	bool		is_carving_;
	bool		is_carving_circle;
	int			chose;
	Polygon		*object_;
	Simulator	*carving_;
	Simulator_Circle *carving_circle;
	
public:
	ViewWidget(QWidget *parent = 0);
	~ViewWidget();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *);

public slots:
	void SetDrawStatus();
	void DeleteFigure();
	void ComputeCarving();
	void ComputeCarving_circle();

private:
	Ui::ViewWidget ui;
};

#endif // VIEWWIDGET_H


