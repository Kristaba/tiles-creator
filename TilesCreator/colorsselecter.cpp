/*  Copyright 2009, 2010 Léo Grange

This file is part of Tiles Creator.

Tiles Creator is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Tiles Creator is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Tiles Creator.  If not, see <http://www.gnu.org/licenses/>. */

#include "colorsselecter.h"

ColorsSelecter::ColorsSelecter(QWidget *parent) : QLabel(parent)
{
    lastButton = Qt::NoButton;

    front = TC::Black;
    back = TC::White;
    image = 0;
    reprintImage();
    m_useAlpha = false;

    setPixmap(QPixmap::fromImage(image->scaled(34, 26, Qt::KeepAspectRatio/*, Qt::SmoothTransformation*/)));
    setFixedSize(34,26);

    actionWhite = new QAction(QIcon(QPixmap::fromImage(colorIcon(TC::White))), QString(), 0);
    actionLightGray = new QAction(QIcon(QPixmap::fromImage(colorIcon(TC::LightGray))), QString(), 0);
    actionDarkGray = new QAction(QIcon(QPixmap::fromImage(colorIcon(TC::DarkGray))), QString(), 0);
    actionBlack = new QAction(QIcon(QPixmap::fromImage(colorIcon(TC::Black))), QString(), 0);
    actionTransp = new QAction(QIcon(QPixmap::fromImage(colorIcon(TC::Transparency))), QString(), 0);

    connect(actionWhite, SIGNAL(triggered()), this, SLOT(setWhite()));
    connect(actionLightGray, SIGNAL(triggered()), this, SLOT(setLightGray()));
    connect(actionDarkGray, SIGNAL(triggered()), this, SLOT(setDarkGray()));
    connect(actionBlack, SIGNAL(triggered()), this, SLOT(setBlack()));
    connect(actionTransp, SIGNAL(triggered()), this, SLOT(setTransparency()));
}


void ColorsSelecter::reprintImage() {
    if (image != 0) delete image;
    image = new QImage(17, 13, QImage::Format_Indexed8);
    TC::setColorsIndexes(image);
    image->fill(TC::White);
    for (int i = 0; i<17; i++) image->setPixel(i, 0, TC::GridColor);
    for (int i = 0; i<17; i++) image->setPixel(i, 12, TC::GridColor);
    for (int i = 0; i<13; i++) image->setPixel(0, i, TC::GridColor);
    for (int i = 0; i<13; i++) image->setPixel(16, i, TC::GridColor);

    for (int i = 2; i<12; i++) image->setPixel(i, 2, TC::Black);
    for (int i = 2; i<12; i++) image->setPixel(i, 7, TC::Black);
    for (int i = 5; i<15; i++) image->setPixel(i, 10, TC::Black);
    for (int i = 12; i<15; i++) image->setPixel(i, 5, TC::Black);
    for (int i = 2; i<8; i++) image->setPixel(2, i, TC::Black);
    for (int i = 2; i<8; i++) image->setPixel(11, i, TC::Black);
    for (int i = 8; i<11; i++) image->setPixel(5, i, TC::Black);
    for (int i = 5; i<11; i++) image->setPixel(14, i, TC::Black);
    for (int w = 3; w<11; w++) for (int h = 3; h<7; h++) image->setPixel(w, h, front);
    for (int w = 6; w<14; w++) for (int h = 8; h<10; h++) image->setPixel(w, h, back);
    for (int w = 12; w<14; w++) for (int h = 6; h<8; h++) image->setPixel(w, h, back);
}


QImage ColorsSelecter::colorIcon(TC::Color c) {
    QImage tmp(20, 12, QImage::Format_Indexed8);
    TC::setColorsIndexes(&tmp);

    for (int i = 0; i<20; i++) tmp.setPixel(i, 0, TC::GridColor);
    for (int i = 0; i<20; i++) tmp.setPixel(i, 11, TC::GridColor);
    for (int i = 0; i<12; i++) tmp.setPixel(0, i, TC::GridColor);
    for (int i = 0; i<12; i++) tmp.setPixel(19, i, TC::GridColor);

    for (int w = 1; w<20; w++) for (int h = 1; h<12; h++)tmp.setPixel(w, h, c);
    return tmp;
}


void ColorsSelecter::mousePressEvent(QMouseEvent *event) {
    QMenu *menu = new QMenu;
    menu->addAction(actionWhite);
    menu->addAction(actionLightGray);
    menu->addAction(actionDarkGray);
    menu->addAction(actionBlack);
    if (m_useAlpha) menu->addAction(actionTransp);
    menu->setMaximumWidth(35);

    lastButton = event->button();

    menu->exec(mapToGlobal(event->pos()));
}

void ColorsSelecter::setColor(TC::Color c) {
    if (lastButton == Qt::LeftButton) {
        front = c;
        emit frontChanged(c);
    }
    else if (lastButton == Qt::RightButton) {
        back = c;
        emit backChanged(c);
    }
    reprintImage();

    setPixmap(QPixmap::fromImage(image->scaled(34, 26, Qt::KeepAspectRatio/*, Qt::SmoothTransformation*/)));
}


void ColorsSelecter::setWhite() {
    setColor(TC::White);
}

void ColorsSelecter::setLightGray() {
    setColor(TC::LightGray);
}

void ColorsSelecter::setDarkGray() {
    setColor(TC::DarkGray);
}

void ColorsSelecter::setBlack() {
    setColor(TC::Black);
}

void ColorsSelecter::setTransparency() {
    setColor(TC::Transparency);
}

void ColorsSelecter::setUseAlpha(bool use) {
    m_useAlpha = use;
}
