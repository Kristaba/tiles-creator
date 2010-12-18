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

#ifndef TILESELECTOR_H
#define TILESELECTOR_H

#include <QtGui>
#include "graphicstileitem.h"
#include "projectwidget.h"

class TileSelector : public QWidget
{
    Q_OBJECT

    public:
        TileSelector(ProjectWidget *project, QWidget *parent = 0);
        void exec(QPersistentModelIndex project, int widthToFind, int heightToFind);

    protected:
        void closeEvent(QCloseEvent *event);
        void changeEvent(QEvent *event);
        void focusOutEvent(QFocusEvent *event);

    protected slots:
        void selectionChanged();

    signals:
        void tileChosen (QPersistentModelIndex);

    protected:
        ProjectWidget *m_projectWidget;
        QPushButton *wm_cancel;
        QGraphicsView *m_view;
        QGraphicsScene *m_scene;
        QVector<QPersistentModelIndex> selectedTiles;
};

#endif // TILESELECTOR_H
