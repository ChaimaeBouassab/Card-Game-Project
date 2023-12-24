//  menu.h

#ifndef MENU_H
#define MENU_H

#include <QObject>

class Menu : public QObject
{
    Q_OBJECT
public:
    Menu(QObject *parent = nullptr);

    QObject *parent;

    int time{0};

signals:

};

#endif // MENU_H
