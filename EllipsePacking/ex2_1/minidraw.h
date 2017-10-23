#ifndef MINIDRAW_H
#define MINIDRAW_H

#include <QtWidgets/QMainWindow>
#include <QEvent>
#include <QPainter>
#include <QMessageBox>
#include "ui_minidraw.h"
#include "viewwidget.h"

class MiniDraw : public QMainWindow
{
	Q_OBJECT

private:
	QMenu* menu; 
	QToolBar* toolbar_main_;

	QAction* draw_action_;
	QAction* delete_action_;
	QAction* carve_action_;
	QAction* carve_action_circle;

	ViewWidget*	view_widget_;

public:
	MiniDraw(QWidget *parent = 0);
	~MiniDraw();
	void CreateButtons();

private:
	Ui::MiniDrawClass ui;
};

#endif // MINIDRAW_H
