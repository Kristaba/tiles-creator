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

#include "tilescreator.h"



TilesCreator::TilesCreator(QWidget *parent)
    : QMainWindow(parent)
{
    settingsFile = new QSettings("settings.ini", QSettings::IniFormat, this);
    undoGroup = new QUndoGroup(this);

    //Création des menus :
    menuFile = new QMenu(tr("&Fichier"), this);
    menuBar()->addMenu(menuFile);
    menuProject = new QMenu(tr("&Projet"), this);
    menuBar()->addMenu(menuProject);
    menuEdit = new QMenu(tr("&Édition"), this);
    menuBar()->addMenu(menuEdit);
    menuOptions = new QMenu(tr("&Options"), this);
    menuBar()->addMenu(menuOptions);
    menuAbout = new QMenu(tr("&Aide"), this);
    menuBar()->addMenu(menuAbout);

    //Menu Fichier :
    actionNewTile = menuFile->addAction(QIcon(":/new"), tr("&Nouveau Tile"));
    actionNewTile->setEnabled(false);
    actionNewTileset = menuFile->addAction(QIcon(":/new"), tr("&Nouveau Tileset"));
    actionNewTileset->setEnabled(false);
    actionNewMap = menuFile->addAction(QIcon(":/new"), tr("&Nouvelle Map"));
    actionNewMap->setEnabled(false);
    actionOpen = menuFile->addAction(QIcon(":/open"), tr("&Ouvrir..."));
    actionOpen->setEnabled(false);
    menuImport = menuFile->addMenu(tr("I&mporter"));
    menuImport->setEnabled(false);
    actionSave = menuFile->addAction(QIcon(":/save"), tr("&Enregistrer"));
    actionSave->setEnabled(false);

    //Menu Projet :
    actionNewProject = menuProject->addAction(QIcon(":/new"), tr("&Nouveau projet"));
    actionOpenProject = menuProject->addAction(QIcon(":/open"), tr("&Ouvrir un projet"));
    actionOpenProject->setShortcut(QKeySequence::Open);
    actionSaveProject = menuProject->addAction(QIcon(":/save"), tr("&Enregistrer projet"));
    actionSaveProject->setShortcut(QKeySequence::Save);
    actionSaveProject->setEnabled(false);
    actionSaveProjectUnder = menuProject->addAction(QIcon(":/save"), tr("&Enregistrer projet sous..."));
    actionSaveProjectUnder->setEnabled(false);
    actionDisplay = menuProject->addAction(tr("&Afficher la liste"));
    actionDisplay->setCheckable(true);
    menuExport = menuProject->addMenu(QIcon(":/send"), tr("&Exporter"));
    menuExport->setEnabled(false);

    //Menu Edition :
    actionUndo = undoGroup->createUndoAction(menuEdit, tr("Annuler"));
    actionUndo->setIcon(QIcon(":/back"));
    actionUndo->setShortcut(QKeySequence::Undo);
    menuEdit->addAction(actionUndo);
    actionRedo = undoGroup->createRedoAction(menuEdit, tr("Refaire"));
    actionRedo->setIcon(QIcon(":/next"));
    actionRedo->setShortcut(QKeySequence::Redo);
    menuEdit->addAction(actionRedo);

    //Menu Options :
    actionOptions = menuOptions->addAction(QIcon(":/options"), tr("&Options"));
    actionOptions->setEnabled(false);
    //Sous menu de choix de langue :
    menuLanguage = menuOptions->addMenu(QIcon(":/langage"), tr("&Langue"));
        actionFrench = menuLanguage->addAction(QIcon(":/france"), "Français");
        actionEnglish = menuLanguage->addAction(QIcon(":/english"), "English");
    actionUpdate = menuOptions->addAction(QIcon(":/download"), tr("&Mise a jour"));
    actionReport = menuOptions->addAction(QIcon(":/send"), tr("&Signaler un bug..."));

    //Menu Aide/About :
    actionAboutPlugins = menuAbout->addAction(tr("A propos des Plugins..."));
    actionAboutTc = menuAbout->addAction(tr("A propos de Tiles Creator"));
    actionAboutQt = menuAbout->addAction(tr("A propos de Qt"));

    projectDock = new QDockWidget(tr("Projet"), this);
    projectList = new ProjectWidget(undoGroup, projectDock);
    projectDock->setWidget(projectList);
    addDockWidget(Qt::LeftDockWidgetArea, projectDock);
    projectDock->hide();

    historicDock = new QDockWidget(tr("Historique des actions"), this);
    historic = new QUndoView(undoGroup, historicDock);
    historicDock->setWidget(historic);
    addDockWidget(Qt::RightDockWidgetArea, historicDock);
    historicDock->hide();


    editor = new QTabWidget;
    setCentralWidget(editor);
    //setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    
    tilesEditor = new TilesEditor(projectList);
    editor->addTab(tilesEditor, "Tile");
    tilesetEditor = new TilesetEditor(projectList);
    editor->addTab(tilesetEditor, "Tileset");
    mapEditor = new MapEditor(projectList);
    editor->addTab(mapEditor, "Map");
    showTileEditor();
    tilesEditor->changeSaveStatus(TilesEditor::SaveDisable);
    
    tilesetEditor->setEnabled(false);
    mapEditor->setEnabled(false);
    tilesEditor->setEnabled(false);

    QVariant vsize = settingsFile->value("window/size");
    if (vsize != QVariant()) {
        QSize size = vsize.toSize();
        if (size.isValid()) resize(size);
    }
    QVariant vpos = settingsFile->value("window/pos");
    if (vpos != QVariant()) {
        QPoint pos = vpos.toPoint();
        if (!pos.isNull()) move(pos);
    }
    QVariant vmax = settingsFile->value("window/maximized");
    if (vmax != QVariant()) {
        bool max = vmax.toBool();
        if (max) showMaximized();
    }

    translator = new QTranslator;
    translatorInternal = new QTranslator;

    initPlugins();

    connect(actionDisplay, SIGNAL(triggered(bool)), projectDock, SLOT(setVisible(bool)));
    connect(projectDock, SIGNAL(visibilityChanged(bool)), this, SLOT(projectVisibilityChanged(bool)));
    connect(projectList, SIGNAL(requestAddActualTile()), this, SLOT(addActualTileToProject()));
    connect(tilesEditor, SIGNAL(saveTile()), this, SLOT(saveActualTile()));
    connect(tilesEditor, SIGNAL(addToProject()), this, SLOT(addActualTileToProject()));
    connect(projectList, SIGNAL(currentItemChanged(QPersistentModelIndex)), this, SLOT(setActualItem(QPersistentModelIndex)));
    connect(projectList, SIGNAL(itemDeleted(QModelIndex)), this, SLOT(itemDeleted(QModelIndex)));
    connect(actionSaveProject, SIGNAL(triggered()), this, SLOT(saveActualProject()));
    connect(actionSaveProjectUnder, SIGNAL(triggered()), this, SLOT(saveActualProjectAs()));
    connect(actionOpenProject, SIGNAL(triggered()), this, SLOT(openProject()));
    connect(actionNewProject, SIGNAL(triggered()), this , SLOT(addNewProject()));
    connect(projectList, SIGNAL(itemModified(QPersistentModelIndex)), this, SLOT(itemModified(QPersistentModelIndex)));
    connect(actionUpdate, SIGNAL(triggered()), this, SLOT(updateSoftware()));
    connect(actionReport, SIGNAL(triggered()), this, SLOT(reportBug()));
    connect(projectList, SIGNAL(lastProjectClosed()), this, SLOT(lastProjectClosed()));
    connect(projectList, SIGNAL(itemAdded(QPersistentModelIndex)), this, SLOT(itemAdded(QPersistentModelIndex)));
    connect(actionNewTile, SIGNAL(triggered()), projectList, SLOT(addNewTile()));
    connect(actionNewTileset, SIGNAL(triggered()), projectList, SLOT(addNewTileset()));
    connect(actionNewMap, SIGNAL(triggered()), projectList, SLOT(addNewMap()));
    //connect(actionExport, SIGNAL(triggered()), this, SLOT(exportCurrentProject()));
    connect(actionFrench, SIGNAL(triggered()), this, SLOT(changeFrench()));
    connect(actionEnglish, SIGNAL(triggered()), this, SLOT(changeEnglish()));
    connect(menuExport, SIGNAL(triggered(QAction*)), this, SLOT(exportCurrentProject(QAction*)));
    connect(menuImport, SIGNAL(triggered(QAction*)), this, SLOT(importData(QAction*)));
    connect(actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(actionAboutTc, SIGNAL(triggered()), this, SLOT(aboutTilesCreator()));
    connect(actionAboutPlugins, SIGNAL(triggered()), this, SLOT(aboutPlugins()));
}



void TilesCreator::projectVisibilityChanged(bool v) {
    actionDisplay->setChecked(v);
}


void TilesCreator::addActualTileToProject() {
    tilesEditor->changeSaveStatus(TilesEditor::SaveEnable);
    projectList->addTile(tilesEditor->tileName(), tilesEditor->tile());
}


void TilesCreator::changeEditingTile(const QPersistentModelIndex &index) {
    showTileEditor();
    tilesEditor->setEnabled(true);
    QImage image = index.data(ProjectWidget::TileRole).value<QImage>();
    QString name = index.data(Qt::DisplayRole).toString();
    actualTile = index;
    tilesEditor->setTileIndex(index);
    tilesEditor->drawTile(name, image);
    tilesEditor->changeSaveStatus(TilesEditor::SaveEnable);
}


void TilesCreator::changeEditingTileset(const QPersistentModelIndex &index) {
    showTilesetEditor();
    tilesetEditor->setEnabled(true);
    if (index != actualTileset) {
        tilesetEditor->setTilesetIndex(index);
        actualTileset = index;
    }
}


void TilesCreator::changeEditingMap(const QPersistentModelIndex &index) {
    showMapEditor();
    mapEditor->setEnabled(true);
    if (index != actualMap) {
        mapEditor->setMapIndex(index);
        actualMap = index;
    }
}


void TilesCreator::saveActualTile() {
    if (actualTile != QPersistentModelIndex()) {
        QImage tmp = tilesEditor->tile();
        projectList->modifyTile(actualTile, tilesEditor->tileName(), tmp);
    }
}


void TilesCreator::setActualItem(const QPersistentModelIndex &index) {
    if (ProjectWidget::typeOfIndex(index) == ProjectWidget::Tile)
        changeEditingTile(index);
    else if (ProjectWidget::typeOfIndex(index) == ProjectWidget::Tileset)
        changeEditingTileset(index);
    else if (ProjectWidget::typeOfIndex(index) == ProjectWidget::Map)
        changeEditingMap(index);
}


void TilesCreator::itemDeleted(QModelIndex item) {
    ProjectWidget::ItemType type = ProjectWidget::typeOfIndex(item);
    switch (type) {
        case ProjectWidget::Tile :
            if (item == actualTile) {
                tilesEditor->setEnabled(false);
                actualTile = QPersistentModelIndex();
            }
            tilesetEditor->tileDeleted(item);
            break;

        case ProjectWidget::Tileset :
            if (item == actualTileset) {
                tilesetEditor->setEnabled(false);
                actualTileset = QPersistentModelIndex();
            }
            mapEditor->tilesetDeleted(item);
            break;

        case ProjectWidget::Map :
            if (item == actualMap) {
                mapEditor->setEnabled(false);
                actualMap = QPersistentModelIndex();
            }
            break;

        default:
            break;
    }
}


void TilesCreator::saveActualProject() {
    QPersistentModelIndex project = projectList->currentProject();
    if (project != QPersistentModelIndex()) {
        projectList->writeProjectFile(project);
    }
}


void TilesCreator::saveActualProjectAs() {
    QPersistentModelIndex project = projectList->currentProject();
    if (project != QPersistentModelIndex()) {
        QString path = QFileDialog::getSaveFileName(this, tr("Enregistrer un projet..."), QString(), tr("Projets Tiles Creator (*.prj)"));
        if (!path.isEmpty()) {
            if (path.section('.', -1) != "prj") path += ".prj";
           projectList->writeProjectFile(project, path);
        }
    }
}


void TilesCreator::openProject() {
    QString path = QFileDialog::getOpenFileName(this, tr("Charger un projet"), QString(), tr("Projets Tiles Creator (*.prj)"));
    if (!path.isEmpty()) {
        projectList->openProjectFile(path);
        projectDock->setVisible(true);
        tilesEditor->changeSaveStatus(TilesEditor::SaveEnable);
    }
}


void TilesCreator::addNewProject() {
    projectList->addProject(tr("Nouveau Projet"));
    projectDock->setVisible(true);
}


void TilesCreator::showTileEditor() {
    if (tilesEditor != editor->currentWidget()) editor->setCurrentWidget(tilesEditor);
}


void TilesCreator::showTilesetEditor() {
    if (tilesetEditor != editor->currentWidget()) editor->setCurrentWidget(tilesetEditor);
}


void TilesCreator::showMapEditor() {
    if (mapEditor != editor->currentWidget()) editor->setCurrentWidget(mapEditor);
}


void TilesCreator::updateSoftware() {
    QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("Lancer le Web Downloader?"),
        tr("Attention!\nEn lançant le logiciel de mises à jour, il est fortement conseillé de fermer Tiles Creator.\n"
        "Le fermer maintenant? Toutes les données non enregistrées seront perdues!"),
        QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);
    if (ret != QMessageBox::Cancel) {
        system(UPDATE_CMD);
        if (ret == QMessageBox::Yes) qApp->quit();
    }
}


