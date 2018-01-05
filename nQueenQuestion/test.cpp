#include "test.h"
#include "ui_test.h"

#include <QPainter>
#include <QResizeEvent>
#include<QColor>
#include<iostream>
#include <stdio.h>
#include <QTime>
#include <sys/select.h>
#include<cstring>
using namespace std;

int a[100][100];
test::test(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent,f),
    ui(new Ui::test)
{
    ui->setupUi(this);
    mPixmap = new QPixmap(1366,768);
    mPainter = new QPainter(mPixmap);
    resize(1366,768);
    mPixmap->fill(QColor(255,255,255));
    count=0;
    numBer=0;
    sum=0;
    upperlim=1;

}

void sleep(unsigned int msec)
{

    QTime dieTime = QTime::currentTime().addMSecs(msec);

    while( QTime::currentTime() < dieTime )

        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

}


test::~test()
{
    delete ui;
    delete mPainter;
    delete mPixmap;
}

void test::resizeEvent(QResizeEvent *event)
{
    if(event->size().width() > mPixmap->width() || event->size().height() > mPixmap->height())
    {
        mPixmap->fill(QColor(255,255,255));
        QPixmap* pixmap = new QPixmap((std::max)(event->size().width(), mPixmap->width()), (std::max)(event->size().height(), mPixmap->height()));
        delete mPainter;
        mPainter = new QPainter(pixmap);
        mPainter->drawPixmap(0, 0, *mPixmap);
        delete mPixmap;
        mPixmap = pixmap;
    }
}

void test::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.drawPixmap(100, 100,500,500,*mPixmap);
}

QPainter *test::getPainter()
{
    return mPainter;
}

QPixmap *test::getPixMap()
{
    return mPixmap;
}
//初始化原始矩阵方块
void test::initBackground(int n)
{
    count=0;
    this->getPainter()->setRenderHint(QPainter::Antialiasing, true);
    this->getPainter()->setPen(QPen(QColor(255,255,255),5));
    this->getPainter()->setBrush(QColor(100,255,0));
    int x=0,y=0;
    for(int i=0;i<n;i++){
        x=0;
        for(int j=0;j<n;j++){
            this->getPainter()->drawRect(1366*1.0/testN*(i),768*1.0/testN*(j),1366*1.0/n,768*1.0/n);
            sleep(1);
            this->update();
        }
        this->update();
    }
}

void test::setBackgroundWhite()
{
    this->getPainter()->setBrush(QColor(255,255,255));
    this->getPainter()->setPen(QPen(QColor(255,255,255),5));
    this->getPainter()->drawRect(0,0,3000,3000);
}



void test::transeAnotherColor(int y, int x)
{
    this->getPainter()->setBrush(QColor(255,64,64));
    this->getPainter()->setPen(QPen(QColor(255,255,255),5));
    this->getPainter()->drawRect(1366*1.0/testN*(x-1),768*1.0/testN*(y-1),1366*1.0/testN,768*1.0/testN);
    this->update();
}




bool test::canIncheckerboard(int k)
{
    for(int i=1;i<k;i++){
        if(abs(x[k] - x[i]) == abs(k-i)||x[i] == x[k])
            return false;
    }
    return true;
}

void test::ttest(int t)
{
    if(t==testN+1){
        count++;
        ui->count->setText(QString::number(count,10));
        sleep(500);
    }
    else{
        for(int i=1;i<=testN;i++){
            x[t]=i;
            if(canIncheckerboard(t)){
                transeAnotherColor(t,i);
                sleep(1);
                ttest(t+1);
                this->getPainter()->setBrush(QColor(100,255,0));
                this->getPainter()->setPen(QPen(QColor(255,255,255),5));
                this->getPainter()->drawRect(1366*1.0/testN*(i-1),768*1.0/testN*(t-1),1366*1.0/testN,768*1.0/testN);
                this->update();
                sleep(1);
            }
        }
    }
}

void test::getNumber(long row, long ld, long rd)
{
    if (row != upperlim)
        {
            // row，ld，rd进行“或”运算，求得所有可以放置皇后的列,对应位为0，
            // 然后再取反后“与”上全1的数，来求得当前所有可以放置皇后的位置，对应列改为1
            // 也就是求取当前哪些列可以放置皇后
            long pos = upperlim & ~(row | ld | rd);
            while (pos)    // 0 -- 皇后没有地方可放，回溯
            {
                // 拷贝pos最右边为1的bit，其余bit置0
                // 也就是取得可以放皇后的最右边的列
                long p = pos & -pos;

                // 将pos最右边为1的bit清零
                // 也就是为获取下一次的最右可用列使用做准备，
                // 程序将来会回溯到这个位置继续试探
                pos -= p;

                // row + p，将当前列置1，表示记录这次皇后放置的列。
                // (ld + p) << 1，标记当前皇后左边相邻的列不允许下一个皇后放置。
                // (ld + p) >> 1，标记当前皇后右边相邻的列不允许下一个皇后放置。
                // 此处的移位操作实际上是记录对角线上的限制，只是因为问题都化归
                // 到一行网格上来解决，所以表示为列的限制就可以了。显然，随着移位
                // 在每次选择列之前进行，原来N×N网格中某个已放置的皇后针对其对角线
                // 上产生的限制都被记录下来了
                getNumber(row + p, (ld + p) << 1, (rd + p) >> 1);
            }
        }
        else
        {
            // row的所有位都为1，即找到了一个成功的布局，回溯
            numBer++;
        }


}
//求得输入框的信号槽
void test::on_lineEdit_cursorPositionChanged(int arg1, int arg2)
{
    testN=ui->lineEdit->text().toInt();
    ui->testNNumber->setText(ui->lineEdit->text());
    ui->count->setText(QString(""));
    ui->number->setText(QString(""));
    setBackgroundWhite();
    ui->number->setText(QString(""));
    this->update();
}

//test2Button的信号槽
void test::on_pushButton_2_clicked()
{
    memset(x,0,sizeof(x));
    count=0;
    ui->count->setText(QString::number(count,10));
    setBackgroundWhite();
    initBackground(testN);
    ttest(1);
    ui->count->setText(QString::number(count,10));
}

void test::on_pushButton_clicked()
{
    if(numBer==0&&testN<17){
        sum=0;
        upperlim=1;
        upperlim = (upperlim << testN) - 1;
        memset(x,0,sizeof(x));
        ui->number->setText(QString("请稍后"));
        this->update();
        sleep(10);
        numBer=0;
        getNumber(0,0,0);
        ui->number->setText(QString::number(numBer,10));
        numBer=0;
    }
    else if(testN>=17){
        ui->number->setText(QString("您输入的数字太大，计算时间过长"));
    }
}
