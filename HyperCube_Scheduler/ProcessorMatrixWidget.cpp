#include "ProcessorMatrixWidget.hpp"
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
	ui.size->setValue(3);
	on_cell_changed(0, 0);

	connect(ui.links, &QTableWidget::cellChanged, this, &ProcessorMatrixWidget::on_cell_changed);
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

double ProcessorMatrixWidget::update_link(size_t i, size_t j, double value, size_t die_out) {
	if (die_out)
		if (!--die_out)
			return std::numeric_limits<double>::infinity();
	if (i == j)
		return value;
	if (ui.links->item(i, j)->text().toDouble() == std::numeric_limits<double>::infinity()) {
		double ret = std::numeric_limits<double>::infinity();
		for (size_t k = 0; k < ui.links->rowCount(); k++)
			if (i != k && ui.links->item(i, k)->text().toDouble() != std::numeric_limits<double>::infinity()) {
				auto temp = update_link(k, j, value + ui.links->item(i, k)->text().toDouble(), (die_out) ? die_out : 4);
				if (temp < ret)
					ret = temp;
			}
		return ret;
	} else
		return ui.links->item(i, j)->text().toDouble() + value;
}
void ProcessorMatrixWidget::on_cell_changed(int row, int col) {
	disconnect(ui.links, &QTableWidget::cellChanged, this, &ProcessorMatrixWidget::on_cell_changed);

	if (col != row) {
		if (ui.links->item(col, row))
			ui.links->item(col, row)->setText(ui.links->item(row, col)->text());
		else
			ui.links->setItem(col, row, new QTableWidgetItem(ui.links->item(row, col)->text()));
	}

	for (size_t i = 0; i < ui.links->rowCount(); i++)
		for (size_t j = i + 1; j < ui.links->columnCount(); j++)
			if (i != j && !(ui.links->item(i, j)->flags() & Qt::ItemFlag::ItemIsEnabled)) {
				ui.links->item(i, j)->setText(QString::number(std::numeric_limits<double>::infinity()));
				ui.links->item(j, i)->setText(QString::number(std::numeric_limits<double>::infinity()));
			}

	for (size_t i = 0; i < ui.links->rowCount(); i++)
		for (size_t j = i + 1; j < ui.links->columnCount(); j++) {
			auto temp = update_link(i, j);
			ui.links->item(i, j)->setText(QString::number(temp));
			ui.links->item(j, i)->setText(QString::number(temp));
		}

	connect(ui.links, &QTableWidget::cellChanged, this, &ProcessorMatrixWidget::on_cell_changed);
}

std::ostream& operator<<(std::ostream &s, ProcessorMatrixWidget const& w) {

	return s;
}
std::istream& operator>>(std::istream &s, ProcessorMatrixWidget &w) {

	return s;
}

#include <random>
void ProcessorMatrixWidget::randomize(double min_n, double max_n, double min_l, double max_l) {
	static std::mt19937_64 g(std::random_device{}());
	static std::uniform_real_distribution<> d_n(min_n, max_n), d_l(min_l, max_l);

	for (size_t i = 0; i < ui.nodes->rowCount(); i++)
		for (size_t j = 0; j < ui.nodes->columnCount(); j++)
			ui.nodes->item(i, j)->setText(QString::number(d_n(g)));
	for (size_t i = 0; i < ui.links->rowCount(); i++)
		for (size_t j = 0; j < ui.links->columnCount(); j++)
			if (ui.links->item(i, j)->flags() & Qt::ItemFlag::ItemIsEnabled)
				ui.links->item(i, j)->setText(QString::number(d_l(g)));
}