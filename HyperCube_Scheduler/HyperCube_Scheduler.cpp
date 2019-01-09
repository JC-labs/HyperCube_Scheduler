#include "HyperCube_Scheduler.hpp"
#include <vector>
#include "ProcessorMatrixWidget.hpp"
#include "TaskGraphWidget.hpp"
#include <qsplitter.h>

HyperCube_Scheduler::HyperCube_Scheduler(QWidget *parent) : QWidget(parent) {
	ui.setupUi(this);

	auto splitter = new QSplitter(Qt::Orientation::Horizontal);
	splitter->addWidget(processor_matrix = new ProcessorMatrixWidget());
	splitter->addWidget(task_graph = new TaskGraphWidget());
	ui.layout->addWidget(splitter);

	connect(ui.run, &QPushButton::clicked, this, &HyperCube_Scheduler::run);
	connect(ui.randomize, &QPushButton::clicked, this, [this]() {
		processor_matrix->randomize(0, 4, 0, 4);
		task_graph->randomize(0, 4, 0, 4);
	});
}

bool is(size_t what, std::list<std::pair<std::shared_ptr<GraphNode>, std::pair<double, size_t>>> const& where) {
	return std::find_if(where.begin(), where.end(), [&what](auto const& a) -> bool {
		return a.first->i == what;
	}) != where.end();
}
bool check(std::shared_ptr<GraphNode> what, std::list<std::pair<std::shared_ptr<GraphNode>, std::pair<double, size_t>>> const& where) {
	for (auto d : what->ds)
		if (is(d.first->i, where))
			return false;
	return true;
}
std::tuple<std::shared_ptr<GraphNode>, size_t, double, double> get(size_t what, std::list<std::tuple<std::shared_ptr<GraphNode>, size_t, double, double>> const& where) {
	return *std::find_if(where.begin(), where.end(), [&what](auto const& a) -> bool {
		return std::get<0>(a)->i == what;
	});
}

#include "ResultWidget.hpp"
void HyperCube_Scheduler::run() {
	auto temp = processor_matrix->get();
	auto nodes = temp.first;
	auto links = temp.second;

	auto b_levels = task_graph->get_b_levels();
	std::vector<std::pair<double, double>> processors;
	for (size_t i = 0; i < nodes.size(); i++)
		processors.push_back(std::make_pair(nodes.at(i), 0.0));

	std::list<std::tuple<std::shared_ptr<GraphNode>, size_t, double, double>> result;

	while (b_levels.size()) {
		auto current = b_levels.begin();
		while (!check(current->first, b_levels))
			current++;

		size_t ret_i = -1;
		double ret_duration = std::numeric_limits<double>::infinity();
		double ret_start = std::numeric_limits<double>::infinity();
		for (size_t i = 0; i < nodes.size(); i++) {
			double start = processors.at(i).second;
			for (auto d : current->first->ds) {
				auto item = get(d.first->i, result);
				auto temp = std::get<2>(item) + std::get<3>(item) 
					+ d.second * links.at(i).at(std::get<1>(item));
				if (temp > start)
					start = temp;
			}
			double duration = current->first->w / nodes.at(i);

			if (start + duration < ret_start + ret_duration) {
				ret_start = start;
				ret_duration = duration;
				ret_i = i;
			}
		}
		result.push_back(std::make_tuple(current->first, ret_i, ret_start, ret_duration));
		processors.at(ret_i).second = ret_start + ret_duration;
		b_levels.erase(current);
	}

	(new ResultWidget(nodes, result))->show();
}