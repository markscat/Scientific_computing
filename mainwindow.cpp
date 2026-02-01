#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "UnitConverterHandler.h"
#include "ledcurrentlimit.h"
#include "Voltage_Divider.h"
#include "ResCap_Conversion.h"
#include "Line_Width.h"
#include "via_current_cal.h"

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

    ui->tab_5->setLayout(Viacurrent_layout);

    //--- Tab 5 End ---




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

