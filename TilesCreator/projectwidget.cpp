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

#include "projectwidget.h"

ProjectWidget::ProjectWidget(QUndoGroup *undoGroup, QWidget *parent) : QTreeView(parent)
{
    m_undoGroup = undoGroup;

    setMaximumWidth(250);
    setMinimumWidth(175);
    m_expandTimer = new QTimer;
    m_expandTimer->setSingleShot(true);
    m_scrollTimer = new QTimer;
    m_scrollTimer->setInterval(75);
    m_scrollTimer->setSingleShot(false);

    model = new QStandardItemModel;
    m_currentProject = 0;
    m_dragItem = 0;

    setModel(model);
    header()->hide();
    setContextMenuPolicy(Qt::CustomContextMenu);
    setAcceptDrops(true);

    m_addActualTile = new QAction(QIcon(":/add"), tr("Ajouter le tile actuel"), 0);
    m_addNewTile = new QAction(QIcon(":/page-add"), tr("Ajouter un nouveau tile"), 0);
    m_addTileFromFile = new QAction(tr("Ajouter un tile depuis un fichier..."), 0);
    m_deleteSelected = new QAction(QIcon(":/remove"), tr("Supprimer"), 0);
    m_quitProject = new QAction(QIcon(":/quit"), tr("Quitter le projet"), 0);
    m_addFileToProject = new QAction(QIcon(":/page-add"), tr("Ajouter des fichiers au projet..."), 0);
    m_addNewTileset = new QAction(QIcon(":/page-add"), tr("Ajouter un nouveau tileset"), 0);
    m_addNewProject = new QAction(QIcon(":/page-add"), tr("Ajouter un nouveau projet"), 0);
    m_addProjectFile = new QAction(QIcon(":/cut"), tr("Ajouter un projet depuis un fichier"), 0);
    m_addNewMap = new QAction(QIcon(":/page-add"), tr("Ajouter une nouvelle map"), 0);

    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));
    connect(m_addActualTile, SIGNAL(triggered()), this, SIGNAL(requestAddActualTile()));
    connect(m_addNewTile, SIGNAL(triggered()), this, SLOT(addNewTile()));
    connect(this, SIGNAL(pressed(QModelIndex)), this, SLOT(anyItemSelected(QModelIndex)));
    connect(m_deleteSelected, SIGNAL(triggered()), this, SLOT(deleteItemSelected()));
    connect(m_addTileFromFile, SIGNAL(triggered()), this, SLOT(openTileFile()));
    connect(m_addNewTileset, SIGNAL(triggered()), this, SLOT(addNewTileset()));
    connect(m_expandTimer, SIGNAL(timeout()), this, SLOT(expandUnderDrop()));
    connect(m_quitProject, SIGNAL(triggered()), this, SLOT(deleteItemSelected()));
    connect(m_addNewProject, SIGNAL(triggered()), this, SLOT(addProject()));
    connect(m_addProjectFile, SIGNAL(triggered()), this, SLOT(openProjectFile()));
    connect(m_scrollTimer, SIGNAL(timeout()), this, SLOT(scrollDuringDrag()));
    connect(m_addNewMap, SIGNAL(triggered()), this, SLOT(addNewMap()));
}



QStandardItem* ProjectWidget::p_addTile(QString name, const QImage &t, int id, int row) {
    if (m_currentProject != 0) {
        //Ajouter l'item du tile configuré :
        QStandardItem *item = new QStandardItem(name);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable |Qt::ItemIsDragEnabled);
        item->setData(QVariant(Tile), IndexTypeRole);
        item->setIcon(QIcon(QPixmap::fromImage(t)));
        QVariant tmp_tile = t;
        item->setData(tmp_tile ,TileRole);
        int idCounter = m_currentProject->data(IdCounterRole).toInt();
        if (id >= 0) item->setData(QVariant(id), IdRole);
        else {
            id = idCounter+1;
            item->setData(QVariant(id), IdRole);
        }
        if (id > idCounter) m_currentProject->setData(QVariant(id), IdCounterRole);
        if (row<0 || row >= (m_currentProject->child(TilesTree, 0)->rowCount()-1))
            m_currentProject->child(TilesTree, 0)->appendRow(item);
        else m_currentProject->child(TilesTree, 0)->insertRow(row+1, item);
        emit itemAdded(item->index());
        return item;
    }
    return 0;
}

void ProjectWidget::closeEvent(QCloseEvent *event) {
    emit closeWindow();
    QWidget::closeEvent(event);
}



void ProjectWidget::contextMenu(const QPoint &pos){
    QMenu *menu = new QMenu;
    ItemType type = typeOfIndex(indexAt(pos));
    if (type == Tile) {
        menu->addAction(m_deleteSelected);
        menu->addSeparator();
    }
    if (type == Tileset) {
        menu->addAction(m_deleteSelected);
        menu->addSeparator();
    }
    if (type == Map) {
        menu->addAction(m_deleteSelected);
        menu->addSeparator();
    }
    else if (type == TilesetTree) {
        menu->addAction(m_addNewTileset);
        menu->addSeparator();
    }
    else if (type == TilesTree) {
        menu->addAction(m_addActualTile);
        menu->addAction(m_addNewTile);
        menu->addAction(m_addTileFromFile);
        menu->addSeparator();
    }
    else if (type == MapsTree) {
        menu->addAction(m_addNewMap);
        menu->addSeparator();
    }
    else if (type == Project) {
        menu->addAction(m_addFileToProject);
        menu->addAction(m_quitProject);
        menu->addSeparator();
    }
    menu->addAction(m_addNewProject);
    menu->addAction(m_addProjectFile);
    menu->exec(mapToGlobal(pos));
}