void TilesCreator::reportBug() {
    BugReporter *reporter = new BugReporter;
    reporter->exec();
    delete reporter;
}


void TilesCreator::lastProjectClosed() {
    tilesEditor->setEnabled(false);
    tilesetEditor->setEnabled(false);
    mapEditor->setEnabled(false);
    actionNewTile->setEnabled(false);
    actionNewMap->setEnabled(false);
    actionNewTileset->setEnabled(false);
    menuImport->setEnabled(false);
    actionSaveProject->setEnabled(false);
    actionSaveProjectUnder->setEnabled(false);
    menuExport->setEnabled(false);
    tilesEditor->changeSaveStatus(TilesEditor::SaveDisable);
}


void TilesCreator::itemAdded(const QPersistentModelIndex &index) {
    ProjectWidget::ItemType type = ProjectWidget::typeOfIndex(index);
    switch (type) {
        case ProjectWidget::Project :
            tilesEditor->setEnabled(true);
            tilesetEditor->setEnabled(true);
            mapEditor->setEnabled(true);
            actionNewTile->setEnabled(true);
            actionNewMap->setEnabled(true);
            actionNewTileset->setEnabled(true);
            actionSaveProject->setEnabled(true);
            actionSaveProjectUnder->setEnabled(true);
            if (!importPlugins.isEmpty()) menuImport->setEnabled(true);
            if (!exportPlugins.isEmpty()) menuExport->setEnabled(true);
            tilesEditor->changeSaveStatus(TilesEditor::AddToProject);
            break;

        case ProjectWidget::Tileset :
            mapEditor->tilesetAdded(index);
            break;

        default:
            break;
    }
}


