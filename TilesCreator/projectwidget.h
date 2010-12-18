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
  * @file projectwidget.h
  * @brief Fichier contenant la classe ProjectWidget.
  * @author L�o Grange
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
  * Cette classe est une des classes les plus importantes de Tiles Creator. C'est elle qui g�re
  * le mod�le et la vue arbor�cente representant les �l�ments des projets ouverts, et permet
  * d'interagir avec eux.\n
  * ProjectWidget utilise uniquement des QModelIndex et QPersistentModelIndex pour �changer
  * des �l�ments du mod�le avec l'ext�rieur de la classe.
  */

class ProjectWidget : public QTreeView
{
    Q_OBJECT

    public:
        /**
          * @if french
          * @brief Enum�ration des roles utilis�s par Tiles Creator dans les data et setData du mod�le.
          * @else
          * @brief Enum of role used by Tiles Creator in data and setData of model.
          * @ifend
          */
        enum TilesCreatorRole {
            TileRole = 33 /**< (QImage) Image representant un Tile. */,
            IdRole /**< (int) Indique l'id interne de l'�l�ment (Tile, Tileset ou Map). */,
            IndexTypeRole /**< (int) Indique le type d'�l�ment que repr�sente l'item. @see ItemType */,
            IdCounterRole /**< (int) Compteur interne des Tiles d'un Projet. @warning A ne modifier que si vous �tes s�r de ce que vous faites!*/,
            TilesetIdCounterRole /**< (int) Compteur interne des Tilesets d'un Projet. @warning A ne modifier que si vous �tes s�r de ce que vous faites!*/,
            MapIdCounterRole /**< (int) Compteur interne des Maps d'un Projet. @warning A ne modifier que si vous �tes s�r de ce que vous faites!*/,
            TilesetRole /**< (QByteArray) QVector<int>, s�rialis� dans un QByteArray, qui contient la liste des id de tiles du tileset.*/,
            TilesWidthRole /**< (int) Largeur des tiles d'un tileset. */,
            TilesHeightRole /**< (int) Hauteur des tiles d'un tileset. */,
            MapRole /**< (QByteArray) QVector<unsigned char>, s�rialis� dans un QByteArray, qui contient les donn�es d'une map (chaque �l�ment indique le num�ro de tile du tileset utilis� assign� � une case de la map).*/,
            MapWidthRole /**< (int) Largeur d'une map. */,
            MapHeightRole /**< (int) Hauteur d'une map. */,
            TilesetIdRole /**< (int) Indique l'id du tileset assign� � une map. */,
            ProjectPathRole /**< (QString) R�pertoire d'enregistrement d'un projet. */,
            TilesetPropertiesRole /**< (QByteArray) QVector<unsigned char>, s�rialis� dans un QByteArray, qui contient les propri�t�s de chaque tile d'un tileset. */
        };
        /**
          * @brief Enum�ration des types d'�l�ment succeptibles d'�tre rencontr�s par le mod�le ou dans un fichier.
          *
          * Pour conna�tre le type d'�l�ment d'un index, utiliser QModelIndex::data(ProjectWidget::IdCounterRole).toInt(&ok)\n
          * Pour conna�tre le type d'�l�ment d'une partir d'un fichier, utiliser readTypeField.
          * @see readTypeField()
          */
        enum ItemType {Unknow = -1 /**< Type inconnu... */,
                       TilesetTree = 0 /**< Une branche de tilesets. */,
                       TilesTree /**< Une branche de tiles. */,
                       MapsTree /**< Une branche de maps. */,
                       SpritesTree /**< Une branche de sprites (non implant�) */,
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
          * @param undoGroup QUndoGroup � utiliser pour l'undo/redo.
          * @param parent un pointeur vers le QWidget parent.
          */
        ProjectWidget(QUndoGroup *undoGroup, QWidget *parent = 0);

        /**
          * @brief Efface le mod�le.
          *
          * Efface enti�rement le mod�le et tout ce qu'il contenait.
          * @see deleteItem()
          */
        void clear();