void ProjectWidget::anyItemSelected(QModelIndex index) {
    if (QApplication::mouseButtons().testFlag(Qt::LeftButton)) {
        ItemType type = typeOfIndex(index);
        if (type == Tile || type == Tileset || type == Map)
            if (typeOfIndex(index.parent().parent()) == Project)
                changeCurrentProject(model->itemFromIndex(index.parent().parent()));

        if (type == TilesTree || type == TilesetTree || type == MapsTree)
            if (typeOfIndex(index.parent()) == Project)
                changeCurrentProject(model->itemFromIndex(index.parent()));

        if (type == Project) changeCurrentProject(model->itemFromIndex(index));

        if (type == Tile) {
            //int id = index.data(IdRole).toInt();
        }
        emit currentItemChanged(index);
    }
}


void ProjectWidget::deleteItemSelected() {
    QModelIndex index = currentIndex();
    deleteItem(index);
}


void ProjectWidget::openTileFile() {
     QStringList files = QFileDialog::getOpenFileNames(this, tr("Selectionnez un ou plusieur tile(s)"), QString(), "Fichiers de tile (*.til)");
     for (int i=0; i<files.size(); i++) {
        addTileFromFile(files.at(i));
    }
}


void ProjectWidget::clear() {
    if (m_currentProject != 0) {
        for (int i=0; m_currentProject->child(TilesTree)->rowCount() != 0; i=0)
            m_currentProject->child(TilesTree)->removeRow(i);
        for (int i=0; m_currentProject->child(TilesetTree)->rowCount() != 0; i=0)
            m_currentProject->child(TilesetTree)->removeRow(i);
        for (int i=0; m_currentProject->child(MapsTree)->rowCount() != 0; i=0)
            m_currentProject->child(MapsTree)->removeRow(i);
    }
}


QStandardItem* ProjectWidget::p_addProject(QString name, int idCounter, int tilesetIdCounter, int mapIdCounter,
                                           QString path) {
    //Ajouter tous les items du projet au modèle
    QStandardItem *project = new QStandardItem(name);

    //Mettre les données internes dans l'item du projet :
    project->setData(QVariant(Project), IndexTypeRole);
    project->setData(QVariant(idCounter), IdCounterRole);
    project->setData(QVariant(tilesetIdCounter), TilesetIdCounterRole);
    project->setData(QVariant(mapIdCounter), MapIdCounterRole);
    project->setData(QVariant(path), ProjectPathRole);

    //Ajouter l'item au TreeView :
    model->appendRow(project);
    expand(project->index());

    //Créer la branche Tilesets :
    QStandardItem *tileset = new QStandardItem("Tilesets");
    tileset->setFlags(Qt::ItemIsEnabled | Qt::ItemIsDropEnabled);
    tileset->setData(QVariant(TilesetTree), IndexTypeRole);
    project->appendRow(tileset);

    //Créer la branche Tiles :
    QStandardItem *tiles = new QStandardItem("Tiles");
    tiles->setFlags(Qt::ItemIsEnabled | Qt::ItemIsDropEnabled);
    tiles->setData(QVariant(TilesTree), IndexTypeRole);
    project->appendRow(tiles);

    //Créer la branche Maps :
    QStandardItem *maps = new QStandardItem("Maps");
    maps->setFlags(Qt::ItemIsEnabled | Qt::ItemIsDropEnabled);
    maps->setData(QVariant(MapsTree), IndexTypeRole);
    project->appendRow(maps);

    //Créer la branche Sprites :
    /*QStandardItem *sprites = new QStandardItem("Sprites");
    sprites->setFlags(Qt::ItemIsEnabled | Qt::ItemIsDropEnabled);
    sprites->setData(QVariant(SpritesTree), IndexTypeRole);
    project->appendRow(sprites);*/

    //Créer la stack d'undo/redo :
    QUndoStack *stack = new QUndoStack();
    m_undoStacks.append(stack);
    m_undoGroup->addStack(stack);

    changeCurrentProject(project);
    emit itemAdded(project->index());
    return project;
}


ProjectWidget::ItemType ProjectWidget::typeOfIndex(const QModelIndex &index) {
    QVariant tmp(index.data(IndexTypeRole));
    if (tmp != QVariant()) {
        bool isOk;
        ItemType type = static_cast<ProjectWidget::ItemType>(tmp.toInt(&isOk));
        if (isOk) return type;
    }
    return Unknow;
}


void ProjectWidget::changeCurrentProject(QStandardItem *project) {
    if (project != m_currentProject) {
        //Enlever le gras de la font du projet actif :
        if (m_currentProject != 0) {
            QFont tmp(m_currentProject->font());
            tmp.setBold(false);
            m_currentProject->setFont(tmp);
        }
        //Atribuer ce projet en tant que projet actif et mettre
        //  en gras sa font :
        m_currentProject = project;
        QFont tmp(project->font());
        tmp.setBold(true);
        project->setFont(tmp);

        //Rendre active sa stack d'undo/redo :
        m_undoGroup->setActiveStack(m_undoStacks.at(project->row()));
    }
}


QPersistentModelIndex ProjectWidget::currentProject() {
    if (m_currentProject != 0)
        return m_currentProject->index();
    else return QPersistentModelIndex();
}


void ProjectWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_oldPos = event->pos();
        m_dragItem = model->itemFromIndex(indexAt(m_oldPos));
    }
    else m_oldPos = QPoint();
    if (indexAt(event->pos()) != QModelIndex()) changeCurrentProject(model->itemFromIndex(projectOfIndex(indexAt(event->pos()))));
    QTreeView::mousePressEvent(event);
}


