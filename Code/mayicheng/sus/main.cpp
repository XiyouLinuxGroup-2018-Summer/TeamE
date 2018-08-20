#include "dialog.h"
#include <QApplication>
int SOCKET_FD = 100;
int times = 0;
extern QTimer tm;
struct haoyouyonghuxinxi
{
    int weiyibiaoshifu;
    int wwwwwwwwwwww;           //只为填充的，没有实际意义
    char *id;
    char *name;
    char *age;
    char *style;
    char *namehead;
};
int haoyouyonghuxinix = 0;
haoyouyonghuxinxi a[200] = { 0 };
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();

    return a.exec();
}
