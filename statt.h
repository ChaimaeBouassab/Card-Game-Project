#ifndef STATT_H
#define STATT_H

#include <QObject>
#include <QVector2D>

class Statt : public QObject
{
    Q_OBJECT
public:
    Statt(QVector2D pos, QVector2D size, QObject *parent);

    QVector2D pos;
    QVector2D size;
    qreal scaleX{0};
    QObject *parent;

    bool isActive{false};

    void draw();


};

#endif // STATT_H
