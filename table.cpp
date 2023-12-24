#include "table.h"
#include "mainwindow.h"
#include <QPainter>
#include <QTime>
#include "card.h"

Table::Table(QVector2D pos, QObject *parent)
    : QObject{parent}
{
    this->pos = pos;
    this->parent=parent;
    initSize();
}



bool Table::collide(QVector2D point) {
    return (point.y() >= pos.y() - size.y() * .5) && (point.y() <= pos.y() + size.y() * .5);
}

// La fonction isMatchToCards est utilisée pour effectuer
//la vérification de correspondance d'indice.
//Si aucune correspondance n'est trouvée,la carte est ajoutée à la table.

Card* isMatchToCards(int cardIndex, QList<Card*> cards){
  //cette fonction recherche dans la liste de cartes la carte qui a l'indice
    //spécifié et la renvoie.
    Card *cardToReturn = nullptr;
    for (Card *_card : cards) {
        if (cardIndex != _card->index) continue;
        cardToReturn = _card;
        break;
    }
    return cardToReturn;
}

// la fonction "onCollectingCards"  implémente la logique de collecte de cartes
//et les actions associées, telles que la mise à jour des positions des cartes,
//la gestion du temps écoulé, et l'ajout des cartes collectées à une liste spécifique
//(cardsCollected).=>

void Table::onCollectingCards()
{
    MainWindow *main = qobject_cast<MainWindow*>(parent);
    time += 1000.0 / main->FPS;

    Card *isMatch = isMatchToCards(cardCollector->index, cards);
    //Appelle la fonction isMatchToCards pour vérifier s'il y a
    //une carte dans la liste cards avec l'indice correspondant à cardCollector->index.
    if (isMatch != nullptr) cardToFollow = isMatch;
    //Si une correspondance de carte a été trouvée,
    //la variable cardToFollow est mise à jour avec cette carte.

    if (time >= 500.0 || isMatch == nullptr) {
        if (isMatch == nullptr) {
            isMatch = isMatchToCards(cardCollector->index + 1, cards);
            if (isMatch != nullptr) cardToFollow = isMatch;

            if (time - 200 >= 200.0 || isMatch == nullptr) {
                if (isMatch != nullptr) {
                    if(playerToCollect== MainWindow::PLAYER) main->player->score+=1;
                    else
                        main->ai->score+=1;
                    //Exécuté si une nouvelle correspondance de carte est trouvée.
                    removeCard(isMatch);
                    //Supprime la carte correspondante de la liste cards.
                    cardCollector->setToFirstPlace();
                    //Positionne cardCollector et la carte correspondante au premier emplacement.
                    cardsCollected.append(cardCollector);
                    //Ajoute cardCollector à la liste cardsCollected.
                    cardCollector = isMatch;
                    cardCollector->setToFirstPlace();
                    //cardCollector est mis à jour avec la nouvelle correspondance.
                    //Le temps est réinitialisé à zéro.
                } else {
                    if(playerToCollect== MainWindow::PLAYER) main->player->score+=2;
                    else
                        main->ai->score+=2;
                    //si aucune correspondance de carte n'a été trouvée dans la recherche suivante.
                    cardsCollected.append(cardCollector);
                    onCollecting = false;
                    isFinishCollecting = true;
                    cardCollector = nullptr;
                }
                time = 0;

            }
        } else {
            //Exécuté si le temps écoulé n'est pas suffisamment
            //long ou s'il y a une correspondance de carte.
            removeCard(isMatch);
            isMatch->setToFirstPlace();
            cardCollector->setToFirstPlace();
            cardsCollected.append(isMatch);
            time = 0;
        }
    }

    if (cardCollector != nullptr) {
        cardCollector->followTarget(cardToFollow->position);

    }
}


//la fonction Table::collectCard ajoute une carte à la table
//uniquement si aucune carte avec le même indice n'est déjà
//présente dans la liste actuelle de cartes sur la table.

void Table::collectCard(Card *card, int mode) {
    MainWindow *main=qobject_cast<MainWindow*>(parent);

    Card *isMatch = isMatchToCards(card->index, cards);
    if (isMatch == nullptr){
        //Ajout de la Carte à la Table
// (Si Aucune Correspondance n'a été Trouvée) .
        addCard(card) ;
        //Si isMatch est égal à nullptr,
        //cela signifie qu'aucune carte ayant
        //le même indice n'a été trouvée dans la liste.

    } else {
        playerToCollect = mode;
        cardCollector = card;
        onCollecting = true;
        main->canPlay = false;
    }


}

