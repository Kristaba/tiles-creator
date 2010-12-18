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
  * @file tilescreator.h
  * @brief Fichier contenant la classe TilesCreator.
  * @author Léo Grange
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
  * @brief La classe TilesCreator est la classe chargée de l'interface utilisateur du projet.
  *
  * TilesCreator est la fenêtre principale de l'application. Elle gère donc l'assemblage de
  * tous les autres widgets et assure leurs interactions via de nombreux slots et connexions
  * signaux/slots.
  * @todo Pour TOUTES les classes de Tiles Creator, normaliser au maximum les noms de
  * méthode : éviter les confusions, les doubles traductions (actual/current par exemple)
  * et les mauvaises traductions.
  * @todo Pour TOUTES les classes, vérifier chaque méthode et en particulier les slots
  * afin de mettre en protected ou private ceux qui semblent sensibles et internes.
  * @todo Pour TOUTES les classes, mettre les attributs en private au lieu de protected
  * lorsqu'ils ne doivent pas être manipulés dirrectement par d'éventuelles classes filles.
  */
class TilesCreator : public QMainWindow
{
    Q_OBJECT

public:
    /**
      * @brief Constructeur par défaut.
      * @param parent widget parent passé au constructeur de QMainWindow.
      */
    TilesCreator(QWidget *parent = 0);

    /**
      * @brief Vérifie les plugins existants et met à jour le menu pour les rendre utilisables.
      */
    void initPlugins ();

    /**
      * @brief Transforme une chaine en une chaine valide en tant que nom de variable en C.
      *
      * Remplace tous les caractères spéciaux et les espaces par des '_', et met la chaine
      * en minuscules. Pour le moment les accents ne sont pas gérés et la chaine peut commencer
      * par un chiffre.
      * @todo Empêcher les accents dans la chaine de sortie.
      * @param s chaine à transformer.
      * @return QString contenant la chaine transformée.
      */
    static QString nameToValidString(const QString &s);

    /**
      * @brief Transforme un QImage correctement indexé en un bitmap.
      *
      * Le bitmap récupéré est un bitmap tel que décrit dans la documentation du SDK
      * Graph 85.
      * @todo Changer le paramètre \a lightOrDark par une énumération contenant par exemple
      * Light, Dark, AlphaMap...
      * @param i image dont on souhaite récupérer le bitmap.
      * @param lightOrDark vaut false si l'on souhaite récupérer le bitmap de poids faible (clair)
      * ou true si l'on veut le bitmap de poids fort (foncé).
      * @return QByteArray contenant le bitmap.
      */
    static QByteArray bitmapFromGrayImage(const QImage &i, bool lightOrDark);

protected:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);

public slots:
    /**
    * @brief Ajoute le tile actuel au modèle du projet.
    * @see saveActualTile()
    */
    void addActualTileToProject();

    /**
    * @brief Modifie le tile en cours d'édition.
    * @param index index du tile à éditer.
    * @see changeEditingTileset
    * @see changeEditingMap
    */
    void changeEditingTile(const QPersistentModelIndex &index);

    /**
    * @brief Modifie le tileset en cours d'édition.
    * @param index index du tileset à éditer.
    * @see changeEditingTile
    * @see changeEditingMap
    */
    void changeEditingTileset(const QPersistentModelIndex &index);

    /**
    * @brief Modifie la map en cours d'édition.
    * @param index index de la map à éditer.
    * @see changeEditingTileset
    * @see changeEditingTile
    */
    void changeEditingMap(const QPersistentModelIndex &index);

    /**
    * @brief Sauvegarde le tile actuel dans le modèle.
    * @see addActualTileToProject()
    */
    void saveActualTile();

    /**
    * @brief Enregistre le projet en cours.
    *
    * Si le projet possède un QString associé à la clé ProjectWidget::ProjectPathRole,
    * sauvegarde à cet emplacement, sinon demande à l'utilisateur où et sous quel nom
    * enregistrer.
    * @see saveActualProjectAs()
    * @see openProject()
    */
    void saveActualProject();

    /**
    * @brief Enregistre le projet sous un nouveau nom.
    *
    * Demande à l'utilisateur l'emplacement et le nom sous lequel enregistrer le projet.
    * @see saveActualProject()
    */
    void saveActualProjectAs();

    /**
    * @brief Ouvre un projet depuis un emplacement spécifié.
    * @see saveActualProject()
    */
    void openProject();

    /**
      * @brief Ajoute un nouveau projet au modèle du ProjectWidget.
      */
    void addNewProject();

    /**
      * @brief Affiche l'éditeur de tile.
      * @see showTilesetEditor()
      * @see showMapEditor()
      */
    void showTileEditor();

    /**
      * @brief Affiche l'éditeur de tile.
      * @see showTileEditor()
      * @see showMapEditor()
      */
    void showTilesetEditor();

    /**
      * @brief Affiche l'éditeur de tile.
      * @see showTilesetEditor()
      * @see showTileEditor()
      */
    void showMapEditor();

    /**
      * @brief Propose de mettre à jour le logiciel.
      */
    void updateSoftware();

    /**
      * @brief Ouvre une fenêtre modale permettant de signaler un bug en ligne.
      */
    void reportBug();

    /**
    * @brief Agit en conséquence d'un item ajouté au modèle.
    * @param index index de l'item venant d'être ajouté.
    * @see itemModified()
    * @see itemDeleted()
    */
    void itemAdded(const QPersistentModelIndex &index);

    /**
    * @brief Agit en conséquence lors d'une modification d'un item du modèle.
    * @param index index de l'item venant d'être modifié.
    * @see itemAdded()
    * @see itemDeleted()
    */
    void itemModified(const QPersistentModelIndex &index);

    /**
    * @brief Agit en conséquence d'un item en cours de suppression.
    * @param index index de l'item qui va être supprimé.
    * @see itemAdded()
    * @see itemModified()
    */
    void itemDeleted(QModelIndex index);

    /**
    * @brief Change la langue de l'interface pour le français.
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
  * Cette documentation est principalement conçue pour faciliter la compréhension du code source
  * de Tiles Creator, dans le cas ou des programmeurs aient pour intention de l'améliorer, de s'en
  * inspirer, d'utiliser ses classes.\n
  * N'hésitez pas à regarder quelques @ref screenshots ;)\n
  * <b>Pages de documentation technique détaillée :</b>\n
  * @ref g85export \n
  * @ref makeplugin
  *
  *
  * @page screenshots Captures d'écran
  * Voici quelques captures d'écran de Tiles Creator et de ses plugins : \n
  * @image html TC1.PNG
  * \n\n @image html TC2.PNG
  * \n\n @image html TC3.PNG
  * \n\n@image html PictImport1.PNG
  */

#endif // TILESCREATOR_H
