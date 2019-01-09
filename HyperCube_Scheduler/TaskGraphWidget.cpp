#include "TaskGraphWidget.hpp"
TaskGraphWidget::TaskGraphWidget(QWidget *parent) : QWidget(parent) {
	ui.setupUi(this);
	trail = false;
	c_n = nullptr;
	//nodes.insert(std::make_pair(0, Node{.4, .5, 1}));
	//nodes.insert(std::make_pair(1, Node{.9, .1, 1}));
	//links.insert(Link{0, 1, 1000});
}
TaskGraphWidget::~TaskGraphWidget() {}

#include <qpainter.h>
void TaskGraphWidget::paintEvent(QPaintEvent *event) {
	size = std::min(width(), height());
	scale = size / 12;

	QPainter p(this);
	p.setPen(QPen(QColor(10, 10, 10)));
	p.setBrush(QBrush(QColor(255, 255, 255)));
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setFont(QFont("Verdana", scale / 6));

	if (trail)
		p.drawLine(c_x * size + scale / 2, c_y * size + scale / 2,
				   b_x * size + scale / 2, b_y * size + scale / 2);
	for (auto &link : links) {
		p.drawLine(nodes[link.n1].x * size + scale / 2, nodes[link.n1].y * size + scale / 2,
				   nodes[link.n2].x * size + scale / 2, nodes[link.n2].y * size + scale / 2);
		auto center = (QPointF(nodes[link.n1].x * size, nodes[link.n1].y * size) + QPointF(nodes[link.n2].x * size, nodes[link.n2].y * size)) / 2;
		p.drawText(QRectF(center, QSizeF(scale, scale / 2)),
				   QString::number(link.w), QTextOption(Qt::AlignCenter));
	}
	for (auto &node : nodes) {
		p.drawEllipse(node.second.x * size, node.second.y * size, scale, scale);
		p.drawText(QRectF(node.second.x * size, node.second.y * size, scale, scale / 2), 
				   QString::number(node.first), QTextOption(Qt::AlignCenter));
		p.drawText(QRectF(node.second.x * size, node.second.y * size + scale / 2, scale, scale / 2), 
				   QString::number(node.second.w), QTextOption(Qt::AlignCenter));
	}
}

#include "WeightDialog.hpp"
#include <qevent.h>
#include <qinputdialog.h>
static size_t INDEX_COUNTER = 0;
void TaskGraphWidget::mouseDoubleClickEvent(QMouseEvent *event) {
	bool insert = true;
	auto m_p = QPointF((event->x() - scale / 2) / size, (event->y() - scale / 2) / size);

	for (auto &link : links) {
		auto d0 = QVector2D(QPointF(nodes[link.n1].x, nodes[link.n1].y) - m_p).length();
		auto d1 = QVector2D(QPointF(nodes[link.n2].x, nodes[link.n2].y) - m_p).length();
		auto d2 = QVector2D(QPointF(nodes[link.n1].x, nodes[link.n1].y) - QPointF(nodes[link.n2].x, nodes[link.n2].y)).length();
		if (fabs(d2 - d1 - d0) < 1.0 / 120 && d0 > 1.0 / 24 && d1 > 1.0 / 24) {
			link.w = QInputDialog::getDouble(this, "New Weight", "Enter new weight: ", 1, 0, 100, 6);
			insert = false;
		}
	}
	if (insert) {
		for (auto &node : nodes) {
			auto distance = QVector2D(QPointF(node.second.x, node.second.y) - m_p).length();
			if (distance < 1.0 / 24)
				node.second.w = QInputDialog::getDouble(this, "New Weight", "Enter new weight: ", 1, 0, 100, 6);
			if (distance < 1.0 / 12)
				insert = false;
		}

		if (insert) {
			nodes.insert(std::make_pair(INDEX_COUNTER,
										Node{INDEX_COUNTER, m_p.x(), m_p.y(), 1}));
			INDEX_COUNTER++;
		}
	}
	update();
	event->accept();
}
void TaskGraphWidget::mouseMoveEvent(QMouseEvent *event) {
	c_x = (event->x() - scale / 2) / size;
	c_y = (event->y() - scale / 2) / size;
	if (c_n && event->buttons() & Qt::MouseButton::LeftButton) {
		c_n->x = c_x;
		c_n->y = c_y;
	}
	update();
	event->accept();
}
void TaskGraphWidget::mousePressEvent(QMouseEvent *event) {
	if (event->buttons() & Qt::MouseButton::LeftButton) {
		auto temp = find_node((event->x() - scale / 2) / size, (event->y() - scale / 2) / size);
		if (temp)
			c_n = &temp->second;
		else
			c_n = nullptr;
	}
	if (event->buttons() & Qt::MouseButton::RightButton) {
		trail = true;
		b_x = c_x = (event->x() - scale / 2) / size;
		b_y = c_y = (event->y() - scale / 2) / size;
	}
	update();
	event->accept();
}
void TaskGraphWidget::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::MouseButton::LeftButton)
		c_n = nullptr;
	if (event->button() == Qt::MouseButton::RightButton) {
		trail = false;

		auto b = find_node(b_x, b_y);
		auto e = find_node((event->x() - scale / 2) / size, (event->y() - scale / 2) / size);
		if (b && e && b != e)
			links.insert(Link{b->first, e->first, 1});
	}
	update();
	event->accept();
}
std::pair<size_t const, Node>* TaskGraphWidget::find_node(double x, double y) {
	for (auto &node : nodes)
		if (QVector2D(QPointF(node.second.x, node.second.y) - QPointF(x, y)).length() < 1.0 / 24)
			return &node;
	return nullptr;
}

