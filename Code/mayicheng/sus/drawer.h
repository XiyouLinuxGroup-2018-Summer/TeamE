#ifndef DRAWER_H
#define DRAWER_H
#include <QDialog>
#include <QWidget>
#include <QToolBox>
#include <qwidget.h>
#include <QToolButton>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>
#include <cJSON.h>
#include <QTime>
#include <QTimer>
#include <QTextEdit>
#include <QTextBrowser>

class mymessagedialog : public QDialog          //重头戏，这个要能聊天！！！
{
    Q_OBJECT

public:
    mymessagedialog(QWidget *parent = 0);
private:
    QLabel *mymessage_label1;
    QTextBrowser *mymessage_textbrowser;
    QPushButton *mymessage_pushbuttonfile;
    QPushButton *mymessage_pushbutton2;
    QTextEdit *mymessage_texteditinput;
    QPushButton *mymessage_sendmessage;

private slots:
    void sendfiles();
    void sendmessage();
    
};

class addfriendbaselog : public QDialog
{
    Q_OBJECT

public:
    addfriendbaselog(QWidget *parent = 0);
private:
    QLineEdit *eidtid;
    QPushButton *uokpushbutton;
private slots:
    //void updatepush();
void sendmessage();



    //void qtimer_start();
};


class everybaselog : public QDialog
{
    Q_OBJECT

public:
    everybaselog(QWidget *parent = 0);
private:
    QLabel *id1,*name1,*age1,*style1,*headphoto1;
    QLineEdit *eidtname1,*editage1,*editstyle1;
    QPushButton *updatepushbutton;
private slots:
    void updatepush();
};
class drawer : public QToolBox
{
    Q_OBJECT    //makefile
public:
    drawer();
private:
    QWidget *ww;
    QToolButton *toolbutton1;
    QToolButton *toolbutton2;
    QToolButton *toolbutton3;
    QToolButton *toolbutton4;
    QToolButton *toolbutton5;
    QToolButton *toolbutton6;
    everybaselog baselog;
    addfriendbaselog addfirendlog;
    mymessagedialog messages;
public slots:
    void showwidget();
    void showmybasic();
    void addhaoyou();


    void qtimer_start();
    void qtimer2_start();

    //void changemysometing();
};

#endif // DRAWER_H
