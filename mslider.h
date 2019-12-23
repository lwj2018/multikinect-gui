#ifndef MSLIDER_H
#define MSLIDER_H
#include<QWidget>
#include<QLineEdit>
#include<QSlider>

class mSlider: public QWidget
{
    Q_OBJECT
public:
    mSlider(QString name,int min, int max, QWidget* parent);
    QWidget* self = nullptr;

signals:
protected slots:
  void setLineEditValue(int value);
public:
    QLineEdit* lineEdit = nullptr;
    QSlider* slider = nullptr;
};

#endif // MSLIDER_H
