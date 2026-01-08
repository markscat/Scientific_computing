/**
 * @file UnitConverterHandler.cpp
 * @brief 科學單位換算處理器 - 核心邏輯實現
 *
 * 【 1. 目的與功能 】
 * 本類別旨在提供電子工程（如 PCB 走線電流、貫孔容量計算）中常見的科學單位換算。
 * 涵蓋範圍自 pico (10⁻¹²) 至 Mega (10⁶)，提供直觀的矩陣對照表與精確的數值計算。
 *
 * 【 2. 核心演算法：指數相減法 (Exponential Difference Algorithm) 】
 * 本程式不使用大量的 if-else 判斷，而是採用數學上的指數法則：
 *
 *    公式： Ratio = 10 ^ (Source_Exponent - Target_Exponent)
 *
 *    - Source_Exponent: 原始單位的 10 的次方數 (如 milli 為 -3)
 *    - Target_Exponent: 目標單位的 10 的次方數 (如 micro 為 -6)
 *    - 舉例： 1 milli 等於多少 micro?
 *            10 ^ (-3 - (-6)) = 10 ^ 3 = 1000
 *
 * 【 3. 計算方式與呈現邏輯 】
 * - 矩陣生成：利用雙重迴圈遍歷所有單位組合，計算其相互比值並填入 QTableWidget。
 * - 數值格式化：
 *   a. 常規區間 (10⁻⁴ ~ 10⁶)：採用固定小數位數（Fixed-point），並移除結尾多餘的「0」，保持介面簡潔。
 *   b. 極端區間：當比值過大或過小時，自動切換至科學記號 (Scientific Notation, 'e')，確保表格寬度不溢出。
 * - 視覺輔助：對角線（比值為 1）標註灰色背景，作為換算基準點。
 *
 * 【 4. 換算執行 】
 * 最終換算結果 = 輸入數值 * 該單位組合的比值。
 */

#include "UnitConverterHandler.h"
#include <cmath>
#include <QTableWidgetItem>
#include <QBrush>
#include <QRegularExpression>


void UnitConverterHandler::setupMatrixTable(QTableWidget* table) {
    int size = units.size();
    table->setRowCount(size);
    table->setColumnCount(size);

    // 1. 設定標題列 (Horizontal) 與 標題行 (Vertical)
    table->setHorizontalHeaderLabels(units);
    table->setVerticalHeaderLabels(units);

    // 2. 雙重迴圈填寫矩陣
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            // 公式：10^(From指數 - To指數)
            double ratio = std::pow(10, exponents[r] - exponents[c]);
            QString display = (ratio >= 1e6 || ratio <= 1e-4)
                                  ? QString::number(ratio, 'e', 2)
                                  : QString::number(ratio, 'g', 6);

            QTableWidgetItem* item = new QTableWidgetItem(display);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            if (r == c) item->setBackground(QBrush(Qt::lightGray));
            table->setItem(r, c, item);

            // 3. 對角線背景變色
            if (r == c) {
                item->setBackground(QBrush(Qt::lightGray));
            }

            table->setItem(r, c, item);
        }
    }
}

double UnitConverterHandler::convert(double value, int sourceIdx, int targetIdx) {
    double ratio = std::pow(10, exponents[sourceIdx] - exponents[targetIdx]);
    return value * ratio;
}

/*

double UnitConverterHandler::decodeSMDCode(QString code) {
    if (code.isEmpty()) return 0;

    // 1. 處理帶有小數點代碼的情況
    // 電阻常用 R (4R7), 電容常用 p (4p7) 或 n (1n2)
    if (code.contains('R', Qt::CaseInsensitive) ||
        code.contains('P', Qt::CaseInsensitive) ||
        code.contains('N', Qt::CaseInsensitive)) {

        QString temp = code;
        temp.replace('R', '.', Qt::CaseInsensitive);
        temp.replace('P', '.', Qt::CaseInsensitive);
        temp.replace('N', '.', Qt::CaseInsensitive);
        return temp.toDouble();
    }

    // 2. 處理標準三位數代碼 (如 103)
    if (code.length() >= 3) {
        // 使用 last(1) 取代 right(1)，避免 Clazy 警告
        int exponent = code.last(1).toInt();

        // 使用 chopped(1) 取得除了最後一碼以外的部分，取代 left()，效能更好
        double prefix = code.chopped(1).toDouble();

        // 核心演算法：前兩位 * 10的(第三位)次方
        return prefix * std::pow(10, exponent);
    }

    return code.toDouble(); // 如果是兩位數，直接視為數值
}

double UnitConverterHandler::calculateLEDResistor(double vcc, double vd, double current, int currentUnitIdx) {
    if (current <= 0) return 0;

    // 1. 將電流換算為標準單位 A (Ampere)
    double currentA = current;
    if (currentUnitIdx == 1) currentA = current / 1000.0;      // mA -> A
    else if (currentUnitIdx == 2) currentA = current / 1000000.0; // uA -> A

    // 2. 檢查壓差是否合理 (Vcc 必須大於 Vd)
    if (vcc <= vd) return -1; // 回傳 -1 代表電壓不足以驅動 LED

    // 3. 歐姆定律：R = (Vcc - Vd) / I
    return (vcc - vd) / currentA;
}

LEDResult UnitConverterHandler::calculateLEDComplex(double vcc, double vd, double current, int iUnitIdx, int series, int parallel) {

    LEDResult res = {0, 0, true};

    // 1. 處理電流單位換算 (換算為 A)
    double branchCurrentA = current;
    if (iUnitIdx == 1) branchCurrentA = current / 1000.0;
    else if (iUnitIdx == 2) branchCurrentA = current / 1000000.0;

    // 2. 串聯計算：總電壓需求
    double totalVd = vd * (series > 0 ? series : 1);

    // 3. 並聯計算：總電流需求
    double totalCurrentA = branchCurrentA * (parallel > 0 ? parallel : 1);

    // 4. 安全檢查：Vcc 必須大於總 LED 壓降
    if (vcc <= totalVd) {
        res.isVoltageOk = false;
        return res;
    }

    // 5. 計算電阻 R = V / I
    res.resistance = (vcc - totalVd) / totalCurrentA;

    // 6. 計算功耗 P = V * I (或是 I^2 * R)
    res.wattage = (vcc - totalVd) * totalCurrentA;

    return res;
}
*/
