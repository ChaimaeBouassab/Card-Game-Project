#ifndef TABLE_H
#define TABLE_H

#include <QWidget>
#include <QObject>
#include <QVector2D>
#include <QList>
#include "card.h"


class Table : public QObject
{
    Q_OBJECT
public:
    explicit Table(QVector2D pos, QObject *parent = nullptr);

    QVector2D pos;
    QVector2D size;
    QObject *parent;
    QList<Card*> cards;
    QList<QVector2D> cardsOrigins;

    Card *cardToFollow;
    int playerToCollect;
    Card *cardCollector;
    bool onCollecting{false};
    QList<Card*> cardsCollected;
    bool isFinishCollecting{false};

    qreal time{0};

    void onCollectingCards();

    void reposition();
    void draw();

    void initSize();
    void initCardOrigins();

    void addCard(Card *card);
    void removeCard(Card *card);

    bool collide(QVector2D point);
    void collectCard(Card *card, int mode) ;
};

#endif // TABLE_H
