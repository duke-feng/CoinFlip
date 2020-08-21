#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>
#include <QTimer>

class Mycoin : public QPushButton
{
    Q_OBJECT
public:
//    explicit Mycoin(QWidget *parent = nullptr);
    //参数代表 传入的金币路径 还是银币路径
    Mycoin(QString btnImg);

    //金币属性
    int posx; //x坐标位置
    int posy; //y坐标位置
    bool flag; //正反标志

    //改变标志的方法
    void changeFlag();
    QTimer *timer1;//正面翻反面的计时器
    QTimer *timer2;//反面翻正面的计时器
    int min = 1;
    int max = 8;

    //执行动画的标志
    bool isAnimation = false;

    //重写 按下 释放
    void mousePressEvent(QMouseEvent *e);

    //是否胜利的标志
    bool isWin = false;//重点

signals:

};

#endif // MYCOIN_H
