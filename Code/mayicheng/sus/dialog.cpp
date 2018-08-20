#include "dialog.h"
#include <QDialog>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QLabel>
#include <QMovie>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <drawer.h>
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
#include <pthread.h>
extern int SOCKET_FD;
extern int times;
char bbstring[105];
char ccstring[105];
char ddstring[105];
char eestring[105];
char ffstring[105];
char ggstring[105];
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
QTimer tm;
QTimer tm2;
int addusersid;
void *recvduan(void *p)
{
    while(1)
    {
        int *size = (int *)malloc(sizeof(int));
        int rcm = 0;
        int sheng = 4;
        const int aaabbbccc = 4;
        while(rcm < 4)
        {
            rcm += recv(SOCKET_FD,size,sheng,0);
            sheng = aaabbbccc - rcm;
        }
        char *buf1;
        rcm = 0;
        sheng = *size;
        buf1 = (char *)malloc(*size);
        while(rcm < *size)
        {
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~@@~~~~~~~~~~~~~~~~~~~~~~~~~~~//
            rcm += recv(SOCKET_FD,&buf1[rcm],sheng,0);
            sheng = *size - rcm;
        }
        if(buf1 == NULL && strcmp(buf1,"") == 0 )
            continue;
        cJSON *CONGZHELIKAISHI = cJSON_Parse(buf1);
        //free(buf1);
        int thissa = cJSON_GetObjectItem(CONGZHELIKAISHI,"weiyibiaoshifu")->valueint;
        switch (thissa)
        {
        case 1:
        {
            cJSON *CONGZHELIKAISHI = cJSON_Parse(buf1);
            char *out111 = NULL;
            out111    =    cJSON_Print(CONGZHELIKAISHI);
            //if(out111)
            //QMessageBox::information(this,"111",QString::number(strlen(out111)));
            //free(buf1);
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~@@@@@@@@@@@@@@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
            if(cJSON_GetObjectItem(CONGZHELIKAISHI,"id")  != NULL)
            {
                int qq = strlen(cJSON_GetObjectItem(CONGZHELIKAISHI,"id")->valuestring);
                a[haoyouyonghuxinix].id = (char *)malloc(qq);
                strcpy(a[haoyouyonghuxinix].id,cJSON_GetObjectItem(CONGZHELIKAISHI,"id")->valuestring);
            }
            if(cJSON_GetObjectItem(CONGZHELIKAISHI,"age")  != NULL)
            {
                int qq = strlen(cJSON_GetObjectItem(CONGZHELIKAISHI,"age")->valuestring);
                a[haoyouyonghuxinix].age = (char *)malloc(qq);
                strcpy(a[haoyouyonghuxinix].age,cJSON_GetObjectItem(CONGZHELIKAISHI,"age")->valuestring);
            }
            if(cJSON_GetObjectItem(CONGZHELIKAISHI,"name")  != NULL)
            {
                int qq = strlen(cJSON_GetObjectItem(CONGZHELIKAISHI,"name")->valuestring);
                a[haoyouyonghuxinix].name = (char *)malloc(qq);
                strcpy(a[haoyouyonghuxinix].name,cJSON_GetObjectItem(CONGZHELIKAISHI,"name")->valuestring);
            }
            if(cJSON_GetObjectItem(CONGZHELIKAISHI,"namehead")  != NULL)
            {
                int qq = strlen(cJSON_GetObjectItem(CONGZHELIKAISHI,"namehead")->valuestring);
                a[haoyouyonghuxinix].namehead = (char *)malloc(qq);
                strcpy(a[haoyouyonghuxinix].namehead,cJSON_GetObjectItem(CONGZHELIKAISHI,"namehead")->valuestring);
            }
            if(cJSON_GetObjectItem(CONGZHELIKAISHI,"style")  != NULL)
            {
                int qq = strlen(cJSON_GetObjectItem(CONGZHELIKAISHI,"style")->valuestring);
                a[haoyouyonghuxinix].style = (char *)malloc(qq);
                strcpy(a[haoyouyonghuxinix].style,cJSON_GetObjectItem(CONGZHELIKAISHI,"style")->valuestring);
            }
            haoyouyonghuxinix++;
            break;
        }
        case 2:         //2是添加好友的
        {
            struct send_add_friend
            {
                int weiyibiaoshifu;     //2
                int send_id;           //通过这个就可以知道你想要发给谁
                int num;                //这个是返回值
            };
            send_add_friend add1;
            add1.num = cJSON_GetObjectItem(CONGZHELIKAISHI,"num")->valueint;
            add1.send_id = cJSON_GetObjectItem(CONGZHELIKAISHI,"send_id")->valueint;
            if(add1.send_id == atoi(bbstring) || add1.num == 1)
            {
                tm.start(50);
            }
            if(add1.num == 10)
            {
                addusersid = add1.send_id;
                tm2.start(50);
            }
            break;
        }
        case 3:         //3是更新客好友列表的
        {

            break;
        }
        }
    }
}
int zheshiyigehenzhongyaodedongxi(int a)
{
    a += 135435;
    a *= 2;
    a -= 5343;
    a++;
    a *= 2;
    a -= 3;
    a++;
    a %= 10005;
    return a;
}
unsigned char *minamd5(char *p)
{
    MD5_CTX md5;
    unsigned char *c;
    c = (unsigned char *)calloc(sizeof(char) * 100,1);
    MD5_Init(&md5);	//初始化
    MD5_Update(&md5, (unsigned char *)p, strlen(p)); //md5加密
    MD5_Final(c,&md5); //将加密后的密文放到c
    unsigned char *d;
    d = (unsigned char *)calloc(sizeof(char) * 100,1);
    int i = 0;
    for (i = 0; i<16; i++)
    {
        unsigned char aa[5] = { 0 };
        sprintf((char *)aa,"%02x", c[i]);
        strcat((char *)d,(const char *)aa);
    }
    free(c);
    return  d;
}
int SOCKETS;
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{

    SOCKETS = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //    if(::connect(SOCKETS,(struct sockaddr *)&addr,sizeof(addr)) == -1)
    //    {
    //            printf("connect faile,%s\n",strerror(errno));
    //            return;
    //    }
    if(::connect(SOCKETS,(struct sockaddr *)&addr,sizeof(addr)) == -1)
    {
        printf("connect faile,%s\n",strerror(errno));
        this->hide();
    }


    setWindowTitle("登陆MM");
    buttonfindpasswd = new QPushButton("找回密码");
    buttonnewusr = new QPushButton("注册帐号");
    buttonlogin = new QPushButton("登录");
    texteditusername = new QLineEdit();
    textedituserpasswd = new QLineEdit();
    checkboxlogin = new QCheckBox("自动登录");
    checkboxspass = new QCheckBox("记住密码");
    texteditusername->setPlaceholderText("MM号码/手机/邮箱");
    //舒服的很
    textedituserpasswd->setPlaceholderText("密码");
    textedituserpasswd->setEchoMode(QLineEdit::Password);
    QVBoxLayout *highlayout = new QVBoxLayout();
    highlayout->addWidget(texteditusername);
    highlayout->addWidget(textedituserpasswd);
    QHBoxLayout *threelayout = new QHBoxLayout();
    threelayout->addWidget(checkboxlogin);
    threelayout->addWidget(checkboxspass);
    threelayout->addWidget(buttonfindpasswd);
    /*
    ui->setupUi(this);
    labels = new QLabel(this);
    QMovie *movie = new QMovie("/home/ma/图片/123.gif");
    labels->setMovie(movie);
    movie->start();
    */
    //setWindowFlags(Qt::FramelessWindowHint);
    QLabel *label = new QLabel();
    QMovie *movie = new QMovie("/home/ma/图片/124.gif");
    label->resize(500,500);
    label->setMovie(movie);
    movie->start();
    QGridLayout *thatlayout = new QGridLayout();
    //thatlayout->addWidget(label,0,0,1,1);
    thatlayout->addLayout(highlayout,1,0,1,1);
    thatlayout->addLayout(threelayout,2,0,1,1);
    //thatlayout->setSizeConstraint(QLayout::SetFixedSize);
    QVBoxLayout *qliftlayout = new QVBoxLayout();
    qliftlayout->addStretch();
    qliftlayout->addWidget(buttonnewusr);
    QHBoxLayout *downlayout = new QHBoxLayout();
    downlayout->addLayout(qliftlayout);
    downlayout->addLayout(thatlayout);
    downlayout->addWidget(buttonlogin);

    QGridLayout *main_layout = new QGridLayout(this);
    main_layout->addWidget(label,0,0,1,1);
    main_layout->addLayout(downlayout,1,0,100,1);
    main_layout->setHorizontalSpacing(10);
    main_layout->setVerticalSpacing(10);
    main_layout->setContentsMargins(10,10,10,10);
    main_layout->setSizeConstraint(QLayout::SetFixedSize);
    connect(buttonlogin,SIGNAL(clicked()),this,SLOT(buttonss()));
    connect(buttonnewusr,SIGNAL(clicked()),this,SLOT(buttonnewusrss()));
    connect(buttonfindpasswd,SIGNAL(clicked()),this,SLOT(buttonnchangemima()));
    connect(checkboxlogin,SIGNAL(clicked()),this,SLOT(zidongdenglu()));
    connect(checkboxspass,SIGNAL(clicked()),this,SLOT(jizhumima()));
    /*
    QPushButton *button = new QPushButton("Animated Button",this);
    button->show();
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(10000);
    animation->setStartValue(QRect(0, 0, 100, 30));
    animation->setEndValue(QRect(250, 250, 100, 30));
    animation->start();
*/

    /*
    QPushButton button("Animated Button");
    button.show();

    QPropertyAnimation animation(&button, "geometry");
    animation.setDuration(10000);
    animation.setStartValue(QRect(0, 0, 100, 30));
    animation.setEndValue(QRect(250, 250, 100, 30));

    animation.start();
    */
    /*
    if(::connect(SOCKETS,(struct sockaddr *)&addr,sizeof(addr)) == -1)
    {
        printf("connect faile,%s\n",strerror(errno));
        this->hide();
    }
    */
}

