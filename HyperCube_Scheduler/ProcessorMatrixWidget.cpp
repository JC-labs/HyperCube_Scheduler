#include "ProcessorMatrixWidget.hpp"
#include <random>
#include <vector>

ProcessorMatrixWidget::ProcessorMatrixWidget(QWidget *parent) : QWidget(parent) {
	ui.setupUi(this);

	connect(ui.size, qOverload<int>(&QSpinBox::valueChanged), this, [this](int value) {
		value = pow(2, value);
		ui.nodes->setRowCount(value);
		ui.nodes->setColumnCount(1);
		ui.links->setRowCount(value);
		ui.links->setColumnCount(value);

		for (size_t i = 0; i < ui.nodes->rowCount(); i++)
			for (size_t j = 0; j < ui.nodes->columnCount(); j++) {
				ui.nodes->setItem(i, j, new QTableWidgetItem("0"));
			}
		for (size_t i = 0; i < ui.links->rowCount(); i++)
			for (size_t j = 0; j < ui.links->columnCount(); j++) {
				if (i < j)
					ui.links->setItem(i, j, new QTableWidgetItem("0"));
				else {
					ui.links->setItem(i, j, new QTableWidgetItem("-"));
					ui.links->item(i, j)->setFlags(Qt::ItemFlag::ItemIsEditable & 0);
				}
			}

		for (size_t j = 0; j < ui.links->columnCount(); j++)
			ui.links->setColumnWidth(j, 12);
		for (size_t j = 0; j < ui.nodes->columnCount(); j++)
			ui.nodes->setColumnWidth(j, 12);
	});
	connect(ui.fill, &QPushButton::clicked, this, [this]() {
		static std::mt19937_64 g(std::random_device{}());
		static std::uniform_real_distribution<> d(0, 100);

		for (size_t i = 0; i < ui.nodes->rowCount(); i++)
			for (size_t j = 0; j < ui.nodes->columnCount(); j++)
				ui.nodes->item(i, j)->setText(QString::number(d(g)));
		for (size_t i = 0; i < ui.links->rowCount(); i++)
			for (size_t j = 0; j < ui.links->columnCount(); j++)
				ui.links->item(i, j)->setText(QString::number(d(g)));
	});

	ui.size->setValue(3);
}
ProcessorMatrixWidget::~ProcessorMatrixWidget() {}

std::vector<std::vector<double>> ProcessorMatrixWidget::get_links() {
	std::vector<std::vector<double>> ret;
	for (size_t i = 0; i < ui.links->rowCount(); i++) {
		std::vector<double> temp;
		for (size_t j = 0; j < ui.links->columnCount(); j++)
			temp.push_back(ui.links->item(i, j)->text().toDouble());
		ret.push_back(temp);
	}
	return ret;
}
std::vector<double> ProcessorMatrixWidget::get_nodes() {
	std::vector<double> ret;
	for (size_t i = 0; i < ui.nodes->rowCount(); i++)
		ret.push_back(ui.nodes->item(i, 0)->text().toDouble());
	return ret;
}