/*
 * Team             |   Chandler
 * Members          |   Braden Fiedel, Caden Erickson, Connor Blood, Josie Fiedel
 * Class            |   CS 3505
 * Project          |   A8: Sprint 2
 * Last modified    |   April 26, 2023
 *
 * This file contains the implementation of the class definition located in mainwindow.h.
 */


#include "mainwindow.h"
#include "ui_mainwindow.h"


/**
 * @brief MainWindow::MainWindow
 * Constructor. Creates a new MainWindow object and sets up its UI, with
 * necessary connect calls.
 *
 * @param parent -- pointer to QWidget parent object
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    // Connect main menu button presses to changing the current stacked widget.
    connect(ui->mainMenu, &MainMenu::buttonPressed,
            ui->stackedWidget, &QStackedWidget::setCurrentIndex);
    connect(ui->learningSection, &LearningSection::buttonPressed,
            ui->stackedWidget, &QStackedWidget::setCurrentIndex);
    connect(ui->mainMenu, &MainMenu::exitPressed,
            this, &QMainWindow::close);
    connect(ui->quizSection, &QuizSection::exitPressed,
            this, &QMainWindow::close);
}

/**
 * @brief MainWindow::~MainWindow
 * Destructor. Deallocates memory for the ui object, and destructs this View.
 */
MainWindow::~MainWindow()
{
    delete ui;
}
