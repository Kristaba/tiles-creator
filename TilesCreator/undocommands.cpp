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


#include "undocommands.h"

static const int tilePixmapModifiedCommandId = 1;
static const int setItemNameCommandCommandId = 2;
static const int setMapTilesetCommandId      = 3;




TilePixmapModifiedCommand::TilePixmapModifiedCommand
        (QPoint point, TC::Color newColor, const QPersistentModelIndex &index, ProjectWidget *pw)
            : QUndoCommand() {

    init(QList<QPoint>() << point, newColor, index, pw);
}


TilePixmapModifiedCommand::TilePixmapModifiedCommand
        (QList<QPoint> points, TC::Color newColor, const QPersistentModelIndex &index, ProjectWidget *pw)
            : QUndoCommand() {

    init(points, newColor, index, pw);
}


void TilePixmapModifiedCommand::init
        (QList<QPoint> points, TC::Color newColor, const QPersistentModelIndex &index, ProjectWidget *pw) {
    m_points.append(points);
    m_newColor = newColor;
    m_tileId = index.data(ProjectWidget::IdRole).toInt();
    m_projectWidget = pw;
    setText(QObject::tr("Modifier le tile \"%1\"").arg(index.data(Qt::DisplayRole).toString()));
}


void TilePixmapModifiedCommand::undo() {
    QPersistentModelIndex index;
    index = m_projectWidget->itemFromTileId(m_tileId, m_projectWidget->currentProject());

    QImage image = index.data(ProjectWidget::TileRole).value<QImage>();
    for (int i=0; i<m_points.size(); i++)
        image.setPixel(m_points.at(i), m_oldColors.at(i));

    m_projectWidget->modifyTile(index, index.data(Qt::DisplayRole).toString(), image);
}


void TilePixmapModifiedCommand::redo() {
    QPersistentModelIndex index;
    index = m_projectWidget->itemFromTileId(m_tileId, m_projectWidget->currentProject());

    m_oldColors.clear();
    QImage image = index.data(ProjectWidget::TileRole).value<QImage>();
    for (int i=0; i<m_points.size(); i++) {
        m_oldColors.append(static_cast<TC::Color>(image.pixelIndex(m_points.at(i))));
        image.setPixel(m_points.at(i), m_newColor);
    }

    m_projectWidget->modifyData(index, QVariant().fromValue(image), ProjectWidget::TileRole);
}


bool TilePixmapModifiedCommand::mergeWith(const QUndoCommand *other) {
    if (other->id() != id()) return false;
    const TilePixmapModifiedCommand *cmd = static_cast<const TilePixmapModifiedCommand*>(other);
    if (m_newColor == cmd->m_newColor && m_tileId == cmd->m_tileId
        && m_projectWidget == cmd->m_projectWidget) {

        m_points.append(cmd->m_points);
        m_oldColors.append(cmd->m_oldColors);
        return true;
    }
    return false;
}


int TilePixmapModifiedCommand::id()  const {
    //return tilePixmapModifiedCommandId;
    return -1; //Le fusionnage de cette commande est plutot contre-productif pour le moment...
}







SetItemNameCommand::SetItemNameCommand
    (QString newName, const QPersistentModelIndex &index, ProjectWidget *pw) : QUndoCommand() {

    m_projectWidget = pw;
    m_itemType = pw->typeOfIndex(index);
    m_itemId = index.data(ProjectWidget::IdRole).toInt();
    m_newName = newName;
    m_oldName = index.data(Qt::DisplayRole).toString();
    setRedoText();
}


void SetItemNameCommand::setRedoText() {
    QString typeName;
    switch (m_itemType) {
    case ProjectWidget::Tile:
        typeName = QObject::tr("le Tile");
        break;
    case ProjectWidget::Tileset:
        typeName = QObject::tr("le Tileset");
        break;
    case ProjectWidget::Map:
        typeName = QObject::tr("la Map");
        break;
    default:
        typeName = QObject::tr("(Type inconnu)");
    }

    setText(QObject::tr("Rennommer %1 \"%2\" en \"%3\"").arg(typeName, m_oldName, m_newName));
}


void SetItemNameCommand::undo() {
    QPersistentModelIndex index;
    switch (m_itemType) {
    case ProjectWidget::Tile:
        index = m_projectWidget->itemFromTileId(m_itemId, m_projectWidget->currentProject());
        break;
    case ProjectWidget::Tileset:
        index = m_projectWidget->itemFromTilesetId(m_itemId, m_projectWidget->currentProject());
        break;
    case ProjectWidget::Map:
        index = m_projectWidget->itemFromMapId(m_itemId, m_projectWidget->currentProject());
        break;
    default:
        break;
    }

    if(index.isValid())
        m_projectWidget->modifyData(index, QVariant(m_oldName), Qt::DisplayRole);
}


