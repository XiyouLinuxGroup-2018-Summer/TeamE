#include "drawer.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPalette>
#include <QLabel>
#include <QMessageBox>
#include <QString>
#include <QByteArray>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <math.h>
#include <openssl/md5.h>
#include <cJSON.h>
#include <QFont>
#include <QFileDialog>
extern int SOCKET_FD;
extern int times;
extern QTimer tm;
extern QTimer tm2;
extern int addusersid;
extern char bbstring[105];
extern char ccstring[105];
extern char ddstring[105];
extern char eestring[105];
extern char ffstring[105];
extern char ggstring[105];
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
struct yonghuxinxi11
{
    char *a;        //id
    char *b;        //name
    char *c;        //age
    char *d;        //mima
    char *e;        //style
    char *f;        //head
};
drawer::drawer() : QToolBox ()
{
    toolbutton1 = new QToolButton();
    toolbutton1->setText("我自己~myself");
    if(ggstring)
    {
        toolbutton1->setIcon(QPixmap("/home/ma/图片/qq/1.jpg"));
        //toolbutton1->setIconSize(QPixmap("/home/ma/图片/qq/1.jpg").size());
        toolbutton1->setIconSize(0.5 * QPixmap("/home/ma/图片/qq/1.jpg").size());
    }
    toolbutton1->setAutoRaise(true);    //button tanqi
    toolbutton1->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    //QLabel *label1 = new QLabel(tr("项目概述"));
    QLabel *label1 = new QLabel();
    if(ffstring)
        label1->setText(ffstring);
    label1->setStyleSheet("color:red");//文本颜色
    QHBoxLayout *label1layout = new QHBoxLayout();
    label1layout->addWidget(toolbutton1);
    label1layout->addWidget(label1);

    toolbutton5 = new QToolButton();
    toolbutton5->setText("点击添加群组");
    toolbutton5->setIcon(QPixmap("/home/ma/图片/qq/22.jpg"));
    toolbutton5->setIconSize(0.5 * QPixmap("/home/ma/图片/qq/22.jpg").size());
    toolbutton5->setAutoRaise(true);
    toolbutton5->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    toolbutton6 = new QToolButton();
    toolbutton6->setText("点击添加好友");
    toolbutton6->setIcon(QPixmap("/home/ma/图片/qq/21.jpg"));
    toolbutton6->setIconSize(0.5 * QPixmap("/home/ma/图片/qq/21.jpg").size());
    toolbutton6->setAutoRaise(true);
    toolbutton6->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);


    QGroupBox *groupbox1 = new QGroupBox();
    QVBoxLayout *layout1 = new QVBoxLayout(groupbox1);  //fuqin
    layout1->setMargin(10);
    //layout1->setAlignment(Qt::AlignHCenter);
    layout1->setAlignment(Qt::AlignLeft);
    layout1->addLayout(label1layout);
    layout1->addWidget(toolbutton1);
    layout1->addWidget(toolbutton6);
    layout1->addWidget(toolbutton5);
    layout1->addStretch();


    toolbutton2 = new QToolButton();
    toolbutton2->setText("李四");
    toolbutton2->setIcon(QPixmap("/home/ma/图片/qq/2.jpg"));
    toolbutton2->setIconSize(0.5 * QPixmap("/home/ma/图片/qq/2.jpg").size());
    toolbutton2->setAutoRaise(true);
    toolbutton2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    toolbutton3 = new QToolButton();
    toolbutton3->setText("王五");
    toolbutton3->setIcon(QPixmap("/home/ma/图片/qq/3.jpg"));
    toolbutton3->setIconSize(QPixmap("/home/ma/图片/qq/3.jpg").size());
    toolbutton3->setAutoRaise(true);
    toolbutton3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    toolbutton4 = new QToolButton();
    toolbutton4->setText("老六");
    toolbutton4->setIcon(QPixmap("/home/ma/图片/qq/4.jpg"));
    toolbutton4->setIconSize(QPixmap("/home/ma/图片/qq/4.jpg").size());
    toolbutton4->setAutoRaise(true);
    toolbutton4->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QGroupBox *groupbox2 = new QGroupBox();
    QVBoxLayout *layout2 = new QVBoxLayout(groupbox2);
    layout2->setMargin(10);
    layout2->setAlignment(Qt::AlignHCenter/*AlignLeft*/);
    layout2->addWidget(toolbutton3);
    layout2->addWidget(toolbutton4);
    layout2->addStretch();

    QGroupBox *groupbox3 = new QGroupBox();
    QVBoxLayout *layout3 = new QVBoxLayout(groupbox3);
    layout3->setMargin(10);
    layout3->setAlignment(Qt::AlignHCenter/*AlignLeft*/);
    layout3->addWidget(toolbutton2);
    layout3->addStretch();
    this->addItem((QWidget *)groupbox1,"我~Myself");
    this->addItem((QWidget *)groupbox2,"我的好友");
    this->addItem((QWidget *)groupbox3,"我的群组");
    //头像，签名等信息
    //    QGridLayout *mmmain_layout = new QGridLayout(this);

    //    QToolButton  *toolbuttonMY = new QToolButton();
    //    toolbuttonMY->setText("张三");
    //    toolbuttonMY->setIcon(QPixmap("/home/ma/图片/qq/1.jpg"));
    //    toolbuttonMY->setIconSize(0.5 * QPixmap("/home/ma/图片/qq/1.jpg").size());
    //    toolbuttonMY->setAutoRaise(true);    //button tanqi
    //    toolbuttonMY->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    //    QLabel *labelMY = new QLabel(tr("项目概述11"));
    //    label1->setStyleSheet("color:red");//文本颜色
    //    QHBoxLayout *label1layoutMY = new QHBoxLayout();
    //    label1layoutMY->addWidget(toolbutton1);
    //    label1layoutMY->addWidget(label1);



    connect(toolbutton1,SIGNAL(clicked()),this,SLOT(showmybasic()));
    connect(toolbutton2,SIGNAL(clicked()),this,SLOT(showwidget()));
    connect(toolbutton3,SIGNAL(clicked()),this,SLOT(showwidget()));
    connect(toolbutton4,SIGNAL(clicked()),this,SLOT(showwidget()));
    connect(toolbutton5,SIGNAL(clicked()),this,SLOT(showwidget()));
    connect(toolbutton6,SIGNAL(clicked()),this,SLOT(addhaoyou()));
    connect(&tm,SIGNAL(timeout()),this,SLOT(qtimer_start()));
    connect(&tm2,SIGNAL(timeout()),this,SLOT(qtimer2_start()));
}

