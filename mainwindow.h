#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "animation.h"
#include <QMainWindow>
#include <QPainter>
#include <QPixmap>
#include "card.h"
#include "mouse.h"
#include <QList>
#include "animation.h"
#include "player.h"
#include "table.h"
#include "statt.h"
#include <QImage>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Mouse mouse;

    QList<Card*> cards;
    void reposition();
    void background(QColor color);
    QList<Animation*> animations;

    QVector2D dealer_pos;

    QVector2D  margin{0,40};

    void initCards();
    void initGame();

    void restart();
    bool firstToPlay{true};

    const static int TABLE{0};
    const static int PLAYER{1};
    const static int AI{2};

    QImage light;

    bool isGameFinished{false};

    bool gameStarted{false};

    QString playerName{""};

    Table *table;
    Player *player;
    Player *ai;

    Statt *stat;



    const qreal FPS{120};

    qreal time{0};

    bool isCardsConfigured{false};

    bool distributionCards(int mode, QList<Card*> &parent_cards, int cardLength, bool show);

    bool canPlay{false};

    int winner;

private:
    Ui::MainWindow *ui;
    QPixmap image;


    // QWidget interface
protected:

    void paintEvent(QPaintEvent *event);


    // QWidget interface

protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};
#endif // MAINWINDOW_H