void TilesCreator::itemModified(const QPersistentModelIndex &index) {
    ProjectWidget::ItemType type = ProjectWidget::typeOfIndex(index);
    switch (type) {
        case ProjectWidget::Tile :
            tilesetEditor->tileModified(index);
            if (index == actualTile) changeEditingTile(index);
            break;

        case ProjectWidget::Tileset :
            mapEditor->tilesetModified(index);
            break;

        case ProjectWidget::Map :
            break;

        default:
            break;
    }
}


void TilesCreator::closeEvent(QCloseEvent *event) {
    bool max = false;
    hide();
    if (isMaximized()) {
        showNormal();
        max = true;
    }
    settingsFile->setValue("window/size", size());
    settingsFile->setValue("window/pos", pos());
    settingsFile->setValue("window/maximized", max);

    QMainWindow::closeEvent(event);
}



void TilesCreator::exportCurrentProject(QAction *action) {
    int index = exportPlugins.indexOf(action);
    if (index >= 0) {
        exportPluginList.at(index)->exportProject(projectList->getProjectStruct(projectList->currentProject()));
    }
}



QString TilesCreator::nameToValidString(const QString &s) {
    QString ret;
    for (int i=0; i<s.size(); i++) {
        QChar c = s.at(i);
        if (c.isLower() || c.isDigit()) ret.append(c);
        else if (c.isUpper()) ret.append(c.toLower());
        else ret.append('_');
    }
    return ret;
}


