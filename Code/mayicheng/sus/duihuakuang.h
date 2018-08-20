#ifndef DUIHUAKUANG_H
#define DUIHUAKUANG_H

#include <QMainWindow>
#include <cJSON.h>
#include <QTime>
#include <QMenu>
#include <QAction>
#include <QTextEdit>
#include <QMenuBar>

class duihuakuang : public QMainWindow
{
    Q_OBJECT
public:
    explicit duihuakuang(QWidget *parent = nullptr);
    //duihuakuang(QWidget *parent = 0);
private:
    QAction *open;
    QAction *fileexit;
    QMenu *menu;
    QTextEdit *text1;
    QTextEdit *text2;


signals:

public slots:
    void openfile();
    void exitfile();
};

#endif // DUIHUAKUANG_H
