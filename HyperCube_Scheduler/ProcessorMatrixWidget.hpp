#pragma once
#include <QWidget>
#include "ui_ProcessorMatrixWidget.h"

class ProcessorMatrixWidget : public QWidget {
	Q_OBJECT
public:
	ProcessorMatrixWidget(QWidget *parent = Q_NULLPTR);
	~ProcessorMatrixWidget();

	void randomize(double min_n, double max_n, double min_l, double max_l);
private:
	Ui::ProcessorMatrixWidget ui;
protected:
	double update_link(size_t i, size_t j, double value = 0, size_t die_out = 0);
public slots:
	std::vector<double> get_nodes() const;
	std::vector<std::vector<double>> get_links() const;
	std::pair<std::vector<double>, std::vector<std::vector<double>>> get() const { return std::make_pair(get_nodes(), get_links()); };

	void on_cell_changed(int row, int col);
};