QByteArray TilesCreator::bitmapFromGrayImage(const QImage &image, bool lightOrDark) {
    //Si lightOrDark == true --> on sort le bitmap dark, sinon on cherche le light.
    QByteArray array = PixelEditor::toGrayTileArray(image);
    QByteArray ret;
    int count = 0;
    for (int h=0; h<image.height(); h++) {
        for (int j=0; j<(image.width()-1)/8+1; j++) {
            unsigned char c = 0;
            for (int k=0; k<8; k++) {
                if (count%image.width() != 0 || count/image.width() <= h) {
                    if (!lightOrDark) {
                        if (array.at(count) == TC::LightGray || array.at(count) == TC::Black)
                            c += 1 << (7-k);
                    }
                    else {
                        if (array.at(count) == TC::DarkGray || array.at(count) == TC::Black)
                            c += 1 << (7-k);
                    }
                    count ++;
                }
                else k=8;
            }
            ret.append(c);
        }
    }
    return ret;
}


void TilesCreator::changeFrench(){
    qApp->removeTranslator(translator);
    qApp->removeTranslator(translatorInternal);
    translatorInternal->load(QString(":/trans/qt_fr"));
    qApp->installTranslator(translatorInternal);
    settingsFile->setValue("config/language", "fr");
}



void TilesCreator::changeEnglish(){
    qApp->removeTranslator(translator);
    qApp->removeTranslator(translatorInternal);
    translator->load(":/trans/tc_en");
    qApp->installTranslator(translator);
    settingsFile->setValue("config/language", "en");
}



