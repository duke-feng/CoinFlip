#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter> //画家头文件
#include "mypushbutton.h"
#include <QDebug>
#include <QTimer>
#include <QSound> //多媒体模块下 的音效头文件

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //配置主场景

    //设置固定大小
    setFixedSize(320, 588);

    //设置图标
    setWindowIcon(QIcon("://resource//Coin0001.png"));

    //设置标题
    setWindowTitle("翻金币");

    //退出按钮实现
    connect(ui->actionQuit, &QAction::triggered,[=](){
        this->close();
    });

    //准备开始按钮的音效
    QSound * startSound = new QSound("://resource//TapButtonSound.wav", this);
    //startSound->setLoops(-1);//设置循环 如果是-1 代表无限
    startSound->play();

    //开始按钮
    MyPushButton *startbtn = new MyPushButton("://resource//MenuSceneStartButton.png");
    startbtn->setParent(this);
    startbtn->move(this->width() * 0.5 - startbtn->width() * 0.5, this->height() * 0.7);

    //实例化选择关卡的场景
    chooseScene = new ChooseLevelScee;

    //监听选择关卡的返回按钮的信号
    connect(chooseScene, &ChooseLevelScee::chooseSceneBack, this, [=](){
        this->setGeometry(chooseScene->geometry());
        chooseScene->hide();//将选择关卡场景隐藏
        this->show();//重新显示主场景
    });

    connect(startbtn, &MyPushButton::clicked, [=](){
        qDebug() << "点击了开始";
        //做一个弹起的特效
        //播放开始音效
        startSound->play();

        startbtn->zoom1();

        startbtn->zoom2();

        //延时进入到选择关卡场景中
        QTimer::singleShot(500, this, [=](){
            //设置chooseScene场景的位置
            chooseScene->setGeometry(this->geometry());
            //自身隐藏
            this->hide();
            //现实选择关卡的场景
            chooseScene->show();

        });

    });


}

//重写paintEvent事件 画背景图
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load("://resource//PlayLevelSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);

    //画背景上的图标
    pix.load("://resource//Title.png");

    pix = pix.scaled(pix.width() * 0.5, pix.height() * 0.5);//尺度变化

    painter.drawPixmap(10, 30, pix);

}

MainWindow::~MainWindow()
{
    delete ui;
}

