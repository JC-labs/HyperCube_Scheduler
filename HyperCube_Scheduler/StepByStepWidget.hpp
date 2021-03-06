#pragma once
#include <QWidget>
#include "ui_StepByStepWidget.h"
#include <utility>
#include <vector>
#include <list>
class StepByStepWidget : public QWidget {
	Q_OBJECT
private:
	Ui::StepByStepWidget ui;
protected:
	class QTableWidget *b_levels, *processors;
	class ResultWidget *results;

	std::list<std::pair<std::shared_ptr<struct GraphNode>, std::pair<double, size_t>>> tasks;
	std::vector<std::vector<double>> links;
	size_t current = 0;

	std::shared_ptr<struct GraphNode> node(size_t id) const;
public:
	StepByStepWidget(std::pair<std::vector<double>, std::vector<std::vector<double>>> processors, 
					 std::list<std::pair<std::shared_ptr<struct GraphNode>, std::pair<double, size_t>>> tasks,
					 QWidget *parent = Q_NULLPTR);
	~StepByStepWidget();
public slots:
	void step(bool first = false);
};