void Table::reposition()
{
    MainWindow *main = qobject_cast<MainWindow*>(parent);

    for(Card *card: cards){

        card->followTarget();
    }

    if (onCollecting) {
        onCollectingCards();
    }

    if (cardsCollected.size() > 0) {
        for (Card *card : cardsCollected) {
            if (cardCollector == nullptr) continue;

            card->followTarget(cardCollector->position, 0.5);
            //la position du cardCollector, avec une transition de 0.5 unité
            //( une animation de mouvement).
        }
            if (isFinishCollecting) {
            //Si playerToCollect est == MainWindow::PLAYER, alors playerCollector pointe vers
            //l'objet player de la classe MainWindow. Sinon, il pointe vers l'objet ai
                Player *playerCollector = (playerToCollect == MainWindow::PLAYER) ? main->player : main->ai;
                for (Card *card : cardsCollected) {
                    card->setToFirstPlace();
                    card->toggleTurnCard();
                    playerCollector->cardsCollected.append(card);
                }

                cardsCollected.clear();
                initCardOrigins();
                isFinishCollecting = false;
                main->canPlay = true;

            }
    }
}

void Table::draw(){

    MainWindow *main = qobject_cast<MainWindow*>(parent);

    bool ondrag = false;
    bool collided = false;
    for (Card *card : main->player->cards) {
            if (!card->ondrag) continue;
            if (collide(main->mouse.pos)) {
                collided = true;
            }
                ondrag = true;
                break;
    }
    if(ondrag){

    QTime time;
    qreal offset = time.currentTime() .msecsSinceStartOfDay();

    QPainter painter(main);
    QList<QLine> lines;

    //painter.drawEllipse(pos.x() - 25, pos.y() - 25, 50, 50);

   lines.append(QLine(0, pos.y() - size.y() * .6 , main->width(), pos.y() - size.y() * .6 ));
   lines.append(QLine(0, pos.y() + size.y() * .6 , main->width(), pos.y() + size.y() * .6));


    QPen pen(QColor(61, 61, 61));
   if (collided) pen.setColor(QColor(150, 255, 255)) ;
    pen.setWidth(4);
    pen.setStyle(Qt::DashLine);

    pen.setDashOffset(offset * .01);
    painter.setPen(pen);
    painter.drawLine(lines[0]);
    pen.setDashOffset(offset * -.01);
    painter.setPen(pen);
    painter.drawLine(lines[1]);
    //ce code crée une ligne horizontale avec des propriétés graphiques...
    //(couleur et largeur du trait) et la dessine sur un objet QPainter.
    //La ligne est positionnée verticalement à pos.y() - size.y() * .5 - 10
    //et s'étend horizontalement de 0 à main->width().
    }

}

void Table::initSize()
{
    MainWindow *main = qobject_cast<MainWindow*>(parent);

    if (cards.size() <= 5) {
        size = QVector2D(main->width(), 150);
    } else {
        size = QVector2D(main->width(), 290);
    }

}

void Table::initCardOrigins()
{
    cardsOrigins.clear();

    int index = 0;
    qreal length = cards.size() < 5 ? cards.size() : 5;

    for (qreal i = -length * 0.5; i < length * 0.5; i++) {
        if (cardsOrigins.size() == 5) break;
        const qreal yStep = cards.size() > 5 ? -70 : 0;
        const qreal step = 93;
        cardsOrigins.append(pos + QVector2D(step * (i + 0.5), yStep));
        cards[index]->targetToFollow = cardsOrigins[index];
        index++;
    }

    if (cards.size() > 5) {
        length = cards.size() - 5;

        for (qreal i = -length * 0.5; i < length * 0.5; i++) {
            const qreal step = 93;
            cardsOrigins.append(pos + QVector2D(step * (i + 0.5), 70));
            cards[index]->targetToFollow = cardsOrigins[index];
            index++;
        }
    }

    initSize();
}

void Table::addCard(Card *card)
{
    card->setShadow(false);
    cards.append(card);
    initCardOrigins();
}

void Table::removeCard(Card *card)
{
    cards.removeOne(card);
}
