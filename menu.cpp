#include "menu.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "mainwindow.h"
#include <QTimer>

Menu::Menu(QObject *parent)
    : QObject{parent}
{

    this->parent = parent;

    MainWindow *main = qobject_cast<MainWindow*>(parent);

    QLabel *backgroundImage = new QLabel(main);
    backgroundImage->setGeometry(0, 0, main->width(), main->height());
    QPixmap bg(":/recent/chaindjn.png");
    bg = bg.scaled(main->width(), main->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    backgroundImage->setPixmap(bg);

    QLabel *cardsImage = new QLabel(main);
    cardsImage->setGeometry(280, 0, 773 * 0.65, 323 * 0.65);
    QPixmap cardsBg(":/recent/Screenshot_2023-12-21_220639-removebg-preview.png");
    cardsBg = cardsBg.scaled(773 * 0.65, 323 * 0.65, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    cardsImage->setPixmap(cardsBg);

    QLabel *inputTexture = new QLabel(main);
    inputTexture->setGeometry(400, 250, 1120 * .25, 376 * .25);
    QPixmap inputTx(":/recent/buttonborder.png");
    inputTx = inputTx.scaled(1120 * .25, 376 * .25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    inputTexture->setPixmap(inputTx);

    // Création du champ de saisie
    QLineEdit *lineEdit = new QLineEdit(main);
    lineEdit->setMaxLength(30);
    lineEdit->setPlaceholderText("Username");
    lineEdit->setGeometry(440, 245, 200, 100);
    lineEdit->setFont(QFont("Segoe Script", 26, 600));
    lineEdit->setStyleSheet("background-color: #00000000; border: none; color: white");

    QLabel *labelUsername = new QLabel("Player", main);
    labelUsername->setGeometry(240, 245, 200, 100);
    labelUsername->setFont(QFont("Segoe Script", 26, 600));
    labelUsername->setStyleSheet("color: white");

    // Connexion du signal textChanged du champ de saisie à la fonction lambda
    connect(lineEdit, &QLineEdit::textChanged, main, [=](QString text) {
        main->playerName = text;
    });

    QLabel *ruleButtonTex = new QLabel(main);
    ruleButtonTex->setGeometry(352, 335, 1120 * .34, 376 * .25);
    QPixmap ruleTx(":/recent/buttonborder.png");
    ruleTx = ruleTx.scaled(1120 * .34, 376 * .25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ruleButtonTex->setPixmap(ruleTx);

    QPushButton *ruleButton = new QPushButton("Rend the Rules", main);
    ruleButton->setStyleSheet("background-color: #00000000; border: none; color: white");
    ruleButton->setFont(QFont("Segoe Script", 26, 600));
    ruleButton->setGeometry(390, 350, 300, 64);

    QLabel *labelRendRules = new QLabel("Rules of the Game", main);
    labelRendRules->setGeometry(10, 350, 380, 64);
    labelRendRules->setFont(QFont("Segoe Script", 24, 600));
    labelRendRules->setStyleSheet("color: white");

    QLabel *startButtonTex = new QLabel(main);
    startButtonTex->setGeometry(300, 420, 1120 * .25, 376 * .25);
    QPixmap startTex(":/recent/buttonborder.png");
    startTex = startTex.scaled(1120 * .25, 376 * .25, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    startButtonTex->setPixmap(startTex);

    QPushButton *startButton = new QPushButton("Start", main);
    startButton->setStyleSheet("background-color: #00000000; border: none; color: white");
    startButton->setFont(QFont("Segoe Script", 26, 600));
    startButton->setGeometry(320, 435, 240, 64);

    QLabel *rulesText = new QLabel(main);
    rulesText->setHidden(true);
    rulesText->setGeometry(0, 0, main->width(), main->height());
    QPixmap rulesImg(":/regles/rules.png");
    rulesImg = rulesImg.scaled(main->width(), main->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    rulesText->setPixmap(rulesImg);

    QPushButton *backToMenuBtn = new QPushButton("✖", main);
    backToMenuBtn->setHidden(true);
    backToMenuBtn->setGeometry(10, 10, 42, 42);
    backToMenuBtn->setFont(QFont("Times", 18, 400));

    connect(backToMenuBtn, &QPushButton::clicked, main, [=]() {
        backToMenuBtn->setHidden(true);
        rulesText->setHidden(true);
    });

    connect(ruleButton, &QPushButton::clicked, main, [=]() {
        backToMenuBtn->setHidden(false);
        rulesText->setHidden(false);
    });


    // Connexion du clic sur le bouton "Start" à la fonction lambda
    connect(startButton, &QPushButton::clicked, main, [=]() {
        // si la longueur (le nombre de caractères) de la chaîne de caractères playerName de l'objet main est supérieure à zéro.
        //Si playerName n'est pas vide (la longueur est supérieure à zéro), la condition est vraie.
        if (main->playerName.length() > 0) {
            delete backgroundImage;
            delete cardsImage;
            delete inputTexture;
            delete lineEdit;
            delete labelUsername;
            delete ruleButtonTex;
            delete ruleButton;
            delete labelRendRules;
            delete startButtonTex;
            delete startButton;

            main->gameStarted = true;
        } else {
            // Changement temporaire de la couleur du texte en cas d'absence de nom !
            labelUsername->setStyleSheet("color: red");

            // Mise en place d'une minuterie pour rétablir la couleur du texte après un certain délai
            QTimer *timer = new QTimer(main);
            connect(timer, &QTimer::timeout, main, [this, labelUsername, timer, main]() {
                if (time >= 1000) {
                    time = 0;
                    labelUsername->setStyleSheet("color: white");
                    delete timer;
                }
                //une minuterie est activée pour rétablir la couleur d'origine après
                //un délai spécifié, assurant ainsi une indication visuelle que
                //le nom du joueur est requis.
                time += 1000 / main->FPS;
            });
            timer->start(1000.0 / main->FPS);
        }
    });
}
