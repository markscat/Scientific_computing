#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "UnitConverterHandler.h"
#include "./inc/UnitConverterHandler.h"

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



private slots:
    void on_actionAbout_triggered();

    void updateLengthConversion(); // 新增長度換算槽函數
    void updatePowerConversion();


    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;
    void updateResult();
    UnitConverterHandler *handler; // <--- 在這裡宣告它！

    bool isCalculating = false;


    // 記錄最後動過的兩個控制項的名稱 (例如 "V", "I", "R", "P")
    QString firstInput = "";
    QString secondInput = "";


};
#endif // MAINWINDOW_H
