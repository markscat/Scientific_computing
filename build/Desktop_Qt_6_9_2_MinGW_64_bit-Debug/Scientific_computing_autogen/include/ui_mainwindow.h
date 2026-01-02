/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *label;
    QTableWidget *matrixTable;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QTextEdit *Input_textEdit;
    QComboBox *Input_comboBox;
    QLabel *label_2;
    QTextEdit *output_textEdit;
    QComboBox *output_comboBox;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QWidget *tab_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(993, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(0, 0, 981, 531));
        tab = new QWidget();
        tab->setObjectName("tab");
        label = new QLabel(tab);
        label->setObjectName("label");
        label->setGeometry(QRect(50, 170, 81, 16));
        matrixTable = new QTableWidget(tab);
        if (matrixTable->columnCount() < 9)
            matrixTable->setColumnCount(9);
        if (matrixTable->rowCount() < 9)
            matrixTable->setRowCount(9);
        matrixTable->setObjectName("matrixTable");
        matrixTable->setGeometry(QRect(50, 190, 1071, 331));
        matrixTable->setRowCount(9);
        matrixTable->setColumnCount(9);
        widget = new QWidget(tab);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(60, 130, 315, 21));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        Input_textEdit = new QTextEdit(widget);
        Input_textEdit->setObjectName("Input_textEdit");

        horizontalLayout->addWidget(Input_textEdit);

        Input_comboBox = new QComboBox(widget);
        Input_comboBox->setObjectName("Input_comboBox");

        horizontalLayout->addWidget(Input_comboBox);

        label_2 = new QLabel(widget);
        label_2->setObjectName("label_2");

        horizontalLayout->addWidget(label_2);

        output_textEdit = new QTextEdit(widget);
        output_textEdit->setObjectName("output_textEdit");

        horizontalLayout->addWidget(output_textEdit);

        output_comboBox = new QComboBox(widget);
        output_comboBox->setObjectName("output_comboBox");

        horizontalLayout->addWidget(output_comboBox);

        widget1 = new QWidget(tab);
        widget1->setObjectName("widget1");
        widget1->setGeometry(QRect(60, 110, 331, 17));
        horizontalLayout_2 = new QHBoxLayout(widget1);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(widget1);
        label_3->setObjectName("label_3");

        horizontalLayout_2->addWidget(label_3);

        label_4 = new QLabel(widget1);
        label_4->setObjectName("label_4");

        horizontalLayout_2->addWidget(label_4);

        label_5 = new QLabel(widget1);
        label_5->setObjectName("label_5");

        horizontalLayout_2->addWidget(label_5);

        label_6 = new QLabel(widget1);
        label_6->setObjectName("label_6");

        horizontalLayout_2->addWidget(label_6);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 993, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", " \345\226\256\344\275\215\346\217\233\347\256\227\347\237\251\351\231\243 ", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "=", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\346\225\270\345\200\274", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\346\257\224\345\200\274", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\346\225\270\345\200\274", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\346\257\224\345\200\274", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Tab 1", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
