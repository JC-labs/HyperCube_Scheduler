#include "HyperCube_Scheduler.hpp"
#include <QtWidgets/QApplication>
int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	HyperCube_Scheduler w;
	w.show();
	return a.exec();
}