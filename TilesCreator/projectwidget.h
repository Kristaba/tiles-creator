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
  * @file projectwidget.h
  * @brief Fichier contenant la classe ProjectWidget.
  * @author Léo Grange
  * @version 1.0
  * @date 12/12/2009
  */

#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QtGui>
#include "pixeleditor.h"
#include "tcproject.h"

/**
  * @brief La classe ProjectWidget fournit un widget d'affichage et d'interaction avec des
  * projets Tiles Creator.
  *
  * Cette classe est une des classes les plus importantes de Tiles Creator. C'est elle qui gère
  * le modèle et la vue arborécente representant les éléments des projets ouverts, et permet
  * d'interagir avec eux.\n
  * ProjectWidget utilise uniquement des QModelIndex et QPersistentModelIndex pour échanger
  * des éléments du modèle avec l'extérieur de la classe.
  */

class ProjectWidget : public QTreeView
{
    Q_OBJECT

    public:
        /**
          * @if french
          * @brief Enumération des roles utilisés par Tiles Creator dans les data et setData du modèle.
          * @else
          * @brief Enum of role used by Tiles Creator in data and setData of model.
          * @ifend
          */
        enum TilesCreatorRole {
            TileRole = 33 /**< (QImage) Image representant un Tile. */,
            IdRole /**< (int) Indique l'id interne de l'élément (Tile, Tileset ou Map). */,
            IndexTypeRole /**< (int) Indique le type d'élément que représente l'item. @see ItemType */,
            IdCounterRole /**< (int) Compteur interne des Tiles d'un Projet. @warning A ne modifier que si vous êtes sûr de ce que vous faites!*/,
            TilesetIdCounterRole /**< (int) Compteur interne des Tilesets d'un Projet. @warning A ne modifier que si vous êtes sûr de ce que vous faites!*/,
            MapIdCounterRole /**< (int) Compteur interne des Maps d'un Projet. @warning A ne modifier que si vous êtes sûr de ce que vous faites!*/,
            TilesetRole /**< (QByteArray) QVector<int>, sérialisé dans un QByteArray, qui contient la liste des id de tiles du tileset.*/,
            TilesWidthRole /**< (int) Largeur des tiles d'un tileset. */,
            TilesHeightRole /**< (int) Hauteur des tiles d'un tileset. */,
            MapRole /**< (QByteArray) QVector<unsigned char>, sérialisé dans un QByteArray, qui contient les données d'une map (chaque élément indique le numéro de tile du tileset utilisé assigné à une case de la map).*/,
            MapWidthRole /**< (int) Largeur d'une map. */,
            MapHeightRole /**< (int) Hauteur d'une map. */,
            TilesetIdRole /**< (int) Indique l'id du tileset assigné à une map. */,
            ProjectPathRole /**< (QString) Répertoire d'enregistrement d'un projet. */,
            TilesetPropertiesRole /**< (QByteArray) QVector<unsigned char>, sérialisé dans un QByteArray, qui contient les propriétés de chaque tile d'un tileset. */
        };
        /**
          * @brief Enumération des types d'élément succeptibles d'être rencontrés par le modèle ou dans un fichier.
          *
          * Pour connaître le type d'élément d'un index, utiliser QModelIndex::data(ProjectWidget::IdCounterRole).toInt(&ok)\n
          * Pour connaître le type d'élément d'une partir d'un fichier, utiliser readTypeField.
          * @see readTypeField()
          */
        enum ItemType {Unknow = -1 /**< Type inconnu... */,
                       TilesetTree = 0 /**< Une branche de tilesets. */,
                       TilesTree /**< Une branche de tiles. */,
                       MapsTree /**< Une branche de maps. */,
                       SpritesTree /**< Une branche de sprites (non implanté) */,
                       Tileset /**< Un tileset. */,
                       Tile /**< Un tile. */,
                       Map /**< Une map. */,
                       Sprite /**< Un sprite. */,
                       Project /**< Un projet Tiles Creator. */,
                       End /**< Fin d'un fichier. */
                   };

