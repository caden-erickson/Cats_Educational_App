/*
 * Team             |   Chandler
 * Members          |   Braden Fiedel, Caden Erickson, Connor Blood, Josie Fiedel
 * Class            |   CS 3505
 * Project          |   A8: Sprint 2
 * Last modified    |   April 26, 2023
 *
 * This file contains the class definition for the QuizSimulation class.
 */


#ifndef QUIZSIMULATION_H
#define QUIZSIMULATION_H

#include <Box2D/Box2D.h>
#include <QPainter>
#include <QTimer>
#include <QWidget>
#include <random>


QT_BEGIN_NAMESPACE
namespace Ui { class QuizSimulation; }
QT_END_NAMESPACE

/**
 * @brief The QuizSimulation class
 * This class represents the simulation screen within the "Quiz" section of the
 * program. The simulation screen is the screen on which cheese is dropped, with
 * a score tracker and an indicator of whether the user answered the previous
 * question correctly.
 * This class makes use of the Box2D physics engine library.
 */
class QuizSimulation : public QWidget
{
    Q_OBJECT

public:
    explicit QuizSimulation(QWidget* parent = nullptr);
    ~QuizSimulation();

private:
    QTimer timer;
    b2World world;
    Ui::QuizSimulation* ui;
    QImage backgroundImage;
    QImage cheeseImage;
    QList<b2Body*> bodies;

    void startSimulation();
    void endSimulation();
    void updateWorld();
    void addBody();

    void paintEvent(QPaintEvent*);

public slots:
    void answerReceived(bool correct, int extraCheese);

signals:
    void nextQuestion();
    void scoreChanged(int);
};

#endif // QUIZSIMULATION_H
