#include "Voltage_Divider.h"
#include "ui_Voltage_Divider.h"



Voltage_Divider::Voltage_Divider(UnitConverterHandler *sharedHandler, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Voltage_Divider),
    handler(sharedHandler) // 承接共用的邏輯處理器
{
    ui->setupUi(this);

    ui->R1_input_comboBox->addItems(handler->resistorUnits);
    ui->R2_input_comboBox->addItems(handler->resistorUnits);

    // 補上模式選項 (如果 UI Designer 裡沒填的話，要在這裡填)
    if(ui->calcMode_comboBox->count() == 0) {
        ui->calcMode_comboBox->addItems({"求輸出電壓 (Vo)", "求輸入電壓 (Vi)", "求上拉電阻 (R1)", "求下拉電阻 (R2)"});
    }

    // 連結訊號：所有輸入框改變時都要計算
    connect(ui->R1_Input_lineEdit, &QLineEdit::textChanged, this, &Voltage_Divider::updateVoltageDivider);
    connect(ui->R1_input_comboBox, &QComboBox::currentIndexChanged, this, &Voltage_Divider::updateVoltageDivider);
    connect(ui->R2_Input_lineEdit, &QLineEdit::textChanged, this, &Voltage_Divider::updateVoltageDivider);
    connect(ui->R2_input_comboBox, &QComboBox::currentIndexChanged, this, &Voltage_Divider::updateVoltageDivider);
    connect(ui->VI_Input_lineEdit, &QLineEdit::textChanged, this, &Voltage_Divider::updateVoltageDivider);
    connect(ui->Vo_Input_lineEdit, &QLineEdit::textChanged, this, &Voltage_Divider::updateVoltageDivider);


    // 【新增】補上 Vo 的連動，這樣反算 R1, R2, Vi 時才會觸發
    connect(ui->Vo_Input_lineEdit, &QLineEdit::textChanged, this, &Voltage_Divider::updateVoltageDivider);

    // 連結模式切換訊號
    connect(ui->calcMode_comboBox, &QComboBox::currentIndexChanged, this, &Voltage_Divider::onVoltageModeChanged);
    connect(ui->calcMode_comboBox, &QComboBox::currentIndexChanged, this, &Voltage_Divider::updateVoltageDivider);
    /*
    connect(ui->calcMode_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onVoltageModeChanged()));
    connect(ui->calcMode_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateVoltageDivider()));*/

    // 【新增】程式啟動時先執行一次，確保 UI 鎖定狀態正確
    onVoltageModeChanged();
}

Voltage_Divider::~Voltage_Divider()
{
    delete ui;
}



void Voltage_Divider::onVoltageModeChanged() {

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

void Voltage_Divider::updateVoltageDivider() {

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

