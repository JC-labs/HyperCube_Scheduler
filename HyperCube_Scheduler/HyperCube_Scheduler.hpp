#pragma once
#include <QtWidgets/QWidget>
#include "ui_HyperCube_Scheduler.h"
#include <vector>
class HyperCube_Scheduler : public QWidget {
	Q_OBJECT
public:
	HyperCube_Scheduler(QWidget *parent = Q_NULLPTR);
private:
	Ui::HyperCube_SchedulerClass ui;
protected:
	class TaskGraphWidget *task_graph;
	class ProcessorMatrixWidget *processor_matrix;
public slots:
	void run();
};
