#include "playscene.h"
#include <QDebug>
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include <mycoin.h>
#include "dataconfig.h"
#include <QPropertyAnimation>
#include <QSound>

//PlayScene::PlayScene(QWidget *parent) : QMainWindow(parent)
//{

//}


PlayScene::PlayScene(int levelNum)
{
    QString str = QString("进入了第 %1 关").arg(levelNum);
    qDebug() << str;
    this->levelIndex = levelNum;

    //初始化游戏场景
    //设置固定大小
    this->setFixedSize(300, 588);

    //设置图标
    this->setWindowIcon(QPixmap("://resource//Coin0001.png"));
    //设置标题
    this->setWindowTitle("翻金币场景");

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

    //添加音效资源
    //返回按钮音效
    QSound * backSound = new QSound("://resource//BackButtonSound.wav", this);
    //翻金币音效
    QSound * flipSound = new QSound("://resource//ConFlipSound.wav", this);
    //胜利按钮音效
    QSound * winSound = new QSound("://resource//LevelWinSound.wav", this);



    //返回按钮
    //返回按钮
    MyPushButton * backbtn = new MyPushButton("://resource//BackButton.png", "://resource//BackButtonSelected.png");
    backbtn->setParent(this);
    backbtn->move(this->width() - backbtn->width(), this->height() - backbtn->height());

    //点击返回按钮
    connect(backbtn, &MyPushButton::clicked, [=](){
        qDebug() << "翻金币场景中点击了返回按钮";
        //播放返回按钮音效
        backSound->play();

        //告诉主场景 我返回了， 主场景监听ChooseLevelScee的返回按钮
        //延时返回
        QTimer::singleShot(500, this, [=](){
             emit this->chooseSceneBack();
        });

    });

    //显示当前关卡号
    QLabel * label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString str1 = QString("Level: %1").arg(this->levelIndex);
    //将字体设置到标签控件中
    label->setFont(font);
    label->setText(str1);
    label->setGeometry(30, this->height() - 50, 120, 50);

    dataConfig config;
    //初始化每个关卡的二维数组
    for(int i = 0; i < 4; i ++)
    {
        for (int j = 0; j < 4; j ++)
        {
            this->gameArray[i][j] = config.mData[this->levelIndex][i][j];
        }
    }

    //胜利图片显示
    QLabel * winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load("://resource//LevelCompletedDialogBg.png");
    winLabel->setGeometry(0, 0, tmpPix.width(), tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move((this->width() - tmpPix.width()) * 0.5, -tmpPix.height());

    //显示金币背景图案
    for (int i = 0; i < 4; i ++)
    {
        for (int j = 0; j < 4; j ++)
        {
            //绘制背景图片
            QPixmap pix = QPixmap("://resource//BoardNode.png");
            QLabel * label = new QLabel;
            label->setGeometry(0, 0, pix.width(), pix.height());
            label->setPixmap(pix);
            label->setParent(this);
            label->move(57 + i * 50, 200 + j * 50);

            //创建金币
            QString str;
            if(this->gameArray[i][j] == 1)
            {
                //显示金币
                str = "://resource//Coin0001.png";
            }
            else
            {
                //显示银币
                str = "://resource//Coin0008.png";
            }
            Mycoin * coin = new Mycoin(str);
            coin->setParent(this);
            coin->move(59 + i * 50, 204 + j * 50);

            //给金币属性赋值
            coin->posx = i;
            coin->posy = j;
            coin->flag = this->gameArray[i][j]; //1正面 0反面；

            //将金币放入到 金币的二维数组中 以便维护后期
            coinbtn[i][j] = coin;

            //点击金币进行翻转
            connect(coin, &Mycoin::clicked, [=](){
                //播放翻金币的音效
                flipSound->play();
                //点击按钮 将所有按钮先禁用
                for(int i = 0; i < 4; i ++)
                    for(int j = 0; j < 4; j ++)
                    {
                        this->coinbtn[i][j]->isWin = true;
                    }


                coin->changeFlag();
                this->gameArray[i][j] = this->gameArray[i][j] == 0 ? 1 : 0;

                //翻转周围硬币
                //延时翻转
                QTimer::singleShot(300, this, [=](){
                    if (coin->posx + 1 <= 3) //周围的右侧金币翻转条件
                        {
                        coinbtn[coin->posx+1][coin->posy]->changeFlag();
                        this->gameArray[coin->posx+1][coin->posy] = this->gameArray[coin->posx+1][coin->posy] == 0 ? 1 : 0;
                    }
                     //周围的左侧金币翻转条件
                    if (coin->posx - 1 >= 0) //周围的右侧金币翻转条件
                        {
                        coinbtn[coin->posx-1][coin->posy]->changeFlag();
                        this->gameArray[coin->posx-1][coin->posy] = this->gameArray[coin->posx-1][coin->posy] == 0 ? 1 : 0;
                    }
                    //周围的上侧金币翻转条件
                   if (coin->posy + 1 <= 3) //周围的右侧金币翻转条件
                       {
                       coinbtn[coin->posx][coin->posy + 1]->changeFlag();
                       this->gameArray[coin->posx][coin->posy + 1] = this->gameArray[coin->posx][coin->posy + 1] == 0 ? 1 : 0;//这应该是取反
                   }
                   //周围的下侧金币翻转条件
                  if (coin->posy - 1 >= 0) //周围的右侧金币翻转条件
                      {
                      coinbtn[coin->posx][coin->posy - 1]->changeFlag();
                      this->gameArray[coin->posx][coin->posy - 1] = this->gameArray[coin->posx][coin->posy - 1] == 0 ? 1 : 0;
                  }

                  //翻完金币后 解开金币翻开禁用
                  for(int i = 0; i < 4; i ++)
                      for(int j = 0; j < 4; j ++)
                      {
                          this->coinbtn[i][j]->isWin = false;
                      }

                  //判断是否胜利
                  this->isWin = true;
                  for (int i = 0; i < 4; i ++)
                  {
                      for (int j = 0; j < 4; j++)
                      {
                          if (coinbtn[i][j]->flag == false) //只有一个是反面 那就算失败
                              {
                              this->isWin = false;
                              break;
                          }
                      }
                  }
                  if (this->isWin == true)
                  {
                      //添加胜利音效
                      winSound->play();
                      //胜利了
                      qDebug() << "游戏胜利";

                      //将所有按钮的胜利改为true
                      for (int i = 0; i < 4; i ++)
                      {
                          for (int j = 0; j < 4; j++)
                          {
                                coinbtn[i][j]->isWin = true;
                          }
                      }

                      //将胜利的图片移动下来
                      QPropertyAnimation * animation = new QPropertyAnimation(winLabel, "geometry");
                      //设置时间间隔
                      animation->setDuration(1000);
                      //设置开始位置
                      animation->setStartValue(QRect(winLabel->x(), winLabel->y(), winLabel->width(), winLabel->height()));
                      //设置结束位置
                      animation->setEndValue(QRect(winLabel->x(), winLabel->y() + 114, winLabel->width(), winLabel->height()));
                      //设置缓和曲线
                      animation->setEasingCurve(QEasingCurve::OutBounce);
                      //执行动画
                      animation->start();
                  }
                });

            });
        }
    }
}

//重写paintevent 事件
void PlayScene::paintEvent(QPaintEvent *)
{
    //创建背景
    QPainter painter(this);
    QPixmap pix;
    pix.load("://resource//PlayLevelSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);

    //画背景上的图标 加载标题
    pix.load("://resource//Title.png");

    pix = pix.scaled(pix.width() * 0.5, pix.height() * 0.5);//尺度变化

    painter.drawPixmap(10, 30, pix);

}
