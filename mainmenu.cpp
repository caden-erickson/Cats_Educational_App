/*
 * Team             |   Chandler
 * Members          |   Braden Fiedel, Caden Erickson, Connor Blood, Josie Fiedel
 * Class            |   CS 3505
 * Project          |   A8: Sprint 2
 * Last modified    |   April 26, 2023
 *
 * This file contains the implementation of the class definition located in mainmenu.h.
 */


#include "mainmenu.h"
#include "ui_mainmenu.h"


/**
 * @brief MainMenu::MainMenu
 * Constructor. Creates a new MainMenu object and sets up its UI, with necessary
 * connect calls.
 *
 * @param parent -- pointer to QWidget parent object
 */
MainMenu::MainMenu(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    // Connections for the two main buttons of the main menu
    connect(ui->learnButton, &QPushButton::clicked,
            this, [this](){emit buttonPressed(1);});
    connect(ui->quizButton, &QPushButton::clicked,
            this, [this](){emit buttonPressed(2);});
    connect(ui->exitButton, &QPushButton::clicked,
            this, [this](){emit exitPressed();});
}

/**
 * @brief MainMenu::~MainMenu
 * Destructor. Deallocates memory for the ui object, and destructs this View.
 */
MainMenu::~MainMenu()
{
    delete ui;
}
