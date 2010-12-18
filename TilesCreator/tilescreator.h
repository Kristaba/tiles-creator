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
  * @file tilescreator.h
  * @brief Fichier contenant la classe TilesCreator.
  * @author L�o Grange
  * @version 1.0
  * @date 18/12/2009
  */

#ifndef TILESCREATOR_H
#define TILESCREATOR_H

#include <QtGui>
#include "tileseditor.h"
#include "tileseteditor.h"
#include "projectwidget.h"
#include "mapeditor.h"
#include "BugReporter.h"
#include "tileselector.h"
#include "exportinterface.h"
#include "importinterface.h"

#ifdef __linux__
#define UPDATE_CMD "./webdownloader &"
#else
#define UPDATE_CMD "start webdownloader.exe"
#endif


/**
  * @brief La classe TilesCreator est la classe charg�e de l'interface utilisateur du projet.
  *
  * TilesCreator est la fen�tre principale de l'application. Elle g�re donc l'assemblage de
  * tous les autres widgets et assure leurs interactions via de nombreux slots et connexions
  * signaux/slots.
  * @todo Pour TOUTES les classes de Tiles Creator, normaliser au maximum les noms de
  * m�thode : �viter les confusions, les doubles traductions (actual/current par exemple)
  * et les mauvaises traductions.
  * @todo Pour TOUTES les classes, v�rifier chaque m�thode et en particulier les slots
  * afin de mettre en protected ou private ceux qui semblent sensibles et internes.
  * @todo Pour TOUTES les classes, mettre les attributs en private au lieu de protected
  * lorsqu'ils ne doivent pas �tre manipul�s dirrectement par d'�ventuelles classes filles.
  */
class TilesCreator : public QMainWindow
{
    Q_OBJECT

public:
    /**
      * @brief Constructeur par d�faut.
      * @param parent widget parent pass� au constructeur de QMainWindow.
      */
    TilesCreator(QWidget *parent = 0);

    /**
      * @brief V�rifie les plugins existants et met � jour le menu pour les rendre utilisables.
      */
    void initPlugins ();

    /**
      * @brief Transforme une chaine en une chaine valide en tant que nom de variable en C.
      *
      * Remplace tous les caract�res sp�ciaux et les espaces par des '_', et met la chaine
      * en minuscules. Pour le moment les accents ne sont pas g�r�s et la chaine peut commencer
      * par un chiffre.
      * @todo Emp�cher les accents dans la chaine de sortie.
      * @param s chaine � transformer.
      * @return QString contenant la chaine transform�e.
      */
    static QString nameToValidString(const QString &s);

    /**
      * @brief Transforme un QImage correctement index� en un bitmap.
      *
      * Le bitmap r�cup�r� est un bitmap tel que d�crit dans la documentation du SDK
      * Graph 85.
      * @todo Changer le param�tre \a lightOrDark par une �num�ration contenant par exemple
      * Light, Dark, AlphaMap...
      * @param i image dont on souhaite r�cup�rer le bitmap.
      * @param lightOrDark vaut false si l'on souhaite r�cup�rer le bitmap de poids faible (clair)
      * ou true si l'on veut le bitmap de poids fort (fonc�).
      * @return QByteArray contenant le bitmap.
      */
    static QByteArray bitmapFromGrayImage(const QImage &i, bool lightOrDark);

protected:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);

