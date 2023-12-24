#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "card.h"
#include <QPainter>
#include <QBrush>
#include <QPixmap>
#include <QMouseEvent>
#include <QTimer>
#include "mouse.h"
#include <QRandomGenerator>
#include <QVector2D>
#include "player.h"
#include "statt.h"
#include <QPainterPath>
#include <QPushButton>
#include "menu.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(866,650);


    QString iconRestartSrc = ":/new/prefix1/textures-ch/restart.png";
    QString iconRestartDisabledSrc = ":/new/prefix1/textures-ch/restart-disabled.png";

    QPushButton *restartButton = new QPushButton(this);
    restartButton->setStyleSheet("background-color: #00000000");
    restartButton->setGeometry(10, 10, 40, 40);
    restartButton->setIcon(QIcon(iconRestartSrc));
    restartButton->setIconSize(QSize(32, 32));
    connect(restartButton, &QPushButton::clicked, this, [=]() {
        restartButton->setEnabled(false);
        restartButton->setIcon(QIcon(iconRestartDisabledSrc));

        Animation *waitToEnableRestartButton = new Animation(1000, true, this);
        connect(waitToEnableRestartButton, &Animation::onFinished, this, [=]() {
            restartButton->setIcon(QIcon(iconRestartSrc));
            restartButton->setEnabled(true);
        });
        waitToEnableRestartButton->start();


        // on recommence le jeu !
        restart();
    });

    light. load(":/new/prefix1/textures-ch/light.png");


    QTimer *timer = new QTimer(this);

    connect(timer, &QTimer :: timeout, [=] {
        if(gameStarted){

        reposition();
        }
    });

    //const qreal FPS = 120;

    timer->start(1000 / FPS);

    //QVector2D margin(0,40);

    dealer_pos = QVector2D(width() - 60, (height()-margin.y()) * .5);

    initCards();
    initGame();

    stat = new Statt(QVector2D(0, 0), QVector2D(width(), height()), this);

    new Menu(this);

}

void MainWindow::initCards()
{

    QString types[] = { "A", "B", "C", "D" };

    for (int i = 0; i < 40; i++) {
        // Calcul du type de carte en fonction de l'indice de la boucle
        // L'expression i / 10 donne l'indice du tableau types pour
        //déterminer le type de carte en fonction de la division de
        //l'indice de la boucle par 10.
        QString type = types[i / 10];
        // Calcul de l'index de carte en fonction de l'indice de la boucle
        qreal index = i%10 + 1;
        // Création d'une nouvelle carte avec le type et l'index calculés
        Card *card = new Card(type, index, this);

        // Positionnement initial de la carte à (300, 300) (on peut ajuster ces valeurs)
        card->position = QVector2D(300, 300);
        card->position=dealer_pos;
        // Ajout de la carte à la liste de cartes
        cards. append (card) ;
    }

    for (int i = 0; i < cards.size(); i++) {
        int j = QRandomGenerator::global()->bounded(cards.size());
        cards. swapItemsAt(i, j);
    }

    for (int i = 0; i < cards. size(); i++) {
        Card *card = cards [i];
        card->position -= QVector2D(0, i * .25);
    }
}
void MainWindow::initGame(){
    table= new Table(QVector2D(width() * .5,( height()-margin.y()) * .5), this);

    player= new Player(QVector2D(width() * .5, height() - (80+margin.y())), PLAYER, this);
    player->cardsCollectedPos=QVector2D(80,(height()-margin.y()) -100);  
    player->canPlay=firstToPlay;

    ai = new Player(QVector2D(width() * .5, 80), AI, this);
    ai->cardsCollectedPos =QVector2D(80,100);
    ai->canPlay=!firstToPlay;


    //stat = new Statt(QVector2D(0, 0), QVector2D(width(), height()), this);

    /* table->addCard(cards[6]);
    table->addCard(cards[7]);
    table->addCard(cards[8]);
    table->addCard(cards[9]);


    player->addCard(cards[0]);
    player->addCard(cards[1]);
    player->addCard(cards[2]);
    //Lorsque l'interface s'ouvre automatiquement, trois cartes sont déposées à la place du joueur.


    ai->addCard(cards[3]);
    ai->addCard(cards[4]);
    ai->addCard(cards[5]);*/


}