void ProjectWidget::mouseMoveEvent(QMouseEvent *event) {
    if (m_oldPos != QPoint()) {
        if (m_dragItem != 0 && m_dragItem->isDragEnabled()) {
            QPoint tmp = m_oldPos - event->pos();
            if (tmp.manhattanLength() > QApplication::startDragDistance()) {
                QDrag *drag = new QDrag(this);
                QMimeData *mimeData = new QMimeData();

                if (typeOfIndex(m_dragItem->index()) == Tile) {
                    QImage image (m_dragItem->data(TileRole).value<QImage>());
                    QByteArray data, metadata;
                    QDataStream dataStream(&data, QIODevice::WriteOnly);
                    dataStream << image;
                    mimeData->setData("application/x-tile", data);
                    QDataStream metadataStream(&metadata, QIODevice::WriteOnly);
                    metadataStream << m_dragItem->data(IdRole).toInt()
                            << projectOfIndex(m_dragItem->index()).row();
                    /*QMessageBox::information(this, "Drag...", "Drag ready.\nProject ID : "
                        + QString::number(reinterpret_cast<int>(projectOfIndex(m_dragItem->index()))));*/
                    mimeData->setData("application/x-metatile", metadata);
                    QByteArray output;
                    QBuffer outputBuffer(&output);
                    outputBuffer.open(QIODevice::WriteOnly);
                    image.save(&outputBuffer, "PNG");
                    mimeData->setData("image/png", output);
                    mimeData->setText(m_dragItem->data(Qt::DisplayRole).toString());
                    drag->setPixmap(QPixmap::fromImage(image));
                }

                drag->setMimeData(mimeData);
                Qt::DropAction action = drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction);
                if (action == Qt::MoveAction) deleteItem(m_dragItem->index());
            }
        }
    }
    QTreeView::mouseMoveEvent(event);
}


void ProjectWidget::dragEnterEvent(QDragEnterEvent *event) {
        event->acceptProposedAction();
}


void ProjectWidget::dragLeaveEvent(QDragLeaveEvent *event) {
    m_expandTimer->stop();
    m_scrollTimer->stop();
}


void ProjectWidget::dragMoveEvent(QDragMoveEvent *event) {
    QModelIndex index = indexAt(event->pos());
    ItemType type = typeOfIndex(index);
    if (event->pos().y() < 10) {
        upScrolling = true;
        if (!m_scrollTimer->isActive()) m_scrollTimer->start();
    }
    else if (event->pos().y() > height()-20) {
        upScrolling = false;
        if (!m_scrollTimer->isActive()) m_scrollTimer->start();
    }
    else m_scrollTimer->stop();

    if (m_oldIndexUnderDrop == QModelIndex()) {
        if (type == TilesTree || type == TilesetTree || type == MapsTree
            || type == Project || (type == Tileset && event->mimeData()->hasFormat("application/x-tile"))) {
            m_oldIndexUnderDrop = index;
            m_expandTimer->start(750);
        }
    }
    else {
        if (index != m_oldIndexUnderDrop) m_oldIndexUnderDrop = QModelIndex();
    }
    if (event->mimeData()->hasFormat("application/x-tile")) {
        if (type == TilesTree || type == Tile || type == Project) {
            event->acceptProposedAction();
        }
        else event->ignore();
    }
    else event->ignore();

}


void ProjectWidget::dropEvent(QDropEvent *event) {
    QModelIndex index = indexAt(event->pos());
    QStandardItem *project = model->itemFromIndex(projectOfIndex(index));

    event->setDropAction(event->proposedAction());

    if (event->mimeData()->hasFormat("application/x-tile")) {
        event->setDropAction(Qt::CopyAction);
        int idOfTile = -1;
        if (event->mimeData()->hasFormat("application/x-metatile")) {
            QByteArray metadata(event->mimeData()->data("application/x-metatile"));
            QDataStream metastream(&metadata, QIODevice::ReadOnly);
            int id, projectRow;
            metastream >> id >> projectRow;
            if (event->source() == this) {
                if (projectRow == project->row()) {
                    event->setDropAction(Qt::MoveAction);
                    idOfTile = id;
                }
            }
        }
        QByteArray data(event->mimeData()->data("application/x-tile"));
        QDataStream stream(&data, QIODevice::ReadOnly);
        QImage image;
        stream >> image;
        changeCurrentProject(project);
        ItemType type = typeOfIndex(indexAt(event->pos()));
        int upperRow = -1;
        if (type == Tile) upperRow = indexAt(event->pos()).row();
        p_addTile(event->mimeData()->text(), image, idOfTile, upperRow);
        event->accept();
        m_scrollTimer->stop();
        m_expandTimer->stop();
    }
}


void ProjectWidget::keyPressEvent(QKeyEvent *event) {
    if (event->matches(QKeySequence::Delete)) {
        QModelIndexList selected = selectedIndexes();
        for (int i=0; i<selected.size(); i++) {
            deleteItem(selected.at(i));
        }
    }
    QTreeView::keyPressEvent(event);
}



QPersistentModelIndex ProjectWidget::projectOfIndex(QModelIndex index) {
    ItemType type = typeOfIndex(index);
    if (type == Tile || type == Tileset || type == Map)
        return index.parent().parent();
    else if (type == TilesTree || type == TilesetTree || type == MapsTree)
        return index.parent();
    else if (type == Project)
        return index;
    else return QPersistentModelIndex();
}


void ProjectWidget::expandUnderDrop() {
    setExpanded(m_oldIndexUnderDrop, !isExpanded(m_oldIndexUnderDrop));
    if (typeOfIndex(m_oldIndexUnderDrop) == Tileset) emit currentItemChanged(m_oldIndexUnderDrop);
    m_oldIndexUnderDrop = QModelIndex();
}


QStandardItem* ProjectWidget::p_addTileset(QString name, const QVector<int> &tileset, const QVector<unsigned char> &properties, int tilesWidth, int tilesHeight, int id) {
    if (m_currentProject != 0) {
        QStandardItem *item = new QStandardItem(name);
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << tileset;
        item->setData(QVariant(data), TilesetRole);
        QByteArray pData;
        QDataStream pStream(&pData, QIODevice::WriteOnly);
        pStream << properties;
        item->setData(QVariant(pData), TilesetPropertiesRole);
        item->setData(QVariant(tilesWidth), TilesWidthRole);
        item->setData(QVariant(tilesHeight), TilesHeightRole);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable |Qt::ItemIsDragEnabled);
        item->setData(QVariant(Tileset), IndexTypeRole);
        int idCounter = m_currentProject->data(TilesetIdCounterRole).toInt();
        if (id >= 0) item->setData(QVariant(id), IdRole);
        else {
            id = idCounter+1;
            item->setData(QVariant(id), IdRole);
        }
        if (id > idCounter) m_currentProject->setData(QVariant(id), TilesetIdCounterRole);
        m_currentProject->child(TilesetTree)->appendRow(item);
        emit itemAdded(item->index());
        return item;
    }
    return 0;
}



