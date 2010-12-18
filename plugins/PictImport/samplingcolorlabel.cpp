/*  Copyright 2010 Léo Grange

This file is part of PictImport, a plugin for Tiles Creator.

PictImport is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

PictImport is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with PictImport.  If not, see <http://www.gnu.org/licenses/>. */

#include "samplingcolorlabel.h"

SamplingColorLabel::SamplingColorLabel(QWidget *parent) :
    QLabel(parent)
{

}


void SamplingColorLabel::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (pixmap() != 0 && event->x() >= 0 && event->x() < pixmap()->width()
                &&  event->y() >= 0 && event->y() < pixmap()->height()) {
            QImage image = pixmap()->toImage();
            if (image.width() > event->x() && image.height() > event->y()) {
                QColor color = image.pixel(event->pos());
                QColor mask = Qt::color1;
                if (pixmap()->hasAlpha()) mask = pixmap()->mask().toImage().pixel(event->pos());
                if (mask == Qt::color0)
                    color.setAlpha(0);
                emit samplingColorChanged(color.rgba());
            }
        }
    }
    else if (event->button() == Qt::RightButton) {
        emit zoomRequested(event->pos());
    }
    lastPress = event->button();
    QLabel::mousePressEvent(event);
}


void SamplingColorLabel::mouseMoveEvent(QMouseEvent *event) {
    if (lastPress == Qt::LeftButton) {
        if (pixmap() != 0 && event->x() >= 0 && event->x() < pixmap()->width()
                &&  event->y() >= 0 && event->y() < pixmap()->height()) {
            QImage image = pixmap()->toImage();
            if (image.width() > event->x() && image.height() > event->y()) {
                QColor color = image.pixel(event->pos());
                QColor mask = Qt::color1;
                if (pixmap()->hasAlpha()) mask = pixmap()->mask().toImage().pixel(event->pos());
                if (mask == Qt::color0)
                    color.setAlpha(0);
                emit samplingColorChanged(color.rgba());
            }
        }
    }
    else if (lastPress == Qt::RightButton) {
        emit zoomRequested(event->pos());
    }
    QLabel::mouseMoveEvent(event);
}
