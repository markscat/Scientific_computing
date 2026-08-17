#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./inc/UnitConverterHandler.h"
#include "./inc/ledcurrentlimit.h"
#include "./inc/Voltage_Divider.h"
#include "./inc/ResCap_Conversion.h"
#include "./inc/Line_Width.h"
#include "./inc/via_current_cal.h"
#include "./inc/opa_analysis.h"

#include <QVBoxLayout>
#include <QMessageBox> // 記得在檔案最上方 include

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 【關鍵】一定要先 new 出來！
    handler = new UnitConverterHandler();

    // 建立處理器
    UnitConverterHandler *handler = new UnitConverterHandler();


    /*tab begin*/

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


    /*尺寸換算_begin*/

    // 連接長度換算的信號
    connect(ui->unit_of_length_Input_lineEdit, &QLineEdit::textChanged, this, &MainWindow::updateLengthConversion);
    connect(ui->unit_of_length_input_comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::updateLengthConversion);
    connect(ui->unit_of_length_output_comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::updateLengthConversion);

    // 設定長度單位下拉選單
    QStringList lengthUnits = {"inch", "cm", "mil", "mm"};
    ui->unit_of_length_input_comboBox->addItems(lengthUnits);
    ui->unit_of_length_output_comboBox->addItems(lengthUnits);
    ui->unit_of_length_input_comboBox->setCurrentIndex(3); // 預設 mm
    ui->unit_of_length_output_comboBox->setCurrentIndex(0); // 預設 inch

    /*尺寸換算_end*/

    /*功率計算_begin*/
    //V_Input_lineEdit
    //I_Input_lineEdit
    //R_Input_lineEdit

    //V_input_comboBox
    //Amp_input_comboBox
    //Ohm_input_comboBox
    //Power_Input_comboBox


    ui->V_input_comboBox->clear(); // 務必先清空，確保 Index 0 就是 mV
    ui->V_input_comboBox->addItem("mV", 0.001);
    ui->V_input_comboBox->addItem("V", 1.0);
    ui->V_input_comboBox->addItem("kV", 1000.0);

    //connect(ui->V_Input_lineEdit, &QLineEdit::textEdited, this, &MainWindow::updatePowerConversion);
    //connect(ui->V_input_comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::updatePowerConversion);


    ui->Amp_input_comboBox->clear(); // 務必先清空，確保 Index 0 就是 mA
    ui->Amp_input_comboBox->addItem("mA", 0.001);
    ui->Amp_input_comboBox->addItem("A", 1.0);
    ui->Amp_input_comboBox->addItem("kA", 1000.0);

    //connect(ui->I_Input_lineEdit, &QLineEdit::textEdited, this, &MainWindow::updatePowerConversion);
    //connect(ui->Amp_input_comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::updatePowerConversion);

    ui->Ohm_input_comboBox->clear(); // 務必先清空，確保 Index 0 就是 mOHM
    ui->Ohm_input_comboBox->addItem("mohm", 0.001);
    ui->Ohm_input_comboBox->addItem("ohm", 1.0);
    ui->Ohm_input_comboBox->addItem("kohm", 1000.0);

    //connect(ui->R_Input_lineEdit, &QLineEdit::textEdited, this, &MainWindow::updatePowerConversion);
    //connect(ui->Ohm_input_comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::updatePowerConversion);


    ui->Power_Input_comboBox->clear(); // 務必先清空，確保 Index 0 就是 mW
    ui->Power_Input_comboBox->addItem("mW", 0.001);
    ui->Power_Input_comboBox->addItem("W", 1.0);
    ui->Power_Input_comboBox->addItem("kW", 1000.0);

    //connect(ui->Power_Input_lineEdit, &QLineEdit::textEdited, this, &MainWindow::updatePowerConversion);
    //connect(ui->Power_Input_comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::updatePowerConversion);

    connect(ui->Enter_pushButton, &QPushButton::clicked, this, &MainWindow::updatePowerConversion);


    /*功率計算_end*/

    /*Tab end*/

    // --- Tab 2 初始化 ---

    // 填寫電阻單位
    // 建立一個垂直佈局，放在 MainWindow UI 的 tab_2 裡面
    QVBoxLayout *RClayout = new QVBoxLayout(ui->tab_2);

    // 建立你的模組實例，並把 handler 傳進去
    ResCap_Conversion *RCpage = new ResCap_Conversion(handler, this);

    // 把這個模組加進佈局中
    RClayout->addWidget(RCpage);

    // 設定佈局邊距（設為 0 會比較緊湊，看起來像原生分頁）
    RClayout->setContentsMargins(0, 0, 0, 0);

    // --- Tab 3 (電阻分壓)---

    // 建立一個垂直佈局，放在 MainWindow UI 的 tab_3 裡面
    QVBoxLayout *VDpagelayout = new QVBoxLayout(ui->tab_3);

    // 建立你的模組實例，並把 handler 傳進去
    Voltage_Divider *VDpage = new Voltage_Divider(handler, this);

    // 把這個模組加進佈局中
    VDpagelayout->addWidget(VDpage);

    // 設定佈局邊距（設為 0 會比較緊湊，看起來像原生分頁）
    VDpagelayout->setContentsMargins(0, 0, 0, 0);

    //--- Tab 3 End ---

    // --- Tab 4 (LED 限流電阻) ---

    // 建立一個垂直佈局，放在 MainWindow UI 的 tab_4 裡面
    QVBoxLayout *LEDlayout = new QVBoxLayout(ui->tab_4);

    // 4. 建立你的 LED 模組實例，並把 handler 傳進去
    LED_current_limit *ledPage = new LED_current_limit(handler, this);

    // 5. 把這個模組加進佈局中
    LEDlayout->addWidget(ledPage);

    // 6. 設定佈局邊距（設為 0 會比較緊湊，看起來像原生分頁）
    LEDlayout->setContentsMargins(0, 0, 0, 0);
    //--- Tab 4 End ---


    // --- Tab 5 (走線電流設計) ---


    // 建立一個垂直佈局，放在 MainWindow UI 的 tab_5 裡面
    QVBoxLayout *LineWidth_layout = new QVBoxLayout(ui->tab_5);

    // 4. 建立你的 LED 模組實例，並把 handler 傳進去
    Line_Width *LineWidth_Page = new Line_Width(handler, ui->tab_5);

    // 5. 把這個模組加進佈局中
    LineWidth_layout->addWidget(LineWidth_Page);

    // 6. 設定佈局邊距（設為 0 會比較緊湊，看起來像原生分頁）
    LineWidth_layout->setContentsMargins(0, 0, 0, 0);

    ui->tab_5->setLayout(LineWidth_layout);

    //--- Tab 5 End ---


    // --- Tab 6 (貫孔電流設計) ---
    // 建立一個垂直佈局，放在 MainWindow UI 的 tab_5 裡面
    QVBoxLayout *Viacurrent_layout = new QVBoxLayout(ui->tab_6);

    // 4. 建立你的 貫孔電流計算 實例，並把 handler 傳進去
    Via_Current_cal *Viacurrent_Page = new Via_Current_cal(handler, ui->tab_6);

    // 5. 把這個模組加進佈局中
    Viacurrent_layout->addWidget(Viacurrent_Page);

    // 6. 設定佈局邊距（設為 0 會比較緊湊，看起來像原生分頁）
    Viacurrent_layout->setContentsMargins(0, 0, 0, 0);

    ui->tab_6->setLayout(Viacurrent_layout);

    //--- Tab 6 End ---

    // --- Tab 7 (運算放大器基本計算) ---
    // 建立一個垂直佈局，放在 MainWindow UI 的 tab_7 裡面
    QVBoxLayout *OPA_layout = new QVBoxLayout(ui->tab_7);

    // 2. 建立實例 (注意這裡加上了 OPA_Analysis* 宣告，並傳入 handler 和指定 tab_7 為 parent)
    // 假設您的 OPA_Analysis 建構式有支援傳入 handler
    OPA_Analysis *opaPage = new OPA_Analysis(handler, ui->tab_7);

    // 建立 OPA 模組實例 (注意：若不需要 handler，傳 this 即可)
    //opaPage = new OPA_Analysis(this);

    // 把這個模組加進佈局中
    OPA_layout->addWidget(opaPage);

    // 設定佈局邊距
    OPA_layout->setContentsMargins(0, 0, 0, 0);

    // 將佈局設定給 tab_7
    ui->tab_7->setLayout(OPA_layout);

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