QStandardItem* ProjectWidget::p_addMap(QString name, const QVector<unsigned char> &map, int mapWidth, int mapHeight, int tilesetId, int id) {
    if (m_currentProject != 0) {
        QStandardItem *item = new QStandardItem(name);
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << map;
        item->setData(data, MapRole);
        item->setData(mapWidth, MapWidthRole);
        item->setData(mapHeight, MapHeightRole);
        item->setData(tilesetId, TilesetIdRole);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        item->setData(Map, IndexTypeRole);
        int idCounter = m_currentProject->data(MapIdCounterRole).toInt();
        if (id >= 0) item->setData(id, IdRole);
        else {
            id = idCounter+1;
            item->setData(id, IdRole);
        }
        if (id > idCounter) m_currentProject->setData(id, MapIdCounterRole);
        m_currentProject->child(MapsTree)->appendRow(item);
        emit itemAdded(item->index());
        return item;
    }
    return 0;
}



void ProjectWidget::deleteItem(QModelIndex index) {
    QModelIndex tmpIndex = index;
    if (typeOfIndex(index) == Tile) {
        emit itemDeleted(index);
        model->itemFromIndex(index)->parent()->removeRow(index.row());
    }
    else if (typeOfIndex(index) == Project) {
        emit itemDeleted(index);
        m_undoGroup->removeStack(m_undoStacks.at(index.row()));
        delete m_undoStacks.at(index.row());
        m_undoStacks.removeAt(index.row());
        model->invisibleRootItem()->removeRow(index.row());
        m_currentProject = 0;
        if (model->invisibleRootItem()->rowCount() == 0) emit lastProjectClosed();
        else changeCurrentProject(model->item(0));
    }
    else if (typeOfIndex(index) == Tileset) {
        emit itemDeleted(tmpIndex);
        model->itemFromIndex(index)->parent()->removeRow(index.row());
    }
    else if (typeOfIndex(index) == Map) {
        emit itemDeleted(index);
        model->itemFromIndex(index)->parent()->removeRow(index.row());
    }
}


QPersistentModelIndex ProjectWidget::itemFromTileId (int tileId, QPersistentModelIndex project) {
    QStandardItem *tilesTree = model->itemFromIndex(project.child(TilesTree, 0));
    int numRow = tilesTree->rowCount();
    for (int i = 0; i<numRow; i++) {
        if (tilesTree->child(i)->data(IdRole).toInt() == tileId)
            return tilesTree->child(i)->index();
    }
    return QPersistentModelIndex();
}


QPersistentModelIndex ProjectWidget::itemFromMapId (int mapId, QPersistentModelIndex project) {
    QStandardItem *mapsTree = model->itemFromIndex(project.child(MapsTree, 0));
    int numRow = mapsTree->rowCount();
    for (int i = 0; i<numRow; i++) {
        if (mapsTree->child(i)->data(IdRole).toInt() == mapId)
            return mapsTree->child(i)->index();
    }
    return QPersistentModelIndex();
}

