#include "ResCap_Conversion.h"
#include "ui_ResCap_Conversion.h"


ResCap_Conversion::ResCap_Conversion(UnitConverterHandler *sharedHandler, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResCap_Conversion),
    handler(sharedHandler) // 承接共用的邏輯處理器
{
    ui->setupUi(this);

    ui->Resistor_output_comboBox->addItems(handler->resistorUnits);
    // 填寫電容單位
    ui->capacitor_output_comboBox->addItems(handler->capacitorUnits);

    // 連結電阻計算
    connect(ui->Resistor_Input_lineEdit, &QLineEdit::textChanged, this, &ResCap_Conversion::updateSMDResistor);
    connect(ui->Resistor_output_comboBox, &QComboBox::currentIndexChanged, this, &ResCap_Conversion::updateSMDResistor);

    // 連結電容計算
    connect(ui->capacitor_Input_lineEdit, &QLineEdit::textChanged, this, &ResCap_Conversion::updateSMDCapacitor);
    connect(ui->capacitor_output_comboBox, &QComboBox::currentIndexChanged, this, &ResCap_Conversion::updateSMDCapacitor);


}

ResCap_Conversion::~ResCap_Conversion()
{
    delete ui;
}


void ResCap_Conversion::updateSMDResistor() {
    QString code = ui->Resistor_Input_lineEdit->text();
    double baseValue = decodeSMDCode(code); // 算出是多少 Ohm


    int unitIdx = ui->Resistor_output_comboBox->currentIndex();
    double finalVal = baseValue;

    if (unitIdx == 1) finalVal = baseValue / 1000.0;       // kΩ
    else if (unitIdx == 2) finalVal = baseValue / 1000000.0; // MΩ

    ui->Resistor_output_lineEdit->setText(QString::number(finalVal, 'g', 6));
}


void ResCap_Conversion::updateSMDCapacitor() {
    QString code = ui->capacitor_Input_lineEdit->text();
    double baseValue = decodeSMDCode(code); // 算出是多少 pF

    int unitIdx = ui->capacitor_output_comboBox->currentIndex();
    double finalVal = baseValue;

    if (unitIdx == 1) finalVal = baseValue / 1000.0;       // nF
    else if (unitIdx == 2) finalVal = baseValue / 1000000.0; // μF

    ui->capacitor_output_lineEdit->setText(QString::number(finalVal, 'g', 6));
}



double ResCap_Conversion::decodeSMDCode(QString code) {
    if (code.isEmpty()) return 0;

    // 1. 處理帶有小數點代碼的情況
    // 電阻常用 R (4R7), 電容常用 p (4p7) 或 n (1n2)
    if (code.contains('R', Qt::CaseInsensitive) ||
        code.contains('P', Qt::CaseInsensitive) ||
        code.contains('N', Qt::CaseInsensitive)) {

        QString temp = code;
        temp.replace('R', '.', Qt::CaseInsensitive);
        temp.replace('P', '.', Qt::CaseInsensitive);
        temp.replace('N', '.', Qt::CaseInsensitive);
        return temp.toDouble();
    }

    // 2. 處理標準三位數代碼 (如 103)
    if (code.length() >= 3) {
        // 使用 last(1) 取代 right(1)，避免 Clazy 警告
        int exponent = code.last(1).toInt();

        // 使用 chopped(1) 取得除了最後一碼以外的部分，取代 left()，效能更好
        double prefix = code.chopped(1).toDouble();

        // 核心演算法：前兩位 * 10的(第三位)次方
        return prefix * std::pow(10, exponent);
    }

    return code.toDouble(); // 如果是兩位數，直接視為數值
}
