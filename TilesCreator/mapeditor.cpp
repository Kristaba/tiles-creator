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

#include "mapeditor.h"
#include "undocommands.h"

MapEditor::MapEditor(ProjectWidget *projectWidget, QWidget *parent) : QWidget(parent)
{
    m_projectWidget = projectWidget;
    m_currentTileBrushId = -1;
    
    tilesHeight = 16;
    tilesWidth = 16;

    mapScene = new GraphicsTileScene(0, 0, 20*tilesWidth, 20*tilesHeight, this);

    //Créer une scene pour le tileset et la cadrier :
    tilesetScene = new QGraphicsScene(0, 0, 6*tilesWidth+4, 44*tilesHeight+4, this);
    for (int i=0; i<7; i++) {
        QGraphicsItem *item = tilesetScene->addLine(i*tilesWidth+2, 2, i*tilesWidth+2, 44*tilesHeight+2, QPen(QColor(0xFF888888)));
        item->setZValue(9);
    }
    for (int i=0; i<45; i++) {
        QGraphicsItem *item = tilesetScene->addLine(2, i*tilesHeight+2, 6*tilesWidth+2, i*tilesHeight+2, QPen(QColor(0xFF888888)));
        item->setZValue(9);
    }

    QHBoxLayout *layout = new QHBoxLayout;
    setLayout(layout);

    QVBoxLayout *l_tilesetLayout = new QVBoxLayout;
        wm_tilesetSelector = new QComboBox;
            wm_tilesetSelector->setEnabled(false);
            l_tilesetLayout->addWidget(wm_tilesetSelector);
        tilesetView =new QGraphicsView(tilesetScene);
            tilesetView->scale(2, 2);
            tilesetView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            tilesetView->setFixedWidth(tilesetView->sizeHint().width()+25);
            //tilesetView->setViewport(new QGLWidget);
            l_tilesetLayout->addWidget(tilesetView);
            tilesetView->verticalScrollBar()->setValue(0);
        wm_groupSettings = new QGroupBox(tr("Parametres"));
            QVBoxLayout *l_wm_groupSettings = new QVBoxLayout;
            wm_groupSettings->setLayout(l_wm_groupSettings);
            wm_name = new QLineEdit;
                l_wm_groupSettings->addWidget(wm_name);
            QHBoxLayout *l_height = new QHBoxLayout;
                wm_height = new QSpinBox;
                wm_height->setKeyboardTracking(false);
                wm_height->setMinimum(2);
                wm_height->setMaximum(200);
                wm_labelHeight = new QLabel(tr("Hauteur map : "));
                l_height->addWidget(wm_labelHeight);
                l_height->addWidget(wm_height);
                l_wm_groupSettings->addLayout(l_height);
            QHBoxLayout *l_width = new QHBoxLayout;
                wm_width = new QSpinBox;
                wm_width->setKeyboardTracking(false);
                wm_width->setMinimum(2);
                wm_width->setMaximum(200);
                wm_labelWidth = new QLabel(tr("Largeur map : "));
                l_width->addWidget(wm_labelWidth);
                l_width->addWidget(wm_width);
                l_wm_groupSettings->addLayout(l_width);
            l_tilesetLayout->addWidget(wm_groupSettings);
        wm_groupSettings->setMaximumWidth(tilesetView->maximumWidth());
        wm_tilesetSelector->setMaximumWidth(tilesetView->maximumWidth());
        layout->addLayout(l_tilesetLayout);
    mapView =new QGraphicsView(mapScene);
        mapView->scale(2, 2);
        //mapView->setViewport(new QGLWidget);
        mapView->setMouseTracking(false);
        layout->addWidget(mapView);

    resizeMap(QSize(10, 10));

    connect(wm_tilesetSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(setTileset(int)));
    connect(mapScene, SIGNAL(selectionChanged()), this, SLOT(changePixmap()));
    connect(tilesetScene, SIGNAL(selectionChanged()), this, SLOT(changeTileBrush()));
    connect(wm_width, SIGNAL(valueChanged(int)), this, SLOT(widthChanged(int)));
    connect(wm_height, SIGNAL(valueChanged(int)), this, SLOT(heightChanged(int)));
    connect(wm_name, SIGNAL(textEdited(QString)), this, SLOT(setMapName(QString)));
}


