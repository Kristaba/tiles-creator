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

#include "tileseteditor.h"
#include "undocommands.h"

TilesetEditor::TilesetEditor(ProjectWidget *projectWidget, QWidget *parent) : QWidget(parent)
{
    m_projectWidget = projectWidget;

    QHBoxLayout *layout = new QHBoxLayout;
    setLayout(layout);

    m_scene = new GraphicsTilesetScene;
    m_scene->setAllowedDragWidget(projectWidget);

    wm_view = new QGraphicsView(m_scene);
    wm_view->scale(2, 2);
    //wm_view->setFrameShape(QFrame::NoFrame);
    layout->addWidget(wm_view);

    oldTilesHeight = 16;
    oldTilesWidth = 16;
    tileSelectedId = 0;

    wm_selector = new TileSelector(m_projectWidget);

    //Layout des paramètres :
    QVBoxLayout *l_settings = new QVBoxLayout;
    layout->addLayout(l_settings);
        //GroupBox concernant les tiles :
        groupTiles = new QGroupBox(tr("Tiles"));
            QVBoxLayout *l_groupTiles = new QVBoxLayout;
            groupTiles->setLayout(l_groupTiles);
            //Layout de la largeur :
            QHBoxLayout *l_width = new QHBoxLayout;
                wm_tilesWidth = new QSpinBox;
                wm_tilesWidth->setKeyboardTracking(false);
                wm_tilesWidth->setMinimum(1);
                wm_tilesWidth->setMaximum(32);
                wm_tilesWidth->setValue(16);
                wm_tilesWidth->setSuffix(tr(" pixel"));
                labelWidth = new QLabel(tr("Largeur : "));
                l_width->addWidget(labelWidth);
                l_width->addWidget(wm_tilesWidth);
                l_width->addStretch();
                l_groupTiles->addLayout(l_width);
            //Layout de la hauteur :
            QHBoxLayout *l_height = new QHBoxLayout;
                wm_tilesHeight = new QSpinBox;
                wm_tilesHeight->setKeyboardTracking(false);
                wm_tilesHeight->setMinimum(1);
                wm_tilesHeight->setMaximum(32);
                wm_tilesHeight->setValue(16);
                wm_tilesHeight->setSuffix(tr(" pixel"));
                labelHeight = new QLabel(tr("Hauteur : "));
                l_height->addWidget(labelHeight);
                l_height->addWidget(wm_tilesHeight);
                l_height->addStretch();
                l_groupTiles->addLayout(l_height);
            //l_settings->addStretch();
            l_settings->addWidget(groupTiles);
            //l_settings->addStretch();
        //GroupBox concernant le tileset:
        groupTileset = new QGroupBox(tr("Tileset"));
            QVBoxLayout *l_groupTileset = new QVBoxLayout;
            groupTileset->setLayout(l_groupTileset);
            //Layout du nombre de tiles :
            QHBoxLayout *l_numberTiles = new QHBoxLayout;
                wm_numberOfTiles = new QSpinBox;
                wm_numberOfTiles->setKeyboardTracking(false);
                wm_numberOfTiles->setMinimum(0);
                wm_numberOfTiles->setMaximum(255);
                wm_numberOfTiles->setValue(0);
                labelNumber = new QLabel(tr("Nombre de tiles : "));
                l_numberTiles->addWidget(labelNumber);
                l_numberTiles->addWidget(wm_numberOfTiles);
                l_groupTileset->addLayout(l_numberTiles);
            //CheckBox de visualisation des propriétés :
            displayProperties = new QCheckBox(tr("Afficher les propriétés"));
            displayProperties->setChecked(true);
            l_groupTileset->addWidget(displayProperties);
            //Label du nom :
            labelName = new QLabel(tr("Nom du tileset :"));
            l_groupTileset->addWidget(labelName);
            //LineEdit du nom :
            name = new QLineEdit;
            l_groupTileset->addWidget(name);
        l_settings->addWidget(groupTileset);
        l_settings->addStretch();
    layout->addStretch();
            
    connect(wm_numberOfTiles, SIGNAL(valueChanged(int)), this, SLOT(numberOfTilesChanged(int)));
    connect(wm_tilesHeight, SIGNAL(valueChanged(int)), this, SLOT(setTilesHeight(int)));
    connect(wm_tilesWidth, SIGNAL(valueChanged(int)), this, SLOT(setTilesWidth(int)));
    connect(wm_selector, SIGNAL(tileChosen(QPersistentModelIndex)), this, SLOT(changeSelectedTile(QPersistentModelIndex)));
    connect(m_scene, SIGNAL(tileModified(GraphicsTileItem*)), this, SLOT(refreshTileId(GraphicsTileItem*)));
    connect(m_scene, SIGNAL(tileSelectorRequested(GraphicsTileItem*,QPoint)), this, SLOT(showTileSelector(GraphicsTileItem*,QPoint)));
    connect(displayProperties, SIGNAL(toggled(bool)), this, SLOT(setDisplayProperties(bool)));
    connect(name, SIGNAL(textEdited(QString)), this, SLOT(setTilesetName(QString)));
}


