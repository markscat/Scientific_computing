#include "Line_Width.h"
#include "ui_Line_Width.h"

#include <cmath>

const double OZ_TO_MM = 0.0342867;


Line_Width::Line_Width(UnitConverterHandler *sharedHandler, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Line_Width),
    handler(sharedHandler) // 承接共用的邏輯處理器
{
    ui->setupUi(this);

    // --- 1. 初始化 ComboBox 選項 ---
    // 銅厚單位 (mm/mil/um)
    if(ui->thickness_comboBox->count() == 0) {
        ui->thickness_comboBox->addItems({"mm", "mil", "um"});
    }

    // 電流單位 (A/mA)
    if(ui->Current_comboBox->count() == 0) {
        ui->Current_comboBox->addItems({"A", "mA"});
    }

    // 長度單位 (mm/mil/cm)
    if(ui->Length_comboBox->count() == 0) {
        ui->Length_comboBox->addItems({"mm", "mil", "cm"});
    }

    // 寬度輸出單位 (mm/mil)
    QStringList distUnits = {"mm", "mil"};
    ui->External_comboBox->addItems(distUnits);
    ui->Internal_comboBox->addItems(distUnits);

    // 總結區單位
    ui->Impedance_comboBox->addItems({"mΩ", "Ω"});
    ui->VoltageDrop_comboBox->addItems({"mV", "V"});
    ui->Consumption_comboBox->addItems({"mW", "W"});

    // --- 2. 設定初始預設值 ---
    ui->Mass_lineEdit->setText("1");     // 預設 1 oz
    ui->temp_lineEdit->setText("10");    // 預設溫升 10 度
    ui->Length_lineEdit->setText("1");  // 預設長度 10mm
    ui->Current_lineEdit->setText("1");  // 預設 1A
    ui->thickness_lineEdit->setText(QString::number(OZ_TO_MM));

    connect(ui->Mass_lineEdit, &QLineEdit::textChanged, this, [this](const QString &text){
        bool ok;
        double oz = text.toDouble(&ok);
        if (ok) {
            // 使用者輸入 oz，我們只幫他換算成 mm 填進去
            ui->thickness_lineEdit->setText(QString::number(oz * 0.034287, 'g', 5));
        }
    });


    // --- 關鍵修改：使用 textEdited 區分誰是輸入源 ---
    connect(ui->Current_lineEdit, &QLineEdit::textEdited, this, &Line_Width::updateCalculation);
    connect(ui->External_lineEdit, &QLineEdit::textEdited, this, &Line_Width::updateCalculation);
    connect(ui->Internal_lineEdit, &QLineEdit::textEdited, this, &Line_Width::updateCalculation);

    // 3. 核心連動：所有會觸發「演算法」的訊號
    // 只要 mm 框變了，或是電流、長度、單位變了，就執行計算
    QList<QLineEdit*> inputs = { ui->thickness_lineEdit,
                                 ui->temp_lineEdit, ui->Length_lineEdit };
    for(auto edit : inputs)
        connect(edit, &QLineEdit::textChanged, this, &Line_Width::updateCalculation);

    QList<QComboBox*> boxes = { ui->thickness_comboBox, ui->Current_comboBox,
                                ui->Length_comboBox, ui->External_comboBox,
                                ui->Internal_comboBox, ui->Impedance_comboBox,
                                ui->VoltageDrop_comboBox, ui->Consumption_comboBox };
    for(auto box : boxes)
        connect(box, &QComboBox::currentIndexChanged, this, &Line_Width::updateCalculation);

}

Line_Width::~Line_Width()
{
    delete ui;
}

