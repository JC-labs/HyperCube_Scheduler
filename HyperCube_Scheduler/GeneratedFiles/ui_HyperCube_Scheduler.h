/********************************************************************************
** Form generated from reading UI file 'HyperCube_Scheduler.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HYPERCUBE_SCHEDULER_H
#define UI_HYPERCUBE_SCHEDULER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HyperCube_SchedulerClass
{
public:

    void setupUi(QWidget *HyperCube_SchedulerClass)
    {
        if (HyperCube_SchedulerClass->objectName().isEmpty())
            HyperCube_SchedulerClass->setObjectName(QStringLiteral("HyperCube_SchedulerClass"));
        HyperCube_SchedulerClass->resize(600, 400);

        retranslateUi(HyperCube_SchedulerClass);

        QMetaObject::connectSlotsByName(HyperCube_SchedulerClass);
    } // setupUi

    void retranslateUi(QWidget *HyperCube_SchedulerClass)
    {
        HyperCube_SchedulerClass->setWindowTitle(QApplication::translate("HyperCube_SchedulerClass", "HyperCube_Scheduler", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HyperCube_SchedulerClass: public Ui_HyperCube_SchedulerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HYPERCUBE_SCHEDULER_H