void TilesetEditor::addNewTile (int id) {
    int i = tiles.size();
    GraphicsTileItem *item = new GraphicsTileItem(oldTilesWidth, oldTilesHeight, tiles.size()+1);
    item->setTileId(id);
    m_scene->addItem(item);
    item->setPos((i%6)*oldTilesWidth, (i/6)*oldTilesHeight);
    item->setShowPolicy(displayProperties->isChecked());
    tiles.append(item);
    tilesId.append(id);
    refreshGeometry();
}



void TilesetEditor::refresh() {
    if (m_tilesetIndex != QPersistentModelIndex() && m_tilesetIndex.isValid()) {
        for (int i=0; i<tiles.size(); i++)
            delete tiles.at(i);
        tiles.clear();
        tilesId.clear();
        m_scene->clear();
        drawGrid();

        //Extraire la liste des tiles du tileset :
        QByteArray data = m_tilesetIndex.data(ProjectWidget::TilesetRole).toByteArray();
        QDataStream stream(&data, QIODevice::ReadOnly);
        QVector<int> tmpTilesId;
        stream >> tmpTilesId;
        QByteArray pData = m_tilesetIndex.data(ProjectWidget::TilesetPropertiesRole).toByteArray();
        QDataStream pStream(&pData, QIODevice::ReadOnly);
        QVector<unsigned char> tmpProperties;
        pStream >> tmpProperties;
        QPersistentModelIndex projectIndex = m_projectWidget->projectOfIndex(m_tilesetIndex);
        for (int i=0; i<tmpTilesId.size(); i++) {
            addNewTile(tmpTilesId.at(i));
            if ((tmpProperties.at(i) & 0x01) == 0x01) tiles.at(i)->setBlocking(true);
            else tiles.at(i)->setBlocking(false);
            //Si le tile est lié à  un tile du projet, on change l'image du tile pour celle du tile correspondant.
            if (tilesId.at(i) >= 0) {
                QPersistentModelIndex tileIndex = m_projectWidget->itemFromTileId(tilesId.at(i), projectIndex);
                if (tileIndex != QPersistentModelIndex()) {
                    QImage tmpImage(tileIndex.data(ProjectWidget::TileRole).value<QImage>());
                    if (tmpImage.size() == QSize(wm_tilesWidth->value(), wm_tilesHeight->value()))
                        tiles.at(i)->setPixmap(QPixmap::fromImage(tmpImage));
                    else {
                        tilesId[i] = -1;
                    }
                }
            }
        }
        //wm_view->setFixedHeight(((tiles.size()-1)/6+1)*oldTilesHeight*2+10);
        wm_view->setFixedWidth(oldTilesWidth*6*2+26);
        m_scene->setSceneRect(0, 0, oldTilesWidth*6, ((tiles.size()-1)/6+1)*oldTilesHeight);
    }
}


void TilesetEditor::refreshTileId (GraphicsTileItem *item) {
    int num = item->data(GraphicsTileItem::XIdRole).toInt();
    if (num <= tilesId.size()) {
        tilesId[num-1] = item->tileId();
    }
}