void Line_Width::updateCalculation() {
    if (isCalculating) return;
    isCalculating = true;

    bool okI, okT, okL, okDelta;

    double current = ui->Current_lineEdit->text().toDouble(&okI);
    if (ui->Current_comboBox->currentIndex() == 1) current /= 1000.0; // mA -> A

    //double deltaT = ui->temp_lineEdit->text().toDouble(&okDelta);
    double deltaT = ui->temp_lineEdit->text().toDouble();
    double length = ui->Length_lineEdit->text().toDouble(&okL);

    // 取得銅厚並轉換為 mm
    //double rawT = ui->thickness_lineEdit->text().toDouble(&okT);
    double rawT = ui->thickness_lineEdit->text().toDouble();

    double thickness_mm = rawT;
    if (ui->thickness_comboBox->currentIndex() == 1){
        thickness_mm = rawT * 0.0254; // mil -> mm
    }
    else if (ui->thickness_comboBox->currentIndex() == 2) {
        thickness_mm = rawT / 1000.0; // um -> mm
    }

    //double thickness_mm = ui->thickness_lineEdit->text().toDouble(&okT);
    double thickness_mil = thickness_mm / 0.0254;

    if (deltaT <= 0 || thickness_mil <= 0){
        isCalculating = false; return;
    }

    // --- 核心邏輯：判斷是誰觸發的 ---
    QObject* s = sender();

    if (s == ui->External_lineEdit) {
        // A. 如果使用者在改【外層寬度】 -> 反推電流
        double wExt = ui->External_lineEdit->text().toDouble();
        if (ui->External_comboBox->currentIndex() == 1){
            wExt *= 0.0254; // mil to mm
            }
        double area_mil2 = (wExt / 0.0254) * thickness_mil;
        // 逆公式: I = 0.048 * dT^0.44 * Area^0.725
        current = 0.048 * std::pow(deltaT, 0.44) * std::pow(area_mil2, 0.725);

        // 更新電流框
        double dispI = (ui->Current_comboBox->currentIndex() == 1) ? current * 1000 : current;
        ui->Current_lineEdit->setText(QString::number(dispI, 'g', 5));

    } else if (s == ui->Internal_lineEdit) {
        // B. 如果使用者在改【內層寬度】 -> 反推電流
        double wInt = ui->Internal_lineEdit->text().toDouble();
        if (ui->Internal_comboBox->currentIndex() == 1) wInt *= 0.0254;
        double area_mil2 = (wInt / 0.0254) * thickness_mil;
        current = 0.024 * std::pow(deltaT, 0.44) * std::pow(area_mil2, 0.725);

        double dispI = (ui->Current_comboBox->currentIndex() == 1) ? current * 1000 : current;
        ui->Current_lineEdit->setText(QString::number(dispI, 'g', 5));
    } else {
        // C. 其他情況 (改電流、改溫升、改銅厚) -> 正常算線寬
        current = ui->Current_lineEdit->text().toDouble();
        if (ui->Current_comboBox->currentIndex() == 1) current /= 1000.0;
    }


    // --- 最後：根據最終產出的 current，更新所有結果欄位 ---
    auto calcWidth = [&](double k) {
        double area = std::pow((current / (k * std::pow(deltaT, 0.44))), (1.0 / 0.725));
        return (area / thickness_mil) * 0.0254; // mm
    };


    double widthExt_mm = (s == ui->External_lineEdit) ?
                             (ui->External_lineEdit->text().toDouble() *
                             (ui->External_comboBox->currentIndex()==1?0.0254:1.0)) : calcWidth(0.048);
    double widthInt_mm = (s == ui->Internal_lineEdit) ?
                             (ui->Internal_lineEdit->text().toDouble() *
                             (ui->Internal_comboBox->currentIndex()==1?0.0254:1.0)) : calcWidth(0.024);


    //double finalExt = calcWidth(0.048);
    //double finalInt = calcWidth(0.024);

    if (s != ui->External_lineEdit) {
        double out = (ui->External_comboBox->currentIndex() == 1) ? widthExt_mm / 0.0254 : widthExt_mm;
        ui->External_lineEdit->setText(QString::number(out, 'f', 4));
    }
    if (s != ui->Internal_lineEdit) {
        double out = (ui->Internal_comboBox->currentIndex() == 1) ? widthInt_mm / 0.0254 : widthInt_mm;
        ui->Internal_lineEdit->setText(QString::number(out, 'f', 4));
    }

        // --- 2. 電阻/壓降/功耗計算 (以外層寬度為例) ---
        if (okL && length > 0) {
            // 長度統一換算為 cm (因為銅電阻率常用單位是 Ohm-cm)
            double len_cm = length;
            int lenIdx = ui->Length_comboBox->currentIndex();
            if (lenIdx == 0) len_cm = length / 10.0;      // mm -> cm
            else if (lenIdx == 1) len_cm = length * 0.00254; // mil -> cm
            else if (lenIdx == 2) len_cm = length;        // cm -> cm

            // 銅電阻率 (20°C) 約 1.72e-6 Ohm-cm
            // 考慮溫升後的電阻率 ρ = ρ0 * (1 + α * ΔT), 銅的 α 約 0.00393
            double res_at_temp = 1.72e-6 * (1 + 0.00393 * (deltaT));

            // R = (ρ * L) / A  (注意單位：L是cm, Area要用 cm^2)
            double area_cm2 = (widthExt_mm * 0.1) * (thickness_mm * 0.1);
            double res_ohm = (res_at_temp * len_cm) / area_cm2;

            // 更新電阻
            double dispRes = (ui->Impedance_comboBox->currentIndex() == 0) ? res_ohm * 1000 : res_ohm;
            ui->Impedance_lineEdit->setText(QString::number(dispRes, 'g', 5));

            // 壓降 V = I * R
            double vDrop = current * res_ohm;
            double dispV = (ui->VoltageDrop_comboBox->currentIndex() == 0) ? vDrop * 1000 : vDrop;
            ui->VoltageDrop_lineEdit->setText(QString::number(dispV, 'g', 5));

            // 功耗 P = I^2 * R
            double pLoss = current * current * res_ohm;
            double dispP = (ui->Consumption_comboBox->currentIndex() == 0) ? pLoss * 1000 : pLoss;
            ui->Consumption_lineEdit->setText(QString::number(dispP, 'g', 5));
        }

    isCalculating = false;

}
