#include "rose.h"
#include <QDebug>

Rose::Rose(QWidget *parent) : QWidget(parent)
{
    //设置窗口
    setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint |Qt::WindowStaysOnTopHint);
    //rose_1图片处理
    for(int i=0;i<20;i++)
    {
        rose_1[i]=new QLabel(this);
        rose_1[i]->setPixmap(QPixmap(":/image/rose_10.png").scaled(72,72));
    }

    //rose_2图片处理
    for(int i=0;i<5;i++)
    {
        rose_2[i]=new QLabel(this);
        rose_2[i]->setPixmap(QPixmap(":/image/kele.png").scaled(72,72));
    }

    //rose_3图片处理
    for(int i=0;i<5;i++)
    {
        rose_3[i]=new QLabel(this);
        rose_3[i]->setPixmap(QPixmap(":/image/rose_3.png").scaled(72,72));
    }

    //rose_4图片处理
    for(int i=0;i<5;i++)
    {
        rose_4[i]=new QLabel(this);
        rose_4[i]->setPixmap(QPixmap(":/image/rose_4.png").scaled(128,128));
    }

    //rose_5图片处理
    for(int i=0;i<5;i++)
    {
        rose_5[i]=new QLabel(this);
        rose_5[i]->setPixmap(QPixmap(":/image/jichi.png").scaled(128,128));
    }

    //初始化rose_1的y轴坐标
    for(int i=0;i<20;i++)
        crood_y_rose_1[i]=-100;

    //初始化rose_2的y轴坐标
    for(int i=0;i<5;i++)
        crood_y_rose_2[i]=-80;

    //初始化rose_3的y轴坐标
    for(int i=0;i<5;i++)
        crood_y_rose_3[i]=-70;

    //初始化rose_4的y轴坐标
    for(int i=0;i<5;i++)
        crood_y_rose_4[i]=-110;

    //初始化rose_5的y轴坐标
    for(int i=0;i<5;i++)
        crood_y_rose_5[i]=-180;

    GetScreenInfo();

    //为rose_1 的x轴分初始分布产生随机数
    srand((unsigned)time(NULL));
    for(int i=0;i<20;i++)
    {
        rand_x_rose_1[i][0]=rand()%Screen_x;
        //qDebug() <<rand_x_rose_1[i][0]<<endl;
    }

    //为rose_2 的x轴分初始分布产生随机数
    for(int i=0;i<5;i++)
        rand_x_rose_2[i][0]=rand()%Screen_x;

    //为rose_3 的x轴分初始分布产生随机数
    for(int i=0;i<5;i++)
        rand_x_rose_3[i][0]=rand()%Screen_x;

    //为rose_4 的x轴分初始分布产生随机数
    for(int i=0;i<5;i++)
        rand_x_rose_4[i][0]=rand()%Screen_x;

    //为rose_5 的x轴分初始分布产生随机数
    for(int i=0;i<5;i++)
        rand_x_rose_5[i][0]=rand()%Screen_x;

    //rose_1位置释放
    for(int i=0;i<20;i++)
        rose_1[i]->setGeometry(rand_x_rose_1[i][0],crood_y_rose_1[i],rand_x_rose_1[i][0]+72,72+72);

    //rose_2位置释放
    for(int i=0;i<5;i++)
        rose_2[i]->setGeometry(rand_x_rose_2[i][0],crood_y_rose_2[i],rand_x_rose_2[i][0]+72,72+72);

    //rose_3位置释放
    for(int i=0;i<5;i++)
        rose_3[i]->setGeometry(rand_x_rose_3[i][0],crood_y_rose_3[i],rand_x_rose_3[i][0]+72,72+72);

    //rose_4位置释放
    for(int i=0;i<5;i++)
        rose_4[i]->setGeometry(rand_x_rose_4[i][0],crood_y_rose_4[i],rand_x_rose_4[i][0]+128,128+128);

    //rose_5位置释放
    for(int i=0;i<5;i++)
        rose_5[i]->setGeometry(rand_x_rose_5[i][0],crood_y_rose_5[i],rand_x_rose_5[i][0]+128,128+128);

    //rose的速度的随机产生
    for(int i=0;i<10;i++)
        RoseSpeed_Kind[i]=rand()%2+1;   //此处可优化 在此作者不做优化

    for(int i=0;i<20;i++)       //rose_1速度分配
    {
        rand_x_rose_1[i][1]=RoseSpeed_Kind[rand()%10];
     //   qDebug() <<rand_x_rose_1[i][1]<<endl;
    }

    for(int i=0;i<5;i++)        //rose_2速度分配
        rand_x_rose_2[i][1]=RoseSpeed_Kind[rand()%10];

    for(int i=0;i<5;i++)        //rose_3速度分配
        rand_x_rose_3[i][1]=RoseSpeed_Kind[rand()%10];

    for(int i=0;i<5;i++)        //rose_4速度分配
        rand_x_rose_4[i][1]=RoseSpeed_Kind[rand()%10];

    for(int i=0;i<5;i++)        //rose_5速度分配
        rand_x_rose_5[i][1]=RoseSpeed_Kind[rand()%10];




    startTimer(10);



}

