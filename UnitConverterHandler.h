#ifndef UNITCONVERTERHANDLER_H
#define UNITCONVERTERHANDLER_H

#include <QTableWidget>
#include <QStringList>
#include <vector>


// 定義一個簡單的結構來回傳計算結果
struct LEDResult {
    double resistance;
    double wattage;
    bool isVoltageOk;
};

class UnitConverterHandler {
public:
    // 定義範圍：pico (-12) 到 Mega (6)
    const QStringList units = {"p (10⁻¹²)", "n (10⁻⁹)", "u (10⁻⁶)", "m (10⁻³)", "1 (Base)", "K (10³)", "M (10⁶)"};
    const std::vector<int> exponents = {-12, -9, -6, -3, 0, 3, 6};

    // 專門負責填寫表格的函數
    void setupMatrixTable(QTableWidget* table);

    // 專門負責計算結果的函數
    double convert(double value, int sourceIdx, int targetIdx);


    // 在 public 加入：
    // 解析 SMD 代碼 (如 "103")，回傳基準單位數值 (電阻為 Ohm, 電容為 pF)
    double decodeSMDCode(QString code);

    // 專為 Tab 2 設計的單位清單
    const QStringList resistorUnits = {"Ω", "kΩ", "MΩ"};
    const QStringList capacitorUnits = {"pF", "nF", "μF"};

    // 新增：電流單位清單宣告
    const QStringList currentUnits = {"A", "mA", "uA"};

    // 新增：LED 計算邏輯宣告
    double calculateLEDResistor(double vSource, double vLed, double current, int currentUnitIdx);
    // 新增：處理串並聯的 LED 計算
    LEDResult calculateLEDComplex(double vcc, double vd, double current, int iUnitIdx, int series, int parallel);


};

#endif
