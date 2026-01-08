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

        LEDResult result = calculateLEDComplex(vcc, vd, current, iUnitIdx, series, parallel);

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
}


double LED_current_limit::calculateLEDResistor(double vcc, double vd, double current, int currentUnitIdx) {

    if (current <= 0) return 0;

    // 1. 將電流換算為標準單位 A (Ampere)
    double currentA = current;
    if (currentUnitIdx == 1) currentA = current / 1000.0;      // mA -> A
    else if (currentUnitIdx == 2) currentA = current / 1000000.0; // uA -> A

    // 2. 檢查壓差是否合理 (Vcc 必須大於 Vd)
    if (vcc <= vd) return -1; // 回傳 -1 代表電壓不足以驅動 LED

    // 3. 歐姆定律：R = (Vcc - Vd) / I
    return (vcc - vd) / currentA;
}

LEDResult LED_current_limit::calculateLEDComplex(double vcc, double vd, double current, int iUnitIdx, int series, int parallel) {

    LEDResult res = {0, 0, true};

    // 1. 處理電流單位換算 (換算為 A)
    double branchCurrentA = current;
    if (iUnitIdx == 1) branchCurrentA = current / 1000.0;
    else if (iUnitIdx == 2) branchCurrentA = current / 1000000.0;

    // 2. 串聯計算：總電壓需求
    double totalVd = vd * (series > 0 ? series : 1);

    // 3. 並聯計算：總電流需求
    double totalCurrentA = branchCurrentA * (parallel > 0 ? parallel : 1);

    // 4. 安全檢查：Vcc 必須大於總 LED 壓降
    if (vcc <= totalVd) {
        res.isVoltageOk = false;
        return res;
    }

    // 5. 計算電阻 R = V / I
    res.resistance = (vcc - totalVd) / totalCurrentA;

    // 6. 計算功耗 P = V * I (或是 I^2 * R)
    res.wattage = (vcc - totalVd) * totalCurrentA;

    return res;
}
