#ifndef TEST_H
#define TEST_H

#include <QPushButton>
#include <QWidget>

namespace Ui {
class test;
}

class test : public QWidget
{
    Q_OBJECT

public:
    explicit test(QWidget *parent = 0,Qt::WindowFlags f = 0);
    ~test();

    int count;
    int x[100];
    int numBer;
    long sum , upperlim ;

    QPushButton qbutton;

    void resizeEvent(QResizeEvent *event);

    void paintEvent(QPaintEvent *event);

    QPainter* getPainter();
    QPixmap* getPixMap();
    void initBackground(int n);
    void setBackgroundWhite();
    void runResult();

    void transeAnotherColor(int x,int y);

    bool canIncheckerboard(int k);

    void ttest(int t);

    void getNumber(long row, long ld, long rd);

private slots:
    void on_lineEdit_cursorPositionChanged(int arg1, int arg2);

    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Ui::test *ui;
    int testN;
    QPixmap* mPixmap;
    QPainter* mPainter;
};

#endif // TEST_H
