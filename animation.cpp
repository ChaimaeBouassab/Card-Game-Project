#include "animation.h"
#include <QDateTime>
#include "mainwindow.h"


Animation::Animation(int durationMs, QObject *parent)
    : QObject(parent)
{
    this->parent = parent;
    this->durationMs = durationMs;

    MainWindow *main = qobject_cast<MainWindow*>(parent);
    main->animations.append(this);
}

Animation::Animation(int durationMs, bool isStatic, QObject *parent):Animation(durationMs,parent)
{
    this->isStatic=isStatic;

}

void Animation::start() {
    emit onUpdate(0);
}

void Animation::stop() {
    MainWindow *main = qobject_cast<MainWindow*>(parent);
    main->animations.removeOne(this);
    delete this;
}


void Animation::update() {
    if(!started) startTime = QDateTime::currentMSecsSinceEpoch();
    //La fonction commence par vérifier si l'animation a déjà démarré
    //(started est initialisé à false au début).

    //Si ce n'est pas le cas, elle enregistre le temps actuel en millisecondes
    //depuis l'époque dans startTime. Cela sert de point de référence pour
    //le calcul de la durée écoulée.
    started=true;

    if (started) {
        //The QDateTime class provides date and time functions
        qint64 now = QDateTime::currentMSecsSinceEpoch();
        qint64 elapsedTime = now - startTime;
        //The elapsed time of an event is the amount of time that
        //has passed between the commencement and the completion of an event.

        //Si l'animation a démarré, elle obtient le temps actuel en millisecondes (now).
        //Ensuite, elle calcule le temps écoulé depuis le début de l'animation
        //en soustrayant le temps de démarrage (startTime) du temps actuel.

        if (elapsedTime <= durationMs) {
            qreal progress = static_cast<qreal>(elapsedTime) / static_cast<qreal>(durationMs);
            emit onUpdate(progress);
        } else {
            //Si le temps écoulé dépasse la durée totale de l'animation,
            emit onUpdate(1);
            //emit dans le code source de Qt, signale que nous déclenchons un signal.
            emit onFinished();
            //Elle émet  un signal onFinished pour
            //indiquer que l'animation est complètement terminée.
            stop();
        }
    }
}