void MapEditor::setMapIndex(const QPersistentModelIndex &index) {
    if (index.isValid()) {
        m_mapIndex = index;
        refreshTilesetList();

        //Récupérer la largeur et la longueur des tiles :
        int w, h;
        w = index.data(ProjectWidget::MapWidthRole).toInt();
        h = index.data(ProjectWidget::MapHeightRole).toInt();

        //Récupérer le nom de la map et les ID des tiles :
        wm_name->setText(index.data(Qt::DisplayRole).toString());
        QVector<unsigned char> mapData;
        QByteArray data(index.data(ProjectWidget::MapRole).toByteArray());
        QDataStream stream(&data, QIODevice::ReadOnly);
        stream >> mapData;

        QImage emptyImage(tilesWidth, tilesHeight, QImage::Format_RGB32);
        emptyImage.fill(0xFFFFFFFF);

        //Actualiser tous les tiles de la map avec les nouvelles informations :
        for (int i=0; i<m_map.size(); i++)
            for (int j=0; j<m_map.at(i).size(); j++) delete m_map.at(i).at(j);
        m_map.clear();
        m_map.resize(h);
        for (int i=0; i<h; i++) {
            m_map[i].resize(w);
            for (int j=0; j<w; j++) {
                GraphicsTileItem *tmpTile = m_map[i][j] = new GraphicsTileItem(tilesWidth, tilesHeight, j, i);
                tmpTile->setTileId(mapData.at(i*w+j));
                tmpTile->setZValue(1);
            }
        }
        resizeMap(QSize(w, h));
    }
}


void MapEditor::refreshTilesetList() {
    //Récupérer la liste des tilesets du projet et les afficher dans tilesetSelector :
    m_tilesetIndexList = m_projectWidget->tilesetsOfProject(m_projectWidget->projectOfIndex(m_mapIndex));
    wm_tilesetSelector->clear();
    for (int i=0; i<m_tilesetIndexList.size(); i++) {
        wm_tilesetSelector->addItem(m_tilesetIndexList.at(i).data(Qt::DisplayRole).toString());
    }
    wm_tilesetSelector->setEnabled(true);

    //Utiliser le tileset lié à  la map si celui-ci existe :
    int tilesetId = m_mapIndex.data(ProjectWidget::TilesetIdRole).toInt();
    int compt = 0;
    while (compt < m_tilesetIndexList.size() && m_tilesetIndexList.at(compt).data(ProjectWidget::IdRole).toInt() != tilesetId) compt++;
    if (compt < m_tilesetIndexList.size()) wm_tilesetSelector->setCurrentIndex(compt);
}


