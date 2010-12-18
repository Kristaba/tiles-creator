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
   * @file tileseteditor.h
   * @brief Fichier contenant la classe TilesetEditor.
   * @author L�o Grange
   * @date 21/10/2009
   * @version 1.0
   */

#ifndef TILESETEDITOR_H
#define TILESETEDITOR_H

#include <QtGui>
#include "projectwidget.h"
#include "tileselector.h"
#include "graphicstilesetscene.h"

/**
  * @brief La classe tilesetEditor fournit un widget d'�dition de tileset.
  *
  * 
  * @todo Ajouter un param�tre bool \p askConfirm � setTilesWidth et setTilesHeight afin de permettre de forcer la modification
  * sans demander � l'utilisateur.  
  */
class TilesetEditor : public QWidget
{
    Q_OBJECT

    public:
		/**
		  * @brief Constructeur.
		  * @param projectWidget pointeur sur le ProjectWidget auquel doit se r�f�rer l'�diteur.
		  * @param parent pointeur pass� au constructeur de QWidget.
		  */
        TilesetEditor(ProjectWidget *projectWidget, QWidget *parent = 0);

    protected:
        void addNewTile(int id = -1);
        bool askForChangeTilesSize();
        void changeEvent(QEvent *event);
        void drawGrid();

    public slots:
		/**
		   * @brief Rafraichi toutes les donn�es des mod�les et actualise les affichage des vues.
		   * @see refreshgeometry()
		   */
        void refresh();
		
		/**
		  * @brief Modifie l'index du tileset � utiliser.
		  *
		  * Modifie l'index utilis� et actualise l'�diteur avec les nouvelles donn�es.
		  * @param tilesetIndex le nouvel index � utiliser.
		  */
        void setTilesetIndex(QPersistentModelIndex tilesetIndex);
		
		/**
		  * @brief Enregistre le tileset dans le mod�le du projet.
		  */
        void saveTileset();
		
		/**
		  * @brief Modifie la largeur des tiles.
		  *
		  * Une bo�te de dialogue demande � l'utilisateur de confirmer. Si c'est le cas, le contenu du tileset en cours sera effac�.
		  * @param w Nouvelle largeur des tiles du tiliset.
		  * @see setTilesHeight()
		  */
        void setTilesWidth(int w);
		
		/**
		  * @brief Modifie la hauteur des tiles.
		  *
		  * Une bo�te de dialogue demande � l'utilisateur de confirmer. Si c'est le cas, le contenu du tileset en cours sera effac�.
		  * @param w Nouvelle hauteur des tiles du tiliset.
		  * @see setTilesWidth
		  */
        void setTilesHeight(int h);
		
		/**
		  * @brief Actualise l'affichage de la scene sans modifier le contenu du tileset.
		  * @see refresh()
		  */
        void refreshGeometry();
		
		/**
		  * @brief Slot permettant d'effectuer les modifiactions n�cessaires lors de la modification d'un tile.
		  * @param index index du tile modifi�.
		  */
        void tileModified(const QPersistentModelIndex &index);
		
		/**
		  * @brief Slot permettant d'effectuer les modifiactions n�cessaires lors de la supression d'un tile.
                  * @param index index du tile modifi�.
		  */
        void tileDeleted(QModelIndex index);
		
		/**
		  * @brief active ou d�sactive la visualisation des propri�t�s des tiles.
		  * @param display true pour afficher les propri�t�s, false sinon.
		  */
        void setDisplayProperties(bool display);

        /**
          * @brief Modifie le nom du tileset courant.
          * @param name nouveau nom du tileset en cours d'�dition.
          */
        void setTilesetName(QString name);
        
    protected slots:
        void refreshTileId (GraphicsTileItem *item);
        void numberOfTilesChanged (int);
        void showTileSelector (GraphicsTileItem *item, QPoint pos);
        void changeSelectedTile (QPersistentModelIndex newTile);

    protected:
        QPersistentModelIndex m_tilesetIndex;
        ProjectWidget *m_projectWidget;
        QLineEdit *name;
        QSpinBox *wm_tilesHeight, *wm_tilesWidth, *wm_numberOfTiles;
        QGraphicsView *wm_view;
        GraphicsTilesetScene *m_scene;
        QVector<GraphicsTileItem*> tiles;
        QVector<int> tilesId;
        QGroupBox *groupTiles, *groupTileset;
        QLabel *labelWidth, *labelHeight, *labelNumber, *labelName;
        QCheckBox *displayProperties;
        int oldTilesWidth, oldTilesHeight;
        TileSelector *wm_selector;
        int tileSelectedId;
};

#endif // TILESETEDITOR_H