void MainWindow::restart()
{
    // on va supprimer les joueurs et la table
    delete player;
    delete ai;
    delete table;

    //on supprime les cartes
    //elle supprime chaque objet de type Card présent dans la liste cards.
    for(Card *card:cards){
        delete card;
    }
    cards.clear();
    //on supprime les animations
   //La boucle parcourt la liste des animations,
    //supprime chaque animation non statique de la liste,
    //puis libère la mémoire associée à chaque animation.
    for (int i = 0; i < animations.size(); i++) {
        Animation *animation = animations[i];
        // Ignorer les animations statiques
        if (animation->isStatic) {
            continue;
        }

        animations.removeOne(animation);

        // Supprimer l'animation
        delete animation;
    }
    // Réinitialiser les indicateurs de fin de jeu et de configuration des cartes
    isGameFinished = false;
    isCardsConfigured = false;
    canPlay = false;

    // Créer une nouvelle animation pour réduire l'échelle du tableau de statistiques
    Animation *redeployStat = new Animation(200, this);
    connect(redeployStat, &Animation::onUpdate, this, [=](qreal progress) {
        // Mettre à jour l'échelle du tableau de statistiques en fonction du progrès
        stat->scaleX = 1 - progress;
    });

    // Connecter l'événement de fin de l'animation à une lambda qui réinitialise l'échelle

    // pour mettre scaleX = 0 parceque onUpdate avec son progression il ne va pas atteint
    //en 0 genre 0.1 ou 0.  donc je veux que ça soit 0 net parceque ce scaleX est multiplié
    //avec la longueur de la tableau de statistique
    connect(redeployStat, &Animation::onFinished, this, [=]() {

        //OnFinish ça veut dire que lorsque l'animation atteint le point
        //finale genre lorsqu'elle atteint le temps donné

        // Réinitialiser l'échelle du tableau de statistiques
        stat->scaleX = 0;

        //redeployStat->start();
    });

    // Démarrer l'animation pour réduire l'échelle du tableau de statistiques
    redeployStat->start();

    // Inverser l'indicateur du premier joueur
    firstToPlay = !firstToPlay;
    // lorsqu'on met 'restart' celui qui jouera en premier
    //sera celui qui etait la deuxieme pour le jeu precedent

// le joueur qui commence en premier change à chaque redémarrage,
    //créant ainsi une dynamique différente à chaque début de jeu.
    //Cela peut rendre l'expérience de jeu plus équilibrée et
    //intéressante pour les joueurs.

    // Initialiser les cartes et le jeu
    initCards();
    initGame();

}


Card* findCard(int index, QList<Card *> &parent_cards) {
    Card *_card = nullptr;
    for (Card *card : parent_cards) {
        if (card->index != index) continue;
        _card = card;
    }
    return _card;
}


bool MainWindow::distributionCards(int mode, QList<Card *> &parent_cards, int cardLength, bool show)
{
    if (parent_cards.size() == cardLength) return true;

    time += 1000 / FPS;

    if (time >= 200) {
        if (mode !=TABLE) {
            for (int i = cards.size() - 1; i >= 0; i -- ) {
                Card *card = cards[i];
                if (card->distributed) continue;
                if (mode == PLAYER) player->addCard(card);
                else ai->addCard(card);
                if (show) card->toggleTurnCard();
                card->distributed = true;
                card->setToFirstPlace();
                break;
            }


        }else {
            for (int i = cards.size() - 1; i >= 0; i -- ) {
                Card *card = cards[i];
                if (card->distributed) continue;
                bool isCardValid = false;
                if (table->cards.size() == 0) isCardValid = true;

                if (!findCard(card->index, table->cards)) {
                    if (!findCard(card->index - 1, table->cards)) {
                        if (!findCard(card->index + 1, table->cards)) {
                            isCardValid = true;
                        }
                    }
                }
                if (isCardValid) {
                    if (show) card->toggleTurnCard();
                    card->distributed = true;
                    card->setToFirstPlace();
                    table->addCard(card);
                    break;
                }
            }
        }

        time = 0;
    }
    return parent_cards.size() == cardLength;

}



