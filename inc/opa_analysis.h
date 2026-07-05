#ifndef OPA_ANALYSIS_H
#define OPA_ANALYSIS_H
//#include "UnitConverterHandler.h"
#include "../inc/UnitConverterHandler.h"
#include <QWidget>

namespace Ui {
class OPA_Analysis;
}

class OPA_Analysis : public QWidget
{
    Q_OBJECT

public:
    //explicit OPA_Analysis(QWidget *parent = nullptr);
    explicit OPA_Analysis(UnitConverterHandler *h, QWidget *parent = nullptr);

    UnitConverterHandler *handler; // <--- 關鍵：必須在這裡宣告成員變數！

    ~OPA_Analysis();

private:
    Ui::OPA_Analysis *ui;

};

#endif // OPA_ANALYSIS_H
