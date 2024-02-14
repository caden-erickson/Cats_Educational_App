/*
 * Team             |   Chandler
 * Members          |   Braden Fiedel, Caden Erickson, Connor Blood, Josie Fiedel
 * Class            |   CS 3505
 * Project          |   A8: Sprint 2
 * Last modified    |   April 26, 2023
 *
 * This file contains the class definition for the CatDiagram class.
 */


#ifndef CATDIAGRAM_H
#define CATDIAGRAM_H

#include <QDirIterator>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class CatDiagram; }
QT_END_NAMESPACE

/**
 * @brief The CatDiagram class
 * This class represents the cat diagram seen in the "Learn" section of the
 * program. It tracks mouse events, and changes the diagram appropriately based
 * on hovering and clicking. It also emits a signal when all the sections have
 * been selected.
 */
class CatDiagram : public QWidget
{
    Q_OBJECT

public:
    explicit CatDiagram(QWidget* parent = nullptr);
    ~CatDiagram();

signals:
    void regionSelected(int);
    void highlightCompletion();

private:
    Ui::CatDiagram* ui;
    QImage drawingLayer;
    QList<int> progressList;
    QString currentlyHoveredSection;

    void mouseMoveEvent(QMouseEvent* event);

public slots:
    void clearProgress();

private slots:
    void onDiagramClick();
};

#endif // CATDIAGRAM_H
