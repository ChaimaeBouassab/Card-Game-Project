// card.h
#ifndef CARD_H
#define CARD_H

#include <QObject>
#include <QVector2D>
#include <QPixmap>
#include <QPropertyAnimation>

class Card : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal scale READ scale WRITE setScale)

public:
    Card(QString type , qreal index,QObject *parent);

    QVector2D position;
    QVector2D size;

    QVector2D unleashed_post{-100,-100};
    QPixmap image;
    QPixmap shadowTexture;


    QString type;
    qreal index =index;

    bool ondrag{false};

    bool distributed{false};


    bool switched = true;

    QVector2D targetToFollow;

    QObject *parent;

    void setToFirstPlace();
    void followTarget(QVector2D target,qreal percent =0.1);
    void followTarget();
    void reposition();

    void setShadow(bool allow);

    void draw();
    qreal scale() const;
    void setScale(qreal newScale);

    void onDrag();
    void noDrag();

    void toggleTurnCard();
private:
    QPropertyAnimation *scaleAnimation;
    qreal m_scale{0.4};
    qreal scaleX{1};
    bool allowShadow{false};

};

#endif // CARD_H