void MapEditor::setTileset(int number) {
    if (number < wm_tilesetSelector->count() && number >= 0) {
        QByteArray data = m_tilesetIndexList.at(number).data(ProjectWidget::TilesetRole).toByteArray();
        m_tilesetIndex = m_tilesetIndexList.at(number);
        QDataStream stream(&data, QIODevice::ReadOnly);
        QVector<int> tiles;
        stream >> tiles;
        int tw, th;
        tw = m_tilesetIndexList.at(number).data(ProjectWidget::TilesWidthRole).toInt();
        th = m_tilesetIndexList.at(number).data(ProjectWidget::TilesHeightRole).toInt();
        bool sizeChanged = false;

        for (int i=0; i<m_tilesetItems.size(); i++)
            if (m_tilesetItems.at(i) != 0) delete m_tilesetItems.at(i);
        m_tilesetItems.clear();

        //Si les tiles ne font plus la meme taille, redessiner la grille
        if (tw != tilesWidth || th != tilesHeight) {
            tilesWidth = tw;
            tilesHeight = th;
            tilesetScene->clear();
            for (int i=0; i<7; i++) {
                QGraphicsItem *item = tilesetScene->addLine(i*tilesWidth+2, 2, i*tilesWidth+2, 44*tilesHeight+2, QPen(QColor(0xFF888888)));
                item->setZValue(9);
            }
            for (int i=0; i<45; i++) {
                QGraphicsItem *item = tilesetScene->addLine(2, i*tilesHeight+2, 6*tilesWidth+2, i*tilesHeight+2, QPen(QColor(0xFF888888)));
                item->setZValue(9);
            }
            sizeChanged = true;
        }

        for (int i=0; i<tiles.size(); i++) {
            QPersistentModelIndex tileIndex = m_projectWidget->itemFromTileId(tiles.at(i), m_projectWidget->projectOfIndex(m_mapIndex));
            if (tileIndex != QPersistentModelIndex()) {
                GraphicsTileItem *item = new GraphicsTileItem(tilesWidth, tilesHeight);
                item->setZValue(1);
                item->setPixmap(QPixmap::fromImage(tileIndex.data(ProjectWidget::TileRole).value<QImage>().scaled(tilesWidth, tilesHeight)));
                item->setTileId(i);
                tilesetScene->addItem(item);
                item->setPos((i%6)*tilesWidth+2, (i/6)*tilesHeight+2);
                m_tilesetItems.append(item);
            }
            else m_tilesetItems.append(0);
        }

        if (sizeChanged) refreshAll();
        else {
            QImage tmpImage(tilesWidth, tilesHeight, QImage::Format_Indexed8);
            tmpImage.setColor(0, 0xFFFFFFFF);
            tmpImage.fill(0);

            for (int i=0; i<m_map.size(); i++) {
                for (int j=0; j<m_map.at(i).size(); j++) {
                    int num = m_map.at(i).at(j)->tileId();
                    if (num >= 0 && num < m_tilesetItems.size()) {
                        if (m_tilesetItems.at(num) != 0) {
                            m_map.at(i).at(j)->setPixmap(m_tilesetItems.at(num)->pixmap());
                        }
                        else m_map.at(i).at(j)->setPixmap(QPixmap::fromImage(tmpImage));
                    }
                    else m_map.at(i).at(j)->setPixmap(QPixmap::fromImage(tmpImage));
                }
            }
        }
    }
}


void MapEditor::changePixmap() {
    QList<QGraphicsItem*> changed = mapScene->selectedItems();
    if (!changed.isEmpty()) {
        QList<QPoint> points;
        for (int i=0; i<changed.size(); i++) {
            GraphicsTileItem *item = qgraphicsitem_cast<GraphicsTileItem*> (changed.at(i));
            if (item != 0) {
                if (!m_brush.isNull() && item->tileId() != m_currentTileBrushId) {
                    /*item->setPixmap(QPixmap::fromImage(m_brush));
                    item->setTileId(m_currentTileBrushId);*/
                    points.append(QPoint(item->data(GraphicsTileItem::XIdRole).toInt(),
                                         item->data(GraphicsTileItem::YIdRole).toInt()));
                }
            }
        }
        if(!points.isEmpty()) {
            m_projectWidget->undoGroup()->activeStack()->push(
                    new SetTilesOnMapCommand(points, m_currentTileBrushId, m_mapIndex, this));
        }
    }
}


void MapEditor::changeTileBrush() {
    QList<QGraphicsItem*> changed = tilesetScene->selectedItems();
    if (!changed.isEmpty()) {
        GraphicsTileItem *item = qgraphicsitem_cast<GraphicsTileItem*> (changed.at(0));
        if (item != 0) {
            m_brush = item->pixmap().toImage();
            m_currentTileBrushId = item->tileId();
        }
    }
    else {
        m_brush = QImage();
        m_currentTileBrushId = -1;
    }
}