void MainWindow::paintEvent(QPaintEvent *)
{
   // background(QColor(226, 156, 38));

    background(QColor(199, 21, 133));

    player->draw();
    table->draw();
    ai->draw();

    for(Card *card :cards){
        card->draw();
    }
    if (stat->isActive) stat->draw();
    QPainter painter(this);

    painter.setRenderHints(
        QPainter::Antialiasing ,
        QPainter::SmoothPixmapTransform);

    // Afficher la lumière
    painter.setOpacity(0.85);
    painter.drawImage(QRect(0, 0, width(), height()), light);
    painter.setOpacity(1);


   // painter.setRenderHint(QPainter::Antialiasing);
    //L'antialiasing est une technique utilisée pour réduire les effets
    //d'escalier (aliasing) dans les dessins, créant ainsi
    //des bords plus lisses et plus esthétiques.

    // Rectangle qui est en bottom.
    qreal h = 35;
    QVector2D size(width(), h);
    painter.fillRect(0, height() - size.y(), size.x(), size.y(), QBrush(QColor(255, 255, 255)));

    qreal r = 21;

    QPainterPath path;
    path.addEllipse(-(r + (r + 5)), -(r + 5), r * 2, r * 2);
    path.addEllipse(-(r - (r + 5)), -(r + 5), r * 2, r * 2);

    QBrush brush(QColor(61, 61, 61));
    QPen pen(QColor(255, 255, 255));
    pen.setWidth(5);
    painter.setPen(pen);
    painter.translate(width() * .5, height() - h * .5);
    painter.fillPath(path, brush);
    painter.drawPath(path);

    painter.setFont(QFont("Times", 14, QFont::Bold));
    // Score du joueur.
    painter.drawText(-(r + (r + 5)), -(r + 5), r * 2, r * 2, Qt::AlignCenter, QString::number(player->score));
    // Score du AI.
    painter.drawText(-(r - (r + 5)), -(r + 5), r * 2, r * 2, Qt::AlignCenter, QString::number(ai->score));
    painter.setPen(QColor(61, 61, 61));
    painter.drawText(-(r + 130), -r, 100, r * 2, Qt::AlignCenter, playerName);
    painter.drawText(-(r - 45), -r, 100, r * 2, Qt::AlignCenter, "AI");
    painter.resetTransform();

}
bool collidewithcard(QVector2D pos, Card *card){
    if (pos.x() >= card->position.x() - card->size.x() / 2 && pos.x() <= card->position.x() + card->size.x() / 2) {
        if (pos.y() >= card->position.y() - card->size.y() / 2 && pos.y() <= card->position.y() + card->size.y() / 2)
        {
            return true;
        }
    }

    return false;

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    mouse.clicked=true;
    mouse.pos = QVector2D(event->position().x(),event->position().y());
    mouse.last_pos = QVector2D(event->position().x(),event->position().y());
    mouse.clicked=true;

    if(canPlay && player->canPlay){
        for (int i = player->cards.size() - 1; i >= 0; i -- ) {
            Card *card = player->cards[i];

            if (collidewithcard(mouse.pos, card)) {
                card->ondrag = true;
                card->onDrag();
                card->setToFirstPlace();

                //card->toggleTurnCard();
                break;

            }
        }
    }

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    mouse.clicked=false;
    for (Card *card : player->cards) {
        if (!card->ondrag) continue;
        card->noDrag();
        card->unleashed_post = QVector2D(event->pos().x(), event->pos().y());
        card->ondrag = false;
    }

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    mouse.pos = QVector2D(event->position().x(),event->position().y());

    QVector2D movement=mouse.pos-mouse.last_pos;
    for(Card *card :cards){

        if (card->ondrag) {
            card->position += movement;
        }
    }


    mouse.last_pos = QVector2D(event->position().x(),event->position().y());


}
MainWindow::~MainWindow()
{
    delete ui;
}





void MainWindow::reposition()
{
    if (isCardsConfigured == false) {
        if (distributionCards(PLAYER, player->cards, 3, true)) {
            if (distributionCards(AI, ai->cards, 3, false)) {
                if (distributionCards(TABLE, table->cards,4, true)) {
                    isCardsConfigured = true;
                    canPlay=true;
                }
            }
        }
    }
    table->reposition();
    player->reposition();
    ai->reposition();

    for(int i=0;i<animations.size();i++){
        Animation *animation=animations[i];
        animation->update();
    }
    for(Card *card:cards){
        card->reposition();

    }



    int cardCount = 0;
    for (Card *card : cards) {
        if (!card->distributed)
            cardCount += 1;
    }

    if (!cardCount && !player->cards.size() && !ai->cards.size() && !isGameFinished && !table->onCollecting) {
        isGameFinished = true;

        Animation *wait = new Animation(1000, this);
        connect(wait, &Animation::onFinished, this,[=]() {
            for (Card *card : table->cards) {
                card->toggleTurnCard();
            }

            Animation *finish = new Animation(1000, this);
            connect(finish, &Animation::onFinished,this, [=]() {
                Player *lastPlayer = table->playerToCollect == PLAYER ? player : ai;
                for (Card *card : table->cards) {
                    card->setToFirstPlace();
                    lastPlayer->cardsCollected.append(card);
                    lastPlayer->score+=1;
                }
                table->cards.clear();

                // Verifie le gagnant.
                //Si le score du joueur est supérieur à celui de l'IA,
                //winner est défini sur 1. Si le score du joueur est inférieur
                //à celui de l'IA, winner est défini sur -1. Sinon, si les scores
                //sont égaux, winner est défini sur 0.
                winner = player->score > ai->score ? 1 : player->score < ai->score ? -1 : 0;

                //Activer le tableau de statistique
                    stat->isActive = true;
                Animation *deployStat = new Animation(200, this);
                    connect(deployStat, &Animation::onUpdate, [=](qreal progress) {
                        stat->scaleX = progress;
                    });
                    deployStat->start();
            });
            finish->start();
        });
        wait->start();
    }
    update();

}

void MainWindow::background(QColor color)
{
    QPainter painter(this);
    QBrush brush(color, Qt::SolidPattern);
    painter.fillRect(0, 0, width(), height(), brush);

}




