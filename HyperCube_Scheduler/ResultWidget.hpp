#pragma once
#include <QWidget>
#include "ui_ResultWidget.h"
#include <vector>
#include <memory>
#include <list>
#include <utility>

class ResultWidget : public QWidget {
	Q_OBJECT
public:
	ResultWidget(std::vector<double> processors, std::list<std::tuple<std::shared_ptr<struct GraphNode>, size_t, double, double>> tasks = {}, QWidget *parent = Q_NULLPTR);
	//ResultWidget(std::vector<double> processors, std::list<std::tuple<std::shared_ptr<struct GraphNode>, size_t, double, double>> tasks, QWidget *parent = Q_NULLPTR);
	~ResultWidget();
protected:
	void paintEvent(QPaintEvent *event) override;
private:
	Ui::ResultWidget ui; 
	std::vector<double> processors;
	std::list<std::tuple<std::shared_ptr<struct GraphNode>, size_t, double, double>> tasks;
};