    public:
        /**
          * Constructeur par defaut.
          * @param undoGroup QUndoGroup à utiliser pour l'undo/redo.
          * @param parent un pointeur vers le QWidget parent.
          */
        ProjectWidget(QUndoGroup *undoGroup, QWidget *parent = 0);

        /**
          * @brief Efface le modèle.
          *
          * Efface entièrement le modèle et tout ce qu'il contenait.
          * @see deleteItem()
          */
        void clear();

        /**
          * @brief Ajoute un tile dans le projet actuel.
          *
          * @param name le nom du nouveau tile.
          * @param t un tile sous forme de QImage indexé.
          * @see m_currentProject  @see PixelEditor @see addTileset()
          * @see addMap()
          */
        void addTile(QString name, const QImage &t);

        /**
          * @brief Ajoute un tileset dans le projet actuel.
          *
          * @param name le nom du nouveau tileset.
          * @param tileset les id des tiles du tileset.
          * @param properties les propriétés de chaque tile du tileset.
          * @param tilesWidth la largeur des tiles utilisés.
          * @param tilesHeight la hauteur des tiles utilisés.
          * @see m_currentProject   @see addTile()    @see addMap()
          */
        void addTileset(QString name, const QVector<int> &tileset, const QVector<unsigned char> &properties, int tilesWidth, int tilesHeight);

        /**
          * @brief Ajoute une map dans le projet actuel.
          *
          * @param name le nom de la nouvelle map.
          * @param map un tableau qui contient les données de la map.
          * @param width la largeur de la map.
          * @param height la hauteur de la map.
          * @param tilesetId l'id du tileset lié à la map, ou -1 si pas de tileset lié.
          * @see m_currentProject   @see addTile()    @see addTileset()
          */
        void addMap(QString name, const QVector<unsigned char> &map, int width, int height, int tilesetId=-1);

        /**
          * @brief Retourne l'index du projet actuel.
          *
          * @return l'index du projet actuel.
          * @see m_currentProject
          */
        QPersistentModelIndex currentProject();

        /**
          * @brief Permet d'obtenir l'index d'un tile grace à son id et l'index du projet
          * auquel il appartient.
          *
          * @param tileId l'id du tile recherché.
          * @param project un index du projet dans lequel le tile désiré doit se trouver.
          * @return l'index du tile désiré si il existe, ou un index invalide si il n'existe pas.
          * @see projectOfIndex() @see itemFromTilesetId()
          */
        QPersistentModelIndex itemFromTileId (int tileId, QPersistentModelIndex project);

        /**
          * @brief Permet d'obtenir l'index d'un tileset grace à son id et l'index du projet
          * auquel il appartient.
          *
          * @param tilesetId l'id du tileset recherché.
          * @param project un index du projet dans lequel le tileset désiré doit se trouver.
          * @return l'index du tileset désiré si il existe, ou un index invalide si il n'existe pas.
          * @see projectOfIndex() @see itemFromTileId()
          */
        QPersistentModelIndex itemFromTilesetId (int tilesetId, QPersistentModelIndex project);

        /**
          * @brief Permet d'obtenir l'index d'une map grace à son id et l'index du projet
          * auquel elle appartient.
          *
          * @param tilesetId l'id de la map recherchée.
          * @param project un index du projet dans lequel la map désirée doit se trouver.
          * @return l'index de la map désirée si elle existe, ou un index invalide si elle n'existe pas.
          * @see projectOfIndex() @see itemFromTileId()
          */
        QPersistentModelIndex itemFromMapId (int mapId, QPersistentModelIndex project);

        /**
          * @brief Enregistre un projet.
          *
          * Enregistre un projet Tiles Creator dans le format .prj.
          * @param project l'index du projet à enregistrer.
          * @param path le chemin du fichier, ou QString() si on veut laisser la fonction
          *     ouvrir un QFileDialog pour laisser le choix à l'utilisateur.
          * @see getProjectStruct()   @see openProjectFile()
          */
        void writeProjectFile (QPersistentModelIndex project, QString path = QString());

