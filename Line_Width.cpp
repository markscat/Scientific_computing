#include "Line_Width.h"
#include "ui_Line_Width.h"

Line_Width::Line_Width(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Line_Width)
{
    ui->setupUi(this);
}

Line_Width::~Line_Width()
{
    delete ui;
}
