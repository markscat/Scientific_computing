#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "UnitConverterHandler.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateResult() {
    bool ok;
    // 取得輸入的數值
    double inputVal = ui->Input_lineEdit->text().toDouble(&ok);

    if (!ok) {
        ui->Outputput_lineEdit->setText("---");
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
