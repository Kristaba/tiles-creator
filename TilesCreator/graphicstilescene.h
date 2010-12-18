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
  * @file graphicstilescene.h
  * @brief Fichier contenant la classe GraphicsTileScene.
  * @author L�o Grange
  * @version 1.0
  * @date 05/10/2009
  */

#ifndef GRAPHICSTILESCENE_H
#define GRAPHICSTILESCENE_H

#include <QGraphicsScene>
#include "graphicstileitem.h"

/**
* @brief GraphicsTileScene fournit une QGraphicsScene con�ue pour recevoir des GraphicsTileItem.
*
* La class GraphicsTileScene fournit une scene utilisable dans le cadre du Framework Graphics
* View de Qt.\n
* Par rapport � une scene QGraphicsScene, cette classe poss�de quelques diff�rences. En effet, lors
* d'un clique maintenu puis un d�placement de la souris, les GraphicsTileItem sur lesquels le curseur
* passe sont succ�civement selectionn�s.\n
* Le reste du fonctionnement de cette classe est similaire � QGraphicsScene.
*/
class GraphicsTileScene : public QGraphicsScene
{
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
    GraphicsTileScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);

    /**
    * @brief Constructeur par d�faut.
    * @param parent parent pass� au constructeur de QObject.
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