        /**
          * @brief Modifie un tile déjà existant.
          *
          * Permet de modifier un tile déjà existant dans le modèle en fournissant son index
          * et les nouveaux contenus.
          * @param index l'index du tile à modifier.
          * @param name le nouveau nom du tile.
          * @param tile le tile sous forme de QImage indexé.
          * @see modifyTileset()  @see modifyMap()  @see PixelEditor
          * @see itemFromTileId()
          */
        void modifyTile (const QPersistentModelIndex &index, QString name, QImage &tile);

        /**
          * @brief Modifie un tileset déjà existant.
          *
          * Permet de modifier un tile déjà existant dans le modèle en fournissant son index
          * et les nouveaux contenus.
          * @param index l'index du tileset à modifier.
          * @param name le nouveau nom du tileset.
          * @param tileset les id des tiles du tileset.
          * @param properties les propriétés de chaque tile du tileset.
          * @param tilesWidth la largeur des tiles utilisés.
          * @param tilesHeight la hauteur des tiles utilisés.
          * @see modifyTile()  @see modifyMap() @see itemFromTilesetId()
          */
        void modifyTileset (const QPersistentModelIndex &index, QString name, const QVector<int> &tileset, const QVector<unsigned char> &properties, int tilesWidth, int tilesHeight);

        /**
          * @brief Modifie une map déjà existante.
          *
          * Permet de modifier une map déjà existante dans le modèle en fournissant son index
          * et les nouveaux contenus.
          * @param index l'index de la map à modifier.
          * @param name le nom de la nouvelle map.
          * @param map un tableau qui contient les données de la map.
          * @param width la largeur de la map.
          * @param height la hauteur de la map.
          * @param tilesetId l'id du tileset lié à la map, ou -1 si pas de tileset lié.
          * @see modifyTile()  @see modifyTileset()
          */
        void modifyMap(const QPersistentModelIndex &index, QString name, const QVector<unsigned char> &map, int width, int height, int tilesetId=-1);

        /**
          * @brief Modifie un champs de données d'un index, si il est permis de le modifier.
          *
          * modifyData() permet de modifier rapidement une unique valeur d'un champs de données
          * d'un index, en vérifiant, selon le type de l'index, si il est permis de modifier
          * ce champs. Le cas échéant la fonction retourne false sans n'avoir rien modifié.\n
          * Les champs autorisés sont, pour les tiles, tilesets et maps, les champs de données
          * de TilesCreatorRole représentant des éléments modifiables via modifyTile(),
          * modifyTileset() et modifyMap(), ainsi que Qt::DisplayRole.
          * @param index index de l'élément à modifier.
          * @param newValue nouvelle valeur du champs à modifier.
          * @param dataId identifiant du champs à modifier.
          * @return true si le champs était modifiable pour cet item, false sinon.
          */
        bool modifyData(const QPersistentModelIndex &index, QVariant newValue, int dataId);

        /**
          * @brief Retourne l'index du projet contenant un index donné.
          *
          * Cette méthode renvoi, grâce à l'index d'un élément du modèle, un index du projet
          * qui le contient. Cependant, elle ne fonctionne que si l'index passé est de type suivant :
          * TilesetTree, TilesTree, MapsTree, Tileset, Tile, Map, Project.\n
          * Cette limitation ne devrait pas se remarquer tant que le modèle est modifié correctement.
          * @param index l'index contenu dans le projet recherché.
          * @return l'index du projet contenant l'index passé, ou un index invalide si l'index n'était
          *     pas valide (type inconnu ou index invalide).
          * @see currentProject()
          */
        QPersistentModelIndex projectOfIndex(QModelIndex index);

