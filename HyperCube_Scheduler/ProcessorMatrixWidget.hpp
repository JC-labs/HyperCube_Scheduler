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
	std::vector<double> get_nodes() const;
	std::vector<std::vector<double>> get_links() const;
	std::pair<std::vector<double>, std::vector<std::vector<double>>> get() const { return std::make_pair(get_nodes(), get_links()); };
	void update_links(std::vector<std::vector<double>> &links);
};