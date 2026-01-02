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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
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
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QPlainTextEdit *plainTextEdit;
    QWidget *widget1;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_4;
    QTextEdit *Input_textEdit;
    QWidget *widget2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_5;
    QPlainTextEdit *plainTextEdit_2;
    QWidget *widget3;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_6;
    QComboBox *output_comboBox;
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
        widget->setGeometry(QRect(60, 80, 71, 41));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(widget);
        label_3->setObjectName("label_3");

        verticalLayout->addWidget(label_3);

        plainTextEdit = new QPlainTextEdit(widget);
        plainTextEdit->setObjectName("plainTextEdit");

        verticalLayout->addWidget(plainTextEdit);

        widget1 = new QWidget(tab);
        widget1->setObjectName("widget1");
        widget1->setGeometry(QRect(130, 80, 71, 41));
        verticalLayout_2 = new QVBoxLayout(widget1);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(widget1);
        label_4->setObjectName("label_4");

        verticalLayout_2->addWidget(label_4);

        Input_textEdit = new QTextEdit(widget1);
        Input_textEdit->setObjectName("Input_textEdit");

        verticalLayout_2->addWidget(Input_textEdit);

        widget2 = new QWidget(tab);
        widget2->setObjectName("widget2");
        widget2->setGeometry(QRect(230, 80, 73, 41));
        verticalLayout_3 = new QVBoxLayout(widget2);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(widget2);
        label_5->setObjectName("label_5");

        verticalLayout_3->addWidget(label_5);

        plainTextEdit_2 = new QPlainTextEdit(widget2);
        plainTextEdit_2->setObjectName("plainTextEdit_2");

        verticalLayout_3->addWidget(plainTextEdit_2);

        widget3 = new QWidget(tab);
        widget3->setObjectName("widget3");
        widget3->setGeometry(QRect(300, 80, 71, 41));
        verticalLayout_4 = new QVBoxLayout(widget3);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(widget3);
        label_6->setObjectName("label_6");

        verticalLayout_4->addWidget(label_6);

        output_comboBox = new QComboBox(widget3);
        output_comboBox->setObjectName("output_comboBox");

        verticalLayout_4->addWidget(output_comboBox);

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
