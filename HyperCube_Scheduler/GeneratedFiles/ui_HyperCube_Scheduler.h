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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HyperCube_SchedulerClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *layout;
    QHBoxLayout *horizontalLayout;
    QPushButton *randomize;
    QPushButton *run;

    void setupUi(QWidget *HyperCube_SchedulerClass)
    {
        if (HyperCube_SchedulerClass->objectName().isEmpty())
            HyperCube_SchedulerClass->setObjectName(QStringLiteral("HyperCube_SchedulerClass"));
        HyperCube_SchedulerClass->resize(1101, 478);
        verticalLayout = new QVBoxLayout(HyperCube_SchedulerClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        layout = new QHBoxLayout();
        layout->setSpacing(6);
        layout->setObjectName(QStringLiteral("layout"));

        verticalLayout->addLayout(layout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        randomize = new QPushButton(HyperCube_SchedulerClass);
        randomize->setObjectName(QStringLiteral("randomize"));

        horizontalLayout->addWidget(randomize);

        run = new QPushButton(HyperCube_SchedulerClass);
        run->setObjectName(QStringLiteral("run"));

        horizontalLayout->addWidget(run);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 4);

        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 1);

        retranslateUi(HyperCube_SchedulerClass);

        QMetaObject::connectSlotsByName(HyperCube_SchedulerClass);
    } // setupUi

    void retranslateUi(QWidget *HyperCube_SchedulerClass)
    {
        HyperCube_SchedulerClass->setWindowTitle(QApplication::translate("HyperCube_SchedulerClass", "HyperCube_Scheduler", nullptr));
        randomize->setText(QApplication::translate("HyperCube_SchedulerClass", "Randomize", nullptr));
        run->setText(QApplication::translate("HyperCube_SchedulerClass", "Run", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HyperCube_SchedulerClass: public Ui_HyperCube_SchedulerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HYPERCUBE_SCHEDULER_H
