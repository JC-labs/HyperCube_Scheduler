#pragma once
#include <QWidget>
#include "ui_ProcessorMatrixWidget.h"

class ProcessorMatrixWidget : public QWidget {
	Q_OBJECT
public:
	ProcessorMatrixWidget(QWidget *parent = Q_NULLPTR);
	~ProcessorMatrixWidget();
private:
	Ui::ProcessorMatrixWidget ui;
public slots:
	std::vector<double> get_nodes();
	std::vector<std::vector<double>> get_links();
	std::pair<std::vector<double>, std::vector<std::vector<double>>> get() { return std::make_pair(get_nodes(), get_links()); };
};