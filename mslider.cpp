#include "mslider.h"
#include<QHBoxLayout>
#include<QLabel>
#include<QString>

mSlider::mSlider(QString name,int min, int max, QWidget* parent):
    QWidget(parent)
{
    self = new QWidget();
    QLabel * label = new QLabel();
    label->setText(name);
    lineEdit = new QLineEdit("0");

    //新建一个水平方向的滑动条QSlider控件
    slider = new QSlider(Qt::Horizontal);
    //设置滑动条控件的最小值
    slider->setMinimum(min);
    //设置滑动条控件的最大值
    slider->setMaximum(max);
    //设置滑动条控件的值
    slider->setValue(min);

    //信号和槽 当滑动条的值发生改变时，即产生一个valueChanged(int)信号 设置QLineEdit控件的显示文本
    QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setLineEditValue(int)));

    QHBoxLayout *layout = new QHBoxLayout(self);
    layout->addWidget(label);
    layout->addWidget(lineEdit);
    layout->addWidget(slider);

    self->show();

}


void mSlider::setLineEditValue(int value)
{
  int pos = slider->value();
  QString str = QString("%1").arg(pos);
  lineEdit->setText(str);
}
