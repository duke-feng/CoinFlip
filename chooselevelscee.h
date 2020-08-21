#ifndef CHOOSELEVELSCEE_H
#define CHOOSELEVELSCEE_H

#include <QMainWindow>
#include "playscene.h"

class ChooseLevelScee : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChooseLevelScee(QWidget *parent = nullptr);

    //重写paintEvent事件 画背景图
    void paintEvent(QPaintEvent *);

    //游戏场景对象指针
    PlayScene * play = NULL;

signals:

    //写一个自定义的信号 告诉主场景 点击了返回
    void chooseSceneBack();


};

#endif // CHOOSELEVELSCEE_H
