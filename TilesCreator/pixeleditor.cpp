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

#include "pixeleditor.h"

PixelEditor::PixelEditor(int width, int height, int zoom) : QLabel()
{
    if (width < 1) m_width = 1;
    else m_width = width;

    if (height < 1) m_height = 1;
    else m_height = height;

    m_zoom = zoom;
    oldx = -1;
    oldy = -1;
    oldButton = Qt::NoButton;
    m_grid = false;
    m_isEditable = true;
    m_resizeInDrop = true;
    m_frontColor = TC::Black;
    m_backColor = TC::White;
    setAcceptDrops(true);

    pix = new QImage(m_width, m_height, QImage::Format_Indexed8);
    TC::setColorsIndexes(pix);

    pix->fill(TC::White);

    setPixmap(QPixmap::fromImage(pix->scaled(m_width * m_zoom, m_height * m_zoom)));
}



void PixelEditor::mousePressEvent(QMouseEvent *event) {
    if (m_isEditable) {
        int realx, realy;

        if (!m_grid) {
            realx = event->x() / m_zoom;
            realy = event->y() / m_zoom;
        }
        else {
            realx = event->x() / (m_zoom+1);
            realy = event->y() / (m_zoom+1);
        }

        if (event->button() == Qt::LeftButton) {
            if (!m_pointsModified.contains(QPoint(realx, realy))) m_pointsModified.append(QPoint(realx, realy));
            pix->setPixel(realx, realy, m_frontColor);
        }
        else if (event->button() == Qt::RightButton) {
            if (!m_pointsModified.contains(QPoint(realx, realy))) m_pointsModified.append(QPoint(realx, realy));
            pix->setPixel(realx, realy, m_backColor);
        }

        refresh();
        oldx = realx;
        oldy = realy;
        oldButton = event->button();
    }
    QLabel::mousePressEvent(event);
}



void PixelEditor::mouseReleaseEvent(QMouseEvent *event) {
    oldx = -1;
    oldy = -1;
    if (oldButton == Qt::LeftButton) emit pixelsModified(m_pointsModified, m_frontColor);
    else if (oldButton == Qt::RightButton) emit pixelsModified(m_pointsModified, m_backColor);
    oldButton = Qt::NoButton;
    m_pointsModified.clear();
    QLabel::mouseReleaseEvent(event);
}



void PixelEditor::mouseMoveEvent(QMouseEvent *event) {
    if (m_isEditable) {
        int realx, realy;

        if (!m_grid) {
            realx = event->x() / m_zoom;
            realy = event->y() / m_zoom;
        }
        else {
            realx = event->x() / (m_zoom+1);
            realy = event->y() / (m_zoom+1);
        }

        if (realx >= 0 && realx < m_width && realy >= 0 && realy < m_height) {
            double lenght = sqrt(pow(oldx-realx, 2) + pow(oldy-realy, 2));
            if (lenght != 0) {
                if (lenght < 2) {
                    if (oldButton == Qt::LeftButton) {
                        if (!m_pointsModified.contains(QPoint(realx, realy))) m_pointsModified.append(QPoint(realx, realy));
                        pix->setPixel(realx, realy, m_frontColor);
                    }
                    else if (oldButton == Qt::RightButton) {
                        if (!m_pointsModified.contains(QPoint(realx, realy))) m_pointsModified.append(QPoint(realx, realy));
                        pix->setPixel(realx, realy, m_backColor);
                    }
                }
                else {
                    if (oldButton == Qt::LeftButton) {
                        drawLine(realx, realy, oldx, oldy, m_frontColor);
                    }
                    else if (oldButton == Qt::RightButton) {
                        drawLine(realx, realy, oldx, oldy, m_backColor);
                    }
                }

                refresh();
                oldx = realx;
                oldy = realy;
            }
        }
    }

    QLabel::mouseMoveEvent(event);
}


