#ifndef LEDCURRENTLIMIT_H
#define LEDCURRENTLIMIT_H

#include <QWidget>
#include "UnitConverterHandler.h"

namespace Ui {
class LED_current_limit ;
}

class LED_current_limit  : public QWidget
{
    Q_OBJECT

public:
    //explicit LEDCurrentLimit(QWidget *parent = nullptr);
    explicit LED_current_limit(UnitConverterHandler *sharedHandler, QWidget *parent = nullptr);

    ~LED_current_limit ();

private:
    Ui::LED_current_limit  *ui;
    UnitConverterHandler *handler; // 保存傳進來的 handler
    void updateLEDCalculator();


    // 新增：LED 計算邏輯宣告
    double calculateLEDResistor(double vSource, double vLed, double current, int currentUnitIdx);

    // 新增：處理串並聯的 LED 計算
    LEDResult calculateLEDComplex(double vcc, double vd, double current, int iUnitIdx, int series, int parallel);
};

#endif // LEDCURRENTLIMIT_H
