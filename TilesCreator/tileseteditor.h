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
   * @file tileseteditor.h
   * @brief Fichier contenant la classe TilesetEditor.
   * @author Léo Grange
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
  * @brief La classe tilesetEditor fournit un widget d'édition de tileset.
  *
  * 
  * @todo Ajouter un paramètre bool \p askConfirm à setTilesWidth et setTilesHeight afin de permettre de forcer la modification
  * sans demander à l'utilisateur.  
  */
class TilesetEditor : public QWidget
{
    Q_OBJECT

    public:
		/**
		  * @brief Constructeur.
		  * @param projectWidget pointeur sur le ProjectWidget auquel doit se référer l'éditeur.
		  * @param parent pointeur passé au constructeur de QWidget.
		  */
        TilesetEditor(ProjectWidget *projectWidget, QWidget *parent = 0);

    protected:
        void addNewTile(int id = -1);
        bool askForChangeTilesSize();
        void changeEvent(QEvent *event);
        void drawGrid();

    public slots:
		/**
		   * @brief Rafraichi toutes les données des modèles et actualise les affichage des vues.
		   * @see refreshgeometry()
		   */
        void refresh();
		
		/**
		  * @brief Modifie l'index du tileset à utiliser.
		  *
		  * Modifie l'index utilisé et actualise l'éditeur avec les nouvelles données.
		  * @param tilesetIndex le nouvel index à utiliser.
		  */
        void setTilesetIndex(QPersistentModelIndex tilesetIndex);
		
		/**
		  * @brief Enregistre le tileset dans le modèle du projet.
		  */
        void saveTileset();
		
		/**
		  * @brief Modifie la largeur des tiles.
		  *
		  * Une boîte de dialogue demande à l'utilisateur de confirmer. Si c'est le cas, le contenu du tileset en cours sera effacé.
		  * @param w Nouvelle largeur des tiles du tiliset.
		  * @see setTilesHeight()
		  */
        void setTilesWidth(int w);
		
		/**
		  * @brief Modifie la hauteur des tiles.
		  *
		  * Une boîte de dialogue demande à l'utilisateur de confirmer. Si c'est le cas, le contenu du tileset en cours sera effacé.
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
		  * @brief Slot permettant d'effectuer les modifiactions nécessaires lors de la modification d'un tile.
		  * @param index index du tile modifié.
		  */
        void tileModified(const QPersistentModelIndex &index);
		
		/**
		  * @brief Slot permettant d'effectuer les modifiactions nécessaires lors de la supression d'un tile.
                  * @param index index du tile modifié.
		  */
        void tileDeleted(QModelIndex index);
		
		/**
		  * @brief active ou désactive la visualisation des propriétés des tiles.
		  * @param display true pour afficher les propriétés, false sinon.
		  */
        void setDisplayProperties(bool display);

        /**
          * @brief Modifie le nom du tileset courant.
          * @param name nouveau nom du tileset en cours d'édition.
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
