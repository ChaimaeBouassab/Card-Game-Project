// card.cpp
#include "card.h"
#include <QBrush>
#include <QPixmap>
#include <QPainter>
#include "mainwindow.h"
#include "animation.h"


Card::Card(QString type , qreal index, QObject *parent)
    : QObject{parent}

{
    this->type=type;
    this->index=index;
   // this->image.load(":/new/prefix1/textures-ch/" +type+ qvariant_cast<QString>(index) + ".png");
    this->image.load(":/new/prefix1/textures-ch/cardBackground.png");
    this->shadowTexture.load(":/new/prefix1/textures-ch/shadow.png");
    this->scaleAnimation = new QPropertyAnimation(this, "scale", this);
    this->size= QVector2D(210,330)*scale();
    this->parent=parent;


}

void Card::setToFirstPlace()
{
    MainWindow *main = qobject_cast<MainWindow*>(parent);

        for (int i = 0; i < main->cards.size(); i++) {
            if (main->cards[i] != this) continue;
            main->cards.remove(i);

            break;
        }
        main->cards.append(this);

}


void Card::setShadow(bool allow){
        allowShadow=allow;
}
void Card::followTarget(QVector2D target,qreal percent )
{
        if (ondrag == false) {
            position += (target - position) * percent;
        }
}

void Card::followTarget()
{
    position += (targetToFollow - position) * 0.1;

}
void Card::reposition()
{
        //followTarget(QVector2D(300,300));

}

void Card::toggleTurnCard() {
        // Assuming Card is a class member of MainWindow, and 'parent' is a parent widget.
        MainWindow *main = qobject_cast<MainWindow*>(parent);

        // Creating an instance of the Animation class.
        // This animation is responsible for the card flip.
        Animation *anim1 = new Animation(100, main);

        // Connecting a lambda function to the onUpdate signal of anim1.
        // This function updates the 'scaleX' property of the Card based on the animation progress.
        connect(anim1, &Animation::onUpdate, [=](qreal progress) {
            scaleX = qPow(qCos(progress * M_PI * 0.5), 2);
        });

        // Connecting a lambda function to the onFinished signal of anim1.
        // This function is executed when the flip animation is finished.
        connect(anim1, &Animation::onFinished, [=]() {
            // Toggling the 'switched' flag, and changing the card image accordingly.
            switched = !switched;
            if (switched) {
                image.load(":/new/prefix1/textures-ch/cardBackground.png");
            } else {
                image.load(":/new/prefix1/textures-ch/" + type + qvariant_cast<QString>(index) + ".png");
            }

            // Creating a second animation (anim2) for flipping the card back to its original state.
            Animation *anim2 = new Animation(100, main);

            // Connecting a lambda function to the onUpdate signal of anim2.
            // This function updates the 'scaleX' property of the Card for the second animation.
            connect(anim2, &Animation::onUpdate, [=](qreal progress) {
                scaleX = qPow(qCos((progress + 1) * M_PI * 0.5), 2);
            });

            // Connecting a lambda function to the onFinished signal of anim2.
            // This function is executed when the second flip animation is finished.
            connect(anim2, &Animation::onFinished, [=]() {
                scaleX = 1; // Setting the 'scaleX' back to its original state.
            });

            // Starting the second animation (anim2).
            anim2->start();
        });

        // Starting the first animation (anim1).
        anim1->start();
}



void Card::draw() {
    MainWindow *main= qobject_cast<MainWindow*>(parent);

    QPainter painter(main);
    painter. translate(position.x(), position.y());


    if (allowShadow) {
            painter.setOpacity(.8);
            QVector2D shadowSize = QVector2D(300, 420) * scale();
            QVector2D shadowDir = position - QVector2D(main->width() * .5, main->height() * .5);

            QVector2D shadowPos = shadowSize - shadowDir * .25;

            painter. drawPixmap(
                shadowPos.x() * -0.5 * scaleX,
                shadowPos.y() * -0.5, shadowSize.x() * scaleX,
                shadowSize.y(),
                shadowTexture

                );

            painter.setOpacity(1);
    }

    painter.drawPixmap(size.x() * -0.5 *scaleX, size.y() *  -0.5, size.x() *scaleX, size.y(), image);
    //Bordure
    QPen pen(QColor(61, 61, 61));
    pen.setJoinStyle(Qt::MiterJoin);
    if (ondrag) pen.setColor(QColor(0, 191, 255)), pen.setWidth(4);
    painter.setPen(pen);
    painter.drawRect(size.x() * scaleX * -.5, size.y() * -.5, size.x() * scaleX, size.y());

    painter. resetTransform();


}




qreal Card::scale() const
{
    return m_scale;
}

void Card::setScale(qreal newScale)
{
    m_scale = newScale;
    this->size=QVector2D(210,330)*scale();
}

void Card::onDrag()
{
    scaleAnimation->stop();
    scaleAnimation->setStartValue(scale());
    scaleAnimation->setEndValue(0.55);
    scaleAnimation->setEasingCurve(QEasingCurve::Linear);
    scaleAnimation->setDuration(100);
    scaleAnimation->start();
}

void Card::noDrag()
{
    scaleAnimation->stop();
    scaleAnimation->setStartValue(scale());
    scaleAnimation->setEndValue(0.4);
    scaleAnimation->setEasingCurve(QEasingCurve::Linear);
    scaleAnimation->setDuration(100);
    scaleAnimation->start();
}