void ProjectWidget::writeProjectFile (QPersistentModelIndex project, QString path) {
    if (project.isValid() && typeOfIndex(project) == Project) {
        if (path.isEmpty()) {
            path = project.data(ProjectPathRole).toString();
            if (path.isEmpty() || !QFile::exists(path))
                path = QFileDialog::getSaveFileName(this, tr("Enregistrer un projet..."), QString(), tr("Projets Tiles Creator (*.prj)"));
        }
        if (!path.isEmpty()) {
            QStandardItem *iProject = model->itemFromIndex(project);
            QFile file(path);
            if (file.open(QIODevice::WriteOnly)) {
                QByteArray main_header("##TCPRJ#v1.0##\n##Project:#");
                file.write(main_header);
                QString name = project.data(Qt::DisplayRole).toString();
                file.write(name.toAscii());
                int icount = project.data(ProjectWidget::IdCounterRole).toInt();
                char count[3] = {'#', icount/256, icount%256};
                file.write(count, 3);
                icount = project.data(ProjectWidget::TilesetIdCounterRole).toInt();
                char count2[3] = {'#', icount/256, icount%256};
                file.write(count2, 3);
                icount = project.data(ProjectWidget::MapIdCounterRole).toInt();
                char count3[3] = {'#', icount/256, icount%256};
                file.write(count3, 3);
                file.write("##\n");
                for (int i=0; i < iProject->child(TilesTree)->rowCount(); i++) {
                    QByteArray name(iProject->child(TilesTree)->child(i)->data(Qt::DisplayRole).toString().toAscii());
                    int id = iProject->child(TilesTree)->child(i)->data(ProjectWidget::IdRole).value<int>();
                    QImage pix = iProject->child(TilesTree)->child(i)->data(ProjectWidget::TileRole).value<QImage>();
                    QByteArray array(PixelEditor::toGrayTileArray(pix));
                    char w = pix.width();
                    char h = pix.height();
                    file.write("##TIL#");
                    file.write(name);
                    file.write("#");
                    char tmp;
                    tmp = id/256;
                    file.write(&tmp, 1);
                    tmp = id%256;
                    file.write(&tmp, 1);
                    file.write("#");
                    file.write(&w, 1);
                    file.write("#");
                    file.write(&h, 1);
                    file.write("#");
                    file.write(array);
                    file.write("##\n");
                }
                for (int i=0; i<iProject->child(TilesetTree)->rowCount(); i++) {
                    QByteArray name(iProject->child(TilesetTree)->child(i)->data(Qt::DisplayRole).toString().toAscii());
                    int id = iProject->child(TilesetTree)->child(i)->data(ProjectWidget::IdRole).value<int>();
                    QByteArray tilesetData(iProject->child(TilesetTree)->child(i)->data(ProjectWidget::TilesetRole).toByteArray());
                    QVector<int> tileset;
                    QDataStream stream(&tilesetData, QIODevice::ReadOnly);
                    stream >> tileset;
                    QByteArray pData(iProject->child(TilesetTree)->child(i)->data(ProjectWidget::TilesetPropertiesRole).toByteArray());
                    QVector<unsigned char> properties;
                    QDataStream pStream(&pData, QIODevice::ReadOnly);
                    pStream >> properties;
                    char w = iProject->child(TilesetTree)->child(i)->data(ProjectWidget::TilesWidthRole).value<int>();
                    char h = iProject->child(TilesetTree)->child(i)->data(ProjectWidget::TilesHeightRole).value<int>();
                    file.write("##TLS#");
                    file.write(name);
                    file.write("#");
                    char tmp;
                    tmp = id/256;
                    file.write(&tmp, 1);
                    tmp = id%256;
                    file.write(&tmp, 1);
                    file.write("#");
                    file.write(&w, 1);
                    file.write("#");
                    file.write(&h, 1);
                    file.write("#");
                    tmp = tileset.size();
                    file.write(&tmp, 1);
                    file.write("#");
                    for (int i=0; i<tileset.size(); i++) {
                        int tmpInt = tileset.at(i);
                        tmp = tmpInt >> 24;
                        file.write(&tmp, 1);
                        tmp = (tmpInt & 0x00FF0000) >> 16;
                        file.write(&tmp, 1);
                        tmp = (tmpInt & 0x0000FF00) >> 8;
                        file.write(&tmp, 1);
                        tmp = (tmpInt & 0x000000FF);
                        file.write(&tmp, 1);
                    }
                    file.write("#$:#");
                    for (int i=0; i<tileset.size(); i++) file.putChar(properties.at(i));
                    file.write("##\n");
                }
                for (int i=0; i<iProject->child(MapsTree)->rowCount(); i++) {
                    //Récupération des paramètres de la map :
                    QStandardItem *item = iProject->child(MapsTree)->child(i);
                    QByteArray name(item->data(Qt::DisplayRole).toString().toAscii());
                    int id = item->data(ProjectWidget::IdRole).value<int>();
                    QByteArray mapData(item->data(ProjectWidget::MapRole).toByteArray());
                    QVector<unsigned char> map;
                    QDataStream stream(&mapData, QIODevice::ReadOnly);
                    stream >> map;
                    unsigned char w = item->data(ProjectWidget::MapWidthRole).value<int>();
                    unsigned char h = item->data(ProjectWidget::MapHeightRole).value<int>();
                    int tilesetId = item->data(ProjectWidget::TilesetIdRole).toInt();

                    //Ecriture dans le fichier :
                    if (map.size() != h*w) {
                        QMessageBox::information(this, tr("Erreur lors de l'écriture"), "Map " + item->data(Qt::DisplayRole).toString()
                            + tr(" invalide.\nLa map n'a pas été écrite dans le fichier."));
                    }
                    else {
                        file.write("##MAP#");
                        file.write(name);
                        file.write("#");
                        char tmp;
                        tmp = id/256;
                        file.write(&tmp, 1);
                        tmp = id%256;
                        file.write(&tmp, 1);
                        file.write("#");
                        tmp = tilesetId/256;
                        file.write(&tmp, 1);
                        tmp = tilesetId%256;
                        file.write(&tmp, 1);
                        file.write("#");
                        file.write((char*)&w, 1);
                        file.write("#");
                        file.write((char*)&h, 1);
                        file.write("#");
                        for (int i=0; i<map.size(); i++) {
                            tmp = static_cast<char> (map[i]);
                            file.write(&tmp, 1);
                        }
                        file.write("##\n");
                    }
                }
                file.write("##END##");
                file.close();
            }
            else QMessageBox::information(this, tr("Erreur lors de l'enregistrement"), tr("Impossible d'enregistrer "
                                          "le projet.\nVérifiez si l'emplacement n'est pas protégé en écriture."));
            iProject->setData(path, ProjectPathRole);
        }
    }
}


QByteArray ProjectWidget::readExpField(QFile &file) {
    QByteArray tmp;
    char actual = 0;
    file.read(&actual, 1);
    while (actual != '#') {
        tmp.append(actual);
        file.read(&actual, 1);
    }
    return tmp;
}


ProjectWidget::ItemType ProjectWidget::readTypeField(QFile &file) {
    int oldPos = file.pos();
    QByteArray type = file.read(6);
//    QMessageBox::information(this, "Index en cours...", type);
    if (type[0] == '#' && type[1] == '#' && type[5] == '#') {
        if (type[2] == 'T' && type[3] == 'I' && type[4] == 'L') return Tile;
        if (type[2] == 'T' && type[3] == 'L' && type[4] == 'S') return Tileset;
        if (type[2] == 'M' && type[3] == 'A' && type[4] == 'P') return Map;
        if (type[2] == 'E' && type[3] == 'N' && type[4] == 'D') return End;
    }
    file.seek(oldPos);
    return Unknow;
}


