#include "via_current_cal.h"
#include "ui_via_current_cal.h"

Via_Current_cal::Via_Current_cal(UnitConverterHandler *h, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Via_Current_cal),
    handler(h)
{
    ui->setupUi(this);

    initUI();

    // 連結輸入訊號：只要任何數值改變，就重新計算
    // 參數設定區
    connect(ui->Current_lineEdit, &QLineEdit::textChanged, this, &Via_Current_cal::onInputsChanged);
    connect(ui->temp_lineEdit, &QLineEdit::textChanged, this, &Via_Current_cal::onInputsChanged);
    connect(ui->BoardThickness, &QLineEdit::textChanged, this, &Via_Current_cal::onInputsChanged);
    connect(ui->ViaDiameter, &QLineEdit::textChanged, this, &Via_Current_cal::onInputsChanged);
    connect(ui->HoleWallThickness, &QLineEdit::textChanged, this, &Via_Current_cal::onInputsChanged);

    // 下拉選單改變
    connect(ui->Current_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Via_Current_cal::onInputsChanged);
    connect(ui->BoardThickness_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Via_Current_cal::onInputsChanged);
    connect(ui->ViaDiameter_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Via_Current_cal::onInputsChanged);
    connect(ui->HoleWallThickness_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Via_Current_cal::onInputsChanged);

    // 銅厚連動
    connect(ui->Mass_lineEdit, &QLineEdit::textChanged, this, &Via_Current_cal::onCopperMassChanged);
    connect(ui->thickness_lineEdit, &QLineEdit::textChanged, this, &Via_Current_cal::onCopperThicknessChanged);

}

Via_Current_cal::~Via_Current_cal()
{
    delete ui;
}


void Via_Current_cal::initUI()
{
    // 1. 電流單位：手動加入 A 和 mA
    ui->Current_comboBox->clear();
    ui->Current_comboBox->addItems({"A", "mA"});
    ui->Current_comboBox->setCurrentIndex(0); // 預設 A

    // 2. 長度單位：從 handler 獲取 (假設你的 handler->units 包含 mm, mil, um)
    // 為了精確，建議直接手動設定，確保 index 與 handler 內部邏輯一致
    QStringList lengthUnits = {"um", "mm", "mil"};
    // 注意：這裡的清單順序必須跟你的 UnitConverterHandler 內部的陣列順序一模一樣

    auto setupLenBox = [&](QComboBox* cb, int defaultIdx) {
        cb->clear();
        cb->addItems(lengthUnits); // 直接用 handler 的清單
        cb->setCurrentIndex(defaultIdx);
    };

    // 初始化單位選單 (假設 handler->units 包含 mm, mil, um 等)
    // 索引對應：0 = um, 1 = mm, 2 = mil
    setupLenBox(ui->BoardThickness_comboBox,1);
    setupLenBox(ui->ViaDiameter_comboBox,1);
    setupLenBox(ui->HoleWallThickness_comboBox,0);
    setupLenBox(ui->thickness_comboBox,0);

    // 設定結果欄位為唯讀
    ui->ViaImpedance_lineEdit->setReadOnly(true);
    ui->ViaVoltageDrop_lineEdit->setReadOnly(true);
    ui->ViaConsumption_lineEdit->setReadOnly(true);

    // 設定背景顏色讓使用者知道這是唯讀輸出區
    QString resStyle = "background-color: #f0f0f0; font-weight: bold; color: #00008b;";
    ui->ViaImpedance_lineEdit->setStyleSheet(resStyle);
    ui->ViaVoltageDrop_lineEdit->setStyleSheet(resStyle);
    ui->ViaConsumption_lineEdit->setStyleSheet(resStyle);

    // 預設初值
    ui->temp_lineEdit->setText("10"); // 預設溫升 10度
    ui->HoleWallThickness->setText("20"); // 預設孔壁 20um (約 0.8 mil)
}

// 當銅重量 (oz) 改變時，自動更新厚度 (um)
void Via_Current_cal::onCopperMassChanged()
{
    if (isUpdating) return;
    isUpdating = true;

    bool ok;
    double oz = ui->Mass_lineEdit->text().toDouble(&ok);
    if (ok) {
        // 1oz = 35um
        double um = oz * 35.0;
        ui->thickness_lineEdit->setText(QString::number(um));
        // 同步更新孔壁厚度 (實務上孔壁通常比表面薄，這裡預設同步以便操作)
        ui->HoleWallThickness->setText(QString::number(um * 0.7)); // 假設孔壁是表面的 70%
    }

    isUpdating = false;
    onInputsChanged();
}

// 當厚度 (um) 改變時，自動更新重量 (oz)
void Via_Current_cal::onCopperThicknessChanged()
{
    if (isUpdating) return;
    isUpdating = true;

    bool ok;
    double um = ui->thickness_lineEdit->text().toDouble(&ok);
    if (ok) {
        double oz = um / 35.0;
        ui->Mass_lineEdit->setText(QString::number(oz, 'g', 3));
    }

    isUpdating = false;
    onInputsChanged();
}

