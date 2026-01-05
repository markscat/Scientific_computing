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



    // --- Tab 3 初始化 ---


    ui->R1_input_comboBox->addItems(handler->resistorUnits);
    ui->R2_input_comboBox->addItems(handler->resistorUnits);

    // 補上模式選項 (如果 UI Designer 裡沒填的話，要在這裡填)
    if(ui->calcMode_comboBox->count() == 0) {
        ui->calcMode_comboBox->addItems({"求輸出電壓 (Vo)", "求輸入電壓 (Vi)", "求上拉電阻 (R1)", "求下拉電阻 (R2)"});
    }

    // 連結訊號：所有輸入框改變時都要計算
    connect(ui->R1_Input_lineEdit, &QLineEdit::textChanged, this, &MainWindow::updateVoltageDivider);
    connect(ui->R1_input_comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::updateVoltageDivider);
    connect(ui->R2_Input_lineEdit, &QLineEdit::textChanged, this, &MainWindow::updateVoltageDivider);
    connect(ui->R2_input_comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::updateVoltageDivider);
    connect(ui->VI_Input_lineEdit, &QLineEdit::textChanged, this, &MainWindow::updateVoltageDivider);
    connect(ui->Vo_Input_lineEdit, &QLineEdit::textChanged, this, &MainWindow::updateVoltageDivider);


    // 【新增】補上 Vo 的連動，這樣反算 R1, R2, Vi 時才會觸發
    connect(ui->Vo_Input_lineEdit, &QLineEdit::textChanged, this, &MainWindow::updateVoltageDivider);

    // 連結模式切換訊號
    connect(ui->calcMode_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onVoltageModeChanged()));
    connect(ui->calcMode_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateVoltageDivider()));

    // 【新增】程式啟動時先執行一次，確保 UI 鎖定狀態正確
    onVoltageModeChanged();


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

void MainWindow::onVoltageModeChanged() {

    int mode = ui->calcMode_comboBox->currentIndex();

    // 先初始化所有框為「可編輯」與「白色背景」
    QLineEdit* edits[] = { ui->VI_Input_lineEdit, ui->Vo_Input_lineEdit,
                          ui->R1_Input_lineEdit, ui->R2_Input_lineEdit };
    for(auto e : edits) {
        e->setReadOnly(false);
        e->setStyleSheet("background-color: white;");
    }

    QString resStyle = "background-color: #eeeeee; color: blue; font-weight: bold;";

    // 使用 switch 處理四種模式的 UI 鎖定
    switch (mode) {
    case 0: // 求 Vo
        ui->Vo_Input_lineEdit->setReadOnly(true);
        ui->Vo_Input_lineEdit->setStyleSheet(resStyle);
        break;
    case 1: // 求 Vi
        ui->VI_Input_lineEdit->setReadOnly(true);
        ui->VI_Input_lineEdit->setStyleSheet(resStyle);
        break;
    case 2: // 求 R1
        ui->R1_Input_lineEdit->setReadOnly(true);
        ui->R1_Input_lineEdit->setStyleSheet(resStyle);
        break;
    case 3: // 求 R2
        ui->R2_Input_lineEdit->setReadOnly(true);
        ui->R2_Input_lineEdit->setStyleSheet(resStyle);
        break;
    default:
        break;
    }
}

void MainWindow::updateVoltageDivider() {

    if (isCalculating) return; // 如果正在計算中，就跳出，防止無窮迴圈
    isCalculating = true;

    int mode = ui->calcMode_comboBox->currentIndex();
    bool okVi, okVo, okR1, okR2;

    // 取得所有數值
    double Vi = ui->VI_Input_lineEdit->text().toDouble(&okVi);
    double Vo = ui->Vo_Input_lineEdit->text().toDouble(&okVo);
    double R1_val = ui->R1_Input_lineEdit->text().toDouble(&okR1);
    double R2_val = ui->R2_Input_lineEdit->text().toDouble(&okR2);

    // 換算電阻基準值 (Ohm)
    double R1_ohm = R1_val * std::pow(10, (ui->R1_input_comboBox->currentIndex() * 3));
    double R2_ohm = R2_val * std::pow(10, (ui->R2_input_comboBox->currentIndex() * 3));

    switch (mode) {
    case 0: // 求 Vo = Vi * (R2 / (R1 + R2))
        if (okVi && okR1 && okR2 && (R1_ohm + R2_ohm) != 0) {
            double res = Vi * (R2_ohm / (R1_ohm + R2_ohm));
            ui->Vo_Input_lineEdit->setText(QString::number(res, 'g', 6));
        }
        break;

    case 1: // 求 Vi = Vo * (R1 + R2) / R2
        if (okVo && okR1 && okR2 && R2_ohm != 0) {
            double res = Vo * (R1_ohm + R2_ohm) / R2_ohm;
            ui->VI_Input_lineEdit->setText(QString::number(res, 'g', 6));
        }
        break;

    case 2: // 求 R1 = R2 * (Vi - Vo) / Vo
        if (okVi && okVo && okR2 && Vo != 0) {
            double res_ohm = R2_ohm * (Vi - Vo) / Vo;
            double display = res_ohm / std::pow(10, (ui->R1_input_comboBox->currentIndex() * 3));
            ui->R1_Input_lineEdit->setText(QString::number(display, 'g', 6));
        }
        break;

    case 3: // 求 R2 = R1 * Vo / (Vi - Vo)
        if (okVi && okVo && okR1 && (Vi - Vo) != 0) {
            double res_ohm = R1_ohm * Vo / (Vi - Vo);
            double display = res_ohm / std::pow(10, (ui->R2_input_comboBox->currentIndex() * 3));
            ui->R2_Input_lineEdit->setText(QString::number(display, 'g', 6));
        }
        break;

    default:
        break;
    }

    isCalculating = false; // 計算結束，解鎖

}