        /**
          * @brief Récupère un index vers chacun des tilesets d'un projet.
          *
          * @param project l'index du projet concerné.
          * @return un QVector contenant l'ensemble des index de chacun des tilesets du projet
          *     dans l'ordre du modèle. Le QVector est vide si l'index du projet passé est invalide.
          * @see tilesOfProject()  @see mapsOfProject()
          */
        QVector<QPersistentModelIndex> tilesetsOfProject(QPersistentModelIndex project);

        /**
          * @brief Récupère un index vers chacun des tiles d'un projet.
          *
          * @param project l'index du projet concerné.
          * @return un QVector contenant l'ensemble des index de chacun des tiles du projet dans
          *     l'ordre du modèle. Le QVector est vide si l'index du projet passé est invalide.
          * @see tilesetsOfProject()  @see mapsOfProject()
          */
        QVector<QPersistentModelIndex> tilesOfProject(QPersistentModelIndex project);

        /**
          * @brief Récupère un index vers chacune des maps d'un projet.
          *
          * @param project l'index du projet concerné.
          * @return un QVector contenant l'ensemble des index de chacun des tilesets du projet
          *     dans l'ordre du modèle. Le QVector est vide si l'index du projet passé est invalide.
          * @see tilesOfProject()  @see tilesetsOfProject()
          */
        QVector<QPersistentModelIndex> mapsOfProject(QPersistentModelIndex project);

        /**
          * @brief Suprimme un élément du modèle.
          *
          * Suprimme l'élément du modèle corespondant à un index donné et tout ce qu'il contient.
          * @param index l'index de l'élément à suprimmer.
          * @see clear()
          */
        void deleteItem(QModelIndex index);

        /**
          * @brief Transforme un projet en une structure TcProject.
          *
          * @param project l'index du projet à transformer en structure.
          * @return un TcProject contenant l'ensemble du projet passé ou un TcProject() si l'index
          *     est invalide.
          * @see TcProject
          * @see mergeProjectStruct()
          */
        TcProject getProjectStruct(QPersistentModelIndex project);

        /**
          * @brief Fusionne une structure TcProject avec un projet existant.
          *
          * Toutes les données de la structure TcProject sont ajoutées au projet project.
          * @param toMerge structure à ajouter.
          * @param project projet où va être ajouter toMerge.
          * @see TcProject
          * @see getProjectStruct()
          */
        void mergeProjectStruct(TcProject toMerge, QPersistentModelIndex project);

        /**
          * @brief Indique le type d'un index.
          *
          * @param project l'index dont on cherche le type.
          * @return le type de l'index ou Unknow si l'index est invalide ou qu'il n'a pas de
          *     données de type.
          * @see ItemType
          */
        static ProjectWidget::ItemType typeOfIndex(const QModelIndex &index);

        /**
          * @brief Modifie le QUndoGroup utilisé.
          *
          * @param undoGroup nouveau QUndoGroup à utiliser.
          * @see undoGroup()
          */
        void setUndoGroup(QUndoGroup *undoGroup);

        /**
          * @brief Renvoie le QUndoGroup utilisé.
          *
          * @return le QUndoGroup utilisé actuellement.
          * @see setUndoGroup()
          */
        QUndoGroup *undoGroup();