public slots:
    /**
    * @brief Ajoute le tile actuel au mod�le du projet.
    * @see saveActualTile()
    */
    void addActualTileToProject();

    /**
    * @brief Modifie le tile en cours d'�dition.
    * @param index index du tile � �diter.
    * @see changeEditingTileset
    * @see changeEditingMap
    */
    void changeEditingTile(const QPersistentModelIndex &index);

    /**
    * @brief Modifie le tileset en cours d'�dition.
    * @param index index du tileset � �diter.
    * @see changeEditingTile
    * @see changeEditingMap
    */
    void changeEditingTileset(const QPersistentModelIndex &index);

    /**
    * @brief Modifie la map en cours d'�dition.
    * @param index index de la map � �diter.
    * @see changeEditingTileset
    * @see changeEditingTile
    */
    void changeEditingMap(const QPersistentModelIndex &index);

    /**
    * @brief Sauvegarde le tile actuel dans le mod�le.
    * @see addActualTileToProject()
    */
    void saveActualTile();

    /**
    * @brief Enregistre le projet en cours.
    *
    * Si le projet poss�de un QString associ� � la cl� ProjectWidget::ProjectPathRole,
    * sauvegarde � cet emplacement, sinon demande � l'utilisateur o� et sous quel nom
    * enregistrer.
    * @see saveActualProjectAs()
    * @see openProject()
    */
    void saveActualProject();

    /**
    * @brief Enregistre le projet sous un nouveau nom.
    *
    * Demande � l'utilisateur l'emplacement et le nom sous lequel enregistrer le projet.
    * @see saveActualProject()
    */
    void saveActualProjectAs();

    /**
    * @brief Ouvre un projet depuis un emplacement sp�cifi�.
    * @see saveActualProject()
    */
    void openProject();

    /**
      * @brief Ajoute un nouveau projet au mod�le du ProjectWidget.
      */
    void addNewProject();

    /**
      * @brief Affiche l'�diteur de tile.
      * @see showTilesetEditor()
      * @see showMapEditor()
      */
    void showTileEditor();

    /**
      * @brief Affiche l'�diteur de tile.
      * @see showTileEditor()
      * @see showMapEditor()
      */
    void showTilesetEditor();

    /**
      * @brief Affiche l'�diteur de tile.
      * @see showTilesetEditor()
      * @see showTileEditor()
      */
    void showMapEditor();

    /**
      * @brief Propose de mettre � jour le logiciel.
      */
    void updateSoftware();

    /**
      * @brief Ouvre une fen�tre modale permettant de signaler un bug en ligne.
      */
    void reportBug();

    /**
    * @brief Agit en cons�quence d'un item ajout� au mod�le.
    * @param index index de l'item venant d'�tre ajout�.
    * @see itemModified()
    * @see itemDeleted()
    */
    void itemAdded(const QPersistentModelIndex &index);

    /**
    * @brief Agit en cons�quence lors d'une modification d'un item du mod�le.
    * @param index index de l'item venant d'�tre modifi�.
    * @see itemAdded()
    * @see itemDeleted()
    */
    void itemModified(const QPersistentModelIndex &index);

    /**
    * @brief Agit en cons�quence d'un item en cours de suppression.
    * @param index index de l'item qui va �tre supprim�.
    * @see itemAdded()
    * @see itemModified()
    */
    void itemDeleted(QModelIndex index);

    /**
    * @brief Change la langue de l'interface pour le fran�ais.
    * @see changeEnglish()
    */
    void changeFrench();

    /**
    * @brief Change la langue de l'interface pour l'anglais.
    * @see changeFrench()
    */
    void changeEnglish();

    /**
    * @brief Ouvre un dialogue "A propos de Tiles Creator".
    * @see aboutPlugins()
    */
    void aboutTilesCreator();

    /**
    * @brief Ouvre un dialogue contenant les diverses informations sur les plugins.
    * @see aboutTilesCreator()
    */
    void aboutPlugins();


protected slots:
    void setActualItem(const QPersistentModelIndex &index);
    void lastProjectClosed();
    void projectVisibilityChanged(bool);
    void exportCurrentProject(QAction *action);
    void importData(QAction *action);
    void detailledPluginDescription(const QModelIndex &index);

protected:
    QMenu *menuFile, *menuOptions, *menuProject, *menuLanguage, *menuExport, *menuAbout;
    QMenu *menuImport, *menuEdit;
    QAction *actionOpen, *actionNewTile, *actionNewTileset, *actionNewMap, *actionSave;
    QAction *actionOptions, *actionUpdate, *actionReport;
    QAction *actionDisplay, *actionSaveProject, *actionSaveProjectUnder,
    *actionOpenProject, *actionNewProject;
    QAction *actionAboutTc, *actionAboutQt, *actionAboutPlugins;
    QAction *actionUndo, *actionRedo;
    QAction *actionFrench, *actionEnglish;
    QList<QAction*> exportPlugins, importPlugins;
    QList<ExportInterface*> exportPluginList;
    QList<ImportInterface*> importPluginList;
    QList<TilesCreatorPlugin*> plugins;
    QTranslator *translator, *translatorInternal;

    TilesEditor *tilesEditor;
    TilesetEditor *tilesetEditor;
    MapEditor *mapEditor;
    ProjectWidget *projectList;
    QPersistentModelIndex actualTile, actualTileset, actualMap;
    QDockWidget *projectDock, *historicDock;
    QUndoView *historic;
    QTabWidget *editor;
    QSettings *settingsFile;
    QUndoGroup *undoGroup;
};


/**
  * @mainpage
  * <b>Documentation de Tiles Creator</b>\n
  * Bienvenue sur la documentation de Tiles Creator!\n
  * Cette documentation est principalement con�ue pour faciliter la compr�hension du code source
  * de Tiles Creator, dans le cas ou des programmeurs aient pour intention de l'am�liorer, de s'en
  * inspirer, d'utiliser ses classes.\n
  * N'h�sitez pas � regarder quelques @ref screenshots ;)\n
  * <b>Pages de documentation technique d�taill�e :</b>\n
  * @ref g85export \n
  * @ref makeplugin
  *
  *
  * @page screenshots Captures d'�cran
  * Voici quelques captures d'�cran de Tiles Creator et de ses plugins : \n
  * @image html TC1.PNG
  * \n\n @image html TC2.PNG
  * \n\n @image html TC3.PNG
  * \n\n@image html PictImport1.PNG
  */

#endif // TILESCREATOR_H
