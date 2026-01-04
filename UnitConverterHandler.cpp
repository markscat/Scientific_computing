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

/*
            // 格式化數字顯示
            QString display;

            if (ratio >= 1e6 || ratio <= 1e-4) {
                display = QString::number(ratio, 'e', 2); // 太大或太小用科學記號
            } else {
                display = QString::number(ratio, 'f', 4).remove(QRegularExpression("0+$")).remove(QRegularExpression("\\.$"));

            }

            QTableWidgetItem* item = new QTableWidgetItem(display);
            item->setTextAlignment(Qt::AlignCenter);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled); // 設為唯讀
*/
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
