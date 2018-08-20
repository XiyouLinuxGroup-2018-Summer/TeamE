#include "duihuakuang.h"
#include <QApplication>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QWidget>
extern QTimer tm;
struct haoyouyonghuxinxi
{
    int weiyibiaoshifu;
    char *id;
    char *name;
    char *age;
    char *style;
    char *namehead;
};
extern int haoyouyonghuxinix;
extern haoyouyonghuxinxi a[200];
duihuakuang::duihuakuang(QWidget *parent) : QMainWindow(parent)
{
    this->resize(600,400);
    open = new  QAction("open",this);
    open->setShortcut(tr("Ctrl+O"));

    fileexit = new  QAction("exit",this);
    fileexit->setShortcut(tr("Ctrl+E"));


    menu = menuBar()->addMenu(tr("file"));
    menu->addAction(open);
    menu->addAction(fileexit);
    text1 = new QTextEdit();
    text1->resize(100,100);
    text2 = new QTextEdit();
    text1->resize(100,100);
    setCentralWidget(text1);
    setCentralWidget(text2);
    QGridLayout *q1 = new QGridLayout(this);
//    q1->addWidget(text1,0,0);
//    q1->addWidget(text2,2,0);
//    setCentralWidget((QWidget *)q1);
    connect(open,SIGNAL(triggered()),this,SLOT(openfile()));    //这个不算点击
    //~~~~~@@~~~~~//
    connect(fileexit,SIGNAL(triggered()),this,SLOT(exitfile()));
}

void duihuakuang::openfile()
{
    QFileDialog::getOpenFileName();
}
void duihuakuang::exitfile()
{
    close();
}
