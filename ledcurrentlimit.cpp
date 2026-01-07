#include "ledcurrentlimit.h"
#include "ui_ledcurrentlimit.h"
//#include "UnitConverterHandler.h"


LED_current_limit::LED_current_limit(UnitConverterHandler *sharedHandler, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LED_current_limit),
    handler(sharedHandler) // 承接共用的邏輯處理器

//ledcurrentlimit.cpp:11:12: Allocation of incomplete type 'Ui::LED_current_limit'
//      ledcurrentlimit.h:8:7: forward declaration of 'Ui::LED_current_limit'
{
    ui->setupUi(this);

    ui->D1_input_comboBox->addItems(handler->currentUnits);
    ui->limit_Input_comboBox->addItems(handler->resistorUnits);

    // 預設設定
    ui->D1_input_comboBox->setCurrentIndex(1);   // 預設 mA
    ui->limit_Input_comboBox->setCurrentIndex(0); // 預設 Ω
    ui->VD_Input_lineEdit->setText("2.0");        // 給一個常見的預設值
    ui->Series_Input_lineEdit->setText("1");      // 預設 1 串
    ui->Parallel_Input_lineEdit->setText("1");    // 預設 1 並


    // 連結所有會影響結果的訊號
    connect(ui->D1_Input_lineEdit, &QLineEdit::textChanged, this, &LED_current_limit::updateLEDCalculator);
    connect(ui->D1_input_comboBox, &QComboBox::currentIndexChanged, this, &LED_current_limit::updateLEDCalculator);
    connect(ui->VCCIO_Input_lineEdit, &QLineEdit::textChanged, this, &LED_current_limit::updateLEDCalculator);
    connect(ui->VD_Input_lineEdit, &QLineEdit::textChanged, this, &LED_current_limit::updateLEDCalculator);
    connect(ui->limit_Input_comboBox, &QComboBox::currentIndexChanged, this, &LED_current_limit::updateLEDCalculator);

    connect(ui->Series_Input_lineEdit, &QLineEdit::textChanged, this, &LED_current_limit::updateLEDCalculator);
    connect(ui->Parallel_Input_lineEdit, &QLineEdit::textChanged, this, &LED_current_limit::updateLEDCalculator);

}

LED_current_limit ::~LED_current_limit ()
{
    delete ui;
}



void LED_current_limit::updateLEDCalculator() {
    if (!handler) return;

    bool okVcc, okVd, okI,okS, okP;
    double vcc = ui->VCCIO_Input_lineEdit->text().toDouble(&okVcc);
    double vd = ui->VD_Input_lineEdit->text().toDouble(&okVd);
    double current = ui->D1_Input_lineEdit->text().toDouble(&okI);

    // 讀取串並聯數量，如果沒填或填錯，預設為 1
    int series = ui->Series_Input_lineEdit->text().toInt(&okS);
    if (!okS || series < 1) series = 1;

    int parallel = ui->Parallel_Input_lineEdit->text().toInt(&okP);
    if (!okP || parallel < 1) parallel = 1;


    if (okVcc && okVd && okI) {
        // 1. 取得電流單位索引
        int iUnitIdx = ui->D1_input_comboBox->currentIndex();

        // 2. 呼叫邏輯層計算基準電阻 (Ohm)

        LEDResult result = handler->calculateLEDComplex(vcc, vd, current, iUnitIdx, series, parallel);

        if (!result.isVoltageOk) {
            ui->limit_Input_lineEdit->setText("Vcc < 總Vd!");
            ui->W_Input_lineEdit->setText("N/A");
            return;
        }

        // 顯示電阻結果
        int rUnitIdx = ui->limit_Input_comboBox->currentIndex();
        double displayR = result.resistance;
        if (rUnitIdx == 1) displayR /= 1000.0;
        else if (rUnitIdx == 2) displayR /= 1000000.0;

        ui->limit_Input_lineEdit->setText(QString::number(displayR, 'g', 6));

        // 顯示功耗結果 (W)
        ui->W_Input_lineEdit->setText(QString::number(result.wattage, 'g', 4));

        // 額外小功能：如果功耗太大，把字變紅色 (示範 Qt 樣式切換)
        if (result.wattage > 0.25) {
            ui->W_Input_lineEdit->setStyleSheet("color: red; font-weight: bold;");
        } else {
            ui->W_Input_lineEdit->setStyleSheet("color: black;");
        }
    } else {
        ui->limit_Input_lineEdit->clear();
        ui->W_Input_lineEdit->clear();
    }

    /*
        //double resBase = handler->calculateLEDResistor(vcc, vd, current, iUnitIdx);
        if (resBase == -1) {
            ui->limit_Input_lineEdit->setText("Vcc < Vd!");
            return;
        }

        // 3. 根據選定的電阻單位換算顯示值
        int rUnitIdx = ui->limit_Input_comboBox->currentIndex();
        double displayVal = resBase;

        if (rUnitIdx == 1) displayVal = resBase / 1000.0;       // kΩ
        else if (rUnitIdx == 2) displayVal = resBase / 1000000.0; // MΩ

        ui->limit_Input_lineEdit->setText(QString::number(displayVal, 'g', 6));
    } else {
        ui->limit_Input_lineEdit->clear();
    }
*/



}
