#include "statt.h"
#include "mainwindow.h"
#include <QPainter>

Statt::Statt(QVector2D pos, QVector2D size, QObject *parent)
    : QObject{parent}
{
    this->pos = pos;
    this->size = size;
    this->parent = parent;
}

void Statt::draw() {
    MainWindow *main = qobject_cast<MainWindow*>(parent);
    QPainter painter(main);

//    QString message =
//        main->winner == 1 ?
//            "🎉 Bravo, vous avez gagné(e)." :
//        main->winner == 0 ? "🤝 Équivalent." :
//        "😕 Vous avez perdu.";

    QString message;

    switch (main->winner) {
    case 1:
        message = " 🎉Bravo, vous avez gagné.";
            break;
    case 0:
        message = " 🤝 Équivalent.";
            break;
    case -1:
        message = " 😕 Vous avez perdu !";
        break;
    }

    painter.setOpacity(0.8);

    painter.setOpacity(0.8);
    painter.fillRect(0, 0, main->width() * scaleX, main->height(), QBrush(QColor(0, 0, 0, 200)));
    painter.setOpacity(1);

    painter.setPen(QPen(QColor(255, 255, 255)));
    painter.setFont(QFont("Times", 18, 600));
    painter.drawText(0, 0, main->width() * scaleX, main->height(), Qt::AlignCenter, message);
}
