#include "WeightDialog.hpp"
WeightDialog::WeightDialog(QWidget *parent)	: QDialog(parent) {
	ui.setupUi(this);
	setModal(true);

}
WeightDialog::~WeightDialog() {}