void TilesCreator::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        menuFile->setTitle(tr("&Fichier"));
        menuProject->setTitle(tr("&Projet"));
        menuOptions->setTitle(tr("&Options"));
        actionNewTile->setText(tr("&Nouveau Tile"));
        actionNewTileset->setText(tr("&Nouveau Tileset"));
        actionNewMap->setText(tr("&Nouvelle Map"));
        actionOpen->setText(tr("&Ouvrir..."));
        actionSave->setText(tr("&Enregistrer"));
        actionNewProject->setText(tr("&Nouveau projet"));
        actionOpenProject->setText(tr("&Ouvrir un projet"));
        actionSaveProject->setText(tr("&Enregistrer projet"));
        actionSaveProjectUnder->setText(tr("&Enregistrer projet sous..."));
        actionDisplay->setText(tr("&Afficher la liste"));
        menuExport->setTitle(tr("&Exporter"));
        actionOptions->setText(tr("&Options"));
        actionUpdate->setText(tr("&Mise a jour"));
        actionReport->setText(tr("&Signaler un bug..."));
        menuImport->setTitle(tr("I&mporter"));
        actionAboutPlugins->setText(tr("A propos des Plugins..."));
        actionAboutTc->setText(tr("A propos de Tiles Creator"));
        actionAboutQt->setText(tr("A propos de Qt"));
        menuAbout->setTitle(tr("&Aide"));
        menuLanguage->setTitle(tr("&Langue"));
        projectDock->setWindowTitle(tr("Projet"));
        historicDock->setWindowTitle(tr("Historique des actions"));
        //actionUndo = undoGroup->createUndoAction(menuEdit, tr("Annuler "));
        //actionRedo = undoGroup->createRedoAction(menuEdit, tr("Refaire "));
    }
    QMainWindow::changeEvent(event);
}


void TilesCreator::initPlugins() {
    foreach (TilesCreatorPlugin *plugin, plugins)
        delete plugin;
    foreach (QAction *action, exportPlugins)
        delete action;
    foreach (QAction *action, importPlugins)
        delete action;
    plugins.clear();
    exportPlugins.clear();
    importPlugins.clear();
    exportPluginList.clear();
    importPluginList.clear();

    QDir pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("plugins");

    menuExport->setEnabled(false);
    menuImport->setEnabled(false);

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin != 0) {
            ExportInterface * op = qobject_cast<ExportInterface *>(plugin);
            ImportInterface *im = qobject_cast<ImportInterface *>(plugin);
            if (op != 0) {
                exportPluginList << op;
                QAction *action = menuExport->addAction(op->pluginName());
                exportPlugins.append(action);
                TilesCreatorPlugin *tcPlugin = dynamic_cast<TilesCreatorPlugin *>(op);
                if (tcPlugin != 0) plugins.append(tcPlugin);
            }
            if (im != 0) {
                importPluginList << im;
                QAction *action = menuImport->addAction(im->pluginName());
                importPlugins.append(action);
                TilesCreatorPlugin *tcPlugin = dynamic_cast<TilesCreatorPlugin *>(im);
                if (tcPlugin != 0) plugins.append(tcPlugin);
            }
        }
    }
}



void TilesCreator::importData(QAction *action) {
    if (projectList->currentProject() != QPersistentModelIndex()) {
        int index = importPlugins.indexOf(action);
        if (index >= 0) {
            TcProject imported = importPluginList.at(index)->import();
            projectList->mergeProjectStruct(imported, projectList->currentProject());
        }
    }
}


