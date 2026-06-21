#ifndef VOLTAGE_DIVIDER_H
#define VOLTAGE_DIVIDER_H

#include <QWidget>
#include "UnitConverterHandler.h"

namespace Ui {
class Voltage_Divider;
}

class Voltage_Divider : public QWidget
{
    Q_OBJECT

public:
    //explicit Voltage_Divider(QWidget *parent = nullptr);
    explicit Voltage_Divider(UnitConverterHandler *sharedHandler, QWidget *parent = nullptr);

    ~Voltage_Divider();

private:
    Ui::Voltage_Divider *ui;

    UnitConverterHandler *handler; // <--- 在這裡宣告它！
    bool isCalculating = false;


public slots:
    void updateVoltageDivider();
    void onVoltageModeChanged();

};

#endif // VOLTAGE_DIVIDER_H