//计时器用于更新paintEvent

void Rose::timerEvent(QTimerEvent *event)
{

    //rose_1 使落到底部的花重新掉落(复位)
    for(int i=0;i<20;i++)
    {
        if(crood_y_rose_1[i]>Screen_y)
        {
                crood_y_rose_1[i]=rand()%-300-80;
                rand_x_rose_1[i][0]=rand()%Screen_x;
        }
    }

    //rose_2 使落到底部的花重新掉落(复位)
    for(int i=0;i<5;i++)
    {
        if(crood_y_rose_2[i]>Screen_y)
        {
                crood_y_rose_2[i]=rand()%-300-80;
                rand_x_rose_2[i][0]=rand()%Screen_x;
        }
    }

    //rose_3 使落到底部的花重新掉落(复位)
    for(int i=0;i<5;i++)
    {
        if(crood_y_rose_3[i]>Screen_y)
        {
                crood_y_rose_3[i]=rand()%-300-80;
                rand_x_rose_3[i][0]=rand()%Screen_x;
        }
    }

    //rose_4 使落到底部的花重新掉落(复位)
    for(int i=0;i<5;i++)
    {
        if(crood_y_rose_4[i]>Screen_y)
        {
                crood_y_rose_4[i]=rand()%-300-80;
                rand_x_rose_4[i][0]=rand()%Screen_x;
        }
    }

    //rose_5 使落到底部的花重新掉落(复位)
    for(int i=0;i<5;i++)
    {
        if(crood_y_rose_5[i]>Screen_y)
        {
                crood_y_rose_5[i]=rand()%-300-80;
                rand_x_rose_5[i][0]=rand()%Screen_x;
        }
    }

    update();
    //qDebug() << "Timer ID:" << event->timerId();
}

void Rose::paintEvent(QPaintEvent *event)
{
    //rose_1 的20朵玫瑰
    for(int i=0;i<20;i++)
    {
        rose_1[i]->setGeometry(rand_x_rose_1[i][0],crood_y_rose_1[i],rand_x_rose_1[i][0]+72,72+72);
        crood_y_rose_1[i]=crood_y_rose_1[i]+rand_x_rose_1[i][1];
    }

    //rose_2 的5朵玫瑰
    for(int i=0;i<5;i++)
    {
        rose_2[i]->setGeometry(rand_x_rose_2[i][0],crood_y_rose_2[i],rand_x_rose_2[i][0]+72,72+72);
        crood_y_rose_2[i]=crood_y_rose_2[i]+rand_x_rose_2[i][1];
    }

    //rose_3 的5朵玫瑰
    for(int i=0;i<5;i++)
    {
        rose_3[i]->setGeometry(rand_x_rose_3[i][0],crood_y_rose_3[i],rand_x_rose_3[i][0]+72,72+72);
        crood_y_rose_3[i]=crood_y_rose_3[i]+rand_x_rose_3[i][1];
    }

    //rose_4 的5朵玫瑰
    for(int i=0;i<5;i++)
    {
        rose_4[i]->setGeometry(rand_x_rose_4[i][0],crood_y_rose_4[i],rand_x_rose_4[i][0]+128,128+128);
        crood_y_rose_4[i]=crood_y_rose_4[i]+rand_x_rose_4[i][1];
    }

    //rose_5 的5朵玫瑰
    for(int i=0;i<5;i++)
    {
        rose_5[i]->setGeometry(rand_x_rose_5[i][0],crood_y_rose_5[i],rand_x_rose_5[i][0]+128,128+128);
        crood_y_rose_5[i]=crood_y_rose_5[i]+rand_x_rose_5[i][1];
    }
}

//获取屏幕大小
void Rose::GetScreenInfo()
{
    Screen_x=QApplication::desktop()->width();
    Screen_y=QApplication::desktop()->height();

    this->move(0,0);

    this->resize( Screen_x, Screen_y);
}

void Rose::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}

