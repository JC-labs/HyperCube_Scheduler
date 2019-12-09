#pragma once
#include <QDialog>
#include "ui_WeightDialog.h"
class WeightDialog : public QDialog {
	Q_OBJECT
public:
	WeightDialog(QWidget *parent = Q_NULLPTR);
	~WeightDialog();
private:
	Ui::WeightDialog ui;
};