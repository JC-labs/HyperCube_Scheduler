#include "StepByStepWidget.hpp"
#include <qsplitter.h>
#include <qtablewidget.h>
#include "ResultWidget.hpp"
#include "TaskGraphWidget.hpp"
StepByStepWidget::StepByStepWidget(std::pair<std::vector<double>, std::vector<std::vector<double>>> _processors,
								   std::list<std::pair<std::shared_ptr<struct GraphNode>, std::pair<double, size_t>>> tasks,
								   QWidget *parent) : QWidget(parent), tasks(tasks), links(_processors.second) {
	ui.setupUi(this);

	auto splitter = new QSplitter(Qt::Orientation::Horizontal);
	splitter->addWidget(processors = new QTableWidget(_processors.first.size(), 4));
	splitter->addWidget(b_levels = new QTableWidget(tasks.size(), 3));
	splitter->addWidget(results = new ResultWidget(_processors.first));
	splitter->setStretchFactor(0, 8);
	splitter->setStretchFactor(1, 6);
	splitter->setStretchFactor(2, 8);
	ui.layout->addWidget(splitter);

	for (size_t i = 0; i < _processors.first.size(); i++) {
		processors->setItem(i, 0, new QTableWidgetItem(QString::number(_processors.first.at(i))));
		processors->setItem(i, 1, new QTableWidgetItem("0.0"));
		processors->setItem(i, 2, new QTableWidgetItem("0.0"));
		processors->setItem(i, 3, new QTableWidgetItem("0.0"));
	}
	processors->setEditTriggers(QAbstractItemView::NoEditTriggers);
	processors->setDisabled(true);
	processors->setHorizontalHeaderItem(0, new QTableWidgetItem("Q"));
	processors->setHorizontalHeaderItem(1, new QTableWidgetItem("S"));
	processors->setHorizontalHeaderItem(1, new QTableWidgetItem("S + W"));
	processors->setHorizontalHeaderItem(2, new QTableWidgetItem("S + W + T"));
	for (size_t i = 0; i < processors->columnCount(); i++)
		processors->setColumnWidth(i, 50);

	size_t i = 0;
	for (auto pair : tasks) {
		b_levels->setItem(i, 0, new QTableWidgetItem(QString::number(pair.first->i)));
		b_levels->setItem(i, 1, new QTableWidgetItem(QString::number(pair.second.first)));
		b_levels->setItem(i++, 2, new QTableWidgetItem(QString::number(pair.second.second)));
	}
	b_levels->setEditTriggers(QAbstractItemView::NoEditTriggers);
	b_levels->setFocusPolicy(Qt::NoFocus);
	b_levels->setDisabled(true);
	b_levels->setHorizontalHeaderItem(0, new QTableWidgetItem("id"));
	b_levels->setHorizontalHeaderItem(1, new QTableWidgetItem("b"));
	b_levels->setHorizontalHeaderItem(2, new QTableWidgetItem("N"));
	for (size_t i = 0; i < b_levels->columnCount(); i++)
		b_levels->setColumnWidth(i, 50);
	for (size_t i = 0; i < b_levels->rowCount(); i++)
		b_levels->setVerticalHeaderItem(i, new QTableWidgetItem(" "));

	connect(ui.step, &QPushButton::clicked, this, &StepByStepWidget::step);
	step(true);
}
StepByStepWidget::~StepByStepWidget() {}

inline bool is(size_t what, QTableWidget const* where) {
	for (size_t i = 0; i < where->rowCount(); i++)
		if (where->item(i, 0)->text().toULongLong() == what)
			return true;
	return false;
}
inline bool check(std::shared_ptr<GraphNode> what, QTableWidget const* where) {
	for (auto d : what->ds)
		if (is(d.first->i, where))
			return false;
	return true;
}
inline std::tuple<std::shared_ptr<GraphNode>, size_t, double, double> get(size_t what, std::list<std::tuple<std::shared_ptr<GraphNode>, size_t, double, double>> const& where) {
	return *std::find_if(where.begin(), where.end(), [&what](auto const& a) -> bool {
		return std::get<0>(a)->i == what;
	});
}
void StepByStepWidget::step(bool first) {
	if (!b_levels->rowCount())
		return;

	if (!first) {
		size_t ret_i = -1;
		double ret_v = std::numeric_limits<double>::infinity();
		double ret_start = std::numeric_limits<double>::infinity();
		double ret_duration = std::numeric_limits<double>::infinity();
		for (size_t i = 0; i < processors->rowCount(); i++)
			if (processors->item(i, 3)->text().toDouble() < ret_v) {
				ret_v = processors->item(i, 3)->text().toDouble();
				ret_i = i;
				ret_start = processors->item(i, 2)->text().toDouble();
				ret_duration = ret_v - ret_start;
			}
		(*results)->push_back(std::make_tuple(node(b_levels->item(current, 0)->text().toULongLong()), ret_i, ret_start, ret_duration));
		processors->item(ret_i, 1)->setText(QString::number(ret_start + ret_duration));
		b_levels->removeRow(current);
		results->update();
	}

	if (!b_levels->rowCount()) {
		for (size_t i = 0; i < processors->rowCount(); i++) {
			processors->item(i, 2)->setText(" ");
			processors->item(i, 3)->setText(" ");
		}
		return;
	}

	current = 0;
	while (!check(node(b_levels->item(current, 0)->text().toULongLong()), b_levels))
		current++;
	b_levels->selectRow(current);

	for (size_t i = 0; i < processors->rowCount(); i++) {
		double start = processors->item(i, 1)->text().toDouble();
		for (auto d : node(b_levels->item(current, 0)->text().toULongLong())->ds) {
			auto item = get(d.first->i, **results);
			auto temp = std::get<2>(item) + std::get<3>(item)
				+ d.second * links.at(i).at(std::get<1>(item));
			if (temp > start)
				start = temp;
		}
		double duration = node(current)->w / processors->item(i, 0)->text().toDouble();

		processors->item(i, 2)->setText(QString::number(start));
		processors->item(i, 3)->setText(QString::number(start + duration));
	}
}

#include <algorithm>
std::shared_ptr<struct GraphNode> StepByStepWidget::node(size_t id) const {
	auto ret = std::find_if(tasks.begin(), tasks.end(), [&id](auto const& a) {
		return a.first->i == id;
	});
	if (ret != tasks.end())
		return ret->first;
	else
		return nullptr;
}