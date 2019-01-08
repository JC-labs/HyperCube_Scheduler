#pragma once
#include <QWidget>
#include "ui_ResultWidget.h"
class ResultWidget : public QWidget {
	Q_OBJECT
public:
	ResultWidget(QWidget *parent = Q_NULLPTR);
	~ResultWidget();

private:
	Ui::ResultWidget ui;
};
