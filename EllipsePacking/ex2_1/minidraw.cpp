#include "minidraw.h"
#include <QtWidgets/QLayout>
#include <qtimer.h>

MiniDraw::MiniDraw(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	view_widget_ = new ViewWidget();
	setCentralWidget(view_widget_);
	setGeometry(300, 150, 700, 450);

	CreateButtons();
}

MiniDraw::~MiniDraw()
{
}

void MiniDraw::CreateButtons()
{
	draw_action_ = new QAction(tr("&Draw"), this);
	delete_action_ = new QAction(tr("&Delete"), this);
	carve_action_ = new QAction(tr("&Carve_ellipse"), this);
	carve_action_circle = new QAction(tr("&Carve_circle"), this);

	connect(draw_action_, SIGNAL(triggered()), view_widget_, SLOT(SetDrawStatus()));
	connect(delete_action_, SIGNAL(triggered()), view_widget_, SLOT(DeleteFigure()));
	connect(carve_action_, SIGNAL(triggered()), view_widget_, SLOT(ComputeCarving()));
	connect(carve_action_circle, SIGNAL(triggered()), view_widget_, SLOT(ComputeCarving_circle()));

	draw_action_->setStatusTip(tr("Draw figure of the object"));
	delete_action_->setStatusTip(tr("Delete last figure"));
	carve_action_->setStatusTip(tr("Carve with ellipses"));
	carve_action_circle->setStatusTip("Carve with circles");

	menu = menuBar()->addMenu(tr("&Menu"));
	menu->addAction(draw_action_);
	menu->addAction(delete_action_);
	menu->addAction(carve_action_);
	menu->addAction(carve_action_circle);

	toolbar_main_ = addToolBar(tr("&Main"));
	toolbar_main_->addAction(draw_action_);
	toolbar_main_->addAction(delete_action_);
	toolbar_main_->addAction(carve_action_);
	toolbar_main_->addAction(carve_action_circle);
}