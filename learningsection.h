/*
 * Team             |   Chandler
 * Members          |   Braden Fiedel, Caden Erickson, Connor Blood, Josie Fiedel
 * Class            |   CS 3505
 * Project          |   A8: Sprint 2
 * Last modified    |   April 26, 2023
 *
 * This file contains the class definition for the LearningSection class.
 */


#ifndef LEARNINGSECTION_H
#define LEARNINGSECTION_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class LearningSection; }
QT_END_NAMESPACE

/**
 * @brief The LearningSection class
 * This class represents the "Learn" section of the program, and contains most
 * of the functionality seen on that page (with the exception of the diagram,
 * which is its own class), including the information panel, Back button, Help
 * button, and Continue to Quiz button. It interfaces with the diagram class to
 * appropriately control the information panel and the Continue To Quiz button.
 */
class LearningSection : public QWidget
{
    Q_OBJECT

public:
    explicit LearningSection(QWidget* parent = nullptr);
    ~LearningSection();

signals:
    void advanceToQuiz();
    void buttonPressed(int);

private:
    Ui::LearningSection* ui;

private slots:
    void changeInfoList(int infoIndex);
    void displayHelpWindow();
    void resetSection();
};

#endif // LEARNINGSECTION_H
