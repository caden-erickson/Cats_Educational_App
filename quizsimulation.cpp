/*
 * Team             |   Chandler
 * Members          |   Braden Fiedel, Caden Erickson, Connor Blood, Josie Fiedel
 * Class            |   CS 3505
 * Project          |   A8: Sprint 2
 * Last modified    |   April 26, 2023
 *
 * This file contains the implementation of the class definition located in quizsimulation.h.
 */


#include "quizsimulation.h"
#include "ui_quizsimulation.h"


/**
 * @brief QuizSimulation::QuizSimulation
 * Constructor. Creates a new QuizSimulation object and sets up its UI, with
 * necessary connect calls.
 *
 * @param parent -- pointer to QWidget parent object
 */
QuizSimulation::QuizSimulation(QWidget* parent)
    : QWidget(parent)
    , timer(this)
    , world(b2Vec2(0.0f, 10.0f))
    , ui(new Ui::QuizSimulation)
    , backgroundImage(":/quizImages/wall.jpg")
    , cheeseImage(":/quizImages/cheese.jpg")
{
    cheeseImage = cheeseImage.scaled(50, 50, Qt::KeepAspectRatio);

    ui->setupUi(this);

    // Create a ground physics object
    b2PolygonShape ground;
    ground.SetAsBox(800.0f, 1.0f);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 525.0f);

    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    groundBody->CreateFixture(&ground, 0.0f);

    // Create a left-wall physics object
    b2PolygonShape lWall;
    lWall.SetAsBox(1.0f, 549.0f);

    b2BodyDef lWallBodyDef;
    lWallBodyDef.position.Set(0.0f, 0.0f);

    b2Body* lWallBody = world.CreateBody(&lWallBodyDef);
    lWallBody->CreateFixture(&lWall, 0.0f);

    // Create a right-wall physics object
    b2PolygonShape rWall;
    rWall.SetAsBox(10.0f, 549.0f);

    b2BodyDef rWallBodyDef;
    rWallBodyDef.position.Set(800.0f, 0.0f);

    b2Body* rWallBody = world.CreateBody(&rWallBodyDef);
    rWallBody->CreateFixture(&rWall, 0.0f);

    // Connect the timer to the world
    connect(&timer, &QTimer::timeout, this,
            &QuizSimulation::updateWorld);
    connect(ui->Continue, &QPushButton::clicked,
            this, &QuizSimulation::endSimulation);
}

/**
 * @brief QuizSimulation::~QuizSimulation
 * Destructor. Deallocates memory for the ui object, and destructs this View.
 */
QuizSimulation::~QuizSimulation()
{
    delete ui;
}

/**
 * @brief QuizSimulation::paintEvent
 * Draws the background and the current position of the cheese square.
 */
void QuizSimulation::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    // Draw the background image.
    painter.drawImage(0, 0, backgroundImage);

    // Draw the cheese squares.
    for (b2Body* worldBody: bodies)
    {
        b2Vec2 position = worldBody->GetPosition();

        double bodyAngle = fmod(worldBody->GetAngle(), M_PI_2);
        if (bodyAngle < 0)
            bodyAngle += M_PI_2;

        QTransform imageAngle;
        imageAngle.rotateRadians(worldBody->GetAngle());

        painter.drawImage((double)(position.x) - 50 * cos(M_PI_4) * sin(M_PI_4 + bodyAngle),
                          (double)(position.y) - 50 * sin(M_PI_4) * sin(M_PI_4 + bodyAngle),
                          cheeseImage.transformed(imageAngle, Qt::SmoothTransformation));
    }

    painter.end();
}

/**
 * @brief QuizSimulation::answerReceived
 * Configures the simulation based on whether or not the user guessed the
 * previous quiz question correctly, and based on how much extra cheese they
 * gathered before guessing.
 *
 * @param correct --  whether or not the user guessed the answer correctly
 * @param extraCheese -- the amount of extra cheese the user gathered
 */
void QuizSimulation::answerReceived(bool correct, int extraCheese)
{
    if (correct)
    {
        ui->rightWrongLabel->setText("Correct!");
        ui->rightWrongLabel->setStyleSheet(QString("QLabel { color: #00ff00; }"));
        startSimulation();
        for(int i = 0; i < extraCheese + 1; i++)
            addBody();
    }
    else
    {
        ui->rightWrongLabel->setText("Incorrect :(");
        ui->rightWrongLabel->setStyleSheet(QString("QLabel { color: #ff0000; }"));
    }
}

/**
 * @brief QuizSimulation::startSimulation
 * Start the timer that controls the physics simulation.
 */
void QuizSimulation::startSimulation()
{
    // Set the timer event interval to 1 millisecond
    timer.start(1);
}

/**
 * @brief QuizSimulation::startSimulation
 * Ends the timer that controls the physics simulation, and emits a signal that
 * the simulation has ended and the quiz can advance to the next question.
 */
void QuizSimulation::endSimulation()
{
    emit nextQuestion();
}

/**
 * @brief QuizSimulation::UpdateWorld
 * Advances the physics simulation forward by one step and redraws the screen.
 */
void QuizSimulation::updateWorld()
{
    world.Step(1.0/60.0, 6, 2);
    update();
}

/**
 * @brief QuizSimulation::addBody
 * Adds a body (a cheese square) to the simulation, dropping in from above the
 * window.
 */
void QuizSimulation::addBody()
{
    // Create a cheese physics object
    b2PolygonShape square;
    square.SetAsBox(25.0f, 25.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.8f;
    fixtureDef.shape = &square;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(400.0f, -50.0f);

    b2Body* body = world.CreateBody(&bodyDef);
    body->CreateFixture(&fixtureDef);
    body->SetAngularVelocity((float)rand() / (float)RAND_MAX);
    bodies.append(body);

    // Set the score
    ui->Score->setText(QString::number(bodies.size()));
    emit scoreChanged(bodies.size());
}
