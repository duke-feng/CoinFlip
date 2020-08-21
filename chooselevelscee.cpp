#include "chooselevelscee.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QSound>

ChooseLevelScee::ChooseLevelScee(QWidget *parent) : QMainWindow(parent)
{
    //配置选择关卡场景
    this->setFixedSize(320, 588);

    //设置图标
    this->setWindowIcon(QPixmap("://resource//Coin0001.png"));

    //设置标题
    this->setWindowTitle("选择场景");

    //创建菜单栏
    QMenuBar * bar = menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");

    //创建退出菜单项
    QAction * quitAction = startMenu->addAction("退出");

    //点击退出 实现退出游戏
    connect(quitAction, &QAction::triggered, [=](){
        this->close();
    });

    //选择关卡音效
    QSound * chooseSound = new QSound("://resource//TapButtonSound.wav", this);

    //返回按钮音效
    QSound * backSound = new QSound("://resource//BackButtonSound.wav", this);

    //返回按钮
    MyPushButton * backbtn = new MyPushButton("://resource//BackButton.png", "://resource//BackButtonSelected.png");
    backbtn->setParent(this);
    backbtn->move(this->width() - backbtn->width(), this->height() - backbtn->height());

    //点击返回按钮
    connect(backbtn, &MyPushButton::clicked, [=](){
        qDebug() << "点击了返回按钮";

        //告诉主场景 我返回了， 主场景监听ChooseLevelScee的返回按钮
        //延时返回
        //播放返回按钮音效
        backSound->play();

        QTimer::singleShot(500, this, [=](){
             emit this->chooseSceneBack();
        });

    });

    //创建选择关卡按钮
    for (int i = 0; i < 20; i ++)
    {
        MyPushButton * menuBtn = new MyPushButton("://resource//LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move(25 + i % 4 * 70, 130 + i / 4 * 70);

        //监听每个按钮的点击事件
        connect(menuBtn, &MyPushButton::clicked, [=](){
            //播放选择关卡音效
            chooseSound->play();

            QString str = QString("您选择的是第 %1 关").arg(i + 1);
            qDebug() << str;


            //进入到游戏的场景
            this->hide(); //将选关场景隐藏掉
            play = new PlayScene(i + 1);
            //设置游戏场景的初始位置
            play->setGeometry(this->geometry());
            play->show();



            connect(play, &PlayScene::chooseSceneBack, [=](){
                this->setGeometry(play->geometry());

                this->show();
                delete play;
                play = NULL;
            });
        });

        QLabel * label = new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(), menuBtn->height());
        label->setText(QString::number(1 + i));
        label->move(25 + i % 4 * 70, 130 + i / 4 * 70);

        //设置label上的文字对齐方式 水平居中 垂直居中
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        //设置让鼠标进行穿透
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
    }
}

//重写paintEvent事件 画背景图
void ChooseLevelScee::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load("://resource//OtherSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);

    //画背景上的图标 加载标题
    pix.load("://resource//Title.png");

    //pix = pix.scaled(pix.width(), pix.height());//尺度变化

    painter.drawPixmap((this->width() - pix.width()) * 0.5, 30, pix);

}
