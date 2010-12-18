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
  * @file graphicstilesetscene.h
  * @brief Fichier contenant la classe GraphicsTilesetScene.
  * @author Léo Grange
  * @version 1.0
  * @date 17/10/2009
  */

#ifndef GRAPHICSTILESETSCENE_H
#define GRAPHICSTILESETSCENE_H

#include <QGraphicsScene>
#include "graphicstileitem.h"

/**
* @brief La classe GraphicsTilesetScene fournit une scene conçue pour servir d'éditeur de
* tileset utilisant des GraphicsTileItem.
*
* La scene fournit par GraphicsTilesetScene permet d'interagir avec les GraphicsTileItem
* présents sur celle-ci.\n
* Pour cela, la gestion du Drag&Drop interne et externe est entièrement assurée : un tile
* venant d'une scène et étant dropé sur un autre tile de cette même scene provoquera un
* inversement des contenus des deux items. De plus le support de drag de l'extérieur est
* géré en prenant en compte un seul widget source, modifiable avec setAllowedDragWidget(),
* et un seul QModelIndex, correspondant au projet Tiles Creator auquel appartient le tileset,
* et permettant d'autoriser uniquement le drop de tiles du même projet. Cet index est
* modifiable via setProjectIndex().\n
* Enfin, cette scene affichera, lors d'un clique droit sur un tile, un menu permettant
* d'interagir avec lui (supression, modification de propriétés, ...). Lors d'un clique
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
    * Construit une scene délimitée par un rectangle.
    * @param x abscisse du point haut gauche du rectangle.
    * @param y ordonnée du point haut gauche du rectangle.
    * @param width largeur du rectangle.
    * @param height hauteur du rectangle.
    * @param parent parent passé au constructeur de QObject.
    */
    GraphicsTilesetScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);

    /**
    * @brief Constructeur par défaut.
    * @param parent parent passé au constructeur de QObject.
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
    * @brief Modifie le widget autorisé à droper des items dans le tileset.
    * @param w pointeur vers le nouveau widget autorisé.
    */
    void setAllowedDragWidget(QWidget *w);

    /**
    * @brief Renvoie le widget autorisé à droper des items dans le tileset.
    * @return un pointeur vers le widget autorisé.
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
    * @brief Signal émis lorsqu'un tile de la scene est modifié par l'une de ses méthodes.
    * @param item pointeur vers le tile modifié.
    */
    void tileModified(GraphicsTileItem *item);

    /**
    * @brief Signal émis pour demander d'afficher un selectioneur rapide de tile.
    * @param item tile qui demande à être modifié par le selectionneur.
    * @param pos position idéale du selectionneur.
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
