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
  * @file graphicstilesetscene.h
  * @brief Fichier contenant la classe GraphicsTilesetScene.
  * @author L�o Grange
  * @version 1.0
  * @date 17/10/2009
  */

#ifndef GRAPHICSTILESETSCENE_H
#define GRAPHICSTILESETSCENE_H

#include <QGraphicsScene>
#include "graphicstileitem.h"

/**
* @brief La classe GraphicsTilesetScene fournit une scene con�ue pour servir d'�diteur de
* tileset utilisant des GraphicsTileItem.
*
* La scene fournit par GraphicsTilesetScene permet d'interagir avec les GraphicsTileItem
* pr�sents sur celle-ci.\n
* Pour cela, la gestion du Drag&Drop interne et externe est enti�rement assur�e : un tile
* venant d'une sc�ne et �tant drop� sur un autre tile de cette m�me scene provoquera un
* inversement des contenus des deux items. De plus le support de drag de l'ext�rieur est
* g�r� en prenant en compte un seul widget source, modifiable avec setAllowedDragWidget(),
* et un seul QModelIndex, correspondant au projet Tiles Creator auquel appartient le tileset,
* et permettant d'autoriser uniquement le drop de tiles du m�me projet. Cet index est
* modifiable via setProjectIndex().\n
* Enfin, cette scene affichera, lors d'un clique droit sur un tile, un menu permettant
* d'interagir avec lui (supression, modification de propri�t�s, ...). Lors d'un clique
* gauche, le signal tileSelectorRequested() servira de demande d'affichage d'un widget
* permettant de selectionner un tile rapidement (actuellement la classe TileSelector).
* @see GraphicsTileItem
* @see GraphicsTileScene
*/
class GraphicsTilesetScene : public QGraphicsScene
{
    Q_OBJECT

public:
    /**
    * @brief Constructeur.
    *
    * Construit une scene d�limit�e par un rectangle.
    * @param x abscisse du point haut gauche du rectangle.
    * @param y ordonn�e du point haut gauche du rectangle.
    * @param width largeur du rectangle.
    * @param height hauteur du rectangle.
    * @param parent parent pass� au constructeur de QObject.
    */
    GraphicsTilesetScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);

    /**
    * @brief Constructeur par d�faut.
    * @param parent parent pass� au constructeur de QObject.
    */
    GraphicsTilesetScene(QObject *parent = 0);


    /**
    * @brief Modifie l'index du projet contenant le tileset.
    * @param index nouvel index du projet contenant le tileset.
    */
    void setProjectIndex(const QPersistentModelIndex &index);

    /**
    * @brief Renvoie l'index du projet contenant le tileset.
    * @return l'index du projet contenant le tileset.
    */
    QPersistentModelIndex projectIndex();

    /**
    * @brief Modifie le widget autoris� � droper des items dans le tileset.
    * @param w pointeur vers le nouveau widget autoris�.
    */
    void setAllowedDragWidget(QWidget *w);

    /**
    * @brief Renvoie le widget autoris� � droper des items dans le tileset.
    * @return un pointeur vers le widget autoris�.
    */
    QWidget *allowedDragWidget();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void contextMenu(QPoint pos, GraphicsTileItem *tile);
    void toDo();

signals:
    /**
    * @brief Signal �mis lorsqu'un tile de la scene est modifi� par l'une de ses m�thodes.
    * @param item pointeur vers le tile modifi�.
    */
    void tileModified(GraphicsTileItem *item);

    /**
    * @brief Signal �mis pour demander d'afficher un selectioneur rapide de tile.
    * @param item tile qui demande � �tre modifi� par le selectionneur.
    * @param pos position id�ale du selectionneur.
    */
    void tileSelectorRequested(GraphicsTileItem *item, QPoint pos);


protected:
    QPersistentModelIndex m_projectIndex;
    QWidget *m_allowedWidget;
    GraphicsTileItem *m_oldTile, *m_dragTileSource;
    QAction *m_actionRemove, *m_actionSetBlock, *m_actionUnsetBlock;
    QPoint m_oldPos;
};

#endif // GRAPHICSTILESETSCENE_H
