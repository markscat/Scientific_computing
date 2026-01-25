#ifndef VIA_CURRENT_CAL_H
#define VIA_CURRENT_CAL_H

#include "UnitConverterHandler.h"
#include <QWidget>

namespace Ui {
class Via_Current_cal;
}

class Via_Current_cal : public QWidget
{
    Q_OBJECT

public:
    //explicit Via_Current_cal(QWidget *parent = nullptr);

    explicit Via_Current_cal(UnitConverterHandler *h, QWidget *parent = nullptr);

    ~Via_Current_cal();

//private:
    Ui::Via_Current_cal *ui;

    UnitConverterHandler *handler;

    // --- 內部數學工具函數 ---

    // 計算貫孔截面積 (sq mils) - IPC 公式通常使用英制 mil 為單位
    double calculateViaArea(double diameter_mm, double wallThick_mm);

    // 計算最大許可電流 (Amps) - 依據 IPC-2221
    double calculateMaxCurrent(double area_sqMil, double tempRise_C);

    // 計算貫孔電阻 (Ohms)
    double calculateResistance(double area_mm2, double boardThick_mm, double temp_C);

    void clearResults();

    // 初始化 UI 狀態 (下拉選單、預設值)
    void initUI();

    // 阻斷訊號用的旗標，防止連動更新時產生無窮迴圈
    bool isUpdating = false;




private slots:
    // 核心計算觸發點
    void onInputsChanged();

    // 銅重量(oz)與厚度(um)的連動處理
    void onCopperMassChanged();
    void onCopperThicknessChanged();




};

#endif // VIA_CURRENT_CAL_H