void MainWindow::on_actionAbout_triggered()
{
    // 使用 QMessageBox 的靜態函數 about
    QMessageBox::about(this,
                       tr("關於本軟體"),
                       tr("<h3>硬體設計 工具箱 V1.0</h3>"
                          "<p>版權所有 © 2024 Ethan Yang (Markscat)</p>"
                          "<p>本程式提供 :<br/>"
                          "1. 科學記號的轉換<br/>"
                          "2. 電阻和電容編碼換算<br/>"
                          "3. 電阻分壓計算<br/>"
                          "4. LED限流電阻計算<br/>"
                          "5. PCB 走線電流計算及單位換算功能。<br/>"
                          "6. PCB貫孔電流計算</p>"
                          "<p>公式參考：IPC-2221 標準。</p>"
                          "有興趣討論的話,請發郵件給我"
                          "<a href='mailto:markscat@gmail.com'>markscat@gmail.com</a></p>"


           "<p>本程式為自由軟體；您可以根據自由軟體基金會所發佈的 "
           "GNU 通用公共許可證 (GNU General Public License) 條款，"
           "對其進行重新分發和/或修改；無論您使用的是許可證的第三版，"

           "<p>發佈此程式是希望它能發揮作用，但<b>不提供任何保證</b>；"
           "甚至不包含對<b>適銷性</b>或<b>特定用途適用性</b>的暗示性保證。"
           "詳情請參閱 GNU 通用公共許可證。</p>"
           "<p>請參閱 <a href='https://www.gnu.org/licenses/'>https://www.gnu.org/licenses/</a>。</p>"));
}

