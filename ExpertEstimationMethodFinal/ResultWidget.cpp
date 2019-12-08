#include "ResultWidget.hpp"
ResultWidget::ResultWidget(std::vector<double> processors, std::list<std::tuple<std::shared_ptr<struct GraphNode>, size_t, double, double>> tasks, QWidget *parent)
						: QWidget(parent), tasks(tasks), processors(processors) {
	ui.setupUi(this);
}
ResultWidget::~ResultWidget() {}

#include <qpainter.h>
#include "TaskGraphWidget.hpp"
void ResultWidget::paintEvent(QPaintEvent *event) {
	double header = 100;
	double v_scale = height() / (processors.size() + 1);
	double h_scale;
	if (tasks.size()) {
		auto max = std::max_element(tasks.begin(), tasks.end(), [](auto const& a, auto const& b) -> bool {
			return std::get<2>(a) + std::get<3>(a) < std::get<2>(b) + std::get<3>(b);
		});
		h_scale = (width() - header) / (std::get<2>(*max) + std::get<3>(*max));
	} else
		h_scale = 1.;

	double mark_size = 100;
	double mark_number = (width() - header) / mark_size;
	double mark_scale = (width() - header) / h_scale / mark_number;

	QPainter p(this);
	p.setPen(QPen(QColor(10, 10, 10)));
	p.setBrush(QBrush(QColor(255, 255, 255)));
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setFont(QFont("Verdana", v_scale * .4));

	p.drawLine(QPointF(header, 0), QPointF(header, height()));
	p.drawLine(QPointF(0, v_scale), QPointF(width(), v_scale));
	size_t i = 0;
	for (double shift = header + mark_size; shift < width() + header; shift += mark_size) {
		p.drawLine(QPointF(shift, 0), QPointF(shift, v_scale));
		p.drawText(QRectF(shift - mark_size, 0, mark_size, v_scale),
				   QString::number(mark_scale * ++i), QTextOption(Qt::AlignBottom | Qt::AlignHCenter));
	}

	for (size_t i = 0; i < processors.size(); i++)
		p.drawText(QRectF(QPointF(0, (i + 1) * v_scale), QSizeF(header, v_scale)),
				   "[" + QString::number(i + 1) + "]", QTextOption(Qt::AlignRight | Qt::AlignVCenter));

	for (auto task : tasks) {
		auto rect = QRectF(std::get<2>(task) * h_scale + header, (std::get<1>(task) + 1) * v_scale, std::get<3>(task) * h_scale, v_scale);
		p.drawRect(rect);
		p.drawText(rect, QString::number(std::get<0>(task)->i), QTextOption(Qt::AlignCenter));
	}
}
