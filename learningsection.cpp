/*
 * Team             |   Chandler
 * Members          |   Braden Fiedel, Caden Erickson, Connor Blood, Josie Fiedel
 * Class            |   CS 3505
 * Project          |   A8: Sprint 2
 * Last modified    |   April 26, 2023
 *
 * This file contains the implementation of the class definition located in learningsection.h.
 */


#include "learningsection.h"
#include "ui_learningsection.h"
#include <QMessageBox>


/**
 * @brief LearningSection::LearningSection
 * Constructor. Creates a new LearningSection object and sets up its UI,
 * with necessary connect calls.
 *
 * @param parent -- pointer to QWidget parent object
 */
LearningSection::LearningSection(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LearningSection)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->quizAdvance->setEnabled(false);
    ui->quizAdvance->setText(QString("Quiz"));

    // Cat diagram connections
    connect(ui->catDiagram, &CatDiagram::regionSelected,
            this, &LearningSection::changeInfoList);
    connect(ui->catDiagram, &CatDiagram::highlightCompletion,
            this, [this](){ui->quizAdvance->setEnabled(true);
                           ui->quizAdvance->setText(QString("Quiz 🡆"));});

    // Quiz button connections
    connect(ui->quizAdvance, &QPushButton::clicked,
            this, [this](){emit buttonPressed(2);});
    connect(ui->quizAdvance, &QPushButton::clicked,
            ui->catDiagram, &CatDiagram::clearProgress);
    connect(ui->quizAdvance, &QPushButton::clicked,
            this, &LearningSection::resetSection);

    // Back button connections
    connect(ui->backButton, &QPushButton::clicked,
            this, [this](){emit buttonPressed(0);});
    connect(ui->backButton, &QPushButton::clicked,
            ui->catDiagram, &CatDiagram::clearProgress);
    connect(ui->backButton, &QPushButton::clicked,
            this, &LearningSection::resetSection);

    // Help button connection
    connect(ui->helpButton, &QPushButton::clicked,
            this, &LearningSection::displayHelpWindow);
}

/**
 * @brief LearningSection::~LearningSection
 * Destructor. Deallocates memory for the ui object, and destructs this View.
 */
LearningSection::~LearningSection()
{
    delete ui;
}

/**
 * @brief LearningSection::changeInfoList
 * Changes the page shown in the stackedWidget information panel to the page of
 * the given index.
 *
 * @param infoIndex -- the index of the page to which to switch the info display
 */
void LearningSection::changeInfoList(int infoIndex)
{
    ui->stackedWidget->setCurrentIndex(infoIndex + 1);
}

/**
 * @brief LearningSection::displayHelpWindow
 * Displays a pop-up window with information about how to use the "Learn" section.
 */
void LearningSection::displayHelpWindow()
{
    QMessageBox msgbox;
    msgbox.setWindowTitle("How to learn");
    msgbox.setIcon(QMessageBox::Information);
    msgbox.setText("Click on different parts of the cat diagram to learn about them.\n\n"
                   "Once you have learned about all the parts of the cat, you can move on to the quiz game!");
    msgbox.setStandardButtons(QMessageBox::Ok);
    msgbox.exec();
}

/**
 * @brief LearningSection::resetSection
 * Resets the stackedWidget information panel and quiz button to default states.
 */
void LearningSection::resetSection()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->quizAdvance->setEnabled(false);
    ui->quizAdvance->setText(QString("Quiz"));
}
