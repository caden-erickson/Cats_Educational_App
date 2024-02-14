/*
 * Team             |   Chandler
 * Members          |   Braden Fiedel, Caden Erickson, Connor Blood, Josie Fiedel
 * Class            |   CS 3505
 * Project          |   A8: Sprint 2
 * Last modified    |   April 26, 2023
 *
 * This file contains the class definition for the the QuizGame class.
 */


#ifndef QUIZGAME_H
#define QUIZGAME_H

#include <Box2D/Box2D.h>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>
#include <cmath>
#include <random>


QT_BEGIN_NAMESPACE
namespace Ui { class QuizGame; }
QT_END_NAMESPACE

/**
 * @brief The QuizGame class
 * This class represents the gameplay portion of the "Quiz" section of the
 * program. The gameplay portion is the mouse-and-tunnel portion, in which the
 * user controls a small mouse using the WASD keys in order to select an answer
 * to a quiz question.
 * This class makes use of the Box2D physics engine library.
 */
class QuizGame : public QWidget
{
    Q_OBJECT

public:
    explicit QuizGame(QWidget* parent = nullptr);
    ~QuizGame();

private:
    Ui::QuizGame* ui;

    QMap<int, bool> keyStatus;
    QList<QString> currLvlAnswers;

    QRect cheese;
    int extraCheese;
    QList<QRect> mouseTraps;

    QTimer timer;
    b2World world;
    b2Body* body;

    b2Vec2 currForce;
    double currAngle;
    double getCurrAngle();

    void checkInAnswerZone();
    void checkInEntityBounds();
    void resetGameState();
    void spawnEntities();
    int generateCoordinate(int lowBound, int uppBound);

    void paintEvent(QPaintEvent*);

signals:
    void onAnswerGuess(bool, int);

public slots:
    void onKeyPress(int key);
    void onKeyRelease(int key);
    void startSimulation();
    void setNextLevel(QList<QString>);

private slots:
    void updateWorld();
};

#endif // QUIZGAME_H
