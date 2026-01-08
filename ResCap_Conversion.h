#ifndef RESCAP_CONVERSION_H
#define RESCAP_CONVERSION_H

#include <QWidget>
#include "UnitConverterHandler.h"


namespace Ui {
class ResCap_Conversion;
}

class ResCap_Conversion : public QWidget
{
    Q_OBJECT

public:
    //explicit ResCap_Conversion(QWidget *parent = nullptr);
    explicit ResCap_Conversion(UnitConverterHandler *sharedHandler, QWidget *parent = nullptr);

    ~ResCap_Conversion();

private:
    Ui::ResCap_Conversion *ui;

    UnitConverterHandler *handler; // 保存傳進來的 handler

    void updateSMDCapacitor();
    void updateSMDResistor();
    double decodeSMDCode(QString code);

};

#endif // RESCAP_CONVERSION_H
