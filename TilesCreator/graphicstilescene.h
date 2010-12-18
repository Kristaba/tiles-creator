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
  * @file graphicstilescene.h
  * @brief Fichier contenant la classe GraphicsTileScene.
  * @author Léo Grange
  * @version 1.0
  * @date 05/10/2009
  */

#ifndef GRAPHICSTILESCENE_H
#define GRAPHICSTILESCENE_H

#include <QGraphicsScene>
#include "graphicstileitem.h"

/**
* @brief GraphicsTileScene fournit une QGraphicsScene conçue pour recevoir des GraphicsTileItem.
*
* La class GraphicsTileScene fournit une scene utilisable dans le cadre du Framework Graphics
* View de Qt.\n
* Par rapport à une scene QGraphicsScene, cette classe possède quelques différences. En effet, lors
* d'un clique maintenu puis un déplacement de la souris, les GraphicsTileItem sur lesquels le curseur
* passe sont succécivement selectionnés.\n
* Le reste du fonctionnement de cette classe est similaire à QGraphicsScene.
*/
class GraphicsTileScene : public QGraphicsScene
{
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
    GraphicsTileScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);

    /**
    * @brief Constructeur par défaut.
    * @param parent parent passé au constructeur de QObject.
    */
    GraphicsTileScene(QObject *parent = 0);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void selectLine(QPoint posA, QPoint posB);
    void changeInt(int &a, int &b);
    void selectOtherItem(QPoint pos);

protected:
    GraphicsTileItem *m_lastItem;
    //QPoint m_oldpos;
};

#endif // GRAPHICSTILESCENE_H
