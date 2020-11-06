#ifndef ROSE_H
#define ROSE_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QSize>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimerEvent>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <QCloseEvent>


class Rose : public QWidget
{
    Q_OBJECT
public:
    explicit Rose(QWidget *parent = 0);
    void GetScreenInfo();   //获得屏幕分辨率
    void closeEvent(QCloseEvent *event);

    int Screen_x;   //屏幕x轴
    int Screen_y;   //屏幕y轴

signals:

public slots:

protected:
    void timerEvent(QTimerEvent *event);

private:
    virtual void paintEvent(QPaintEvent *event);

    QLabel *rose_1[20];
    QLabel *rose_2[5];
    QLabel *rose_3[5];
    QLabel *rose_4[5];
    QLabel *rose_5[5];

    int rand_x_rose_1[20][2];    //rose1 20个的玫瑰花坐标  第二个为玫瑰的速度
    int rand_x_rose_2[5][2];    //rose2 5个的玫瑰花坐标  第二个为玫瑰的速度
    int rand_x_rose_3[5][2];     //rose3 5个的玫瑰花坐标  第二个为玫瑰的速度
    int rand_x_rose_4[5][2];    //rose4 5个的玫瑰花坐标  第二个为玫瑰的速度
    int rand_x_rose_5[5][2];    //rose4 5个的玫瑰花坐标  第二个为玫瑰的速度


    int crood_y_rose_1[20];       //rose_1 的y坐标
    int crood_y_rose_2[5];      //rose_2 的y坐标
    int crood_y_rose_3[5];      //rose_3 的y坐标
    int crood_y_rose_4[5];      //rose_4 的y坐标
    int crood_y_rose_5[5];      //rose_5 的y坐标


    int RoseSpeed_Kind[10];      //玫瑰的10种不同的速度


};

#endif // ROSE_H
