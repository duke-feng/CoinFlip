#include "mycoin.h"
#include <QDebug>
#include <QTimer>

//Mycoin::Mycoin(QWidget *parent) : QWidget(parent)
//{

//}

Mycoin::Mycoin(QString btnImg)
{
    QPixmap pix;
    bool ret = pix.load(btnImg);

    if (!ret)
    {
        QString str = QString("图片 %1 加载失败").arg(btnImg);
        qDebug() << str;
        return;
    }

    this->setFixedSize(pix.width(), pix.height());
    this->setStyleSheet("QPushButton(border:0px)");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(), pix.height()));

    //初始化定时器对象
    timer1 = new QTimer(this);
    timer2 = new QTimer(this);

    //监听正面反面的信号 并且翻转金币
    connect(timer1, &QTimer::timeout,[=](){
        QPixmap pix;
        QString str = QString("://resource//Coin000%1.png").arg(this->min++);
        pix.load(str);

        this->setFixedSize(pix.width(), pix.height());
        this->setStyleSheet("QPushButton(border:0px)");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(), pix.height()));

        //判断 如果翻完了 将min重置为1
        if (this->min > this->max)
        {
            this->min = 1;
            isAnimation = false;//停止动画
            timer1->stop();
        }
    });


    //监听反面正面的信号 并且翻转金币
    connect(timer2, &QTimer::timeout,[=](){
        QPixmap pix;
        QString str = QString("://resource//Coin000%1.png").arg(this->max--);
        pix.load(str);

        this->setFixedSize(pix.width(), pix.height());
        this->setStyleSheet("QPushButton(border:0px)");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(), pix.height()));

        //判断 如果翻完了 将min重置为8
        if (this->max < this->min)
        {
            this->max = 8;
            isAnimation = false;//停止动画
            timer2->stop();
        }
    });

}

//重写 按下 释放
void Mycoin::mousePressEvent(QMouseEvent *e)
{
    if (this->isAnimation || this->isWin)
    {
        return;
    }
    else
    {
       QPushButton::mousePressEvent(e);
    }
}

//改变标志的方法
void Mycoin::changeFlag()
{
    //如果是正面 翻成反面
    if(this->flag)
    {
        //开始正面翻反面的定时器
        timer1->start(30);
        isAnimation = true;//开始做动画了
        this->flag = false;
    }
    else
    {
        //开始正面翻反面的定时器
        timer2->start(30);
        isAnimation = true;//
        this->flag = true;
    }
}