void ProjectWidget::openProjectFile (QString path) {
    if (path.isEmpty())
        path = QFileDialog::getOpenFileName(this, tr("Charger un projet"), QString(), tr("Projets Tiles Creator (*.prj)"));
    if (!path.isEmpty()) {
        QFile file(path);
        if (file.open(QIODevice::ReadOnly)) {
            char main_header[16];
            file.read(main_header, 15);
            if (main_header[9] == '1' && main_header[10] == '.' && main_header[11] == '0') {
                QByteArray b;
                file.seek(file.pos()+11);
                b = readExpField(file);
                QString projectName(b);
                int icompt, iTilesetCompt, iMapCompt;
                char compt[2];
                file.read(compt, 2);
                file.seek(file.pos()+1);
                icompt = (unsigned char)compt[1];
                icompt += (unsigned char)compt[0]*256;
                file.read(compt, 2);
                file.seek(file.pos()+1);
                iTilesetCompt = (unsigned char)compt[1];
                iTilesetCompt += (unsigned char)compt[0]*256;
                file.read(compt, 2);
                file.seek(file.pos()+1);
                iMapCompt = (unsigned char)compt[1];
                iMapCompt += (unsigned char)compt[0]*256;
                p_addProject(projectName, icompt, iTilesetCompt, iMapCompt, path);
                file.seek(file.pos()+2);
                ItemType actual;
                while ((actual = readTypeField(file)) != End && actual != Unknow) {
                    if (actual == Tile) {
                        QString tileName(readExpField(file));
                        char tmp;
                        int id;
                        file.read(&tmp, 1);
                        id=(unsigned char)tmp*256;
                        file.read(&tmp, 1);
                        id+=(unsigned char)tmp;
                        file.seek(file.pos()+1);
                        unsigned char w;
                        file.read(&tmp, 1);
                        file.seek(file.pos()+1);
                        w = tmp;
                        unsigned char h;
                        file.read(&tmp, 1);
                        file.seek(file.pos()+1);
                        h = tmp;
                        //QMessageBox::information(this, "Dimensions :", "Id :"+QString::number(id)+"\nPosition :"+QString::number(w)+" ; "+QString::number(h));
                        b = file.read(h*w);
                        file.seek(file.pos()+3);
                        p_addTile(tileName, PixelEditor::fromGrayTileArray(b, w, h), id);
                    }
                    else if (actual == Tileset) {
                        QString tilesetName(readExpField(file));
                        char tmp;
                        int id, arraySize;
                        file.read(&tmp, 1);
                        id=(unsigned char)tmp*256;
                        file.read(&tmp, 1);
                        id+=(unsigned char)tmp;
                        file.seek(file.pos()+1);
                        unsigned char w;
                        file.read(&tmp, 1);
                        file.seek(file.pos()+1);
                        w = tmp;
                        unsigned char h;
                        file.read(&tmp, 1);
                        file.seek(file.pos()+1);
                        h = tmp;
                        file.read(&tmp, 1);
                        arraySize=(unsigned char)tmp;
                        file.seek(file.pos()+1);
                        b = file.read(arraySize*4);
                        file.seek(file.pos()+1);
                        QVector<int> list;
                        list.resize(arraySize);
                        for (int i=0; i<arraySize; i++) {
                            int tmpInt;
                            tmpInt = b.at(i*4) << 24;
                            tmpInt += b.at(i*4+1) << 16;
                            tmpInt += b.at(i*4+2) << 8;
                            tmpInt += b.at(i*4+3);
                            list[i] = tmpInt;
                        }
                        b = file.read(2);
                        QVector<unsigned char> properties;
                        properties.resize(arraySize);
                        if (b.at(0) == '$' && b.at(1) == ':') {
                            file.seek(file.pos()+1);
                            b = file.read(arraySize);
                            for (int i=0; i < arraySize; i++) properties[i] = b[i];
                            file.seek(file.pos()+3);
                        }
                        p_addTileset(tilesetName, list, properties, w, h, id);
                    }
                    else if (actual == Map) {
                        QString mapName(readExpField(file));
                        char tmp;
                        int id, tilesetId;
                        file.read(&tmp, 1);
                        id=(unsigned char)tmp*256;
                        file.read(&tmp, 1);
                        id+=(unsigned char)tmp;
                        file.seek(file.pos()+1);
                        file.read(&tmp, 1);
                        tilesetId=(unsigned char)tmp*256;
                        file.read(&tmp, 1);
                        tilesetId+=(unsigned char)tmp;
                        file.seek(file.pos()+1);
                        unsigned char w;
                        file.read(&tmp, 1);
                        file.seek(file.pos()+1);
                        w = tmp;
                        unsigned char h;
                        file.read(&tmp, 1);
                        file.seek(file.pos()+1);
                        h = tmp;
                        b = file.read(w*h);
                        file.seek(file.pos()+3);
                        QVector<unsigned char> map;
                        map.resize(h*w);
                        for (int i=0; i < h*w; i++) map[i] = b[i];
                        p_addMap(mapName, map, w, h, tilesetId, id);
                    }
                }
            }
        }
    }
}


void ProjectWidget::addTileFromFile (QString path) {
    if (!path.isEmpty()) {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        if (file.isOpen()) {
            QByteArray bitmap = file.read(32);
            QString name = path.section('/', -1).section('.', 0, 0);
            addTile(name, PixelEditor::fromNBBitmap(bitmap, 16, 16));
        }
    }
}


void ProjectWidget::addProject(QString name) {
    QModelIndex index = p_addProject(name)->index();
    emit currentItemChanged(index);
}

void ProjectWidget::addTile(QString name, const QImage &t) {
    emit currentItemChanged(p_addTile(name, t)->index());
}

void ProjectWidget::addTileset(QString name, const QVector<int> &tileset, const QVector<unsigned char> &properties, int tilesWidth, int tilesHeight) {
    emit currentItemChanged(p_addTileset(name, tileset, properties, tilesWidth, tilesHeight)->index());
}

void ProjectWidget::addMap(QString name, const QVector<unsigned char> &map, int mapWidth, int mapHeight, int tilesetId) {
    emit currentItemChanged(p_addMap(name, map, mapWidth, mapHeight, tilesetId)->index());
}


