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

	double size = std::min(width(), height());
	double scale = size / 12;

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

		if (insert)
			nodes.insert(std::make_pair(INDEX_COUNTER++,
										Node{m_p.x(), m_p.y(), 1}));
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
		if (b && e)
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