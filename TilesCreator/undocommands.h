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

/**
   * @file undocommands.h
   * @brief Fichier contenant les classes de commandes utilisées par l'undo/redo.
   * @author Léo Grange
   * @date Créé le 24/04/2010
   *       Dernière modification le 25/04/2010
   * @version 1.0
   */

#ifndef UNDOCOMMANDS_H
#define UNDOCOMMANDS_H

#include <QtGui>
#include "globaltilescreator.h"
#include "projectwidget.h"
#include "mapeditor.h"

class MapEditor;

/**
  * @brief Classe fournissant la commande de modification de pixels d'un tile.
  *
  * Pour le moment, la classe n'est pas mergeable (effet contre productif lors de l'annulation de
  * masse d'un ensemble de pixels).\n
  * Une seule couleur est traité par commande, si plusieurs couleurs ont étés appliquées il sera
  * nécessaire de pusher plusieurs commandes sur la pile d'annulation.
  */
class TilePixmapModifiedCommand : public QUndoCommand
{
public:
    TilePixmapModifiedCommand(QPoint point, TC::Color newColor, const QPersistentModelIndex &index, ProjectWidget *pw);
    TilePixmapModifiedCommand(QList<QPoint> points, TC::Color newColor, const QPersistentModelIndex &index, ProjectWidget *pw);
    void undo();
    void redo();
    bool mergeWith(const QUndoCommand *other);
    int id() const;

private:
    void init(QList<QPoint> points, TC::Color newColor, const QPersistentModelIndex &index, ProjectWidget *pw);

private:
    QList<QPoint> m_points;
    TC::Color m_newColor;
    QList<TC::Color> m_oldColors;
    int m_tileId;
    ProjectWidget *m_projectWidget;
};



class SetItemNameCommand : public QUndoCommand
{
public:
    SetItemNameCommand(QString newName, const QPersistentModelIndex &index, ProjectWidget *pw);
    void undo();
    void redo();
    bool mergeWith(const QUndoCommand *other);
    int id() const;

private:
    void setRedoText();

private:
    QString m_oldName, m_newName;
    ProjectWidget::ItemType m_itemType;
    int m_itemId;
    ProjectWidget *m_projectWidget;
};



class SetTilesOnMapCommand : public QUndoCommand
{
public:
    SetTilesOnMapCommand(QList<QPoint> points, unsigned char newTile, const QPersistentModelIndex &index, MapEditor *mapEditor);
    void undo();
    void redo();

private:
    QList<QPoint> m_points;
    unsigned char m_newTile;
    QList<int> m_oldTiles;
    int m_mapId;
    ProjectWidget *m_projectWidget;
    MapEditor *m_mapEditor;
};



class SetMapSizeCommand : public QUndoCommand
{
public:
    SetMapSizeCommand(QSize newSize, const QPersistentModelIndex &index, MapEditor *mapEditor);
    void undo();
    void redo();

private:
    QSize m_newSize, m_oldSize;
    Array2D<unsigned char> m_oldDownTiles, m_oldRightTiles;
    int m_mapId;
    ProjectWidget *m_projectWidget;
    MapEditor *m_mapEditor;
};


class SetMapTilesetCommand : public QUndoCommand
{
public:
    SetMapTilesetCommand(const QPersistentModelIndex &newTileset, const QPersistentModelIndex &index, MapEditor *mapEditor);
    void undo();
    void redo();
    bool mergeWith(const QUndoCommand *other);
    int id() const;

private:
    int m_oldTileset, m_newTileset;
    int m_itemId;
    ProjectWidget *m_projectWidget;
    MapEditor *m_mapEditor;
};


#endif // UNDOCOMMANDS_H