void ProjectWidget::modifyTile (const QPersistentModelIndex &index, QString name, QImage &tile) {
    if (typeOfIndex(index) == Tile) {
        QStandardItem *item = model->itemFromIndex(index);
        if (item != 0) {
            QVariant tmp_pix = tile;
            item->setText(name);
            item->setData(tmp_pix, ProjectWidget::TileRole);
            item->setIcon(QIcon(QPixmap::fromImage(tile)));
            emit itemModified(index);
        }
    }
}


void ProjectWidget::modifyTileset (const QPersistentModelIndex &index, QString name, const QVector<int> &tileset, const QVector<unsigned char> &properties, int tilesWidth, int tilesHeight) {
    if (typeOfIndex(index) == Tileset) {
        QStandardItem *item = model->itemFromIndex(index);
        if (item != 0) {
            item->setText(name);
            item->setData(tilesHeight, TilesHeightRole);
            item->setData(tilesWidth, TilesWidthRole);
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream << tileset;
            item->setData(data, TilesetRole);
            QByteArray pData;
            QDataStream pStream(&pData, QIODevice::WriteOnly);
            pStream << properties;
            item->setData(QVariant(pData), TilesetPropertiesRole);
            emit itemModified(index);
        }
    }
}


void ProjectWidget::modifyMap(const QPersistentModelIndex &index, QString name, const QVector<unsigned char> &map, int mapWidth, int mapHeight, int tilesetId) {
    if (index.isValid() && typeOfIndex(index) == Map) {
        QStandardItem *item = model->itemFromIndex(index);
        if (item != 0) {
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream << map;
            item->setData(data, MapRole);
            item->setData(name, Qt::DisplayRole);
            item->setData(mapWidth, MapWidthRole);
            item->setData(mapHeight, MapHeightRole);
            item->setData(tilesetId, TilesetIdRole);
            emit itemModified(index);
        }
    }
}


bool ProjectWidget::modifyData(const QPersistentModelIndex &index, QVariant newValue, int dataId) {
    ItemType type = typeOfIndex(index);
    if ((type==Tile && (dataId==TileRole || dataId==Qt::DisplayRole)) ||
        (type==Tileset && (dataId==TilesetRole || dataId==TilesWidthRole || dataId==TilesHeightRole || dataId==TilesetPropertiesRole || dataId==Qt::DisplayRole)) ||
        (type==Map && (dataId==MapRole || dataId==MapWidthRole || dataId==MapHeightRole || dataId==TilesetIdRole || dataId==Qt::DisplayRole)))
    {
        QStandardItem *item = model->itemFromIndex(index);
        item->setData(newValue, dataId);
        if (type==Tile && dataId==ProjectWidget::TileRole)
            item->setIcon(QIcon(QPixmap::fromImage(newValue.value<QImage>())));
        emit itemModified(index);
        return true;
    }
    return false;
}


void ProjectWidget::addNewTile() {
    QImage tmp(16, 16, QImage::Format_Indexed8);
    TC::setColorsIndexes(&tmp);
    tmp.fill(TC::White);
    addTile(tr("Nouveau Tile"), tmp);
}

void ProjectWidget::addNewTileset() {
    QVector<int> tmp;
    tmp.resize(6);
    QVector<unsigned char> tmp2;
    tmp2.resize(6);
    addTileset(tr("Nouveau Tileset"), tmp, tmp2, 16, 16);
}


void ProjectWidget::addNewMap() {
    QVector<unsigned char> tmp;
    tmp.resize(4*4);
    addMap(tr("Nouvelle Map"), tmp, 4, 4);
}


void ProjectWidget::scrollDuringDrag() {
    if (upScrolling)
        verticalScrollBar()->setValue(verticalScrollBar()->value()-1);
    else
        verticalScrollBar()->setValue(verticalScrollBar()->value()+1);
}


QVector<QPersistentModelIndex> ProjectWidget::tilesetsOfProject(QPersistentModelIndex project) {
    QVector<QPersistentModelIndex> ret;
    if (project.isValid() && typeOfIndex(project) == Project) {
        QStandardItem *item = model->itemFromIndex(project.child(TilesetTree, 0));
        for (int i=0; i<item->rowCount(); i++)
            ret.append(item->child(i)->index());
    }
    return ret;
}


QVector<QPersistentModelIndex> ProjectWidget::tilesOfProject(QPersistentModelIndex project) {
    QVector<QPersistentModelIndex> ret;
    if (project.isValid() && typeOfIndex(project) == Project) {
        QStandardItem *item = model->itemFromIndex(project.child(TilesTree, 0));
        for (int i=0; i<item->rowCount(); i++)
            ret.append(item->child(i)->index());
    }
    return ret;
}


QVector<QPersistentModelIndex> ProjectWidget::mapsOfProject(QPersistentModelIndex project) {
    QVector<QPersistentModelIndex> ret;
    if (project.isValid() && typeOfIndex(project) == Project) {
        QStandardItem *item = model->itemFromIndex(project.child(MapsTree, 0));
        for (int i=0; i<item->rowCount(); i++)
            ret.append(item->child(i)->index());
    }
    return ret;
}


QPersistentModelIndex ProjectWidget::itemFromTilesetId (int tilesetId, QPersistentModelIndex project) {
    QStandardItem *tilesetTree = model->itemFromIndex(project.child(TilesetTree, 0));
    int numRow = tilesetTree->rowCount();
    for (int i = 0; i<numRow; i++) {
        if (tilesetTree->child(i)->data(IdRole).toInt() == tilesetId)
            return tilesetTree->child(i)->index();
    }
    return QPersistentModelIndex();
}


void ProjectWidget::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        m_addActualTile->setText(tr("Ajouter le tile actuel"));
        m_addNewTile->setText(tr("Ajouter un nouveau tile"));
        m_addTileFromFile->setText(tr("Ajouter un tile depuis un fichier..."));
        m_deleteSelected->setText(tr("Supprimer"));
        m_quitProject->setText(tr("Quitter le projet"));
        m_addFileToProject->setText(tr("Ajouter des fichiers au projet..."));
        m_addNewTileset->setText(tr("Ajouter un nouveau tileset"));
        m_addNewProject->setText(tr("Ajouter un nouveau projet"));
        m_addProjectFile->setText(tr("Ajouter un projet depuis un fichier"));
        m_addNewMap->setText(tr("Ajouter une nouvelle map"));
    }
    QTreeView::changeEvent(event);
}


