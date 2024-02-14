/*
 * Team             |   Chandler
 * Members          |   Braden Fiedel, Caden Erickson, Connor Blood, Josie Fiedel
 * Class            |   CS 3505
 * Project          |   A8: Sprint 2
 * Last modified    |   April 26, 2023
 *
 * This file contains the class definition for the QuizSection class.
 */


#ifndef QUIZSECTION_H
#define QUIZSECTION_H

#include <QMediaPlayer>
#include <QVector>
#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class QuizSection; }
QT_END_NAMESPACE

/**
 * @brief The QuizSection class
 * This class represents the entirety of the "Quiz" section of the program, and
 * controls the control flow and data flow among the various parts and screens
 * within that section. Objects of the QuizGame and QuizSimulation screens are
 * used by this class.
 */
class QuizSection : public QWidget
{
    Q_OBJECT

public:
    explicit QuizSection(QWidget* parent = nullptr);
    ~QuizSection();

    QMediaPlayer* player;
    QAudioOutput* audioOutput;

signals:
    void sendKeyPress(int);
    void sendKeyRelease(int);
    void exitPressed();

private:
    Ui::QuizSection* ui;
    QVector<QUrl> videoUrls;
    QVector<QString> imageUrls;
    QVector<QList<QString>> quizAnswers;
    int currentVideoQuestionIndex;
    int currentImageQuestionIndex;
    bool lastPlayedWasVid;

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    void updateFinalScore(int score);

public slots:
    void playVideo();
    void displayImage();
    void nextQuestion();
    void checkVideoState(int videoState);
};

#endif // QUIZSECTION_H
