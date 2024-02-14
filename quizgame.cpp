/*
 * Team             |   Chandler
 * Members          |   Braden Fiedel, Caden Erickson, Connor Blood, Josie Fiedel
 * Class            |   CS 3505
 * Project          |   A8: Sprint 2
 * Last modified    |   April 26, 2023
 *
 * This file contains the implementation of the class definition located in quizgame.h.
 */


#include "quizgame.h"
#include "ui_quizgame.h"

using std::acos;


/**
 * @brief QuizGame::QuizGame
 * Constructor. Creates a new QuizGame object and sets up its UI,
 * with necessary connect calls.
 *
 * @param parent --  pointer to QWidget parent object
 */
QuizGame::QuizGame(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::QuizGame)
    , cheese(0, 0, 20, 20)
    , extraCheese(0)
    , timer(this)
    , world(b2World(b2Vec2(0, 0)))
    , currForce(b2Vec2(0, 0))
    , currAngle(-M_PI / 2)
{
    ui->setupUi(this);

    // Create the mouse object
    b2PolygonShape square;
    square.SetAsBox(1.0f, 1.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.9;
    fixtureDef.shape = &square;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(380, 460);

    body = world.CreateBody(&bodyDef);
    body->CreateFixture(&fixtureDef);

    // Applies "friction" to the mouse movements
    body->SetLinearDamping(3);

    // Create the map border restrictions (bottom, top, left, right walls)
    b2PolygonShape horizontalWall;
    b2PolygonShape verticalWall;
    horizontalWall.SetAsBox(800.0f, 1.0f);
    verticalWall.SetAsBox(1.0f, 550.0f);

    b2BodyDef bottomWallDef;
    bottomWallDef.position.Set(0.0f, 528.0f);
    b2Body* bottomWall = world.CreateBody(&bottomWallDef);
    bottomWall->CreateFixture(&horizontalWall, 0.0f);

    b2BodyDef topWallDef;
    topWallDef.position.Set(0.0f, 210.0f);
    b2Body* topWall = world.CreateBody(&topWallDef);
    topWall->CreateFixture(&horizontalWall, 0.0f);

    b2BodyDef lWallDef;
    lWallDef.position.Set(20.0f, 0.0f);
    b2Body* lWall = world.CreateBody(&lWallDef);
    lWall->CreateFixture(&verticalWall, 0.0f);

    b2BodyDef rWallDef;
    rWallDef.position.Set(720.0f, 0.0f);
    b2Body* rWall = world.CreateBody(&rWallDef);
    rWall->CreateFixture(&verticalWall, 0.0f);

    // Spawn in the cheese and mouse traps
    spawnEntities();

    // Connection for updating the world incrementally
    connect(&timer, &QTimer::timeout, this, &QuizGame::updateWorld);
}

/**
 * @brief QuizGame::~QuizGame
 * Destructor. Deallocates memory for the ui object, and destructs this View.
 */
QuizGame::~QuizGame()
{
    delete ui;
}

/**
 * @brief QuizGame::updateWorld
 * Updates the state of the world. Specifically, an impulse force is applied,
 * which calls for the mouse to be redrawn.
 */
void QuizGame::updateWorld()
{
    world.Step(1.0/60.0, 6, 2);

    // Apply an impulse (+- 40 Ns) to the mouse depending on the current key presses.
    if(keyStatus[Qt::Key_W])
        currForce.y - 5 > -40 ? currForce.y -= 5 : currForce.y == -40;
    else if(currForce.y <= -1)
        currForce.y++;

    if(keyStatus[Qt::Key_S])
        currForce.y + 5 < 40 ? currForce.y += 5 : currForce.y == 40;
    else if(currForce.y >= 1)
        currForce.y--;

    if(keyStatus[Qt::Key_D])
        currForce.x + 5 < 40 ? currForce.x += 5 : currForce.x == 40;
    else if(currForce.x >= 1)
        currForce.x--;

    if(keyStatus[Qt::Key_A])
        currForce.x - 5 > -40 ? currForce.x -= 5 : currForce.x == -40;
    else if(currForce.x <= -1)
        currForce.x++;

    body->ApplyLinearImpulse(currForce, body->GetWorldCenter(), true);

    // Check if the mouse is within an answer zone.
    checkInAnswerZone();
    checkInEntityBounds();
    update();
}

/**
 * @brief QuizGame::startSimulation
 * Begins the mouse physics simulation with a timeout interval of 10 ms.
 */
void QuizGame::startSimulation()
{
    timer.start(10);
}

/**
 * @brief QuizGame::onKeyPress
 * Updates the currently pressed key's status to true, if not already true.
 *
 * @param key -- the currently pressed key
 */
void QuizGame::onKeyPress(int key)
{
    if(!keyStatus[key])
        keyStatus.insert(key, true);
}

/**
 * @brief QuizGame::onKeyRelease
 * Updates the released key's status to false, if not already false.
 *
 * @param key -- the released key
 */
void QuizGame::onKeyRelease(int key)
{
    if(keyStatus[key])
        keyStatus.insert(key, false);
}

/**
 * @brief QuizGame::checkInAnswerZone
 * Checks the bounds of the current body position to determine whether or not
 * the mouse is within an answer zone (zones 1-3).
 * If the mouse is within one, a signal is emitted to notify the quiz section.
 * Additionally, if the answer is incorrect, then the game is reset.
 */
void QuizGame::checkInAnswerZone()
{
    int posX = body->GetPosition().x;
    int posY = body->GetPosition().y;

    // Loop through all of the answer zones to determine if the mouse is inside one
    QList<QLabel*> answers = ui->zoneLayout->findChildren<QLabel*>();
    for(int i = 0; i < answers.count(); i++)
    {
        int xCoord = answers[i]->x() + ui->zoneLayout->x();
        int yCoord = answers[i]->y() + ui->zoneLayout->y();

        // If the mouse body is within a zone, their "correctness" is determined
        if(posX >= xCoord && posX <= (xCoord + answers[i]->width())
           && posY >= yCoord && posY <= (yCoord + answers[i]->height()))
        {
            QList<QLabel*> answerText = ui->answerLayout->findChildren<QLabel*>();

            // True if the index is 0, otherwise false.
            emit onAnswerGuess(currLvlAnswers.indexOf(answerText[i]->text()) == 0, extraCheese);
            resetGameState();
        }
    }
}

/**
 * @brief QuizGame::checkInEntityBounds
 * If the mouse is on top of the cheese, the cheese is "collected". If the mouse
 * is on top of a mouse trap, then the level ends.
 */
void QuizGame::checkInEntityBounds()
{
    QRect mousePos(body->GetPosition().x - 22, body->GetPosition().y - 22, 45, 45);

    // Check if the mouse is within bounds of the cheese.
    if(mousePos.intersects(cheese))
    {
        cheese = QRect(0, 0, 20, 20);
        extraCheese++;
    }

    // Check if the mouse is within the bounds of the mouse traps.
    for(QRect mouseTrap : mouseTraps)
    {
        // Kill the mouse, advance the level.
        if(mousePos.intersects(mouseTrap))
        {
            resetGameState();
            emit onAnswerGuess(false, 0);
        }
    }
}

/**
 * @brief QuizGame::setNextLevel
 * Sets the next level by resetting the mouse body position and labeling the
 * answer zones with new, randomly-placed answers.
 *
 * @param currAnswers -- the QList of potential answers to display in the next level
 */
void QuizGame::setNextLevel(QList<QString> currAnswers)
{
    currLvlAnswers = currAnswers;
    QList<QString> currRandomizedAnswers = currAnswers;
    std::random_shuffle(currRandomizedAnswers.begin(), currRandomizedAnswers.end());

    ui->textOne->setText(currRandomizedAnswers.at(0));
    ui->textTwo->setText(currRandomizedAnswers.at(1));
    ui->textThree->setText(currRandomizedAnswers.at(2));
}

/**
 * @brief QuizGame::resetGameState
 * Resets the game state by stopping the timer and resetting the mouse body
 * and other entities.
 */
void QuizGame::resetGameState()
{
    timer.stop();
    body->SetTransform(b2Vec2(380, 460), 0);
    body->SetLinearVelocity(b2Vec2(0,0));
    currAngle = -M_PI / 2;
    currForce = b2Vec2(0, 0);

    // Spawn in a cheese and mouse traps.
    mouseTraps.clear();
    spawnEntities();
    extraCheese = 0;
}

/**
 * @brief QuizGame::spawnEntities
 * Creates a cheese and two mouse traps in the level.
 */
void QuizGame::spawnEntities()
{
    // Spawn in the cheese
    int xCoord = generateCoordinate(20, 720);
    int yCoord = generateCoordinate(210, 528);

    // If the coordinate is not generated within bounds, it is recalculated
    QColor pixelColor = QImage(":/quizImages/cheeseSpawnZone.jpg").pixel(xCoord, yCoord);
    while(pixelColor != Qt::black)
    {
        xCoord = generateCoordinate(20, 720);
        yCoord = generateCoordinate(210, 528);
        pixelColor = QImage(":/quizImages/cheeseSpawnZone.jpg").pixel(xCoord, yCoord);
    }
    cheese = QRect(xCoord, yCoord, 20, 20);

    // Spawn in the two mouse traps
    for(int i = 0; i < 2; i++)
    {
        QRect mouseTrap(generateCoordinate(20, 670), generateCoordinate(255, 410), 50, 38);
        while(mouseTrap.intersects(cheese) || (mouseTraps.count() > 0 && mouseTrap.intersects(mouseTraps[0])))
        {
            mouseTrap = QRect(generateCoordinate(20, 670), generateCoordinate(255, 410), 50, 38);
        }
        mouseTraps.append(mouseTrap);
    }
}

/**
 * @brief QuizGame::generateCoordinate
 * Generates a random point between the lower and upper bounds.
 *
 * @param lowBound -- lower point bound
 * @param uppBound -- upper point bound
 * @return randomly-generated point between the bounds
 */
int QuizGame::generateCoordinate(int lowBound, int uppBound)
{
    return lowBound + rand() % ( uppBound - lowBound + 1 );
}

/**
 * @brief QuizGame::paintEvent
 * Paints the mouse body depending on its current location, force, and angle.
 *
 * @param -- unused
 */
void QuizGame::paintEvent(QPaintEvent*)
{
    QPixmap drawLayer(QPixmap::fromImage(QImage(":/quizImages/quizGameBackdrop2.jpg")));
    QPainter painter(&drawLayer);

    // Draw the mouse:
    QTransform angle;
    angle.rotateRadians(getCurrAngle());

    QImage mouseImage = QImage(":/quizImages/mouse.png").transformed(angle, Qt::FastTransformation);
    QRectF drawPos(body->GetPosition().x - (mouseImage.width() / 2),
                   body->GetPosition().y - (mouseImage.height() / 2),
                   mouseImage.width(), mouseImage.height());
    painter.drawImage(drawPos, mouseImage);

    // Draw the cheese, if alive:
    if(cheese.x() != 0)
        painter.drawImage(cheese, QImage(":/quizImages/cheese.jpg"));

    // Draw the mouse traps:
    painter.drawImage(mouseTraps[0], QImage(":/quizImages/mouseTrap.jpg"));
    painter.drawImage(mouseTraps[1], QImage(":/quizImages/mouseTrap.jpg"));

    painter.end();
    ui->backdrop->setPixmap(drawLayer);
}

/**
 * @brief QuizGame::getCurrAngle
 * Calculates the new current angle depending on the current force applied.
 * If no force exists, then the new angle is simply the old angle.
 *
 * @return currAngle -- the current mouse angle
 */
double QuizGame::getCurrAngle()
{
    if(currForce.Length() != 0)
    {
        currAngle = -acos(currForce.x / currForce.Length());
        if(currForce.y > 0)
            currAngle *= -1;
    }
    return currAngle;
}
