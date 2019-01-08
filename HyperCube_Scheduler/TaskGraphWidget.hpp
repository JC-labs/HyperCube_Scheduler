#pragma once
#include <QWidget>
#include "ui_TaskGraphWidget.h"
#include <set>
#include <map>

struct Node {
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

class TaskGraphWidget : public QWidget {
	Q_OBJECT
public:
	TaskGraphWidget(QWidget *parent = Q_NULLPTR);
	~TaskGraphWidget();
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
};