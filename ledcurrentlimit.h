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

};

#endif // LEDCURRENTLIMIT_H
