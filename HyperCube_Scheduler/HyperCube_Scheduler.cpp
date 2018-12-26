#include "HyperCube_Scheduler.hpp"
HyperCube_Scheduler::HyperCube_Scheduler(QWidget *parent) : QWidget(parent) {
	ui.setupUi(this);

	connect(ui.run, &QPushButton::clicked, this, &HyperCube_Scheduler::run);
	connect(ui.size, &QLineEdit::textChanged, this, [this]() {
		auto size = ui.size->text().toInt();
		ui.nodes->setRowCount(size);
		ui.nodes->setColumnCount(1);
		ui.links->setRowCount(size);
		ui.links->setColumnCount(size);
	});

	ui.size->setText("6");
}

void HyperCube_Scheduler::run() {

}