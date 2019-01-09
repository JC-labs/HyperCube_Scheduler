#include "ProcessorMatrixWidget.hpp"
#include <random>
#include <vector>

#include <functional>
void fill_connections(std::function<void(size_t, size_t)> f, size_t n) {
	for (size_t k = 0; k < pow(2, n); k++) {
		for (size_t j = 0; j < pow(2, k); j++)
			for (size_t i = j; i < pow(2, n); i += pow(2, k + 1))
				f(i, i + pow(2, k));
	}
}

ProcessorMatrixWidget::ProcessorMatrixWidget(QWidget *parent) : QWidget(parent) {
	ui.setupUi(this);

	connect(ui.size, qOverload<int>(&QSpinBox::valueChanged), this, [this](int input) {
		size_t value = pow(2, input);
		ui.nodes->setRowCount(value);
		ui.nodes->setColumnCount(1);
		ui.nodes->setHorizontalHeaderItem(0, new QTableWidgetItem(" "));
		ui.links->setRowCount(value);
		ui.links->setColumnCount(value);

		//for (size_t j = 0; j < ui.links->columnCount(); j++)
		//	ui.links->setColumnWidth(j, 15);
		//for (size_t j = 0; j < ui.nodes->columnCount(); j++)
		//	ui.nodes->setColumnWidth(j, 15);

		for (size_t i = 0; i < ui.nodes->rowCount(); i++)
			for (size_t j = 0; j < ui.nodes->columnCount(); j++) {
				ui.nodes->setItem(i, j, new QTableWidgetItem("1.0"));
				ui.nodes->item(i, j)->setTextAlignment(Qt::AlignCenter);
			}
		for (size_t i = 0; i < ui.links->rowCount(); i++)
			for (size_t j = 0; j < ui.links->columnCount(); j++) {
				if (i == j)
					ui.links->setItem(i, j, new QTableWidgetItem("0.0"));
				else
					ui.links->setItem(i, j, new QTableWidgetItem("-"));
				ui.links->item(i, j)->setFlags(ui.links->item(i, j)->flags().setFlag(Qt::ItemFlag::ItemIsEnabled, false));
				ui.links->item(i, j)->setTextAlignment(Qt::AlignCenter);
			}
		fill_connections([this, &value](size_t i, size_t j) {
			if (i != j && i < value && j < value) {
				auto item = ui.links->item(i, j);
				item->setText("1.0");
				item->setFlags(item->flags().setFlag(Qt::ItemFlag::ItemIsEnabled, true));

				item = ui.links->item(j, i);
				item->setText("1.0");
				item->setFlags(item->flags().setFlag(Qt::ItemFlag::ItemIsEnabled, true));
			}
		}, input);
	});
	connect(ui.fill, &QPushButton::clicked, this, [this]() {
		static std::mt19937_64 g(std::random_device{}());
		static std::uniform_real_distribution<> d(0, 100);

		for (size_t i = 0; i < ui.nodes->rowCount(); i++)
			for (size_t j = 0; j < ui.nodes->columnCount(); j++)
				ui.nodes->item(i, j)->setText(QString::number(d(g)));
		for (size_t i = 0; i < ui.links->rowCount(); i++)
			for (size_t j = 0; j < ui.links->columnCount(); j++)
				if (ui.links->item(i, j)->flags() & Qt::ItemFlag::ItemIsEnabled)
					ui.links->item(i, j)->setText(QString::number(d(g)));
	});

	ui.size->setValue(3);

	connect(ui.links, &QTableWidget::cellChanged, this, [this](int row, int col) {
		if (ui.links->item(col, row))
			ui.links->item(col, row)->setText(ui.links->item(row, col)->text());
		else
			ui.links->setItem(col, row, new QTableWidgetItem(ui.links->item(row, col)->text()));
	});
}
ProcessorMatrixWidget::~ProcessorMatrixWidget() {}

std::vector<std::vector<double>> ProcessorMatrixWidget::get_links() const {
	std::vector<std::vector<double>> ret;
	for (size_t i = 0; i < ui.links->rowCount(); i++) {
		std::vector<double> temp;
		for (size_t j = 0; j < ui.links->columnCount(); j++)
			temp.push_back(ui.links->item(i, j)->text().toDouble());
		ret.push_back(temp);
	}
	return ret;
}
std::vector<double> ProcessorMatrixWidget::get_nodes() const {
	std::vector<double> ret;
	for (size_t i = 0; i < ui.nodes->rowCount(); i++)
		ret.push_back(ui.nodes->item(i, 0)->text().toDouble());
	return ret;
}

void ProcessorMatrixWidget::update_links(std::vector<std::vector<double>>& links) {
	for (size_t i = 0; i < ui.links->rowCount(); i++)
		for (size_t j = 0; j < ui.links->columnCount(); j++)
			ui.links->item(i, j)->setText(QString::number(links.at(i).at(j)));
}