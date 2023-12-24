#include "player.h"
#include <QPainter>
#include "mainwindow.h"
#include  <QRandomGenerator>

Player::Player(QVector2D position,int mode,QObject *parent)
    : QObject{parent}

{
    this->position=position;
    this->mode=mode;
    this->parent=parent;


}

void Player::initCardOrigins()
{
    cardsOrigins.clear();
    for (qreal i = static_cast<qreal>(cards.size()) * -0.5f; i < static_cast<qreal>(cards.size()) * 0.5f; i++)
    {
        const int step = 90;
        cardsOrigins.append(position + QVector2D(step * (i + 0.5), 0));
    }
}

void Player::annonceCard(Card *card)
{
    Table *table = qobject_cast<MainWindow*>(parent)->table;
    card->setToFirstPlace();
    table->collectCard(card, mode);
    removeCard(card) ;
    togglePlay();
}


void Player :: addCard(Card *card) {
    card->setShadow(true);
    cards. append (card) ;
    initCardOrigins();

}


void Player ::removeCard(Card *card){
    card->setShadow(false);
    cards.removeOne(card);
    initCardOrigins();
}


Player *Player::getAdversary()
{

    MainWindow *main = qobject_cast<MainWindow*>(parent);
    return mode == MainWindow::PLAYER ? main->ai : main->player;

}

void Player::togglePlay()
{
    canPlay=false;
    getAdversary()->canPlay=true;
}

void Player::autoPlay()
{
    MainWindow *main = qobject_cast<MainWindow*>(parent);
    onPlaying = true;

    if (cards.size() == 0) return;

    Card *card = cards[QRandomGenerator::global()->bounded(cards.size())];

    Animation *waitToTurnCard = new Animation(500, main);
    connect(waitToTurnCard, &Animation::onFinished, [=] {
        card->toggleTurnCard();

        Animation *waitToAnnonceCard = new Animation(500, main);
        connect(waitToAnnonceCard, &Animation::onFinished, [=]() {
            annonceCard(card);
            onPlaying = false;
        });

        waitToAnnonceCard->start();
    });

    waitToTurnCard->start();
}


void Player::reposition()
{
    for (int i = 0; i < cards.size(); i++) {
        cards[i]->followTarget(cardsOrigins[i]);
    }

    if (cardsCollected.size() > 0) {
        int i = 0;
        for (Card *card : cardsCollected) {
            card->followTarget(cardsCollectedPos + QVector2D(0, - i * .5));
            i++;
        }
    }
    for (Card *card : cards) {
        if (!card->ondrag && mode == MainWindow::PLAYER) {
            Table *table = qobject_cast<MainWindow*>(parent)->table;
            if (table->collide(card->unleashed_post)) {
                annonceCard(card) ;
            }
                card->unleashed_post = QVector2D(-100, -100);
        }
    }
    MainWindow *main=qobject_cast<MainWindow*>(parent);
    if (mode == MainWindow::AI && canPlay && main->canPlay) {
        if (!onPlaying) autoPlay();
    }

    if (cards.size() == 0 && !onReloadingCards && main->isCardsConfigured) {
        onReloadingCards = true;
    }

        if (onReloadingCards) {
                if (main->distributionCards(mode, cards, 3, mode == MainWindow::PLAYER)) {
                onReloadingCards = false;
                }
        }

}

void Player::draw()
{
    MainWindow *main= static_cast<MainWindow*>(parent);
    QPainter painter(main);

    //painter.drawEllipse(position.x(),position.y(),50,50);

    //Le cercle est positionné au centre comme prévu et proche du bas.
    //L'objectif est de prendre la position du joueur, c'est là que les
    //cartes des joueurs apparaîtront.==> j'ai fait ce test pour cette raison.


}