Dialog::~Dialog()
{

}
void Dialog::buttonss()
{
    //QMessageBox::information(this,"guanyv","mayicheng --version 2000-5000\ndaobanbijiu");
    QString s1 = texteditusername->text();
    QString s2 = textedituserpasswd->text();
    if(s1.isEmpty() || s2.isEmpty())
    {
        QMessageBox::information(this,"???","咋有的框框没输入就想进去？？");
        return;
    }
    willlogin logins1;
    QByteArray asd = s1.toLatin1();
    logins1.weiyibiaoshifu = 3;
    logins1.yonghuming = asd.data();
    asd = s2.toLatin1();
    logins1.yongbumima = asd.data();
    logins1.yongbumima = (char *)minamd5(logins1.yongbumima);
    cJSON *wewilllogin = cJSON_CreateObject();
    cJSON_AddNumberToObject(wewilllogin,"weiyibiaoshifu",logins1.weiyibiaoshifu);
    cJSON_AddStringToObject(wewilllogin,"yonghuming",logins1.yonghuming);
    cJSON_AddStringToObject(wewilllogin,"yongbumima",logins1.yongbumima);
    //char *out = cJSON_Print(wewilllogin);
    char *out = cJSON_PrintUnformatted(wewilllogin);
    int a = strlen(out) + 1;
    send(SOCKETS,&a,4,0);
    send(SOCKETS,out,a,0);
    //TODO //~~~socket~~~/////
    SOCKET_FD = SOCKETS;
    //TODO //~~~~socket~~/////
    int *size = (int *)malloc(sizeof(int));
    int rcm = 0;
    int sheng = 4;
    const int aaabbbccc = 4;
    int shengi = 5;
    while(rcm < 4)
    {
        rcm += recv(SOCKETS,size,sheng,0);
        sheng = aaabbbccc - rcm;
        shengi--;
        if(shengi == 0)
            break;
    }
    if(rcm == 0)
    {
        printf("%s",strerror(errno));
        QMessageBox::information(this,"error","服务器正在更新，请稍后重试！");
        return;
    }
    if((*size) == 0)
    {
        QMessageBox::information(this,"666","你想一个号登陆几次？？？\n你想上天吗？？？？?");
        return;
    }
    if((*size) == 2)
    {
        QMessageBox::information(this,"666","这么秀,号和密码不对就敢登陆？？？");
        return;
    }



    size = (int *)malloc(sizeof(int));
    rcm = 0;
    sheng = 4;
    shengi = 5;
    while(rcm < 4)
    {
        rcm += recv(SOCKET_FD,size,sheng,0);
        sheng = aaabbbccc - rcm;
        shengi--;
        if(shengi == 0)
            break;
    }
    if(shengi == 0)
    {
        //QMessageBox::information(this,"error","服务器正在维护，请稍后再试,sorry");
    }
    rcm = 0;
    sheng = *size;
    char *buf1 = (char *)calloc(*size,0);
    while(rcm < *size)
    {
        rcm += recv(SOCKET_FD,&buf1[rcm],sheng,0);
        sheng = *size - rcm;
    }
    //QMessageBox::information(this,"111",buf1);
    //printf("buf1 = %s\n",buf1);
    //QMessageBox::information(this,"error",buf1);
    cJSON *CONGZHELIKAISHI = cJSON_Parse(buf1);
    char *out111 = NULL;
    out111    =    cJSON_Print(CONGZHELIKAISHI);
    //if(out111)
    //QMessageBox::information(this,"111",QString::number(strlen(out111)));
    //free(buf1);
    if(cJSON_GetObjectItem(CONGZHELIKAISHI,"0")  != NULL)
        strcpy(bbstring,cJSON_GetObjectItem(CONGZHELIKAISHI,"0")->valuestring);
    if(cJSON_GetObjectItem(CONGZHELIKAISHI,"1")  != NULL)
        strcpy(ccstring,cJSON_GetObjectItem(CONGZHELIKAISHI,"1")->valuestring);
    if(cJSON_GetObjectItem(CONGZHELIKAISHI,"2")  != NULL)
        strcpy(ddstring,cJSON_GetObjectItem(CONGZHELIKAISHI,"2")->valuestring);
    if(cJSON_GetObjectItem(CONGZHELIKAISHI,"3")  != NULL)
        strcpy(eestring,cJSON_GetObjectItem(CONGZHELIKAISHI,"3")->valuestring);
    if(cJSON_GetObjectItem(CONGZHELIKAISHI,"4")  != NULL)
        strcpy(ffstring,cJSON_GetObjectItem(CONGZHELIKAISHI,"4")->valuestring);
    if(cJSON_GetObjectItem(CONGZHELIKAISHI,"5")  != NULL)
        strcpy(ggstring,cJSON_GetObjectItem(CONGZHELIKAISHI,"5")->valuestring);



    this->hide();
    drawerss.resize(400,800);
    drawerss.show();

    static int openthethread = 1;
    if(openthethread)
    {
        //只能进来一次
        openthethread = 0;
        pthread_t t1;
        pthread_create(&t1,NULL,recvduan,NULL);
    }
}
void Dialog::buttonnewusrss()
{
    abclog11.exec();
}