        /**
          * @brief Ajoute un tile dans le projet actuel.
          *
          * @param name le nom du nouveau tile.
          * @param t un tile sous forme de QImage index�.
          * @see m_currentProject  @see PixelEditor @see addTileset()
          * @see addMap()
          */
        void addTile(QString name, const QImage &t);

        /**
          * @brief Ajoute un tileset dans le projet actuel.
          *
          * @param name le nom du nouveau tileset.
          * @param tileset les id des tiles du tileset.
          * @param properties les propri�t�s de chaque tile du tileset.
          * @param tilesWidth la largeur des tiles utilis�s.
          * @param tilesHeight la hauteur des tiles utilis�s.
          * @see m_currentProject   @see addTile()    @see addMap()
          */
        void addTileset(QString name, const QVector<int> &tileset, const QVector<unsigned char> &properties, int tilesWidth, int tilesHeight);

        /**
          * @brief Ajoute une map dans le projet actuel.
          *
          * @param name le nom de la nouvelle map.
          * @param map un tableau qui contient les donn�es de la map.
          * @param width la largeur de la map.
          * @param height la hauteur de la map.
          * @param tilesetId l'id du tileset li� � la map, ou -1 si pas de tileset li�.
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
          * @brief Permet d'obtenir l'index d'un tile grace � son id et l'index du projet
          * auquel il appartient.
          *
          * @param tileId l'id du tile recherch�.
          * @param project un index du projet dans lequel le tile d�sir� doit se trouver.
          * @return l'index du tile d�sir� si il existe, ou un index invalide si il n'existe pas.
          * @see projectOfIndex() @see itemFromTilesetId()
          */
        QPersistentModelIndex itemFromTileId (int tileId, QPersistentModelIndex project);

        /**
          * @brief Permet d'obtenir l'index d'un tileset grace � son id et l'index du projet
          * auquel il appartient.
          *
          * @param tilesetId l'id du tileset recherch�.
          * @param project un index du projet dans lequel le tileset d�sir� doit se trouver.
          * @return l'index du tileset d�sir� si il existe, ou un index invalide si il n'existe pas.
          * @see projectOfIndex() @see itemFromTileId()
          */
        QPersistentModelIndex itemFromTilesetId (int tilesetId, QPersistentModelIndex project);

        /**
          * @brief Permet d'obtenir l'index d'une map grace � son id et l'index du projet
          * auquel elle appartient.
          *
          * @param tilesetId l'id de la map recherch�e.
          * @param project un index du projet dans lequel la map d�sir�e doit se trouver.
          * @return l'index de la map d�sir�e si elle existe, ou un index invalide si elle n'existe pas.
          * @see projectOfIndex() @see itemFromTileId()
          */
        QPersistentModelIndex itemFromMapId (int mapId, QPersistentModelIndex project);

        /**
          * @brief Enregistre un projet.
          *
          * Enregistre un projet Tiles Creator dans le format .prj.
          * @param project l'index du projet � enregistrer.
          * @param path le chemin du fichier, ou QString() si on veut laisser la fonction
          *     ouvrir un QFileDialog pour laisser le choix � l'utilisateur.
          * @see getProjectStruct()   @see openProjectFile()
          */
        void writeProjectFile (QPersistentModelIndex project, QString path = QString());

        /**
          * @brief Modifie un tile d�j� existant.
          *
          * Permet de modifier un tile d�j� existant dans le mod�le en fournissant son index
          * et les nouveaux contenus.
          * @param index l'index du tile � modifier.
          * @param name le nouveau nom du tile.
          * @param tile le tile sous forme de QImage index�.
          * @see modifyTileset()  @see modifyMap()  @see PixelEditor
          * @see itemFromTileId()
          */
        void modifyTile (const QPersistentModelIndex &index, QString name, QImage &tile);

        /**
          * @brief Modifie un tileset d�j� existant.
          *
          * Permet de modifier un tile d�j� existant dans le mod�le en fournissant son index
          * et les nouveaux contenus.
          * @param index l'index du tileset � modifier.
          * @param name le nouveau nom du tileset.
          * @param tileset les id des tiles du tileset.
          * @param properties les propri�t�s de chaque tile du tileset.
          * @param tilesWidth la largeur des tiles utilis�s.
          * @param tilesHeight la hauteur des tiles utilis�s.
          * @see modifyTile()  @see modifyMap() @see itemFromTilesetId()
          */
        void modifyTileset (const QPersistentModelIndex &index, QString name, const QVector<int> &tileset, const QVector<unsigned char> &properties, int tilesWidth, int tilesHeight);