void SetItemNameCommand::redo() {
    QPersistentModelIndex index;
    switch (m_itemType) {
    case ProjectWidget::Tile:
        index = m_projectWidget->itemFromTileId(m_itemId, m_projectWidget->currentProject());
        break;
    case ProjectWidget::Tileset:
        index = m_projectWidget->itemFromTilesetId(m_itemId, m_projectWidget->currentProject());
        break;
    case ProjectWidget::Map:
        index = m_projectWidget->itemFromMapId(m_itemId, m_projectWidget->currentProject());
        break;
    default:
        break;
    }

    if(index.isValid())
        m_projectWidget->modifyData(index, QVariant(m_newName), Qt::DisplayRole);
}


bool SetItemNameCommand::mergeWith(const QUndoCommand *other) {
    if (other->id() != id()) return false;
    const SetItemNameCommand *cmd = static_cast<const SetItemNameCommand *>(other);
    if (m_itemType == cmd->m_itemType && m_itemId == cmd->m_itemId &&
        m_projectWidget == cmd->m_projectWidget)
        {
        m_newName = cmd->m_newName;
        setRedoText();
        return true;
    }
    return false;
}


int SetItemNameCommand::id() const {
    return setItemNameCommandCommandId;
}





SetTilesOnMapCommand::SetTilesOnMapCommand
    (QList<QPoint> points, unsigned char newTile, const QPersistentModelIndex &index, MapEditor *mapEditor)
        : QUndoCommand() {
    m_points = points;
    m_newTile = newTile;
    m_projectWidget = mapEditor->projectWidget();
    m_mapEditor = mapEditor;
    m_mapId = index.data(ProjectWidget::IdRole).toInt();
    QString name = index.data(Qt::DisplayRole).toString();
    setText(QObject::tr("Modifier la map \"%1\"").arg(name));
}


void SetTilesOnMapCommand::redo() {
    QPersistentModelIndex index = m_projectWidget->itemFromMapId(m_mapId, m_projectWidget->currentProject());
    m_oldTiles.clear();
    if (index.isValid()) {
        QByteArray data = index.data(ProjectWidget::MapRole).toByteArray();
        int width = index.data(ProjectWidget::MapWidthRole).toInt();

        QDataStream stream(&data, QIODevice::ReadOnly);
        QVector<unsigned char> map;
        stream >> map;
        for(int i=0; i<m_points.size(); i++) {
            QPoint tmp = m_points.at(i);
            m_oldTiles.append(map[tmp.y()*width+tmp.x()]);
            map[tmp.y()*width+tmp.x()] = m_newTile;
        }

        QByteArray wdata;
        QDataStream wstream(&wdata, QIODevice::WriteOnly);
        wstream << map;
        m_projectWidget->modifyData(index, QVariant(wdata), ProjectWidget::MapRole);

        if (m_mapEditor->itemIndex() == index) {
            for (int i=0; i<m_points.size(); i++) {
                m_mapEditor->setTile(m_points[i], m_newTile);
            }
        }
    }
}


void SetTilesOnMapCommand::undo() {
    QPersistentModelIndex index = m_projectWidget->itemFromMapId(m_mapId, m_projectWidget->currentProject());
    if (index.isValid()) {
        QByteArray data = index.data(ProjectWidget::MapRole).toByteArray();
        int width = index.data(ProjectWidget::MapWidthRole).toInt();

        QDataStream stream(&data, QIODevice::ReadOnly);
        QVector<unsigned char> map;
        stream >> map;
        for(int i=0; i<m_points.size(); i++) {
            QPoint tmp = m_points.at(i);
            map[tmp.y()*width+tmp.x()] = m_oldTiles[i];
        }

        QByteArray wdata;
        QDataStream wstream(&wdata, QIODevice::WriteOnly);
        wstream << map;
        m_projectWidget->modifyData(index, QVariant(wdata), ProjectWidget::MapRole);

        if (m_mapEditor->itemIndex() == index) {
            for (int i=0; i<m_points.size(); i++) {
                m_mapEditor->setTile(m_points[i], m_oldTiles[i]);
            }
        }
    }
}






SetMapSizeCommand::SetMapSizeCommand(QSize newSize, const QPersistentModelIndex &index, MapEditor *mapEditor)
    : QUndoCommand()
{
    m_newSize = newSize;
    m_oldSize = QSize(index.data(ProjectWidget::MapWidthRole).toInt(), index.data(ProjectWidget::MapHeightRole).toInt());
    m_projectWidget = mapEditor->projectWidget();
    m_mapEditor = mapEditor;
    m_mapId = index.data(ProjectWidget::IdRole).toInt();
    QString name = index.data(Qt::DisplayRole).toString();
    setText(QObject::tr("Redimensionner la map \"%1\"").arg(name));
}


