#pragma once
#include <QtWidgets/QWidget>
#include "ui_HyperCube_Scheduler.h"
class HyperCube_Scheduler : public QWidget {
	Q_OBJECT
public:
	HyperCube_Scheduler(QWidget *parent = Q_NULLPTR);
private:
	Ui::HyperCube_SchedulerClass ui;
};