    protected:
        void closeEvent(QCloseEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void dragEnterEvent(QDragEnterEvent *event);
        void dragMoveEvent(QDragMoveEvent *event);
        void dragLeaveEvent(QDragLeaveEvent *event);
        void keyPressEvent(QKeyEvent *event);
        void dropEvent(QDropEvent *event);
        QByteArray readExpField(QFile &file);
        ItemType readTypeField(QFile &file);
        QStandardItem* p_addProject(QString name, int idCounter=0, int tilesetIdCounter=0, int mapIdCounter=0,
                                    QString path = QString());
        QStandardItem* p_addTile(QString name, const QImage &t, int id=-1, int row=-1);
        QStandardItem* p_addTileset(QString name, const QVector<int> &tileset, const QVector<unsigned char> &properties, int tilesWidth, int tilesHeight, int id=-1);
        QStandardItem* p_addMap(QString name, const QVector<unsigned char> &map, int width, int height, int tilesetId=-1, int id=-1);
        void changeEvent(QEvent *event);

    public slots:
        /**
          * @brief Ouvre des fichiers de tile et les ajoutes au projet actuel.
          *
          * Cette méthode ouvre un QFileDialog afin de sélectioner un ou plusieurs fichiers de tile
          * puis ouvre chaque fichier et l'ajoute au projet courrant.
          * @see addTileFromFile()  @see addNewTile()   @see openProjectFile()
          */
        void openTileFile();

        /**
          * @brief Ouvre un projet Tiles Creator et l'ajoute au modèle.
          *
          * Ouvre un projet situé par path. Si path est vide, la méthode ouvre un QFileDialog.
          * @param path chemin du projet à ouvrir.
          * @see addTileFromFile()  @see addProject()
          */
        void openProjectFile (QString path = QString());

        /**
          * @brief Ouvre un fichier de tile et l'ajoute au projet courrant.
          *
          * Ajoute un tile situé par path. Si path est vide, la méthode ne fait rien.
          * @param chemin du tile à ouvrir.
          * @see openTileFile()  @see addNewTile()   @see addTile()
          */
        void addTileFromFile (QString path =  QString());

        /**
          * @brief Ajoute un nouveau tile vide au projet actuel.
          *
          * @see addTileFromFile()  @see addTile()  @see modifyTile()
          */
        void addNewTile();

        /**
          * @brief Ajoute un nouveau tileset vide au projet actuel.
          *
          * @see addTileset() @see modifyTileset()
          */
        void addNewTileset();

        /**
          * @brief Ajoute une nouvelle map vide au projet actuel.
          *
          * @see addMap()  @see modifyMap()
          */
        void addNewMap();

        /**
          * @brief Ajoute un nouveau projet au modèle.
          *
          * @param name nom du nouveau projet.
          */
        void addProject(QString name =  tr("Nouveau Projet"));

    protected slots:
        void anyItemSelected(QModelIndex index);
        void deleteItemSelected();
        void contextMenu(const QPoint &pos);
        void changeCurrentProject(QStandardItem *project);
        void expandUnderDrop();
        void scrollDuringDrag();

    signals:
        void closeWindow(); /**< @brief Signal émis lors de la fermeture de la fenêtre. */
        void requestAddActualTile(); /**< @brief Demande d'ajout du tile en cours d'édition. */

        /**
          * @brief Signal émis à chaque fois que l'élément sélectionné du tree change.
          * @param index index du nouvel élément sélectionné.
          */
        void currentItemChanged(const QPersistentModelIndex &index);

        /**
          * @brief Signal émis à chaque fois que qu'un élément va être supprimé.
          * @param index index de l'élément qui va être suprimmer.
          */
        void itemDeleted(QModelIndex index);

        /**
          * @brief Signal émis à chaque fois qu'un élément est modifié.
          * @param index index de l'élément modifié.
          */
        void itemModified(const QPersistentModelIndex &index);

        /**
          * @brief Signal émis à chaque fois qu'un élément est ajouté.
          * @param index index du nouvel élément.
          */
        void itemAdded(const QPersistentModelIndex &index);

        void lastProjectClosed(); /**< @brief Signal émis lors de la fermeture du dernier projet. */


    protected:
        QStandardItemModel *model; /**< Modèle utilisé par le QTreeView. */
        QStandardItem *m_currentProject;
        QAction *m_addActualTile, *m_deleteSelected, *m_addTileFromFile, *m_addNewTile,
            *m_quitProject, *m_addFileToProject, *m_addNewTileset, *m_addNewProject, *m_addProjectFile,
            *m_addNewMap;
        QPoint m_oldPos;
        QStandardItem *m_dragItem;
        QModelIndex m_oldIndexUnderDrop;
        QTimer *m_expandTimer, *m_scrollTimer;
        bool upScrolling;
        QUndoGroup *m_undoGroup;
        QList<QUndoStack*> m_undoStacks;
};

#endif // PROJECTWIDGET_H
