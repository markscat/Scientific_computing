#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "UnitConverterHandler.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updateVoltageDivider();
    void onVoltageModeChanged();


private:
    Ui::MainWindow *ui;
    void updateResult();
    UnitConverterHandler *handler; // <--- 在這裡宣告它！

    bool isCalculating = false;

    void updateSMDCapacitor();
    void updateSMDResistor();

};
#endif // MAINWINDOW_H
