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

#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include <QtGui>
#include <QtOpenGL>
#include "projectwidget.h"
#include "graphicstileitem.h"
#include "graphicstilescene.h"
#include "array2d.h"
#include "undocommands.h"

class MapEditor : public QWidget
{
    Q_OBJECT

    public:
        MapEditor(ProjectWidget *projectWidget, QWidget *parent = 0);
        QVector<unsigned char> mapArray();

        /**
          * @brief Renvoie un morceau désiré de la map en cours d'édition.
          *
          * partOfMap() permet de récupérer, sous la forme d'un Array2D, une partie de
          * la map actuellement éditée, désignée par un rectangle.\n
          * Le tableau renvoyé est des dimmensions de \a area et contient les numéros de tile
          * dans le tileset utilisé correspondant à chaque tile de la zone demandée.\n
          * Si l'argument par défaut est utilisé, renvoie toute la map.
          * @param area rectangle définissant la zone de la map à récupérer.
          * @return un Array2D de \c unsigned \c char représentant la zone désirée, ou un tableau
          * de dimenssions nulles si \a area est hors limite.
          * @see setPartOfMap(), setTile()
          */
        Array2D<unsigned char> partOfMap(QRect area = QRect());

        /**
          * @brief Remplace un rectangle défini de la map en cours d'édition.
          *
          * setPartOfMap() permet de remplacer le contenu d'un rectangle de la map en cours d'édition
          * par les valeurs d'un Array2D.\n
          * Le coin haut-gauche de ce rectangle est défini par \a topleft, et ses dimenssions sont
          * celles du tableau \a map.\n
          * @param topleft coordonnées du point haut-gauche du rectangle à éditer dans la map.
          * @param map un Array2D de \c unsigned \c char représentant les nouvelles valeurs
          * des tiles de la zone modifiée.
          * @see partOfMap(), tile()
          */
        void setPartOfMap(Array2D<unsigned char> map, QPoint topleft = QPoint());

        /**
          * @brief Modifie le nom de la map en cours d'édition.
          * @param name nouveau nom de la map.
          * @see itemName()
          */
        void setItemName(QString name);

        /**
          * @param Renvoie le nom de la map en cours d'édition.
          * @return le nom de la map courante.
          * @see setItemName()
          */
        QString itemName();

        /**
          * @brief Renvoie la taille de la map actuelle, en tiles.
          * @return la taille en tiles de la map en cours d'édition.
          * @see setWidth(), setHeight(), resizeMap()
          */
        QSize mapSize();

        /**
          * @brief Renvoie l'ID du tileset utilisé actuellement.
          * @return l'identifiant du tileset, ou -1 si aucun tileset n'est utilisé
          * lors de l'appel à la fonction.
          * @see setTilesetId()
          */
        int tilesetId();

        /**
          * @brief Modifie le tileset utilisé par l'éditeur dans la map courrante.
          * @param id identifiant du nouveau tileset à utiliser.
          * @see tilesetId()
          * @todo Utiliser setTilesetId() à la place de setTileset() pour y placer le
          *         code principal de modification de tileset (plus court et logique).
          */
        void setTilesetId(int id);

        /**
          * @brief Renvoie l'index de la map courante.
          * @return l'index de la map en cours d'édition.
          */
        const QPersistentModelIndex& itemIndex();

        /**
          * @brief Renvoie le ProjectWidget rataché à l'éditeur.
          * @return un pointeur sur le ProjectWidget lié à l'éditeur.
          */
        ProjectWidget *projectWidget();

        /**
          * @brief Modifie un tile de la map, et actualise l'affichage de ce tile.
          * @param pos position du tile sur la map.\n (0;0) est le tile haut-gauche.
          * @param newtile nouvelle valeur à assigner à ce tile. Cette valeur correspond
          * à l'identifiant du tile pour le tileset.
          * @see tile(), setPartOfMap()
          */
        void setTile(QPoint pos, int newValue);

        /**
          * @brief Renvoie la valeur d'identifiant d'un tile de la map.
          * @return l'identifiant (pour le tileset) du tile en position \a pos.
          * @param pos position du tile.
          * @see setTile(), partOfMap()
          */
        int tile(QPoint pos);

    protected:
        void changeEvent(QEvent *event);

    private:


    public slots:
        void setMapIndex(const QPersistentModelIndex &index);
        void saveMap();
        //void setWidth(int w);
        //void setHeight(int h);
        void resizeMap (QSize newSize);
        void tilesetModified (const QPersistentModelIndex &index);
        void tilesetDeleted (QModelIndex index);
        void tilesetAdded (const QPersistentModelIndex &index);
        void refreshTileset();
        void refreshTilesetList();

        /**
          * @brief Modifie le nom de la map courante.
          * @param name nouveau nom de la map en cours d'édition.
          */
        void setMapName(QString name);

    private slots:
        void setTileset(int number);
        void changePixmap();
        void changeTileBrush();
        void refreshAll();
        void widthChanged(int w);
        void heightChanged(int h);


    protected:
        ProjectWidget *m_projectWidget;
        QGraphicsView *mapView, *tilesetView;
        QGraphicsScene *mapScene, *tilesetScene;
        QImage m_brush;
        int m_currentTileBrushId;
        QComboBox *wm_tilesetSelector;
        QLineEdit *wm_name;
        QSpinBox *wm_width, *wm_height;
        QGroupBox *wm_groupSettings;
        QLabel *wm_labelHeight, *wm_labelWidth;
        QPersistentModelIndex m_mapIndex, m_tilesetIndex;
        QVector<GraphicsTileItem*> m_tilesetItems;
        QVector< QVector<GraphicsTileItem*> > m_map;
        QVector<QPersistentModelIndex> m_tilesetIndexList;
        int oldWidth, oldHeight;
        int tilesWidth, tilesHeight;
};

#endif // MAPEDITOR_H