abclog::abclog(QWidget *parent) : QDialog(parent)
{
    texteditusername11 = new QLineEdit();
    textedituserpasswd11 = new QLineEdit();
    QPushButton *button = new QPushButton(tr("确定"));
    texteditusername11->setPlaceholderText("请输入你的用户名");
    textedituserpasswd11->setPlaceholderText("请输入你的密码");
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(texteditusername11);
    layout->addWidget(textedituserpasswd11);
    layout->addWidget(button);
    connect(button,SIGNAL(clicked()),this,SLOT(buttonnewusrssok()));
}

void abclog::buttonnewusrssok()
{
    QString s1 = texteditusername11->text();
    QString s2 = textedituserpasswd11->text();
    if(!s1.isEmpty() && !s2.isEmpty())  //上传服务器
    {
        zhuceyonghu zhuce1;         //这个就是一个注意的点，转换
        zhuce1.weiyibiaoshifu = 1;
        QByteArray asd = s1.toLatin1();
        zhuce1.yonghuming = asd.data();
        asd = s2.toLatin1();
        zhuce1.yongbumima = asd.data();
        zhuce1.yongbumima = (char *)minamd5(zhuce1.yongbumima);
        //texteditusername11->setText(zhuce1.yongbumima);
        //cjson
        //这一步其实相当于打包
        cJSON *jsonzhuceyonghu = cJSON_CreateObject();
        cJSON_AddNumberToObject(jsonzhuceyonghu,"weiyibiaoshifu",zhuce1.weiyibiaoshifu);
        cJSON_AddStringToObject(jsonzhuceyonghu,"yonghuming",zhuce1.yonghuming);
        cJSON_AddStringToObject(jsonzhuceyonghu,"yongbumima",zhuce1.yongbumima);
        char *out = cJSON_Print(jsonzhuceyonghu);
        //texteditusername11->setText(out);
        out = cJSON_PrintUnformatted(jsonzhuceyonghu);
        int a = strlen(out) + 1;
        send(SOCKETS,&a,4,0);
        send(SOCKETS,out,a,0);

        int *size = (int *)malloc(sizeof(int));
        int rcm = 0;
        int sheng = 4;
        const int aaabbbccc = 4;
        int shengi = 5;
        while(rcm < 4)
        {
            rcm += recv(SOCKETS,size,sheng,0);
            sheng = aaabbbccc - rcm;
            shengi--;
            if(shengi == 0)
                break;
        }
        if(rcm == 0)
        {
            printf("%s",strerror(errno));
            return;
        }
        char yonghuming[100];
        char yonghuweiyixvlie[100];
        sprintf(yonghuming,"请记住您的MM号:%d",*size);
        sprintf(yonghuweiyixvlie,"请记住您的MM唯一密匙:%d",zheshiyigehenzhongyaodedongxi(*size));
        texteditusername11->setText(yonghuming);
        textedituserpasswd11->setText(yonghuweiyixvlie);
    }

    QMessageBox::information(this,"提示","输入框不能为空!\n或者有个小惊喜^_^");
}
void Dialog::buttonnchangemima()
{
    bcdlog22.exec();
}
bcdlog::bcdlog(QWidget *parent) : QDialog(parent)
{
    texteditusername11 = new QLineEdit();
    textedituserpasswd11 = new QLineEdit();
    textedituserpasswd22 = new QLineEdit();
    textedituserpasswd33 = new QLineEdit();
    QPushButton *button = new QPushButton(tr("确定"));
    texteditusername11->setPlaceholderText("请输入您的MM号");
    textedituserpasswd11->setPlaceholderText("请输入您的唯一密匙");
    textedituserpasswd22->setPlaceholderText("请输入您的新密码");
    textedituserpasswd22->setEchoMode(QLineEdit::Password);
    textedituserpasswd33->setPlaceholderText("请再次输入您的密码");
    textedituserpasswd33->setEchoMode(QLineEdit::Password);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(texteditusername11);
    layout->addWidget(textedituserpasswd11);
    layout->addWidget(textedituserpasswd22);
    layout->addWidget(textedituserpasswd33);
    layout->addWidget(button);
    connect(button,SIGNAL(clicked()),this,SLOT(buttonnewusrssok()));
}
void bcdlog::buttonnewusrssok()
{
    QString s1 = texteditusername11->text();
    QString s2 = textedituserpasswd11->text();
    QString s3 = textedituserpasswd22->text();
    QString s4 = textedituserpasswd33->text();
    if(!s1.isEmpty() && !s2.isEmpty() && !s3.isEmpty() && !s4.isEmpty())  //上传服务器
    {
        if(s3 != s4)
        {
            QMessageBox::information(this,"皮！","老哥，两次密码不一样咋玩！");
            return;
        }
        QByteArray asd = s1.toLatin1();
        int mimajiance1 = atoi(asd.data());
        asd = s2.toLatin1();
        int mimajiance2 = atoi(asd.data());
        if(zheshiyigehenzhongyaodedongxi(mimajiance1) != mimajiance2)
        {
            QMessageBox::information(this,"提示","MM号或密匙错误，请重新输入！");
            return;
        }

        asd = s1.toLatin1();

        xiugaimima zhuce1;         //这个就是一个注意的点，转换
        zhuce1.weiyibiaoshifu = 2;

        zhuce1.yonghumishi = asd.data();
        asd = s3.toLatin1();
        zhuce1.yongbumima = asd.data();
        zhuce1.yongbumima = (char *)minamd5(zhuce1.yongbumima);
        //texteditusername11->setText(zhuce1.yongbumima);
        //cjson
        //这一步其实相当于打包
        cJSON *jsonzhuceyonghu = cJSON_CreateObject();
        cJSON_AddNumberToObject(jsonzhuceyonghu,"weiyibiaoshifu",zhuce1.weiyibiaoshifu);
        cJSON_AddStringToObject(jsonzhuceyonghu,"yonghumishi",zhuce1.yonghumishi);
        cJSON_AddStringToObject(jsonzhuceyonghu,"yongbumima",zhuce1.yongbumima);
        char *out = cJSON_Print(jsonzhuceyonghu);
        //texteditusername11->setText(out);
        out = cJSON_PrintUnformatted(jsonzhuceyonghu);
        int a = strlen(out) + 1;
        send(SOCKETS,&a,4,0);
        send(SOCKETS,out,a,0);

        int *size = (int *)malloc(sizeof(int));
        int rcm = 0;
        int sheng = 4;
        const int aaabbbccc = 4;
        int shengi = 5;
        while(rcm < 4)
        {
            rcm += recv(SOCKETS,size,sheng,0);
            sheng = aaabbbccc - rcm;
            shengi--;
            if(shengi == 0)
                break;
        }
        if(rcm == 0)
        {
            printf("%s",strerror(errno));
            return;
        }
        if(*size == 0)
        {
            QMessageBox::information(this,"提示","失败，请重试");
            return;
        }
        char yonghuming[100];
        char yonghuweiyixvlie[100];
        sprintf(yonghuming,"恭喜，您已修改成功！");
        sprintf(yonghuweiyixvlie,"请牢记您的密码～。～");
        texteditusername11->setText(yonghuming);
        textedituserpasswd11->setText(yonghuweiyixvlie);
    }

    QMessageBox::information(this,"提示","输入框不能为空!\n或者有个小惊喜^_^");
}
void Dialog::zidongdenglu()
{
    QMessageBox::warning(this,"警告","为安全考虑，不再启用该功能");
}
void Dialog::jizhumima()
{
    QMessageBox::warning(this,"警告","为安全考虑，不再启用该功能");
}
