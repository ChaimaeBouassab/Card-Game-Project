#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QVector2D>
#include <card.h>

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QVector2D position,int mode,QObject *parent);

    QVector2D position;
    int mode;

    QVector2D cardsCollectedPos;

    QList<Card*> cards;
    QList<QVector2D> cardsOrigins;

    QList<Card*> cardsCollected;


    QObject *parent;

    int score{0};

    bool canPlay={false};
    bool onPlaying={false};

    bool onReloadingCards{false};

    void draw();
    void reposition();
    void initCardOrigins();

    void annonceCard(Card *card);

    void addCard(Card *card);
    void removeCard(Card *card);

    void togglePlay();
    void autoPlay();
    Player *getAdversary();

};

#endif // PLAYER_H
