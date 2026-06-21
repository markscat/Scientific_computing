#ifndef LINE_WIDTH_H
#define LINE_WIDTH_H

#include "UnitConverterHandler.h"

#include <QWidget>

namespace Ui {
class Line_Width;
}

class Line_Width : public QWidget
{
    Q_OBJECT

public:
    //explicit Line_Width(QWidget *parent = nullptr);

    explicit Line_Width(UnitConverterHandler *sharedHandler, QWidget *parent = nullptr);

    ~Line_Width();

    UnitConverterHandler *handler; // 保存傳進來的 handler

    bool isCalculating = false;



private:
    Ui::Line_Width *ui;




private slots:
    void updateCalculation();
};

#endif // LINE_WIDTH_H