        /**
          * @brief Modifie une map d�j� existante.
          *
          * Permet de modifier une map d�j� existante dans le mod�le en fournissant son index
          * et les nouveaux contenus.
          * @param index l'index de la map � modifier.
          * @param name le nom de la nouvelle map.
          * @param map un tableau qui contient les donn�es de la map.
          * @param width la largeur de la map.
          * @param height la hauteur de la map.
          * @param tilesetId l'id du tileset li� � la map, ou -1 si pas de tileset li�.
          * @see modifyTile()  @see modifyTileset()
          */
        void modifyMap(const QPersistentModelIndex &index, QString name, const QVector<unsigned char> &map, int width, int height, int tilesetId=-1);

        /**
          * @brief Modifie un champs de donn�es d'un index, si il est permis de le modifier.
          *
          * modifyData() permet de modifier rapidement une unique valeur d'un champs de donn�es
          * d'un index, en v�rifiant, selon le type de l'index, si il est permis de modifier
          * ce champs. Le cas �ch�ant la fonction retourne false sans n'avoir rien modifi�.\n
          * Les champs autoris�s sont, pour les tiles, tilesets et maps, les champs de donn�es
          * de TilesCreatorRole repr�sentant des �l�ments modifiables via modifyTile(),
          * modifyTileset() et modifyMap(), ainsi que Qt::DisplayRole.
          * @param index index de l'�l�ment � modifier.
          * @param newValue nouvelle valeur du champs � modifier.
          * @param dataId identifiant du champs � modifier.
          * @return true si le champs �tait modifiable pour cet item, false sinon.
          */
        bool modifyData(const QPersistentModelIndex &index, QVariant newValue, int dataId);

        /**
          * @brief Retourne l'index du projet contenant un index donn�.
          *
          * Cette m�thode renvoi, gr�ce � l'index d'un �l�ment du mod�le, un index du projet
          * qui le contient. Cependant, elle ne fonctionne que si l'index pass� est de type suivant :
          * TilesetTree, TilesTree, MapsTree, Tileset, Tile, Map, Project.\n
          * Cette limitation ne devrait pas se remarquer tant que le mod�le est modifi� correctement.
          * @param index l'index contenu dans le projet recherch�.
          * @return l'index du projet contenant l'index pass�, ou un index invalide si l'index n'�tait
          *     pas valide (type inconnu ou index invalide).
          * @see currentProject()
          */
        QPersistentModelIndex projectOfIndex(QModelIndex index);

        /**
          * @brief R�cup�re un index vers chacun des tilesets d'un projet.
          *
          * @param project l'index du projet concern�.
          * @return un QVector contenant l'ensemble des index de chacun des tilesets du projet
          *     dans l'ordre du mod�le. Le QVector est vide si l'index du projet pass� est invalide.
          * @see tilesOfProject()  @see mapsOfProject()
          */
        QVector<QPersistentModelIndex> tilesetsOfProject(QPersistentModelIndex project);

        /**
          * @brief R�cup�re un index vers chacun des tiles d'un projet.
          *
          * @param project l'index du projet concern�.
          * @return un QVector contenant l'ensemble des index de chacun des tiles du projet dans
          *     l'ordre du mod�le. Le QVector est vide si l'index du projet pass� est invalide.
          * @see tilesetsOfProject()  @see mapsOfProject()
          */
        QVector<QPersistentModelIndex> tilesOfProject(QPersistentModelIndex project);

        /**
          * @brief R�cup�re un index vers chacune des maps d'un projet.
          *
          * @param project l'index du projet concern�.
          * @return un QVector contenant l'ensemble des index de chacun des tilesets du projet
          *     dans l'ordre du mod�le. Le QVector est vide si l'index du projet pass� est invalide.
          * @see tilesOfProject()  @see tilesetsOfProject()
          */
        QVector<QPersistentModelIndex> mapsOfProject(QPersistentModelIndex project);