bool compute_direction(size_t n1, size_t n2, std::map<size_t, Node> const& nodes) {
	if (nodes.at(n1).y == nodes.at(n2).y)
		return nodes.at(n1).x < nodes.at(n2).x;
	else
		return nodes.at(n1).y < nodes.at(n2).y;
}

std::list<std::shared_ptr<GraphNode>> TaskGraphWidget::to_graph() const {
	std::map<size_t, std::pair<std::shared_ptr<GraphNode>, bool>> _nodes;

	for (auto &node : nodes)
		_nodes.insert(std::make_pair(node.first,
									 std::make_pair(std::make_shared<GraphNode>(node.second), true)));

	for (auto &link : links) {
		if (compute_direction(link.n1, link.n2, nodes)) {
			_nodes.at(link.n1).second = false;
			_nodes.at(link.n2).first->ds.push_back(std::make_pair(_nodes.at(link.n1).first, link.w));
		} else {
			_nodes.at(link.n2).second = false;
			_nodes.at(link.n1).first->ds.push_back(std::make_pair(_nodes.at(link.n2).first, link.w));
		}
	}

	std::list<std::shared_ptr<GraphNode>> ret;
	for (auto &node : _nodes)
		if (node.second.second)
			ret.push_back(node.second.first);
	return ret;
}
std::list<std::pair<std::shared_ptr<GraphNode>, std::pair<double, size_t>>> TaskGraphWidget::get_b_levels() const {
	auto graph = to_graph();

	std::map<std::shared_ptr<GraphNode>, std::pair<double, size_t>> _nodes;
	for (auto sub_graph : graph)
		add_b_levels(sub_graph, _nodes);

	std::list<std::pair<std::shared_ptr<GraphNode>, std::pair<double, size_t>>> ret{_nodes.begin(), _nodes.end()};

	ret.sort([](auto const& a, auto const& b) -> bool {
		if (a.first->ds.size() == 0 && b.first->ds.size() != 0)
			return true;
		if (a.first->ds.size() != 0 && b.first->ds.size() == 0)
			return false;
		return a.second > b.second;
	});

	return ret;
}
void TaskGraphWidget::add_b_levels(std::shared_ptr<GraphNode> node, std::map<std::shared_ptr<GraphNode>, std::pair<double, size_t>>& ret, double current, size_t path) const {
	auto next = current + node->w;
	auto found = ret.find(node);
	if (found == ret.end() || next < found->second.first || path < found->second.second) {
		ret[node].first = next;
		ret[node].second = path + 1;
	}
	for (auto d : node->ds)
		add_b_levels(d.first, ret, next, path + 1);
}
