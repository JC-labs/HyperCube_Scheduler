#pragma once
#include <QWidget>
#include "ui_TaskGraphWidget.h"
#include <set>
#include <map>
#include <list>
#include <memory>

struct Node {
	size_t i;
	double x;
	double y;
	mutable double w;
};
struct Link {
	size_t n1, n2;
	mutable double w;

	inline bool operator<(Link const& o) const {
		if (n1 != o.n1)
			return n1 < o.n1;
		else if (n2 != o.n2)
			return n2 < o.n2;
		else
			return w < o.w;
	}
};

struct GraphNode : public Node {
	std::list<std::pair<std::shared_ptr<GraphNode>, double>> ds;

	GraphNode(Node const& n) : Node{n.i, n.x, n.y, n.w} {}
};

class TaskGraphWidget : public QWidget {
	Q_OBJECT
private:
	double size, scale;

	Ui::TaskGraphWidget ui;
	std::map<size_t, Node> nodes;
	std::set<Link> links;

	double b_x, b_y;
	double c_x, c_y;
	Node *c_n;
	bool trail;
protected:
	void paintEvent(QPaintEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

	std::pair<size_t const, Node>* find_node(double x, double y);

	void add_b_levels(std::shared_ptr<GraphNode> node, std::map<std::shared_ptr<GraphNode>, std::pair<double, size_t>> &ret, double current = 0.0, size_t path = 0) const;
public:
	TaskGraphWidget(QWidget *parent = Q_NULLPTR);
	~TaskGraphWidget();

	std::list<std::pair<std::shared_ptr<GraphNode>, std::pair<double, size_t>>> get_b_levels() const;
	std::list<std::shared_ptr<GraphNode>> to_graph() const;

	void randomize(double min_n, double max_n, double min_l, double max_l);

	friend std::ostream& operator<<(std::ostream &s, TaskGraphWidget const& w);
	friend std::istream& operator>>(std::istream &s, TaskGraphWidget &w);
};