#include "HyperCube_Scheduler.hpp"
#include <vector>
#include "ProcessorMatrixWidget.hpp"
#include "TaskGraphWidget.hpp"
#include <qsplitter.h>

HyperCube_Scheduler::HyperCube_Scheduler(QWidget *parent) : QWidget(parent) {
	ui.setupUi(this);

	auto splitter = new QSplitter(Qt::Orientation::Horizontal);
	splitter->addWidget(processor_matrix = new ProcessorMatrixWidget());
	splitter->addWidget(task_graph = new TaskGraphWidget());
	ui.layout->addWidget(splitter);

	connect(ui.run, &QPushButton::clicked, this, &HyperCube_Scheduler::run);
}

void HyperCube_Scheduler::run() {
	auto temp = processor_matrix->get();
	auto nodes = temp.first;
	auto links = temp.second;

	auto b_levels = task_graph->get_b_levels();
}