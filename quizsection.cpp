/*
 * Team             |   Chandler
 * Members          |   Braden Fiedel, Caden Erickson, Connor Blood, Josie Fiedel
 * Class            |   CS 3505
 * Project          |   A8: Sprint 2
 * Last modified    |   April 26, 2023
 *
 * This file contains the implementation of the class definition located in quizsection.h.
 */


#include "quizsection.h"
#include "ui_quizsection.h"
#include <QAudioOutput>
#include <QMediaPlayer>


/**
 * @brief QuizSection::QuizSection
 * Constructor. Creates a new QuizSection and sets up its UI, with necessary
 * connect calls.
 *
 * @param parent -- pointer to QWidget parent object.
 */
QuizSection::QuizSection(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::QuizSection)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    // Initialize video player objects
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;

    // Prioritize the focus for mouse events
    setFocusPolicy(Qt::StrongFocus);

    // Connections for starting and cycling through the quiz
    connect(ui->beginButton, &QPushButton::clicked,
            this, &QuizSection::nextQuestion);
    connect(ui->videoAnswerButton, &QPushButton::clicked,
             ui->stackedWidget, [this](){ui->stackedWidget->setCurrentIndex(3);});
    connect(ui->videoAnswerButton, &QPushButton::clicked,
             this, [this](){lastPlayedWasVid = true;});
    connect(ui->imageAnswerButton, &QPushButton::clicked,
             ui->stackedWidget, [this](){ui->stackedWidget->setCurrentIndex(3);});
    connect(ui->imageAnswerButton, &QPushButton::clicked,
             this, [this](){lastPlayedWasVid = false;});
    connect(ui->gameScreen, &QuizGame::onAnswerGuess,
             this, [this](){ui->stackedWidget->setCurrentIndex(4);});

    // Connections for playing videos
    connect(player, &QMediaPlayer::playbackStateChanged,
            this, &QuizSection::checkVideoState);
    connect(ui->startVideoButton, &QPushButton::clicked,
            this, &QuizSection::playVideo);

    // Connections for incrementing indices
    connect(ui->videoAnswerButton, &QPushButton::clicked,
             this, [this](){currentVideoQuestionIndex++;});
    connect(ui->imageAnswerButton, &QPushButton::clicked,
             this, [this](){currentImageQuestionIndex++;});

    // Connections for TEMPORARY CONTINUE BUTTONS
    connect(ui->gameContinue, &QPushButton::clicked,
            ui->stackedWidget, [this](){ui->stackedWidget->setCurrentIndex(4);
                                        ui->simulationScreen->answerReceived(false, 0);});

    // Connections for managing the movement of the mouse character in the game
    connect(this, &QuizSection::sendKeyPress,
            ui->gameScreen, &QuizGame::onKeyPress);
    connect(this, &QuizSection::sendKeyRelease,
            ui->gameScreen, &QuizGame::onKeyRelease);

    // Connections for quiz simulation
    connect(ui->gameScreen, &QuizGame::onAnswerGuess,
            ui->simulationScreen, &QuizSimulation::answerReceived);
    connect(ui->simulationScreen, &QuizSimulation::nextQuestion,
            this, &QuizSection::nextQuestion);

    // Temp timer start
    connect(ui->videoAnswerButton, &QPushButton::clicked,
            ui->gameScreen, &QuizGame::startSimulation);
    connect(ui->imageAnswerButton, &QPushButton::clicked,
            ui->gameScreen, &QuizGame::startSimulation);

    // Connections for celebration screen
    connect(ui->simulationScreen, &QuizSimulation::scoreChanged,
            this, &QuizSection::updateFinalScore);
    connect(ui->exitButton, &QPushButton::clicked,
            this, [this](){emit exitPressed();});


    // Set up QVectors with all video URLs and image URLS
    videoUrls.push_back(QUrl("qrc:/quizQuestions/CatScratching.mp4"));
    videoUrls.push_back(QUrl("qrc:/quizQuestions/catplayful.mp4"));
    videoUrls.push_back(QUrl("qrc:/quizQuestions/catslowblink.mp4"));
    videoUrls.push_back(QUrl("qrc:/quizQuestions/bunting.mp4"));
    videoUrls.push_back(QUrl("qrc:/quizQuestions/trilling.mp4"));
    videoUrls.push_back(QUrl("qrc:/quizQuestions/zoomies.mp4"));

    imageUrls.push_back((":/quizQuestions/angryCat.jpg"));
    imageUrls.push_back((":/quizQuestions/bellyCat.jpg"));
    imageUrls.push_back((":/quizQuestions/elevatorButt.jpg"));
    imageUrls.push_back((":/quizQuestions/friendlyCat.jpg"));
    imageUrls.push_back((":/quizQuestions/scaredCat.jpg"));
    imageUrls.push_back((":/quizQuestions/scruffing.jpg"));

    // Start the quiz with a video, initialize indices
    lastPlayedWasVid = false;
    currentVideoQuestionIndex = 0;
    currentImageQuestionIndex = 0;

    // Set up the quiz answers (ORDER MATTERS)
    quizAnswers.push_back(QList<QString>{"Marking Territory","Saying Hello","Making Biscuits"});
    quizAnswers.push_back(QList<QString>{"Aggression","Fear","Happiness"});
    quizAnswers.push_back(QList<QString>{"Happy & Playful","Aggressive & Scared","Sleepy & Calm"});
    quizAnswers.push_back(QList<QString>{"Safe & Comfortable","Attacking","Sleeping"});
    quizAnswers.push_back(QList<QString>{"Expressing Affection","Tired","Bunting"});
    quizAnswers.push_back(QList<QString>{"Elevator Butt","Stretching","Defensive"});
    quizAnswers.push_back(QList<QString>{"Greeting","Annoyance","Hungry"});
    quizAnswers.push_back(QList<QString>{"Friendly","Defensive","Sleepy"});
    quizAnswers.push_back(QList<QString>{"Trilling","Purring","Chattering"});
    quizAnswers.push_back(QList<QString>{"Fear","Anger","Elevator Butt"});
    quizAnswers.push_back(QList<QString>{"Zoomies","Scared","Trilling"});
    quizAnswers.push_back(QList<QString>{"Scruffing","Hunting","Bullying"});
}

