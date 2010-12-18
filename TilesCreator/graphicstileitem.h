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
  * @file graphicstileitem.h
  * @brief Fichier contenant la classe GraphicsTileItem.
  * @author L�o Grange
  * @version 1.0
  * @date 05/10/2009
  */

#ifndef GRAPHICSTILEITEM_H
#define GRAPHICSTILEITEM_H

#include <QtGui>

/**
  * @brief La classe GraphicsTileItem fournit un QGraphicsItem d'affichage de tile.
  *
  * La classe GraphicsTileItem permet de manipuler un tile grace au Framework Graphics
  * View de Qt.\n
  * Il est possible de donner un id au tile avec la m�thode setTileId() et de l'obtenir avec tileId().\n
  * Pour effacer le tile (le remplir de blanc), utiliser clearTile().\n
  * Il est aussi possible de donner un effet correspondant � un tile bloquant avec setBlocking() et
  * d'afficher ou non ces effets grace � setShowPolicy().\n
  * Enfin, la classe peut stocker des informations supl�mentaires comme des id de sa position gr�ce
  * � QGraphicsItem::setData(). L'�num�ration DataRole donne des key utilisables.\n
  * Lorsqu'on clique sur un GraphicsTileItem, celui-ci d�selectionne tous les items de sa scene
  * puis se selectionne lui-m�me. Seul un clique gauche est accept� par d�faut.
  * @todo Remplacer l'enum DataRole par des m�thodes get/set.
  * @todo Permettre d'ajouter des effets personalis�s de la m�me mani�re que setBlocking().
  */
class GraphicsTileItem : public QGraphicsPixmapItem
{
public:
    /**
      * @brief Enum�ration servant � utiliser qgraphicsitem_cast() sur cette classe.
      */
    enum {Type = UserType +1 /**< Identifiant du type de la classe. */};

    /**
      * @brief Enum�ration des r�les utilisable en tant que key dans QGraphicsItem::setData().
      */
    enum DataRole {XIdRole=0 /**< (int) id de la position en X du tile. */,
                   YIdRole /**< (int) id de la position en Y du tile. */};


    /**
    * @brief Constructeur par d�faut.
    *
    * @param tileWidth largeur du tile. Ne peut pas �tre modifier plus tard!
    * @param tileHeight hauteur du tile. Ne peut pas �tre modifier plus tard!
    * @param xId id de la position en X.
    * @param yId id de la position en Y.
    */
    GraphicsTileItem(int tileWidth=16, int tileHeight=16, int xId=0, int yId=0);

    /**
    * @brief Modifie l'id du tile.
    * @param id nouvel id.
    */
    void setTileId (int id);

    /**
    * @brief Renvoie l'id du tile.
    * @return int l'id du tile.
    */
    int tileId();


    /**
    * @brief Efface le tile et remplis l'item en blanc.
    * @see QGraphicsPixmapItem::setPixmap()
    */
    void clearTile();

    /**
    * @brief Modifie l'�tat bloquant on non bloquant du tile.
    * @param b true si on veut que le tile soit bloquant, false sinon.
    */
    void setBlocking(bool b);

    /**
    * @brief Renvoie l'�tat bloquant on non bloquant du tile.
    * @return true si le tile est bloquant, false sinon.
    */
    bool blocking();

    /**
    * @brief Modifie la politique d'affichage des status (tel que bloquant).
    * @param b true si on veut visualiser les status, false sinon.
    */
    void setShowPolicy(bool p);

    /**
    * @brief Renvoie la politique d'affichage des status (tel que bloquant).
    * @param b true si les status sont visualis�s, false sinon.
    */
    bool showPolicy();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    int type() const {
        //Pour pouvoir utiliser qgraphics_cast avec cette classe :
        return Type;
    }

protected:
    bool m_blocking;
    bool m_showPolicy;
    int m_tileId, m_tileWidth, m_tileHeight;
};

#endif // GRAPHICSTILEITEM_H
