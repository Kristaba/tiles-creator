/*  Copyright 2009, 2010 L�o Grange

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
  * @file tcproject.h
  * @brief Fichier contenant les classes TcProject, TcTileset, TcTile et TcMap.
  * @author L�o Grange
  * @version 1.0
  * @date 08/12/2009
  */

#ifndef TCPROJECT_H
#define TCPROJECT_H

#include <QtCore>
#include "array2d.h"
#include "globaltilescreator.h"

class TcTile;
class TcTileset;
class TcMap;

/**
  * @brief TcProject fournit une structure simplifi�e d'un projet Tiles Creator.
  */
class TcProject {
    public:
        QString name; /**< @brief Nom du projet. */
        QList<TcTile> tiles; /**< @brief Liste des tiles. */
        QList<TcTileset> tilesets; /**< @brief Liste des tilesets. */
        QList<TcMap> maps; /**< @brief Liste des maps. */
};


/**
  * @brief TcTile fournit une structure simplifi�e d'un tile.
  */
class TcTile {
    public:
        QString name; /**< @brief Nom du tile. */
        QImage tile; /**< @brief QImage index�e repr�sentant le tile. @see PixelEditor::color */
};


/**
  * @brief TcTileset fournit une structure simplifi�e d'un tileset.
  */
class TcTileset {
    public:
        QString name; /**< @brief Nom du tileset. */
        QList<int> tiles; /**< @brief Liste des id des tiles du tileset. */
        QList<unsigned char> properties; /**< @brief Liste des propri�t�s des tiles du tileset. */
        QSize tilesSize; /**< @brief Taille des tiles du tileset. */
};


/**
  * @brief TcMap fournit une structure simplifi�e d'une map.
  */
class TcMap {
    public:
        QString name; /**< @brief Nom de la map. */
        int tilesetId; /**< @brief Id du tileset utilis� par la map. */

        /**
          * @brief Tableau en deux dimensions representant la map.
          *
          * Chaque �l�ment du tableau est le num�ro du tile du tileset utilis� par la map,
          * correspondant au tile de la map de coordon�es respectives.
          */
        Array2D<unsigned char> map;
};

#endif // TCPROJECT_H
