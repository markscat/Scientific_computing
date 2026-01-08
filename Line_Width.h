#ifndef LINE_WIDTH_H
#define LINE_WIDTH_H

#include <QWidget>

namespace Ui {
class Line_Width;
}

class Line_Width : public QWidget
{
    Q_OBJECT

public:
    explicit Line_Width(QWidget *parent = nullptr);
    ~Line_Width();

private:
    Ui::Line_Width *ui;
};

#endif // LINE_WIDTH_H