void MapEditor::saveMap() {
    if (wm_tilesetSelector->currentIndex() >= 0 && m_tilesetIndexList.size() > wm_tilesetSelector->currentIndex()) {
        int tilesetId = m_tilesetIndexList.at(wm_tilesetSelector->currentIndex()).data(ProjectWidget::IdRole).toInt();

        //Recopier les ID de tous les tiles de la map dans un vector de int :
        QVector<unsigned char> mapData = mapArray();

        //Enregistrer les modifications...
        m_projectWidget->modifyMap(m_mapIndex, wm_name->text(), mapData, oldWidth, oldHeight, tilesetId);
    }
}


void MapEditor::refreshAll() {
    QImage emptyImage(tilesWidth, tilesHeight, QImage::Format_RGB32);
    emptyImage.fill(0xFFFFFFFF);

    QGraphicsScene *tmpScene = mapScene;
    mapScene = new GraphicsTileScene(0, 0, oldWidth*tilesWidth, oldHeight*tilesHeight, this);
    mapView->setScene(mapScene);
    mapView->setSceneRect(0, 0, oldWidth*tilesWidth, oldHeight*tilesHeight);
    //mapView->centerOn(0, 0);

    //Dessiner la grille horizontale et verticale :
    for (int i=0; i <= oldWidth; i++) {
        QGraphicsItem *item = mapScene->addLine(i*tilesWidth, 0, i*tilesWidth, oldHeight*tilesHeight, QPen(QColor(0xFF888888)));
        item->setZValue(5);
    }
    for (int i=0; i <= oldHeight; i++) {
        QGraphicsItem *item = mapScene->addLine(0, i*tilesHeight, oldWidth*tilesWidth, i*tilesHeight, QPen(QColor(0xFF888888)));
        item->setZValue(6);
    }

    //Redimensioner la matrice d'items en cas de changement de taille
    int difW, difH;
    difH = oldHeight - m_map.size();
    if (m_map.isEmpty()) difW = oldWidth;
    else difW = oldWidth - m_map.at(0).size();

    if (difH < 0) {
        for (int i=0; i>difH; i--) {
            for (int j=0; j<m_map.at(m_map.size()+i-1).size(); j++)
                delete m_map.at(m_map.size()+i-1).at(j);
        }
        m_map.resize(oldHeight);
    }

    if (difW < 0) {
        for (int i=0; i<m_map.size(); i++) {
            for (int j=0; j>difW; j--) delete m_map.at(i).at(m_map.at(i).size()+j-1);
            m_map[i].resize(oldWidth);
        }
    }

    if (difW > 0) {
        for (int i=0; i<m_map.size(); i++) {
            for (int j=0; j<difW; j++) {
                GraphicsTileItem *tile = new GraphicsTileItem(tilesWidth, tilesHeight, j, m_map.size()-1);
                tile->setTileId(0);
                tile->setZValue(1);
                m_map[i].append(tile);
            }
        }
    }

    if (difH > 0) {
        for (int i=0; i<difH; i++) {
            m_map.append(QVector<GraphicsTileItem*>(oldWidth));
            for (int j=0; j<oldWidth; j++) {
                GraphicsTileItem *tile = new GraphicsTileItem(tilesWidth, tilesHeight, j, m_map.size()-1);
                tile->setTileId(0);
                tile->setZValue(1);
                m_map[m_map.size()-1][j] = tile;
            }
        }
    }
    //Actualiser et afficher chaque tile :
    for (int i=0; i<oldHeight; i++) {
        for (int j=0; j<oldWidth; j++) {
            int tmpId = m_map.at(i).at(j)->tileId();
            if (tmpId >= 0 && tmpId < m_tilesetItems.size() && m_tilesetItems.at(tmpId) != 0)
                m_map[i][j]->setPixmap(m_tilesetItems.at(tmpId)->pixmap());
            else m_map[i][j]->setPixmap(QPixmap::fromImage(emptyImage));
            mapScene->addItem(m_map[i][j]);
            m_map[i][j]->setPos(j*tilesWidth, i*tilesHeight);
        }
    }
    connect(mapScene, SIGNAL(selectionChanged()), this, SLOT(changePixmap()));
    delete tmpScene;
}


