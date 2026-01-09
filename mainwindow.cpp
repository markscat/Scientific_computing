#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "UnitConverterHandler.h"
#include "ledcurrentlimit.h"
#include "Voltage_Divider.h"
#include "ResCap_Conversion.h"
#include "Line_Width.h"

#include <QVBoxLayout>

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
