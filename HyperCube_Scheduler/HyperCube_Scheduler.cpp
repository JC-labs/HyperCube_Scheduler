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
}

double fill_link(size_t i, size_t j, std::vector<std::vector<double>> &links, double value = 0, size_t die_out = 0) {
	if (die_out)
		if (!--die_out)
			return std::numeric_limits<double>::infinity();
	if (i == j)
		return value;
	if (links.at(i).at(j) == std::numeric_limits<double>::infinity()) {
		double ret = std::numeric_limits<double>::infinity();
		for (size_t k = 0; k < links.size(); k++)
			if (i != k && links.at(i).at(k) != std::numeric_limits<double>::infinity()) {
				auto temp = fill_link(k, j, links, value + links.at(i).at(k), (die_out) ? die_out : 4);
				if (temp < ret)
					ret = temp;
		}
		return ret;
	} else
		return links.at(i).at(j) + value;
}

void fill_links(std::vector<std::vector<double>> &links) {
	for (size_t i = 0; i < links.size(); i++)
		for (size_t j = 0; j < links.at(i).size(); j++)
			if (i != j && !links.at(i).at(j))
				links.at(i).at(j) = std::numeric_limits<double>::infinity();

	for (size_t i = 0; i < links.size(); i++)
		for (size_t j = i + 1; j < links.at(i).size(); j++)
			links.at(i).at(j) = links.at(j).at(i) = fill_link(i, j, links);

}

#include "ResultWidget.hpp"
void HyperCube_Scheduler::run() {
	auto temp = processor_matrix->get();
	auto nodes = temp.first;
	auto links = temp.second;

	fill_links(links);

	auto b_levels = task_graph->get_b_levels();

	std::vector<std::tuple<size_t, double, double>> leveled_nodes;
	for (size_t i = 0; i < nodes.size(); i++)
		leveled_nodes.push_back(std::make_tuple(i, nodes.at(i), 0.0));
	std::sort(leveled_nodes.begin(), leveled_nodes.end(), [](auto const& a, auto const& b) -> bool {
		return std::get<1>(a) > std::get<1>(b);
	});

	std::list<std::tuple<std::shared_ptr<GraphNode>, size_t, double, double>> result;

	for (size_t i = 0; !b_levels.empty() && !b_levels.front().first->ds.size() && i < leveled_nodes.size(); i++) {
		auto duration = b_levels.front().second.first / std::get<1>(leveled_nodes.at(i));
		result.push_back(std::make_tuple(b_levels.front().first, std::get<0>(leveled_nodes.at(i)), 0.0, duration));
		std::get<2>(leveled_nodes.at(i)) += duration;
		b_levels.pop_front();
	}

	(new ResultWidget(nodes, result))->show();
}