void PixelEditor::drawLine (int xA, int yA, int xB, int yB, TC::Color col) {
    if ((xA>=0 && xA<width() && yA>=0 && yA<height()) && (xB>=0 && xB<width() && yB>=0 && yB<height())){
        int d, dx, dy, aincr, bincr, xincr, yincr, x, y;

        if (abs(xB - xA) < abs(yB - yA)) {
            /* parcours par l'axe vertical */

            if (yA > yB) {
              changeInt(xA, xB);
              changeInt(yA, yB);
            }
            xincr = -1;
            if (xB > xA) xincr = 1;
            dy = yB - yA;
            dx = abs(xB - xA);
            d = 2 * dx - dy;
            aincr = 2 * (dx - dy);
            bincr = 2 * dx;
            x = xA;
            y = yA;

            if (!m_pointsModified.contains(QPoint(x, y))) m_pointsModified.append(QPoint(x, y));
            pix->setPixel(x, y, col);

            for (y = yA+1; y <= yB; ++y) {
              if (d >= 0) {
                x += xincr;
                d += aincr;
              } else
                d += bincr;

              if (!m_pointsModified.contains(QPoint(x, y))) m_pointsModified.append(QPoint(x, y));
              pix->setPixel(x, y, col);
            }
        }
        else {
            /* parcours par l'axe horizontal */

            if (xA > xB) {
              changeInt(xA, xB);
              changeInt(yA, yB);
            }

            yincr = -1;
            if (yB > yA) yincr = 1;
            dx = xB - xA;
            dy = abs(yB - yA);
            d = 2 * dy - dx;
            aincr = 2 * (dy - dx);
            bincr = 2 * dy;
            x = xA;
            y = yA;

            if (!m_pointsModified.contains(QPoint(x, y))) m_pointsModified.append(QPoint(x, y));
            pix->setPixel(x, y, col);

            for (x = xA+1; x <= xB; ++x) {
              if (d >= 0) {
                y += yincr;
                d += aincr;
              } else
                d += bincr;

              if (!m_pointsModified.contains(QPoint(x, y))) m_pointsModified.append(QPoint(x, y));
              pix->setPixel(x, y, col);
            }
        }
    }
}


void PixelEditor::changeInt (int &a, int &b){
    int tmp;
    tmp=a;
    a=b;
    b=tmp;
    return;
}



void PixelEditor::refresh() {
    if (m_width != pix->width() || m_height != pix->height()) {
        QImage *newPix = new QImage(m_width, m_height, QImage::Format_Indexed8);
        TC::setColorsIndexes(newPix);
        newPix->fill(TC::White);
        for (int i=0; i<pix->width() && i<m_width; i++) {
            for (int j=0; j<pix->height() && j<m_height; j++) {
                newPix->setPixel(i, j, pix->pixelIndex(i, j));
            }
        }
    
        delete pix;
        pix = newPix;
    }

    if (!m_grid) {
        setPixmap(QPixmap::fromImage(pix->scaled(m_width * m_zoom, m_height * m_zoom)));
        setFixedSize(m_width * m_zoom, m_height * m_zoom);
    }
    else {
        QImage tempPix(m_width*(m_zoom+1)+1, m_height*(m_zoom+1)+1, QImage::Format_Indexed8);
        TC::setColorsIndexes(&tempPix);
        tempPix.fill(TC::White);

        for (int i=0; i<pix->width(); i++) {
            for (int j=0; j<pix->height(); j++) {
                if (pix->pixelIndex(i, j) != TC::White) {
                    for (int k=0; k<m_zoom; k++) {
                        for (int l=0; l<m_zoom; l++) {
                            tempPix.setPixel((i*(m_zoom+1))+k+1, (j*(m_zoom+1))+l+1, pix->pixelIndex(i,j));
                        }
                    }
                }
            }
        }

        for (int i=0; i<=m_width; i++) {
            for (int j=0; j<=(m_height*(m_zoom+1)); j++) {
                tempPix.setPixel(i*(m_zoom+1), j, TC::GridColor);
            }
        }
        for (int i=0; i<=m_height; i++) {
            for (int j=0; j<=(m_width*(m_zoom+1)); j++) {
                tempPix.setPixel(j, i*(m_zoom+1), TC::GridColor);
            }
        }
        setPixmap(QPixmap::fromImage(tempPix));
        setFixedSize(m_width*(m_zoom+1)+1, m_height*(m_zoom+1)+1);
    }
}



void PixelEditor::setWidthHeight (int w, int h) {
    m_width = w;
    m_height = h;
    refresh();
}

QImage* PixelEditor::image() {
    return pix;
}

void PixelEditor::setImage(QImage *img) {
    if (pix != 0) delete pix;
    pix = img;
    setWidthHeight(img->width(), img->height());
}


void PixelEditor::changeWidth(int width) {
    m_width = width;
    refresh();
}

void PixelEditor::changeHeight(int height) {
    m_height = height;
    refresh();
}

void PixelEditor::changeZoom(int zoom) {
    m_zoom = zoom;
    refresh();
}

void PixelEditor::setGrid (bool visible) {
    m_grid = visible;
    refresh();
}

void PixelEditor::clearTile() {
    pix->fill(TC::White);
    refresh();
}