TcProject ProjectWidget::getProjectStruct(QPersistentModelIndex project) {
    TcProject ret;
    if (project.isValid() && typeOfIndex(project) == Project) {
        QStandardItem *iProject = model->itemFromIndex(project);
        ret.name = project.data(Qt::DisplayRole).toString();
        for (int i=0; i < iProject->child(TilesTree)->rowCount(); i++) {
            ret.tiles.append(TcTile());
            ret.tiles[ret.tiles.size()-1].name = iProject->child(TilesTree)->child(i)->data(Qt::DisplayRole).toString();
            ret.tiles[ret.tiles.size()-1].tile = iProject->child(TilesTree)->child(i)->data(ProjectWidget::TileRole).value<QImage>();
        }
        for (int i=0; i<iProject->child(TilesetTree)->rowCount(); i++) {
            ret.tilesets.append(TcTileset());
            ret.tilesets[ret.tilesets.size()-1].name = iProject->child(TilesetTree)->child(i)->data(Qt::DisplayRole).toString();
            QByteArray tilesetData(iProject->child(TilesetTree)->child(i)->data(ProjectWidget::TilesetRole).toByteArray());
            QVector<int> tileset;
            QDataStream stream(&tilesetData, QIODevice::ReadOnly);
            stream >> tileset;
            for (int j=0; j<tileset.size(); j++) {
                int tmpTile;
                if (tileset.at(j)>=0) {
                    QPersistentModelIndex tmpIndex = itemFromTileId(tileset.at(j), project);
                    if (tmpIndex.isValid()) tmpTile = tmpIndex.row();
                    else tmpTile = -1;
                }
                else tmpTile = -1;
                ret.tilesets[ret.tilesets.size()-1].tiles.append(tmpTile);
            }
            QByteArray pData(iProject->child(TilesetTree)->child(i)->data(ProjectWidget::TilesetPropertiesRole).toByteArray());
            QVector<unsigned char> properties;
            QDataStream pStream(&pData, QIODevice::ReadOnly);
            pStream >> properties;
            ret.tilesets[ret.tilesets.size()-1].properties = properties.toList();
            ret.tilesets[ret.tilesets.size()-1].tilesSize.setWidth(
                    iProject->child(TilesetTree)->child(i)->data(ProjectWidget::TilesWidthRole).value<int>());
            ret.tilesets[ret.tilesets.size()-1].tilesSize.setHeight(
                    iProject->child(TilesetTree)->child(i)->data(ProjectWidget::TilesHeightRole).value<int>());
        }
        for (int i=0; i<iProject->child(MapsTree)->rowCount(); i++) {
            ret.maps.append(TcMap());
            QStandardItem *item = iProject->child(MapsTree)->child(i);
            ret.maps[ret.maps.size()-1].name = item->data(Qt::DisplayRole).toString();
            QByteArray mapData(item->data(ProjectWidget::MapRole).toByteArray());
            QVector<unsigned char> map;
            QDataStream stream(&mapData, QIODevice::ReadOnly);
            stream >> map;
            int w = item->data(ProjectWidget::MapWidthRole).value<int>();
            ret.maps[ret.maps.size()-1].map = Array2D<unsigned char>(map.toStdVector(), w);
            int tilesetId = item->data(ProjectWidget::TilesetIdRole).toInt();
            int tmpTileset;
            if (tilesetId>=0) {
                QPersistentModelIndex tmpIndex = itemFromTilesetId(tilesetId, project);
                if (tmpIndex.isValid()) tmpTileset = tmpIndex.row();
                else tmpTileset = 0;
            }
            else tmpTileset = 0;
            ret.maps[ret.maps.size()-1].tilesetId = tmpTileset;
        }
    }
    return ret;
}


void ProjectWidget::mergeProjectStruct(TcProject toMerge, QPersistentModelIndex project) {
    QList<int> mergedTilesId;
    QList<int> mergedTilesetsId;
    changeCurrentProject(model->itemFromIndex(project));

    for (int i=0; i<toMerge.tiles.size(); i++) {
        QStandardItem *item = p_addTile(toMerge.tiles.at(i).name, toMerge.tiles.at(i).tile);
        mergedTilesId.append(item->data(IdRole).toInt());
    }

    for (int i=0; i<toMerge.tilesets.size(); i++) {
        QVector<int> tileset;
        QSize size = toMerge.tilesets.at(i).tilesSize;
        for (int j=0; j<toMerge.tilesets.at(i).tiles.size(); j++)
            tileset.append(mergedTilesId.at(toMerge.tilesets.at(i).tiles.at(j)));
        QStandardItem *item = p_addTileset(toMerge.tilesets.at(i).name, tileset,
                toMerge.tilesets.at(i).properties.toVector(), size.width(), size.height());
        mergedTilesetsId.append(item->data(IdRole).toInt());
    }

    for (int i=0; i<toMerge.maps.size(); i++) {
        QVector<unsigned char> vector = QVector<unsigned char>::fromStdVector(toMerge.maps.at(i).map.toVector());
        int w = toMerge.maps.at(i).map.columnCount();
        int h = toMerge.maps.at(i).map.rowCount();
        int tilesetId = mergedTilesetsId.at(toMerge.maps.at(i).tilesetId);
        p_addMap(toMerge.maps.at(i).name, vector, w, h, tilesetId);
    }
}


void ProjectWidget::setUndoGroup(QUndoGroup *undoGroup) {
    m_undoGroup = undoGroup;
}

QUndoGroup *ProjectWidget::undoGroup() {
    return m_undoGroup;
}
