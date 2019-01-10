#pragma once
#include <QWidget>
#include "ui_StepByStepWidget.h"
class StepByStepWidget : public QWidget{
	Q_OBJECT
private:
	Ui::StepByStepWidget ui;
public:
	StepByStepWidget(QWidget *parent = Q_NULLPTR);
	~StepByStepWidget();
};