void TilesetEditor::setTilesetIndex(QPersistentModelIndex tilesetIndex) {
    m_tilesetIndex = tilesetIndex;
    m_scene->setProjectIndex(m_projectWidget->projectOfIndex(tilesetIndex));
    QByteArray data = m_tilesetIndex.data(ProjectWidget::TilesetRole).toByteArray();
    QDataStream stream(&data, QIODevice::ReadOnly);
    QVector<int> list;
    stream >> list;
    wm_numberOfTiles->blockSignals(true);
    wm_tilesHeight->blockSignals(true);
    wm_tilesWidth->blockSignals(true);
    wm_numberOfTiles->setValue(list.size());
    wm_tilesHeight->setValue(tilesetIndex.data(ProjectWidget::TilesHeightRole).toInt());
    wm_tilesWidth->setValue(tilesetIndex.data(ProjectWidget::TilesWidthRole).toInt());
    name->setText(tilesetIndex.data(Qt::DisplayRole).toString());
    wm_numberOfTiles->blockSignals(false);
    wm_tilesHeight->blockSignals(false);
    wm_tilesWidth->blockSignals(false);
    oldTilesWidth = wm_tilesWidth->value();
    oldTilesHeight = wm_tilesHeight->value();
    refresh();
}


void TilesetEditor::numberOfTilesChanged(int newNumber) {
    int dif = newNumber - tiles.size();
    if (dif < 0) {
        for (int i=0; i<(-dif); i++)
            delete tiles.at(tiles.size()-i-1);
        tiles.resize(newNumber);
        tilesId.resize(newNumber);
        refreshGeometry();
    }
    else if (dif > 0) {
        QPersistentModelIndex projectIndex = m_projectWidget->projectOfIndex(m_tilesetIndex);
        for (int i=0; i<dif; i++) {
            addNewTile ();
        }
    }
}



void TilesetEditor::saveTileset() {
    QVector<unsigned char> properties;
    for (int i=0; i<tiles.size(); i++) {
        unsigned char ret = 0x00;
        if (tiles.at(i)->blocking()) ret = 0x01;
        properties.append(ret);
    }
    m_projectWidget->modifyTileset(m_tilesetIndex, name->text(), tilesId, properties, wm_tilesWidth->value(), wm_tilesHeight->value());
}


void TilesetEditor::setTilesWidth(int w) {
    if (askForChangeTilesSize()) {
        if (w != wm_tilesWidth->value()) wm_tilesWidth->setValue(w);
        else {
            oldTilesWidth = wm_tilesWidth->value();
            refresh();
        }
    }
}


void TilesetEditor::setTilesHeight(int h) {
    if (askForChangeTilesSize()) {
        if (h != wm_tilesHeight->value()) wm_tilesHeight->setValue(h);
        else {
            oldTilesHeight = wm_tilesHeight->value();
            refresh();
        }
    }
}


void TilesetEditor::refreshGeometry() {    
    //wm_view->setFixedHeight((tiles.size()/6+1)*oldTilesHeight*2+10);
    wm_view->setFixedWidth(oldTilesWidth*6*2+26);

    for (int i=0; i<tiles.size(); i++)
        m_scene->removeItem(tiles.at(i));

    m_scene->clear();
    drawGrid();

    for (int i=0; i<tiles.size(); i++) {
        m_scene->addItem(tiles.at(i));
        tiles.at(i)->setPos((i%6)*oldTilesWidth, (i/6)*oldTilesHeight);
    }

    //wm_view->setFixedHeight(((tiles.size()-1)/6+1)*oldTilesHeight*2+10);
    wm_view->setFixedWidth(oldTilesWidth*6*2+26);
    m_scene->setSceneRect(0, 0, oldTilesWidth*6, ((tiles.size()-1)/6+1)*oldTilesHeight);
}


bool TilesetEditor::askForChangeTilesSize() {
    QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("Changer la taille des tiles?"),
        tr("Voulez-vous vraiment changer la taille des tiles de ce tileset?\nEn faisait cela le tileset sera efface."),
        QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
    if (ret == QMessageBox::Ok) {
        for (int i=0; i<tiles.size(); i++) {
            tiles.at(i)->clearTile();
        }
        return true;
    }
    else {
        wm_tilesHeight->blockSignals(true);
        wm_tilesWidth->blockSignals(true);
        wm_tilesHeight->setValue(oldTilesHeight);
        wm_tilesWidth->setValue(oldTilesWidth);
        wm_tilesHeight->blockSignals(false);
        wm_tilesWidth->blockSignals(false);
        return false;
    }
}


