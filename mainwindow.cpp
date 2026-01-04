#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "UnitConverterHandler.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 【關鍵】一定要先 new 出來！
    handler = new UnitConverterHandler();

    // 建立處理器
    UnitConverterHandler *handler = new UnitConverterHandler();

    // 1. 填寫表格
    handler->setupMatrixTable(ui->matrixTable);

    // 2. 填寫下拉選單 (ComboBox)

    ui->Input_comboBox->addItems(handler->units);
    ui->output_comboBox->addItems(handler->units);

    // 預設單位設為 1 (Base)
    ui->Input_comboBox->setCurrentIndex(4);
    ui->output_comboBox->setCurrentIndex(4);

    // 3. 連結訊號：只要輸入框變了、或是選單變了，就執行計算
    connect(ui->Input_lineEdit, &QLineEdit::textChanged, this, &MainWindow::updateResult);
    connect(ui->Input_comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::updateResult);
    connect(ui->output_comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::updateResult);



    // --- Tab 2 初始化 ---
    // 填寫電阻單位
    ui->Resistor_output_comboBox_2->addItems(handler->resistorUnits);
    // 填寫電容單位
    ui->capacitor_output_comboBox->addItems(handler->capacitorUnits);

      // 連結電阻計算
    connect(ui->Resistor_Input_lineEdit, &QLineEdit::textChanged, this, &MainWindow::updateSMDResistor);
    connect(ui->Resistor_output_comboBox_2, &QComboBox::currentIndexChanged, this, &MainWindow::updateSMDResistor);

    // 連結電容計算
    connect(ui->capacitor_Input_lineEdit, &QLineEdit::textChanged, this, &MainWindow::updateSMDCapacitor);
    connect(ui->capacitor_output_comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::updateSMDCapacitor);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateResult() {
    // 防呆檢查：如果 handler 沒建立好，先不要計算
    if (!handler) return;



    bool ok;
    // 取得輸入的數值
    double inputVal = ui->Input_lineEdit->text().toDouble(&ok);

    if (!ok) {
        ui->Outputput_lineEdit->clear(); // 如果輸入不是數字，清空輸出
        return;
    }

    // 取得選中的單位索引
    int fromIdx = ui->Input_comboBox->currentIndex();
    int toIdx = ui->output_comboBox->currentIndex();

    // 呼叫我們寫好的處理器進行換算
    double result = handler->convert(inputVal, fromIdx, toIdx);

    // 顯示結果 (使用 'g' 格式，自動處理 0 且避免太長)
    ui->Outputput_lineEdit->setText(QString::number(result, 'g', 10));
}

void MainWindow::updateSMDResistor() {
    QString code = ui->Resistor_Input_lineEdit->text();
    double baseValue = handler->decodeSMDCode(code); // 算出是多少 Ohm

    int unitIdx = ui->Resistor_output_comboBox_2->currentIndex();
    double finalVal = baseValue;

    if (unitIdx == 1) finalVal = baseValue / 1000.0;       // kΩ
    else if (unitIdx == 2) finalVal = baseValue / 1000000.0; // MΩ

    ui->Resistor_output_lineEdit->setText(QString::number(finalVal, 'g', 6));
}


void MainWindow::updateSMDCapacitor() {
    QString code = ui->capacitor_Input_lineEdit->text();
    double baseValue = handler->decodeSMDCode(code); // 算出是多少 pF

    int unitIdx = ui->capacitor_output_comboBox->currentIndex();
    double finalVal = baseValue;

    if (unitIdx == 1) finalVal = baseValue / 1000.0;       // nF
    else if (unitIdx == 2) finalVal = baseValue / 1000000.0; // μF

    ui->capacitor_output_lineEdit->setText(QString::number(finalVal, 'g', 6));
}
