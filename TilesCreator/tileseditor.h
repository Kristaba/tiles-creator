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
  * @file tileseditor.h
  * @brief Fichier contenant la classe TilesEditor.
  * @author L�o Grange
  * @version 1.0
  * @date 18/12/2009
  */

#ifndef TILESEDITOR_H
#define TILESEDITOR_H

#include <QtGui>
#include "pixeleditor.h"
#include "colorsselecter.h"
#include "projectwidget.h"
#include "undocommands.h"


/**
* @brief La classe TilesEditor fournit un widget d'�dition de tile.
*
* TilesEditor est un widget complexe compos� de plusieurs menu et d'un widget central d'�dition
* de tile.\n
* Il est possible d'avoir une copie du tile en cours d'�dition avec tile(), et de r�cup�rer le
* nom donn� au tile avec tileName(). Pour modifier le tile et son nom, il est possible d'utiliser
* drawTile(). Enfin, le slot setAlphaChanel() permet d'utiliser ou non une couleur de transparence.
* \n Les signaux saveTile() et addToProject() permettent un interfa�age avec une classe qui g�re
* la sauvegarde des tiles tel que ProjectWidget.
* @todo Ajouter une gestion du tra�age de lignes droites et l'utiliser pour relier deux
* points successifs lors d'un mouseMoveEvent afin d'�viter des trous dans le trac�.
* @todo Remanier la structure et la fa�on de proc�der afin que TilesEditor agisse de fa�on
* similaire � TilesetEditor et MapEditor (passage d'un pointeur vers le ProjectWidget par
* exemple)... ou l'inverse? A r�fl�chir...
*/
class TilesEditor : public QWidget
{
    Q_OBJECT

public:
    /**
      * @brief Enum�ration des �tats possible du bouton de savegarde.
      * @see changeSaveStatus()
      */
    enum SaveButtonStatus {SaveEnable /**< Le bouton est clickable et est connect� � saveTile(). */,
                           SaveDisable /**< Le bouton n'est pas clickable. */,
                           AddToProject /**< Le bouton est clickable et est connect� � addToProject(). */
                       };

    /**
    * @brief Constructeur par d�faut.
    * @param project pointeur sur le ProjectWidget auquel doit se r�f�rer l'�diteur.
    * @param parent parent transmis au constructeur de QWidget.
    */
    TilesEditor(ProjectWidget *project, QWidget *parent = 0);

    /**
      * @brief Renvoie le nom du tile.
      * @return le nom du tile.
      */
    QString tileName() {
        return name->text();
    }

    /**
      * @brief Renvoie l'image representant le tile.
      * @return une image index�e representant le tile.
      */
    QImage tile() {
        return *editor->image();
    }

protected:
    QImage createNewImage();
    void changeEvent(QEvent *event);

public slots:
    /**
    * @brief Remplace le tile en cours d'�dition.
    * @param n nom du nouveau tile.
    * @param tile image index�e representant le tile.
    */
    void drawTile(QString n, QImage tile);

    /**
    * @brief Modifie le mode du bouton de sauvegarde.
    * @param s nouvel �tat du bouton.
    * @see SaveButtonStatus
    */
    void changeSaveStatus (SaveButtonStatus s);

    /**
    * @brief Active ou d�sactive l'utilisation de la couleur de transparence.
    * @param alpha vaut true si l'on veut utilise la couleur de transparence, false sinon.
    */
    void setAlphaChanel(bool alpha);

    /**
      * @brief Modifie l'index en cours d'�dition (non pris enti�rement en charge pour le moment).
      * @todo Pour le moment, setTileIndex() n'a pas la meme utilit� que pour les autres
      *     �diteurs. Il faudra modifier le fonctionnement de TilesEditor pour qu'ils
      *     soient similaires.
      * @param index index du nouveau tile � �diter.
      */
    void setTileIndex(const QPersistentModelIndex &index);

    /**
      * @brief Modifie le nom du tile courant.
      * @param name nouveau nom du tile en cours d'�dition.
      */
    void setTileName(QString name);

private slots:
    void pixelsModified(QList<QPoint> points, TC::Color newColor);

signals:
    /**
    * @brief Signal �mis pour demander de sauvegarder le tile.
    */
    void saveTile();

    /**
    * @brief Signal �mis pour demander d'ajouter le tile au projet.
    */
    void addToProject();

protected:
    QSpinBox *tileHeight, *tileWidth, *zoom;
    QCheckBox *grid, *alphaChanel;
    QPushButton *clear, *save;
    QLineEdit *name;
    QGroupBox *groupLenght, *groupAff, *groupTile;
    QLabel *labelWidth, *labelHeight, *labelZoom, *nameLabel;
    PixelEditor *editor;
    SaveButtonStatus saveStatus;
    ColorsSelecter *colorsSelecter;
    ProjectWidget *m_project;
    QPersistentModelIndex m_index;
};

#endif // TILESEDITOR_H