void MapEditor::setTile(QPoint pos, int newValue) {
    if (pos.x()<wm_width->value() && pos.y()<wm_height->value()) {
        int i=pos.y();
        int j=pos.x();
        m_map.at(i).at(j)->setTileId(newValue);
        if (newValue >= 0 && newValue < m_tilesetItems.size() && m_tilesetItems.at(newValue) != 0)
            m_map[i][j]->setPixmap(m_tilesetItems.at(newValue)->pixmap());
        else {
            QImage emptyImage(tilesWidth, tilesHeight, QImage::Format_RGB32);
            emptyImage.fill(0xFFFFFFFF);
            m_map[i][j]->setPixmap(QPixmap::fromImage(emptyImage));
        }
    }
}


int MapEditor::tile(QPoint pos) {
    if (pos.x()<wm_width->value() && pos.y()<wm_height->value())
        return m_map[pos.y()][pos.x()]->tileId();
    else return -1;
}

/*
void MapEditor::setWidth(int w) {
    if (wm_width->value() != w) wm_width->setValue(w);
    oldWidth = w;
    refreshAll();
}

void MapEditor::setHeight(int h) {
    if (wm_height->value() != h) wm_height->setValue(h);
    oldHeight = h;
    refreshAll();
}*/

void MapEditor::resizeMap(QSize newSize) {
    wm_width->blockSignals(true);
    wm_height->blockSignals(true);
    wm_width->setValue(newSize.width());
    wm_height->setValue(newSize.height());
    wm_width->blockSignals(false);
    wm_height->blockSignals(false);
    oldHeight = newSize.height();
    oldWidth = newSize.width();
    refreshAll();
}


void MapEditor::tilesetModified (const QPersistentModelIndex &index) {
    if (index.isValid() && ProjectWidget::typeOfIndex(index) == ProjectWidget::Tileset) {
        if (m_tilesetIndex == index) {
            refreshTileset();
            refreshAll();
        }
    }
}


void MapEditor::tilesetDeleted (QModelIndex index) {
    if (m_projectWidget->projectOfIndex(index) == m_projectWidget->projectOfIndex(m_mapIndex)) {
        refreshTilesetList();
        m_tilesetIndexList.remove(index.row());
        wm_tilesetSelector->removeItem(index.row());
        if (wm_tilesetSelector->count() == 0) {
            for (int i=0; i<m_tilesetItems.size(); i++)
                if (m_tilesetItems.at(i) != 0) delete m_tilesetItems.at(i);
            m_tilesetItems.clear();
        }
    }
}


void MapEditor::tilesetAdded (const QPersistentModelIndex &index) {
    refreshTilesetList();
}


void MapEditor::refreshTileset() {
    if (m_tilesetIndex.isValid()) {
        QByteArray data = m_tilesetIndex.data(ProjectWidget::TilesetRole).toByteArray();
        QDataStream stream(&data, QIODevice::ReadOnly);
        QVector<int> tiles;
        stream >> tiles;

        for (int i=0; i<m_tilesetItems.size(); i++)
            if (m_tilesetItems.at(i) != 0) delete m_tilesetItems.at(i);
        m_tilesetItems.clear();

        for (int i=0; i<tiles.size(); i++) {
            QPersistentModelIndex tileIndex = m_projectWidget->itemFromTileId(tiles.at(i), m_projectWidget->projectOfIndex(m_mapIndex));
            if (tileIndex != QPersistentModelIndex()) {
                GraphicsTileItem *item = new GraphicsTileItem(tilesWidth, tilesHeight);
                item->setZValue(1);
                item->setPixmap(QPixmap::fromImage(tileIndex.data(ProjectWidget::TileRole).value<QImage>().scaled(tilesWidth, tilesHeight)));
                item->setTileId(i);
                tilesetScene->addItem(item);
                item->setPos((i%6)*tilesWidth+2, (i/6)*tilesHeight+2);
                m_tilesetItems.append(item);
            }
            else m_tilesetItems.append(0);
        }
    }
}


