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

#include "tileselector.h"

TileSelector::TileSelector(ProjectWidget *project, QWidget *parent) : QWidget(parent)
{
    //setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::ToolTip);
    //setWindowModality(Qt::ApplicationModal);
    setFixedSize(200, 200);
    m_projectWidget = project;

    m_scene = new QGraphicsScene(0, 0, 150, 140);
    m_view = new QGraphicsView(m_scene);
    m_view->scale(2, 2);
    m_view->setFrameShape(QFrame::NoFrame);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(2);

    layout->addWidget(m_view);

    wm_cancel = new QPushButton(tr("Annuler"));
    //layout->addWidget(wm_cancel);

    setLayout(layout);
    setFocusPolicy(Qt::StrongFocus);

    connect (m_scene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
    connect (wm_cancel, SIGNAL(pressed()), this, SLOT(hide()));
}


void TileSelector::exec(QPersistentModelIndex project, int widthToFind, int heightToFind) {
    show();
    m_scene->clear();
    selectedTiles.clear();
    QVector<QPersistentModelIndex> tiles(m_projectWidget->tilesOfProject(project));

    //Selectionner tous les tiles ayant la bonne dimension et les afficher :
    for (int i=0; i<tiles.size(); i++) {
        QImage tmpTile = tiles.at(i).data(ProjectWidget::TileRole).value<QImage>();
        int tileWidth = tmpTile.width();
        int tileHeight = tmpTile.height();
        if (tileWidth == widthToFind && tileHeight == heightToFind) {
            selectedTiles.append(tiles.at(i));
            int j = selectedTiles.size()-1;
            GraphicsTileItem *item = new GraphicsTileItem(widthToFind, heightToFind, j);
            item->setPixmap(QPixmap::fromImage(tmpTile));
            item->setToolTip(tiles.at(i).data(Qt::DisplayRole).toString());
            m_scene->addItem(item);
            item->setPos(j%6 * widthToFind, j/6 * heightToFind);
        }
    }

    //Dessiner la grille :
    int vert = 7;
    if (selectedTiles.size() < 6) vert = selectedTiles.size()+1;
    if (selectedTiles.size() != 0) {
        for (int i=0; i<vert; i++) {
            int h = (selectedTiles.size()-i)/6 + 1;
            if (i==0 && selectedTiles.size()%6 == 0) h--;
            QGraphicsItem *item = m_scene->addLine(i*widthToFind, 0, i*widthToFind, h*heightToFind, QPen(QColor(0xFF888888)));
            item->setZValue(9);
        }
        int h = (selectedTiles.size()-1)/6 + 1;
        for (int i=0; i<h+1; i++) {
            int w=6;
            if ((i == h || selectedTiles.size() < 6) && selectedTiles.size()%6 != 0) w = selectedTiles.size()%6;
            QGraphicsItem *item = m_scene->addLine(0, i*heightToFind, w*widthToFind, i*heightToFind, QPen(QColor(0xFF888888)));
            item->setZValue(9);
        }
    }

    //Donner des tailles fixes idéales (à voir selon les OS...) :
    setFixedSize(widthToFind*6*2+10, ((selectedTiles.size()-1)/6+1)*heightToFind*2+12);
    m_scene->setSceneRect(0, 0, widthToFind*6, ((selectedTiles.size()-1)/6+1)*heightToFind);

    //Mettre en fenêtre active et passer le Focus :
    qApp->setActiveWindow(this);
    setFocus();
}


void TileSelector::selectionChanged() {
    QPersistentModelIndex ret;
    QList<QGraphicsItem*> selected = m_scene->selectedItems();
    if (!selected.isEmpty()) {
        GraphicsTileItem *item = qgraphicsitem_cast<GraphicsTileItem*>(selected.at(0));
        if (item != 0) {
            int id = item->data(GraphicsTileItem::XIdRole).toInt();
            if (id >= 0 && id < selectedTiles.size()) {
                emit tileChosen(selectedTiles.at(id));
                hide();
            }
        }
    }
}


void TileSelector::closeEvent(QCloseEvent *event) {
    hide();
    event->ignore();
    QWidget::closeEvent(event);
}


void TileSelector::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        wm_cancel->setText(tr("Annuler"));
    }
    QWidget::changeEvent(event);
}



void TileSelector::focusOutEvent(QFocusEvent *event) {
    hide();
    QWidget::focusOutEvent(event);
}
