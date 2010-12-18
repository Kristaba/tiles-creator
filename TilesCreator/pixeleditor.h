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

#ifndef PIXELEDITOR_H
#define PIXELEDITOR_H

#include <QtGui>
#include "globaltilescreator.h"

class PixelEditor : public QLabel
{

    Q_OBJECT

    public:
        PixelEditor(int width = 16, int height = 16, int zoom = 8);
        QImage* image();
        void setImage(QImage *img);
        void setWidthHeight (int w, int h);
        static QByteArray toNBBitmap (const QImage &pix);
        static QImage fromNBBitmap (const QByteArray &bitmap, int w, int h);
        static QByteArray toGrayTileArray (const QImage &pix);
        static QImage fromGrayTileArray (const QByteArray &array, int w, int h);
        TC::Color frontColor();
        TC::Color backColor();

    signals:
        void pixelsModified(QList<QPoint> points, TC::Color newColor);

    protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void drawLine (int xA, int yA, int xB, int yB, TC::Color col);
        void changeInt(int &a, int &b);
        void mouseReleaseEvent(QMouseEvent *event);
        void refresh();
        void dragEnterEvent(QDragEnterEvent *event);
        void dragMoveEvent(QDragMoveEvent *event);
        void dropEvent(QDropEvent *event);

    public slots:
        void changeWidth(int width);
        void changeHeight(int height);
        void changeZoom(int zoom);
        void setGrid (bool visible);
        void clearTile();
        void changeFrontColor(TC::Color);
        void changeBackColor(TC::Color);
        void setEditable(bool editable);
        //void setResizeInDrop(bool resized);

    protected:
        QList<QPoint> m_pointsModified;
        int m_width, m_height, m_zoom;
        bool m_grid, m_isEditable, m_resizeInDrop;
        QImage *pix;
        int oldx, oldy;
        Qt::MouseButton oldButton;
        TC::Color m_frontColor, m_backColor;
};

#endif // PIXELEDITOR_H
