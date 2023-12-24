#ifndef ANIMATION_H
#define ANIMATION_H

#include <QObject>

class Animation : public QObject
{
    Q_OBJECT

public:
    Animation(int durationMs, QObject *parent);
    Animation(int durationMs,bool isStatic, QObject *parent);

    QObject *parent;

    bool isStatic{false};

    void update();
    void start();
    void stop();


signals:
    void onUpdate(qreal progress) ;
        void onFinished();

private:
    qint64 startTime;
    int durationMs;
    bool started{false};

};

#endif // ANIMATION_H