QByteArray PixelEditor::toNBBitmap (const QImage &pix) {
    int byteWidth;
    if (pix.width() % 8 != 0) byteWidth = pix.width()/8 + 1;
    else byteWidth = pix.width()/8;

    QByteArray bitmap(byteWidth * pix.height(), 0x00);

    int tmp = 0;
    for (int i=0; i<pix.height(); i++) {
        for (int j=0; j<pix.width(); j += 8) {
            unsigned char byte = 0;
            byte += 0x80 * pix.pixelIndex(j, i);
            if ((j+1)<pix.width()) byte += 0x40 * pix.pixelIndex(j+1, i);
            if ((j+2)<pix.width()) byte += 0x20 * pix.pixelIndex(j+2, i);
            if ((j+3)<pix.width()) byte += 0x10 * pix.pixelIndex(j+3, i);
            if ((j+4)<pix.width()) byte += 0x08 * pix.pixelIndex(j+4, i);
            if ((j+5)<pix.width()) byte += 0x04 * pix.pixelIndex(j+5, i);
            if ((j+6)<pix.width()) byte += 0x02 * pix.pixelIndex(j+6, i);
            if ((j+7)<pix.width()) byte += 0x01 * pix.pixelIndex(j+7, i);
            bitmap[tmp] = byte;
            tmp++;
        }
    }
    return bitmap;
}



QImage PixelEditor::fromNBBitmap (const QByteArray &bitmap, int w, int h) {
    QImage pix = QImage(w, h, QImage::Format_Indexed8);
    TC::setColorsIndexes(&pix);

    for (int i=0; i<bitmap.size(); i++) {
        unsigned char B = bitmap.at(i);
        int tmp_w = (i*8)%w;
        int tmp_h = (i*8)/w;
        pix.setPixel(tmp_w, tmp_h, (B & 0x80) >> 7);
        if (tmp_w+1 <= w) pix.setPixel(tmp_w+1, tmp_h, (B & 0x40) >> 6);
        if (tmp_w+2 <= w) pix.setPixel(tmp_w+2, tmp_h, (B & 0x20) >> 5);
        if (tmp_w+3 <= w) pix.setPixel(tmp_w+3, tmp_h, (B & 0x10) >> 4);
        if (tmp_w+4 <= w) pix.setPixel(tmp_w+4, tmp_h, (B & 0x08) >> 3);
        if (tmp_w+5 <= w) pix.setPixel(tmp_w+5, tmp_h, (B & 0x04) >> 2);
        if (tmp_w+6 <= w) pix.setPixel(tmp_w+6, tmp_h, (B & 0x02) >> 1);
        if (tmp_w+7 <= w) pix.setPixel(tmp_w+7, tmp_h, (B & 0x01));
    }
    return pix;
}



QByteArray PixelEditor::toGrayTileArray (const QImage &pix) {
    QByteArray array(pix.width() * pix.height(), 0x00);

    for (int h=0; h<pix.height(); h++) {
        for (int w=0; w<pix.width(); w++) array[w+h*pix.width()] = pix.pixelIndex(w, h);
    }
    return array;
}



QImage PixelEditor::fromGrayTileArray (const QByteArray &array, int w, int h) {
    QImage image(w, h, QImage::Format_Indexed8);
    TC::setColorsIndexes(&image);

    for (int i=0; i<w*h; i++) if (i < array.size()) image.setPixel(i%w, i/w, array.at(i));
    return image;
}



void PixelEditor::changeFrontColor(TC::Color c) {
    m_frontColor = c;
}


void PixelEditor::changeBackColor(TC::Color c) {
    m_backColor = c;
}


TC::Color PixelEditor::frontColor() {
    return m_frontColor;
}


TC::Color PixelEditor::backColor() {
    return m_backColor;
}


void PixelEditor::setEditable(bool editable) {
    m_isEditable = editable;
}


void PixelEditor::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("application/x-tile")) {
        event->acceptProposedAction();
    }
    else event->ignore();
}


void PixelEditor::dragMoveEvent(QDragMoveEvent *event) {
    if (event->mimeData()->hasFormat("application/x-tile")) {
        event->acceptProposedAction();
    }
    else event->ignore();
}


void PixelEditor::dropEvent(QDropEvent *event) {
    /*QComboBox *types = new QComboBox;
    types->addItems(event->mimeData()->formats());
    types->show();*/
    if (event->mimeData()->hasFormat("application/x-tile")) {
        event->accept();
        QByteArray data(event->mimeData()->data("application/x-tile"));
        QDataStream stream(&data, QIODevice::ReadOnly);
        stream >> *pix;
    }
    refresh();
}