        /**
          * @brief Suprimme un �l�ment du mod�le.
          *
          * Suprimme l'�l�ment du mod�le corespondant � un index donn� et tout ce qu'il contient.
          * @param index l'index de l'�l�ment � suprimmer.
          * @see clear()
          */
        void deleteItem(QModelIndex index);

        /**
          * @brief Transforme un projet en une structure TcProject.
          *
          * @param project l'index du projet � transformer en structure.
          * @return un TcProject contenant l'ensemble du projet pass� ou un TcProject() si l'index
          *     est invalide.
          * @see TcProject
          * @see mergeProjectStruct()
          */
        TcProject getProjectStruct(QPersistentModelIndex project);

        /**
          * @brief Fusionne une structure TcProject avec un projet existant.
          *
          * Toutes les donn�es de la structure TcProject sont ajout�es au projet project.
          * @param toMerge structure � ajouter.
          * @param project projet o� va �tre ajouter toMerge.
          * @see TcProject
          * @see getProjectStruct()
          */
        void mergeProjectStruct(TcProject toMerge, QPersistentModelIndex project);

        /**
          * @brief Indique le type d'un index.
          *
          * @param project l'index dont on cherche le type.
          * @return le type de l'index ou Unknow si l'index est invalide ou qu'il n'a pas de
          *     donn�es de type.
          * @see ItemType
          */
        static ProjectWidget::ItemType typeOfIndex(const QModelIndex &index);

        /**
          * @brief Modifie le QUndoGroup utilis�.
          *
          * @param undoGroup nouveau QUndoGroup � utiliser.
          * @see undoGroup()
          */
        void setUndoGroup(QUndoGroup *undoGroup);

        /**
          * @brief Renvoie le QUndoGroup utilis�.
          *
          * @return le QUndoGroup utilis� actuellement.
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
          * Cette m�thode ouvre un QFileDialog afin de s�lectioner un ou plusieurs fichiers de tile
          * puis ouvre chaque fichier et l'ajoute au projet courrant.
          * @see addTileFromFile()  @see addNewTile()   @see openProjectFile()
          */
        void openTileFile();

        /**
          * @brief Ouvre un projet Tiles Creator et l'ajoute au mod�le.
          *
          * Ouvre un projet situ� par path. Si path est vide, la m�thode ouvre un QFileDialog.
          * @param path chemin du projet � ouvrir.
          * @see addTileFromFile()  @see addProject()
          */
        void openProjectFile (QString path = QString());

        /**
          * @brief Ouvre un fichier de tile et l'ajoute au projet courrant.
          *
          * Ajoute un tile situ� par path. Si path est vide, la m�thode ne fait rien.
          * @param chemin du tile � ouvrir.
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
          * @brief Ajoute un nouveau projet au mod�le.
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
        void closeWindow(); /**< @brief Signal �mis lors de la fermeture de la fen�tre. */
        void requestAddActualTile(); /**< @brief Demande d'ajout du tile en cours d'�dition. */

        /**
          * @brief Signal �mis � chaque fois que l'�l�ment s�lectionn� du tree change.
          * @param index index du nouvel �l�ment s�lectionn�.
          */
        void currentItemChanged(const QPersistentModelIndex &index);

        /**
          * @brief Signal �mis � chaque fois que qu'un �l�ment va �tre supprim�.
          * @param index index de l'�l�ment qui va �tre suprimmer.
          */
        void itemDeleted(QModelIndex index);

        /**
          * @brief Signal �mis � chaque fois qu'un �l�ment est modifi�.
          * @param index index de l'�l�ment modifi�.
          */
        void itemModified(const QPersistentModelIndex &index);

        /**
          * @brief Signal �mis � chaque fois qu'un �l�ment est ajout�.
          * @param index index du nouvel �l�ment.
          */
        void itemAdded(const QPersistentModelIndex &index);

        void lastProjectClosed(); /**< @brief Signal �mis lors de la fermeture du dernier projet. */


    protected:
        QStandardItemModel *model; /**< Mod�le utilis� par le QTreeView. */
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