mymessagedialog::mymessagedialog(QWidget *parent) : QDialog(parent)
{
    this->resize(600,400);
    //QPixmap pixmap("/home/ma/图片/qq/1.jpg");
    //this->setStyleSheet("background-color:yellow;"); //设置背景颜色
      this->setStyleSheet("background-image:url(/home/ma/下载/111 (1).jpg);"); //设置背景图片
    this->setWindowTitle(tr("我的聊天室 ｡:.ﾟヽ(｡◕‿◕｡)ﾉﾟ.:｡+ﾟ"));
    QPixmap qq("/home/ma/图片/qq/2.jpg");
    mymessage_textbrowser = new QTextBrowser;
    mymessage_textbrowser->resize(200,200);
    mymessage_pushbuttonfile = new QPushButton("发送文件");
    mymessage_pushbutton2 = new QPushButton(tr("暂定"));
    mymessage_texteditinput = new QTextEdit;
    mymessage_sendmessage = new QPushButton(tr("发送"));
    QFont font1;
    font1.setBold(true);//粗体
    font1.setPointSize(16);//文字大小
    font1.setCapitalization(QFont::Capitalize);//首字母大写
    mymessage_texteditinput->setFont(font1);

    QGridLayout *mess_layout = new QGridLayout(this);
    mess_layout->addWidget(mymessage_textbrowser,0,0,1,2);
    mess_layout->addWidget(mymessage_pushbuttonfile,1,0);
    mess_layout->addWidget(mymessage_pushbutton2,1,1);
    mess_layout->addWidget(mymessage_texteditinput,2,0,1,2);
    mess_layout->addWidget(mymessage_sendmessage,3,1);


    connect(mymessage_pushbuttonfile,SIGNAL(clicked()),this,SLOT(sendfiles()));
    connect(mymessage_sendmessage,SIGNAL(clicked()),this,SLOT(sendmessage()));
}
void mymessagedialog::sendfiles()
{
    QFileDialog::getOpenFileName();
    return;
}
void mymessagedialog::sendmessage()         //发消息
{
//    QString s1 = mymessage_texteditinput->toPlainText();
//    QByteArray a1 = s1.toUtf8();
    this->setStyleSheet("background-image:url(/home/ma/图片/qq/1.jpg);"); //设置背景图片
    return;
}
addfriendbaselog::addfriendbaselog(QWidget *parent) : QDialog(parent)
{
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~@@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//    QLineEdit *eidtid;
//    QPushButton *uokpushbutton;
    eidtid = new QLineEdit();
    uokpushbutton = new QPushButton(tr("确定"));
    eidtid->setPlaceholderText("请输入你要添加好友的MM号");
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(eidtid);
    layout->addWidget(uokpushbutton);
    connect(uokpushbutton,SIGNAL(clicked()),this,SLOT(sendmessage()));
    //connect(&tm,SIGNAL(timeout()),this,SLOT(qtimer_start()));
}