void MainWindow::updateLengthConversion() {
    bool ok;
    double inputVal = ui->unit_of_length_Input_lineEdit->text().toDouble(&ok);

    if (!ok) {
        ui->unit_of_length_output_lineEdit->clear();
        return;
    }

    // 獲取單位索引
    /* fromIdx,toIdx 結果為0到3的整數值,用來對應mm,cm,inch,mil
     *
     * 轉換倍率表：每個單位換算成 mm 的倍率
     * 順序: inch, cm, mil, mm
     *
     */
    int fromIdx = ui->unit_of_length_input_comboBox->currentIndex();
    int toIdx = ui->unit_of_length_output_comboBox->currentIndex();

    // 所有單位轉換到 mm 的轉換因子
    // inch -> mm: *25.4, cm -> mm: *10, mil -> mm: *0.0254, mm -> mm: *1
    double toMM[] = {25.4, 10.0, 0.0254, 1.0};
    //                   ↑     ↑      ↑     ↑
    //            1 inch  1 cm  1 mil  1 mm
    //            =25.4mm =10mm =0.0254mm =1mm

    // 先轉換成 mm

    // 步驟1: 不管輸入什麼單位，先統統轉換成 mm
    // 例如：輸入 2 inch → 2 × 25.4 = 50.8 mm
    // 例如：輸入 100 mil → 100 × 0.0254 = 2.54 mm
    // 例如：輸入 5 cm → 5 × 10.0 = 50 mm

    double valueInMM = inputVal * toMM[fromIdx];

    // 再從 mm 轉換到目標單位

    // 步驟2: 從 mm 轉換到目標單位
    // 例如：50.8 mm → 要轉成 cm → 50.8 ÷ 10.0 = 5.08 cm
    // 例如：2.54 mm → 要轉成 inch → 2.54 ÷ 25.4 = 0.1 inch
    // 例如：50 mm → 要轉成 mm → 50 ÷ 1.0 = 50 mm (不變)
    double result = valueInMM / toMM[toIdx];

    ui->unit_of_length_output_lineEdit->setText(QString::number(result, 'g', 10));
}