/**
 * @brief QuizSection::~QuizSection
 * Destructor. Deallocates memory for the ui object, and destructs this View.
 */
QuizSection::~QuizSection()
{
    delete ui;
}

/**
 * @brief QuizSection::nextQuestion
 * Advances the quiz to the next question. Control is set up to ensure that the
 * questions of the quiz alternate between videos and images. It is set so that
 * a video will play first.
 */
void QuizSection::nextQuestion()
{
    // If all videos and images have been answered, go to the celebration screen
    if(currentVideoQuestionIndex > 5 && currentImageQuestionIndex > 5)
    {
        ui->stackedWidget->setCurrentIndex(5);
        return;
    }

    // If we played a video last, do an image question
    if(lastPlayedWasVid)
    {
        ui->stackedWidget->setCurrentIndex(2);
        displayImage();

        lastPlayedWasVid = false;
    }
    // If an image question was used last, do a video question
    else
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->startVideoButton->setEnabled(true);

        lastPlayedWasVid = true;
    }

    // Send in the next quiz answers, and remove them from the vector
    ui->gameScreen->setNextLevel(quizAnswers.front());
    quizAnswers.pop_front();
}

/**
 * @brief QuizSection::playVideo
 * Sets up and plays a video corresponding to the current question as tracked by
 * the currentVideoQuestionIndex tracker.
 */
void QuizSection::playVideo()
{
    // This block of code will set up a video to play with audio
    player->setVideoOutput(ui->videoWidget);
    player->setAudioOutput(audioOutput);
    player->setSource(videoUrls.at(currentVideoQuestionIndex));
    ui->videoWidget->show();
    audioOutput->setVolume(100);
    player->play();
}

/**
 * @brief QuizSection::checkVideoState
 * Adjusts the UI based on the given integer, which corresponds the the state of
 * a video (either stopped, playing, or paused). If the video is playing, all
 * buttons are disabled; if the video is stopped, all buttons are enabled.
 *
 * @param videoState --  the current state of the video
 */
void QuizSection::checkVideoState(int videoState)
{
    // Index 0 means that the video is stopped. Enable buttons.
    if(videoState == 0)
    {
        ui->startVideoButton->setEnabled(true);
        ui->videoAnswerButton->setEnabled(true);
    }
    // Index 1 means that the video is playing, disable buttons.
    else if(videoState == 1)
    {
        ui->startVideoButton->setEnabled(false);
        ui->videoAnswerButton->setEnabled(false);
    }
}

/**
 * @brief QuizSection::displayImage
 * Sets up and displays an image corresponding to the current question as
 * tracked by the currentImageQuestionIndex tracker.
 */
void QuizSection::displayImage()
{
    // Set up a pixmap from the current image
    QPixmap questionImage(imageUrls.at(currentImageQuestionIndex));

    // Use a label to display the image scaled to the right size
    ui->imageLabel->setPixmap(questionImage.scaled(ui->imageLabel->width(), ui->imageLabel->height(),Qt::KeepAspectRatio));
    ui->imageLabel->show();
}

/**
 * @brief QuizSection::keyPressEvent
 * Emits a signal with the pressed key, to be recieved by the QuizGame object.
 *
 * @param event -- the keyPress event
 */
void QuizSection::keyPressEvent(QKeyEvent* event)
{
    emit sendKeyPress(event->key());
}

/**
 * @brief QuizSection::keyReleaseEvent
 * Emits a signal with the released key, to be recieved by the QuizGame object.
 *
 * @param event -- the keyRelease event
 */
void QuizSection::keyReleaseEvent(QKeyEvent* event)
{
    emit sendKeyRelease(event->key());
}

/**
 * @brief QuizSection::updateFinalScore
 * Updates the score displayed on the final page of the "Quiz" section.
 *
 * @param score -- the user's score
 */
void QuizSection::updateFinalScore(int score)
{
    // It's good form to not have "1 points" displayed
    QString scoreDisplay = QString("You finished with %1 point").arg(score);
    scoreDisplay.append(score == 1 ? "." : "s.");

    ui->scoreLabel->setText(scoreDisplay);
}
