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

#ifndef SAMPLINGCOLORLABEL_H
#define SAMPLINGCOLORLABEL_H

#include <QtGui>

class SamplingColorLabel : public QLabel
{
Q_OBJECT
public:
    explicit SamplingColorLabel(QWidget *parent = 0);

signals:
    void samplingColorChanged(QRgb newColor);
    void zoomRequested(QPoint origin);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

protected:
    Qt::MouseButton lastPress;

};

#endif // SAMPLINGCOLORLABEL_H