void MainWindow::updatePowerConversion(){

    bool V_ok,I_ok,R_ok,P_ok;

    double V_inputVal = (ui->V_Input_lineEdit->text().toDouble(&V_ok)) * (ui->V_input_comboBox->currentData().toDouble());
    double I_inputVal = (ui->I_Input_lineEdit->text().toDouble(&I_ok)) * (ui->Amp_input_comboBox->currentData().toDouble());
    double R_inputVal = (ui->R_Input_lineEdit->text().toDouble(&R_ok)) * (ui->Ohm_input_comboBox->currentData().toDouble());
    double P_inputVal = (ui->Power_Input_lineEdit->text().toDouble(&P_ok)) * (ui->Power_Input_comboBox->currentData().toDouble());


    // 2. 檢查是否至少有兩個輸入
    int count = (V_ok ? 1 : 0) + (I_ok ? 1 : 0) + (R_ok ? 1 : 0) + (P_ok ? 1 : 0);

    if (count < 2) {
        QMessageBox::warning(this, "提示", "請至少輸入兩個參數以進行計算。");
        return;
    }


    for (int i = 0; i < 2; ++i) {
        // V = I * R
        if (!V_ok && I_ok && R_ok) { V_inputVal = I_inputVal * R_inputVal; V_ok = true; }
        if (!I_ok && V_ok && R_ok && R_inputVal != 0) { I_inputVal = V_inputVal / R_inputVal; I_ok = true; }
        if (!R_ok && V_ok && I_ok && I_inputVal != 0) { R_inputVal = V_inputVal / I_inputVal; R_ok = true; }

        // P = V * I
        if (!P_ok && V_ok && I_ok) { P_inputVal = V_inputVal * I_inputVal; P_ok = true; }
        if (!V_ok && P_ok && I_ok && I_inputVal != 0) { V_inputVal = P_inputVal / I_inputVal; V_ok = true; }
        if (!I_ok && P_ok && V_ok && V_inputVal != 0) { I_inputVal = P_inputVal / V_inputVal; I_ok = true; }

        // P = I^2 * R (補充公式，處理只有 P 和 R 的特殊情況)
        if (!I_ok && P_ok && R_ok && R_inputVal > 0) { I_inputVal = std::sqrt(P_inputVal / R_inputVal); I_ok = true; }
        if (!V_ok && P_ok && R_ok && R_inputVal > 0) { V_inputVal = std::sqrt(P_inputVal * R_inputVal); V_ok = true; }
    }


    // 輸出時，要把標準單位的數值「除回」當前選中的單位倍率
    if (V_ok) ui->V_Input_lineEdit->setText(QString::number(V_inputVal / ui->V_input_comboBox->currentData().toDouble(), 'g', 6));
    if (I_ok) ui->I_Input_lineEdit->setText(QString::number(I_inputVal / ui->Amp_input_comboBox->currentData().toDouble(), 'g', 6));
    if (R_ok) ui->R_Input_lineEdit->setText(QString::number(R_inputVal / ui->Ohm_input_comboBox->currentData().toDouble(), 'g', 6));
    if (P_ok) ui->Power_Input_lineEdit->setText(QString::number(P_inputVal / ui->Power_Input_comboBox->currentData().toDouble(), 'g', 6));
}


void MainWindow::on_clearButton_clicked() {
    ui->V_Input_lineEdit->clear();
    ui->I_Input_lineEdit->clear();
    ui->R_Input_lineEdit->clear();
    ui->Power_Input_lineEdit->clear();
}

