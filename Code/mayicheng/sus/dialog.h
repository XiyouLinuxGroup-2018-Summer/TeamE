#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QCheckBox>
#include <QTextEdit>
#include <QLineEdit>
#include <drawer.h>
#include <cJSON.h>
#include <QTime>
struct zhuceyonghu
{
public:
    int weiyibiaoshifu;     //1
    char *yonghuming;
    char *yongbumima;
};

struct xiugaimima
{
public:
    int weiyibiaoshifu;     //2
    char *yongbumima;
    char *yonghumishi;
};
struct willlogin
{
public:
    int weiyibiaoshifu;     //3
    char *yonghuming;
    char *yongbumima;
};

class abclog : public QDialog
{
    Q_OBJECT

public:
    abclog(QWidget *parent = 0);
private:
    QLineEdit *texteditusername11;
    QLineEdit *textedituserpasswd11;

private slots:
    void buttonnewusrssok();

};
class bcdlog : public QDialog
{
    Q_OBJECT

public:
    bcdlog(QWidget *parent = 0);
private:
    QLineEdit *texteditusername11;
    QLineEdit *textedituserpasswd11;
    QLineEdit *textedituserpasswd22;
    QLineEdit *textedituserpasswd33;
private slots:
    void buttonnewusrssok();

};
class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();
private:
    QPushButton *buttonfindpasswd,*buttonnewusr,*buttonlogin;
    //QTextEdit *texteditusername,*textedituserpasswd;
    QCheckBox *checkboxspass,*checkboxlogin;
    QLineEdit *texteditusername,*textedituserpasswd;

    QDialog qq1;    //这三个是为注册帐号用的
    QLineEdit *texteditusername11;
    QLineEdit *textedituserpasswd11;
    drawer drawerss;
    abclog abclog11;
    bcdlog bcdlog22;

private slots:
    void buttonss();
    void buttonnewusrss();      //这个2是为注册帐号用的
    void buttonnchangemima();
    void zidongdenglu();
    void jizhumima();
    //void buttonnewusrssok();

};
#endif // DIALOG_H
