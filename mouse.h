#ifndef MOUSE_H
#define MOUSE_H

#include <QVector2D>

class Mouse {
public:
    QVector2D pos;
    QVector2D last_pos;

    bool clicked{false};

    Mouse() {
        this->pos = QVector2D(0, 0);
        this->last_pos = QVector2D(0, 0);
    }
};

#endif // MOUSE_H
