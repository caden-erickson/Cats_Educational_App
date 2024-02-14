/*
 * Team             |   Chandler
 * Members          |   Braden Fiedel, Caden Erickson, Connor Blood, Josie Fiedel
 * Class            |   CS 3505
 * Project          |   A8: Sprint 2
 * Last modified    |   April 26, 2023
 *
 * This file contains the implementation of the class definition located in catdiagram.h.
 */


#include "catdiagram.h"
#include "ui_catdiagram.h"


/**
 * @brief CatDiagram::CatDiagram
 * Constructor. Creates a new CatDiagram object and sets up its UI,
 * with necessary connect calls.
 *
 * @param parent -- pointer to QWidget parent object
 */
CatDiagram::CatDiagram(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::CatDiagram)
    , drawingLayer(QImage(":/catDiagramBackground/catDiagramBody.png"))
{
    ui->setupUi(this);

    // Connection for the action of a user click.
    connect(ui->clickable, &QPushButton::clicked,
            this, &CatDiagram::onDiagramClick);
}

/**
 * @brief CatDiagram::~CatDiagram
 * Destructor. Deallocates memory for the ui object, and destructs this View.
 */
CatDiagram::~CatDiagram()
{
    delete ui;
}

/**
 * @brief learningView::handleBodyClick
 * Highlights the clicked body part when the cat diagram is clicked. A signal is
 * emitted when this happens so that the LearningSection UI can display the
 * appropriate learning information.
 */
void CatDiagram::onDiagramClick()
{
    // Retrieve the current widget point from the clicked window point.
    QPoint currPoint = ui->partsLayer->mapFromGlobal(QCursor::pos());

    // Iterate over all cat body parts to change the region.
    int count = 0;
    QDirIterator it(":/catDiagramParts-opaque", QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QString bodyPart = it.next();

        // If a body part region is clicked, then a color exists at this pixel.
        QColor color = QImage(bodyPart).pixel(currPoint.x(), currPoint.y());
        if(color != Qt::black)
        {
            // If this body part is newly selected, it is added to the progress list
            // and composed with the body parts layer image.
            if(!progressList.contains(count))
            {
                progressList.append(count);

                QPainter painter(&drawingLayer);
                painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
                painter.drawImage(0, 0, QImage(bodyPart));

                ui->partsLayer->setPixmap(QPixmap::fromImage(drawingLayer));
            }

            // The learning section is notified to enable the quiz button once all
            // body parts have been selected.
            if(progressList.size() == 8)
                emit highlightCompletion();

            emit regionSelected(count);
            break;
        }
        count++;
    }
}

/**
 * @brief CatDiagram::clearProgress
 * Clears the diagram selection and resets the progress list.
 */
void CatDiagram::clearProgress()
{
    progressList.clear();
    drawingLayer = QImage(":/catDiagramBackground/catDiagramBody.png");
    ui->partsLayer->setPixmap(QPixmap::fromImage(QImage(drawingLayer)));
}

/**
 * @brief CatDiagram::mouseMoveEvent
 * Sets the mouse cursor to be a pointing hand if it's over the cat diagram.
 * Otherwise, sets the mouse cursor to the default arrow cursor.
 *
 * @param event -- the mouseMove event
 */
void CatDiagram::mouseMoveEvent(QMouseEvent* event)
{
    QImage image(":/catDiagramBackground/catDiagramBody.png");
    QColor color(image.pixel(event->pos().x(), event->pos().y()));
    if(color != Qt::black && color != QColor(88, 88, 90))
    {
        QApplication::setOverrideCursor(Qt::PointingHandCursor);

        // Retrieve the current widget point from the clicked window point.
        QPoint currPoint = ui->partsLayer->mapFromGlobal(QCursor::pos());

        // Iterate over all cat body parts to change the region.
        QDirIterator it(":/catDiagramParts-translucent", QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            QString bodyPart = it.next();

            // If a body part region is clicked, then a color exists at this pixel.
            QColor color = QImage(bodyPart).pixel(currPoint.x(), currPoint.y());
            if(color != Qt::black && currentlyHoveredSection != bodyPart)
            {
                currentlyHoveredSection = bodyPart;
                QImage layerWithHover = drawingLayer.copy();
                QPainter painter(&layerWithHover);
                painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
                painter.drawImage(0, 0, QImage(bodyPart));
                ui->partsLayer->setPixmap(QPixmap::fromImage(layerWithHover));
            }
        }
    }
    else
    {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        if (currentlyHoveredSection != "")
        {
            currentlyHoveredSection = QWidget::tr("");
            ui->partsLayer->setPixmap(QPixmap::fromImage(drawingLayer));
        }
    }
}