void TilesetEditor::tileModified(const QPersistentModelIndex &mTile) {
    if (m_projectWidget->projectOfIndex(mTile) == m_projectWidget->projectOfIndex(m_tilesetIndex)) {
        int mId = mTile.data(ProjectWidget::IdRole).toInt();
        int i=0;
        while (i<tilesId.size() && tilesId.at(i) != mId) i++;
        if (i<tilesId.size() && tilesId.at(i) == mId) {
            QImage tmpImage(mTile.data(ProjectWidget::TileRole).value<QImage>());
            if (tmpImage.size() == QSize(wm_tilesWidth->value(), wm_tilesHeight->value()))
                tiles.at(i)->setPixmap(QPixmap::fromImage(tmpImage));
            else {
                tilesId[i] = -1;
                tiles.at(i)->clearTile();
            }
        }
    }
}


void TilesetEditor::tileDeleted(QModelIndex dTile) {
    if (m_projectWidget->projectOfIndex(dTile) == m_projectWidget->projectOfIndex(m_tilesetIndex)) {
        int mId = dTile.data(ProjectWidget::IdRole).toInt();
        int i=0;
        while (i<tilesId.size() && tilesId.at(i) != mId) i++;
        if (i<tilesId.size() && tilesId.at(i) == mId) {
            tilesId[i] = -1;
            tiles.at(i)->clearTile();
        }
    }
}


void TilesetEditor::showTileSelector (GraphicsTileItem *tile, QPoint pos) {
    QRect screen = qApp->desktop()->availableGeometry(this);
    tileSelectedId = tile->data(GraphicsTileItem::XIdRole).toInt() - 1;
    wm_selector->exec(m_projectWidget->projectOfIndex(m_tilesetIndex), oldTilesWidth, oldTilesHeight);
    if (pos.y() < screen.height()/2) wm_selector->move(pos);
    else wm_selector->move(QPoint(pos.x(), pos.y()-wm_selector->height()));

}


void TilesetEditor::changeSelectedTile (QPersistentModelIndex newTile) {
    if (tileSelectedId >= 0 && tileSelectedId < tiles.size()) {
        if (newTile.isValid() && m_projectWidget->typeOfIndex(newTile) == ProjectWidget::Tile) {
            QImage tmpImage(newTile.data(ProjectWidget::TileRole).value<QImage>());
            tiles[tileSelectedId]->setPixmap(QPixmap::fromImage(tmpImage));
            tilesId[tileSelectedId] = newTile.data(ProjectWidget::IdRole).toInt();
        }
    }
}


void TilesetEditor::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        groupTiles->setTitle(tr("Tiles"));
        labelWidth->setText(tr("Largeur : "));
        labelHeight->setText(tr("Hauteur : "));
        groupTileset->setTitle(tr("Tileset"));
        labelNumber->setText(tr("Nombre de tiles : "));
        labelName->setText(tr("Nom du tileset :"));
        displayProperties->setText(tr("Afficher les propriétés"));
        wm_tilesHeight->setSuffix(tr(" pixel"));
        wm_tilesWidth->setSuffix(tr(" pixel"));
    }
    QWidget::changeEvent(event);
}


void TilesetEditor::drawGrid() {
    int vert = 7;
    if (tiles.size() < 6) vert = tiles.size()+1;
    if (tiles.size() != 0) {
        for (int i=0; i<vert; i++) {
            int h = (tiles.size()-i)/6 + 1;
            if (i==0 && tiles.size()%6 == 0) h--;
            QGraphicsItem *item = m_scene->addLine(i*oldTilesWidth, 0, i*oldTilesWidth, h*oldTilesHeight, QPen(QColor(0xFF888888)));
            item->setZValue(9);
        }
        int h = (tiles.size()-1)/6 + 1;
        for (int i=0; i<h+1; i++) {
            int w=6;
            if ((i == h || tiles.size() < 6) && tiles.size()%6 != 0) w = tiles.size()%6;
            QGraphicsItem *item = m_scene->addLine(0, i*oldTilesHeight, w*oldTilesWidth, i*oldTilesHeight, QPen(QColor(0xFF888888)));
            item->setZValue(9);
        }
    }
}


void TilesetEditor::setDisplayProperties(bool display) {
    for (int i=0; i<tiles.size(); i++) {
        tiles.at(i)->setShowPolicy(display);
    }
    refreshGeometry();
}



void TilesetEditor::setTilesetName(QString newName) {
    QUndoCommand *cmd = new SetItemNameCommand(newName, m_tilesetIndex, m_projectWidget);
    //if (name->setText(newName);
    m_projectWidget->undoGroup()->activeStack()->push(cmd);
}