void MapEditor::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        wm_groupSettings->setTitle(tr("Parametres"));
        wm_labelHeight->setText(tr("Hauteur map : "));
        wm_labelWidth->setText(tr("Largeur map : "));
    }
    QWidget::changeEvent(event);
}


QVector<unsigned char> MapEditor::mapArray() {
    QVector<unsigned char> ret;
    for (int i=0; i<m_map.size(); i++) {
        for (int j=0; j<m_map.at(1).size(); j++) {
            int tmp = m_map.at(i).at(j)->tileId();
            if (tmp >= 0x00 || tmp <= 0xFF) ret.append((unsigned char)tmp);
            else ret.append(0x00);
        }
    }
    return ret;
}



Array2D<unsigned char> MapEditor::partOfMap(QRect area) {
    Array2D<unsigned char> ret(0,0);

    if (area == QRect()) area = QRect(0, 0, wm_width->value(), wm_height->value());

    if (area.x()>=0 && area.y()>=0 && area.width()>0 && area.height()>0 &&
        area.width()+area.x()<=wm_width->value() && area.height()+area.y()<=wm_height->value())
        {
        ret.resize(area.height(), area.width());
        for (int i=0; i<area.height(); i++) {
            for (int j=0; j<area.width(); j++)
                ret(i, j) = m_map[i+area.y()][j+area.x()]->tileId();
        }
    }

    return ret;
}


void MapEditor::setPartOfMap(Array2D<unsigned char> map, QPoint topleft) {
    if (topleft.x()>=0 && topleft.y()>=0 && map.columnCount()>0 && map.rowCount()>0 &&
        map.columnCount()+topleft.x()<=wm_width->value() && map.rowCount()+topleft.y()<=wm_height->value())
        {
        for (int i=0; i<map.rowCount(); i++)
            for (int j=0; j<map.columnCount(); j++)
                setTile(topleft+QPoint(j,i), map(i, j));
    }
}


void MapEditor::setItemName(QString name) {
    wm_name->setText(name);
}


QString MapEditor::itemName() {
    return wm_name->text();
}


QSize MapEditor::mapSize() {
    return QSize(wm_width->value(), wm_height->value());
}


int MapEditor::tilesetId() {
    if (m_tilesetIndex.isValid())
        return m_tilesetIndex.data(ProjectWidget::IdRole).toInt();
    else return -1;
}


void MapEditor::setTilesetId(int id) {
    for (int i=0; i<m_tilesetIndexList.size(); i++) {
        if (m_tilesetIndexList.at(i).data(ProjectWidget::IdRole).toInt() == id) {
            setTileset(i);
            return;
        }
    }
}


const QPersistentModelIndex& MapEditor::itemIndex() {
    return m_mapIndex;
}


ProjectWidget *MapEditor::projectWidget() {
    return m_projectWidget;
}


void MapEditor::widthChanged(int w) {
    m_projectWidget->undoGroup()->activeStack()->push(
            new SetMapSizeCommand(QSize(w, oldHeight), m_mapIndex, this));
    oldWidth = w;
}


void MapEditor::heightChanged(int h) {
    m_projectWidget->undoGroup()->activeStack()->push(
            new SetMapSizeCommand(QSize(oldWidth, h), m_mapIndex, this));
    oldHeight = h;
}


void MapEditor::setMapName(QString newName) {
    QUndoCommand *cmd = new SetItemNameCommand(newName, m_mapIndex, m_projectWidget);
    //if (name->setText(newName);
    m_projectWidget->undoGroup()->activeStack()->push(cmd);
}
