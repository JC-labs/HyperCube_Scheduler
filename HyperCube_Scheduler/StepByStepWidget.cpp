#include "StepByStepWidget.hpp"
#include <qsplitter.h>
#include <qtablewidget.h>
#include "ResultWidget.hpp"
#include "TaskGraphWidget.hpp"
StepByStepWidget::StepByStepWidget(std::pair<std::vector<double>, std::vector<std::vector<double>>> _processors, 
								   std::list<std::pair<std::shared_ptr<struct GraphNode>, std::pair<double, size_t>>> tasks,
								   QWidget *parent) : QWidget(parent) {
	ui.setupUi(this);

	auto splitter = new QSplitter(Qt::Orientation::Horizontal);
	splitter->addWidget(processors = new QTableWidget(_processors.first.size(), 3));
	splitter->addWidget(b_levels = new QTableWidget(tasks.size(), 3));
	splitter->addWidget(results = new ResultWidget(_processors.first));
	splitter->setStretchFactor(0, 6);
	splitter->setStretchFactor(1, 6);
	splitter->setStretchFactor(2, 8);
	ui.layout->addWidget(splitter);

	for (size_t i = 0; i < _processors.first.size(); i++) {
		processors->setItem(i, 0, new QTableWidgetItem(QString::number(_processors.first.at(i))));
		processors->setItem(i, 1, new QTableWidgetItem("0.0"));
		processors->setItem(i, 2, new QTableWidgetItem("0.0"));
	}
	processors->setEditTriggers(QAbstractItemView::NoEditTriggers);
	processors->setHorizontalHeaderItem(0, new QTableWidgetItem("Q"));
	processors->setHorizontalHeaderItem(1, new QTableWidgetItem("S"));
	processors->setHorizontalHeaderItem(2, new QTableWidgetItem("S + T"));
	for (size_t i = 0; i < processors->columnCount(); i++)
		processors->setColumnWidth(i, 50);

	size_t i = 0;
	for (auto pair : tasks) {
		b_levels->setItem(i, 0, new QTableWidgetItem(QString::number(pair.first->i)));
		b_levels->setItem(i, 1, new QTableWidgetItem(QString::number(pair.second.first)));
		b_levels->setItem(i++, 2, new QTableWidgetItem(QString::number(pair.second.second)));
	}
	b_levels->setEditTriggers(QAbstractItemView::NoEditTriggers);
	b_levels->setHorizontalHeaderItem(0, new QTableWidgetItem("id"));
	b_levels->setHorizontalHeaderItem(1, new QTableWidgetItem("b"));
	b_levels->setHorizontalHeaderItem(2, new QTableWidgetItem("N"));
	for (size_t i = 0; i < b_levels->columnCount(); i++)
		b_levels->setColumnWidth(i, 50);
	for (size_t i = 0; i < b_levels->rowCount(); i++)
		b_levels->setVerticalHeaderItem(i, new QTableWidgetItem(" "));

	connect(ui.step, &QPushButton::clicked, this, &StepByStepWidget::step);
	step();
}
StepByStepWidget::~StepByStepWidget() {}

void StepByStepWidget::step() {

}