void SetMapSizeCommand::redo() {
    QPersistentModelIndex index = m_projectWidget->itemFromMapId(m_mapId, m_projectWidget->currentProject());

    m_oldDownTiles.clear();
    m_oldRightTiles.clear();

    if (index.isValid()) {
        QByteArray data = index.data(ProjectWidget::MapRole).toByteArray();

        QDataStream stream(&data, QIODevice::ReadOnly);
        QVector<unsigned char> mapArray;
        stream >> mapArray;
        Array2D<unsigned char> map(mapArray.toStdVector(), m_oldSize.width());
//Traiter les cas où on agrandi la map (mise à 0 des tiles, agrandissement de la map metadonnées, etc...)!!!
        m_oldRightTiles.resize(m_oldSize.height(), 0);
        for (int i=0; m_newSize.width()+i < m_oldSize.width(); i++)
            m_oldRightTiles.appendColumn(map.getColumn(m_newSize.width()+i));

        m_oldDownTiles.resize(0, m_oldSize.width());
        for (int i=0; m_newSize.height()+i < m_oldSize.height(); i++)
            m_oldDownTiles.appendRow(map.getRow(m_newSize.height()+i));

        map.resize(m_newSize.height(), m_newSize.width());
        for (int i=m_oldSize.width(); i<m_newSize.width(); i++)
            for (int j=0; j<m_newSize.height(); j++) map(j, i) = 0;
        for (int i=m_oldSize.height(); i<m_newSize.height(); i++)
            for (int j=0; j<m_oldSize.width(); j++) map(i, j) = 0;
        mapArray = QVector<unsigned char>::fromStdVector(map.toVector());

        QByteArray wdata;
        QDataStream wstream(&wdata, QIODevice::WriteOnly);
        wstream << mapArray;
        m_projectWidget->modifyData(index, QVariant(wdata), ProjectWidget::MapRole);
        m_projectWidget->modifyData(index, QVariant(m_newSize.width()), ProjectWidget::MapWidthRole);
        m_projectWidget->modifyData(index, QVariant(m_newSize.height()), ProjectWidget::MapHeightRole);

        if (m_mapEditor->itemIndex() == index) {
            m_mapEditor->resizeMap(m_newSize);
        }
    }
}


void SetMapSizeCommand::undo() {
    QPersistentModelIndex index = m_projectWidget->itemFromMapId(m_mapId, m_projectWidget->currentProject());

    if (index.isValid()) {
        QByteArray data = index.data(ProjectWidget::MapRole).toByteArray();

        QDataStream stream(&data, QIODevice::ReadOnly);
        QVector<unsigned char> mapArray;
        stream >> mapArray;
        Array2D<unsigned char> map(mapArray.toStdVector(), m_newSize.width());

        for (int i=0; i<m_oldRightTiles.columnCount(); i++) map.appendColumn(m_oldRightTiles.getColumn(i));
        for (int i=0; i<m_oldDownTiles.rowCount(); i++) map.appendRow(m_oldDownTiles.getRow(i));

        mapArray = QVector<unsigned char>::fromStdVector(map.toVector());
        QByteArray wdata;
        QDataStream wstream(&wdata, QIODevice::WriteOnly);
        wstream << mapArray;
        m_projectWidget->modifyData(index, QVariant(wdata), ProjectWidget::MapRole);
        m_projectWidget->modifyData(index, QVariant(m_oldSize.width()), ProjectWidget::MapWidthRole);
        m_projectWidget->modifyData(index, QVariant(m_oldSize.height()), ProjectWidget::MapHeightRole);

        if (m_mapEditor->itemIndex() == index) {
            m_mapEditor->resizeMap(m_oldSize);
            if (m_newSize.width()<m_oldSize.width())
                m_mapEditor->setPartOfMap(m_oldRightTiles, QPoint(m_newSize.width(), 0));
            if (m_newSize.height()<m_oldSize.height())
                m_mapEditor->setPartOfMap(m_oldDownTiles, QPoint(0, m_newSize.height()));
        }

        m_oldDownTiles.clear();
        m_oldRightTiles.clear();
    }
}







SetMapTilesetCommand::SetMapTilesetCommand
    (const QPersistentModelIndex &newTileset, const QPersistentModelIndex &index, MapEditor *mE) : QUndoCommand() {

    m_mapEditor = mE;
    m_projectWidget = m_mapEditor->projectWidget();
    m_itemId = index.data(ProjectWidget::IdRole).toInt();
    m_newTileset = newTileset.data(ProjectWidget::IdRole).toInt();
    m_oldTileset = mE->tilesetId();

    QString name = index.data(Qt::DisplayRole).toString();
    setText(QObject::tr("Modifier le tileset utilisé par la map \"%1\"").arg(name));
}



void SetMapTilesetCommand::undo() {
    if(index.isValid())
        m_projectWidget->modifyData(index, QVariant(m_oldName), Qt::DisplayRole);
}


void SetMapTilesetCommand::redo() {

}


bool SetMapTilesetCommand::mergeWith(const QUndoCommand *other) {
    if (other->id() != id()) return false;
    const SetItemNameCommand *cmd = static_cast<const SetItemNameCommand *>(other);
    if (m_itemType == cmd->m_itemType && m_itemId == cmd->m_itemId &&
        m_projectWidget == cmd->m_projectWidget)
        {
        m_newName = cmd->m_newName;
        setRedoText();
        return true;
    }
    return false;
}


int SetMapTilesetCommand::id() const {
    return setMapTilesetCommandId;
}
