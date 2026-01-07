#include "ResCap_Conversion.h"
#include "ui_ResCap_Conversion.h"


ResCap_Conversion::ResCap_Conversion(UnitConverterHandler *sharedHandler, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResCap_Conversion),
    handler(sharedHandler) // 承接共用的邏輯處理器
{
    ui->setupUi(this);
}

ResCap_Conversion::~ResCap_Conversion()
{
    delete ui;
}