void Via_Current_cal::onInputsChanged()
{
    bool ok;
    // 1. 取得輸入並統一轉為標準單位 (mm)

    // --- 1. 取得並處理電流 (處理 A / mA 轉換) ---
    double rawI = ui->Current_lineEdit->text().toDouble(&ok);
    if (!ok) rawI = 0;
    double i_input = (ui->Current_comboBox->currentIndex() == 1) ? (rawI / 1000.0) : rawI; // 轉為 Amps


    // --- 2. 取得溫升 ---
    double deltaT = ui->temp_lineEdit->text().toDouble(&ok);
    if (!ok || deltaT <= 0) deltaT = 10.0; // 預設溫升 10 度

    //double i_input = ui->Current_lineEdit->text().toDouble(&ok);
    //double deltaT = ui->temp_lineEdit->text().toDouble();

    // --- 3. 長度換算輔助 Lambda (統一轉成 mm) ---
    // 索引對應：0=um, 1=mm, 2=mil
    auto getValInMM = [&](QLineEdit* edit, QComboBox* combo) -> double {
        double val = edit->text().toDouble();
        int idx = combo->currentIndex();
        if (idx == 0) return val / 1000.0;    // um -> mm
        if (idx == 1) return val;             // mm -> mm
        if (idx == 2) return val * 0.0254;    // mil -> mm
        return val;
    };

    double boardL_mm = getValInMM(ui->BoardThickness, ui->BoardThickness_comboBox);
    double viaD_mm   = getValInMM(ui->ViaDiameter, ui->ViaDiameter_comboBox);
    double wallT_mm  = getValInMM(ui->HoleWallThickness, ui->HoleWallThickness_comboBox);

  /*
    double boardL_mm = handler->convert(ui->BoardThickness->text().toDouble(), ui->BoardThickness_comboBox->currentIndex(), 4);
    double viaD_mm = handler->convert(ui->ViaDiameter->text().toDouble(), ui->ViaDiameter_comboBox->currentIndex(), 4);
    double wallT_mm = handler->convert(ui->HoleWallThickness->text().toDouble(), ui->HoleWallThickness_comboBox->currentIndex(), 4);
*/
    //if (viaD_mm <= 0 || wallT_mm <= 0 || boardL_mm <= 0) return;

    if (viaD_mm <= 0 || wallT_mm <= 0 || boardL_mm <= 0) {
        clearResults();
        return;
    }


    // 2. 計算截面積 (Area) - 圓柱管攤平長方形
    // Area = PI * (D + t) * t
    double area_mm2 = M_PI * (viaD_mm + wallT_mm) * wallT_mm;

    // 轉換為 sq mil (IPC 公式標準單位)
    double area_sqMil = area_mm2 * 1550.0031;

    // 3. IPC-2221 公式計算最大許可電流 (I_max)
    // I = k * (deltaT^0.44) * (Area^0.725)
    // k 取 0.048 (對應外層/貫孔)
    double i_max = 0.048 * qPow(deltaT, 0.44) * qPow(area_sqMil, 0.725);

    // 4. 計算電阻 (Resistance)
    // 銅電阻率 @ 20C = 1.72e-8 Ohm-m = 1.72e-5 Ohm-mm
    double rho_20 = 1.724e-5;
    // 考慮溫度補償 (預設環境 25C + 溫升)
    double temp_final = 25.0 + deltaT;
    double rho_hot = rho_20 * (1.0 + 0.00393 * (temp_final - 20.0));

    double resistance = rho_hot * (boardL_mm / area_mm2); // Ohm

    // 5. 計算壓降與功耗 (基於使用者輸入的電流)
    double v_drop = i_input * resistance;
    double p_loss = i_input * i_input * resistance;

    // 6. 顯示結果
    // 電阻顯示為 mOhm
    ui->ViaImpedance_lineEdit->setText(QString::number(resistance * 1000.0, 'f', 2));

    // 壓降與功耗
    ui->ViaVoltageDrop_lineEdit->setText(QString::number(v_drop, 'f', 4));
    ui->ViaConsumption_lineEdit->setText(QString::number(p_loss, 'f', 4));

    // 如果輸入電流超過 I_max，改變文字顏色提醒 (選做)
    if (i_input > i_max) {
        ui->ViaConsumption_lineEdit->setStyleSheet("color: red; font-weight: bold;");
    } else {
        ui->ViaConsumption_lineEdit->setStyleSheet("");
    }
}

void Via_Current_cal::clearResults() {
    ui->ViaImpedance_lineEdit->clear();
    ui->ViaVoltageDrop_lineEdit->clear();
    ui->ViaConsumption_lineEdit->clear();
}
