//#include "opa_analysis.h"
#include "../inc/opa_analysis.h"
#include "ui_opa_analysis.h"

OPA_Analysis::OPA_Analysis(UnitConverterHandler *sharedHandler, QWidget *parent):
    QWidget(parent),
    ui(new Ui::OPA_Analysis),
    handler(sharedHandler) // 承接共用的邏輯處理器
{ // 把 handler 存起來備用
    ui->setupUi(this);
}

OPA_Analysis::~OPA_Analysis()
{
    delete ui;
}