void TilesCreator::aboutTilesCreator() {
    QLabel *about = new QLabel (tr("<CENTER>Tiles Creator<BR/><STRONG>Developpé par Léo Grange (Kristaba)</STRONG>"
        "<BR /><STRONG><A href=\"http://www.iconarchive.com/category/system/sleek-xp-basic-icons-by-deleket.html\">"
        "Deleket</a></STRONG> et <STRONG><A href=\"http://dryicons.com/\">DryIcons</a></STRONG>"
        " (création des icônes)<BR /><BR />Programme sous license GPL.<BR/>2009-2010</CENTER>")
        , this);
    about->setTextInteractionFlags(Qt::TextBrowserInteraction);
    about->setOpenExternalLinks(true);
    about->setWindowModality(Qt::WindowModal);
    about->setWindowFlags(Qt::Window);
    about->setAttribute(Qt::WA_DeleteOnClose);
    about->show();
}


void TilesCreator::aboutPlugins() {
    QTableWidget *view = new QTableWidget(this);
    view->setWindowModality(Qt::WindowModal);
    view->setWindowFlags(Qt::Window);
    view->setWindowTitle(tr("Plugins"));

    view->setColumnCount(4);
    view->setRowCount(plugins.size());

    for (int i=0; i<plugins.size(); i++) {
        QTableWidgetItem *name = new QTableWidgetItem(plugins.at(i)->pluginName());
        name->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        QTableWidgetItem *version = new QTableWidgetItem(plugins.at(i)->pluginVersion());
        version->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        QTableWidgetItem *author = new QTableWidgetItem();
        author->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        if (plugins.at(i)->pluginAuthors().size() == 1)
            author->setText(plugins.at(i)->pluginAuthors().at(0));
        else if (plugins.at(i)->pluginAuthors().size() > 1)
            author->setText(plugins.at(i)->pluginAuthors().at(0) + ", ...");
        else author->setText(tr("Inconnu"));

        QTableWidgetItem *type = new QTableWidgetItem();
        type->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        if (plugins.at(i)->pluginType() == TilesCreatorPlugin::Import)
            type->setText(tr("Importation"));
        else if (plugins.at(i)->pluginType() == TilesCreatorPlugin::Export)
            type->setText(tr("Exportation"));
        else type->setText(tr("Autre"));

        view->setItem(i, 0, name);
        view->setItem(i, 1, version);
        view->setItem(i, 2, type);
        view->setItem(i, 3, author);
    }

    view->setAlternatingRowColors(true);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setHorizontalHeaderLabels(QStringList() << tr("Nom") << tr("Version") << tr("Type")
                                    << tr("Auteurs"));
    view->horizontalHeader()->setStretchLastSection(true);
    view->resizeColumnsToContents();
    view->resizeRowsToContents();
    view->resize(500, 150);
    view->verticalHeader()->setVisible(false);
    view->show();
    view->setAttribute(Qt::WA_DeleteOnClose);

    connect(view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(detailledPluginDescription(QModelIndex)));
}


void TilesCreator::detailledPluginDescription(const QModelIndex &index) {
    QWidget *description = new QWidget(this);
    description->setWindowModality(Qt::WindowModal);
    description->setWindowFlags(Qt::Window);
    description->setWindowTitle("Plugin : " + plugins.at(index.row())->pluginName());

    QFormLayout *layout = new QFormLayout;

    QLabel *name = new QLabel(plugins.at(index.row())->pluginName());
    layout->addRow(tr("Nom :"), name);

    QLabel *version = new QLabel(plugins.at(index.row())->pluginVersion());
    layout->addRow(tr("Version :"), version);

    QLabel *oVersion = new QLabel(plugins.at(index.row())->tcVersion());
    layout->addRow(tr("Optimisé pour la version TC :"), oVersion);

    QLabel *websitelink = new QLabel("<A HREF=\""+plugins.at(index.row())->relatedWebsite()+"\">"
                                     +plugins.at(index.row())->relatedWebsite()+"</A>");
    websitelink->setTextInteractionFlags(Qt::TextBrowserInteraction);
    websitelink->setOpenExternalLinks(true);
    layout->addRow(tr("Site web :"), websitelink);

    QTextEdit *authors = new QTextEdit();
    for (int i=0; i<plugins.at(index.row())->pluginAuthors().size(); i++)
        authors->append(plugins.at(index.row())->pluginAuthors().at(i));
    authors->setReadOnly(true);
    authors->setLineWrapMode(QTextEdit::NoWrap);
    layout->addRow(tr("Auteurs :"), authors);

    QTextEdit *details = new QTextEdit();
    details->setText(plugins.at(index.row())->pluginDetails());
    details->setReadOnly(true);
    layout->addRow(tr("Détails :"), details);

    description->setLayout(layout);

    description->show();
    description->setAttribute(Qt::WA_DeleteOnClose);
}