void addfriendbaselog :: sendmessage()
{
    QString q1 = eidtid->text();
    if(q1.isEmpty())
    {
        QMessageBox::information(this,"???","咋没输入就想进去？？");
        return;
    }
    QByteArray asd = q1.toLatin1();
    if(strcmp(asd.data(),bbstring) == 0)
    {
        QMessageBox::information(this,"???","咋的拉，还想自己加自己好友\n当你会分身术吗??");
        return;
    }
//    extern int haoyouyonghuxinix;
//    extern haoyouyonghuxinxi a[200];
    int ii = 0;
    for(ii = 0;ii < haoyouyonghuxinix;ii++)
    {
        if(strcmp(asd.data(),a[ii].id) == 0)
        {
            QMessageBox::information(this,"???","人家都是你好友了，你还想干啥？？\n（情侣吗`.`）");
            return;
        }
    }
    int bbb = atoi(asd.data());
    struct adduser2
    {
        int idme;
        int weiyibiaoshifu;     //15;
        int id_want_to_add;
    };
    adduser2 add2;
    add2.idme = atoi(bbstring);
    add2.weiyibiaoshifu = 15;
    add2.id_want_to_add = bbb;
    cJSON *adds = cJSON_CreateObject();
    cJSON_AddNumberToObject(adds,"idme",add2.idme);
    cJSON_AddNumberToObject(adds,"weiyibiaoshifu",add2.weiyibiaoshifu);
    cJSON_AddNumberToObject(adds,"id_want_to_add",add2.id_want_to_add);
    char *out = cJSON_PrintUnformatted(adds);
    int a = strlen(out) + 1;
    send(SOCKET_FD,&a,4,0);
    send(SOCKET_FD,out,a,0);
    QMessageBox::information(this,"提示","消息发送成功，请耐心等待");

}
void drawer::addhaoyou()
{
    addfirendlog.exec();
}
void drawer::showwidget()
{
    //ww->show();
    messages.show();
}
void drawer::showmybasic()
{
    baselog.exec();
}
everybaselog::everybaselog(QWidget *parent) : QDialog(parent)
{
    //this->resize(200,200);
    id1 = new QLabel(bbstring);
    name1 = new QLabel("姓名:");
    age1 = new QLabel("年龄:");
    style1 = new QLabel("个性签名:");
    headphoto1 = new QLabel("头像:");

    updatepushbutton = new QPushButton(tr("刷新:"));
    editage1 = new QLineEdit;
    eidtname1 = new QLineEdit;

    QGridLayout *main_layout = new QGridLayout(this);
    main_layout->addWidget(id1,0,0,1,2);
    main_layout->addWidget(name1,1,0);
    main_layout->addWidget(eidtname1,1,1);

    main_layout->addWidget(age1,2,0);
    main_layout->addWidget(editage1,2,1);

    main_layout->addWidget(headphoto1,0,3);

    main_layout->addWidget(style1,3,0,1,5);

    main_layout->addWidget(updatepushbutton,4,1);

    connect(updatepushbutton,SIGNAL(clicked()),this,SLOT(updatepush()));













}
void everybaselog::updatepush()
{
    QMessageBox::information(this,"error",QString::number(SOCKET_FD));
    if(ccstring)
        eidtname1->setText(ccstring);
    editage1 = new QLineEdit;
    if(ddstring)
        editage1->setText(ddstring);
    editstyle1 = new QLineEdit;
    if(ffstring)
        editstyle1->setText(ffstring);
}
//tm->start(50);
//tm->stop();
void drawer:: qtimer_start()
{
    static int i = 0;
    QMessageBox::warning(this,"!!!","察无此人!!!");
    i++;
    if(i == 2)
    {
        tm.stop();
        i = 0;
    }
}
void drawer:: qtimer2_start()
{
    static int i = 0;
    char c[100] = { 0 };
    sprintf(c,"%d想加你为好友，认识一下，加吗？",addusersid);
    if((QMessageBox::question(this,"约吗？",c,QMessageBox::Yes | QMessageBox::No)) == QMessageBox::Yes)
    {
        struct recv_add_friend
        {
            int weiyibiaoshifu;
            int myid;
            int herid;
        };
        recv_add_friend abc;
        abc.weiyibiaoshifu = 16;
        abc.myid = atoi(bbstring);
        abc.herid = addusersid;

        cJSON *jsonzhuceyonghu = cJSON_CreateObject();
        cJSON_AddNumberToObject(jsonzhuceyonghu,"weiyibiaoshifu",abc.weiyibiaoshifu);
        cJSON_AddNumberToObject(jsonzhuceyonghu,"myid",abc.myid);
        cJSON_AddNumberToObject(jsonzhuceyonghu,"herid",abc.herid);
        char *out = cJSON_Print(jsonzhuceyonghu);
        //texteditusername11->setText(out);
        out = cJSON_PrintUnformatted(jsonzhuceyonghu);
        int a = strlen(out) + 1;
        send(SOCKET_FD,&a,4,0);
        send(SOCKET_FD,out,a,0);
        free(out);
        cJSON_Delete(jsonzhuceyonghu);
    }
    else
    {
        ;
    }
    i++;
    if(i == 1)
    {
        tm2.stop();
        i = 0;
    }
}
