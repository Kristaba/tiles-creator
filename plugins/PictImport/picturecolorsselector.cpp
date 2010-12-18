/*  Copyright 2010 Léo Grange

This file is part of PictImport, a plugin for Tiles Creator.

PictImport is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

PictImport is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with PictImport.  If not, see <http://www.gnu.org/licenses/>. */

#include "picturecolorsselector.h"

PictureColorsSelector::PictureColorsSelector(QWidget *parent) :
    QDialog(parent)
{
    layout = new QHBoxLayout();
    setLayout(layout);

    //Déclaration de la GUI :
    l_subLayout = new QVBoxLayout();
        wm_scroll = new QScrollArea();
            wm_picture = new SamplingColorLabel();
                wm_picture->setMaximumSize(16777215, 16777215);
                wm_scroll->setWidget(wm_picture);
            l_subLayout->addWidget(wm_scroll);
        l_horizontalSettings = new QHBoxLayout();
            l_horizontalSettings->addStretch();
            wm_groupFile = new QGroupBox(tr("Fichier"));
                l_groupFile = new QVBoxLayout();
                QHBoxLayout *fileLayout = new QHBoxLayout();
                    wm_fileName = new QLineEdit();
                        fileLayout->addWidget(wm_fileName);
                    wm_buttonFile = new QPushButton("...");
                        fileLayout->addWidget(wm_buttonFile);
                    l_groupFile->addLayout(fileLayout);
                wm_boxMultipleFiles = new QCheckBox(tr("Utiliser plusieurs fichiers avec les mêmes paramètres"));
                    l_groupFile->addWidget(wm_boxMultipleFiles);
                QHBoxLayout *multipleFilesLayout = new QHBoxLayout();
                    wm_setMultipleFiles = new QPushButton(tr("Ajouter"));
                        wm_setMultipleFiles->setEnabled(false);
                        multipleFilesLayout->addWidget(wm_setMultipleFiles);
                    wm_buttonClearFiles = new QPushButton(tr("Effacer liste"));
                        wm_buttonClearFiles->setEnabled(false);
                        multipleFilesLayout->addWidget(wm_buttonClearFiles);
                    wm_viewMultipleFiles = new QPushButton(tr("Voir les fichiers"));
                        wm_viewMultipleFiles->setEnabled(false);
                        multipleFilesLayout->addWidget(wm_viewMultipleFiles);
                    l_groupFile->addLayout(multipleFilesLayout);
                wm_groupFile->setLayout(l_groupFile);
                l_horizontalSettings->addWidget(wm_groupFile);
            wm_groupSize = new QGroupBox(tr("Dimensions tiles/sprites"));
                l_groupSize = new QVBoxLayout();
                QHBoxLayout *widthLayout = new QHBoxLayout();
                    QLabel *labelWidth = new QLabel(tr("Longueur"));
                    widthLayout->addWidget(labelWidth);
                    wm_width = new QSpinBox();
                        wm_width->setMinimum(1);
                        wm_width->setMaximum(128);
                        wm_width->setValue(16);
                    widthLayout->addWidget(wm_width);
                    l_groupSize->addLayout(widthLayout);
                QHBoxLayout *heightLayout = new QHBoxLayout();
                    QLabel *labelHeight = new QLabel(tr("Hauteur"));
                    heightLayout->addWidget(labelHeight);
                    wm_height = new QSpinBox();
                        wm_height->setMinimum(1);
                        wm_height->setMaximum(64);
                        wm_height->setValue(16);
                    heightLayout->addWidget(wm_height);
                    l_groupSize->addLayout(heightLayout);
                wm_groupSize->setLayout(l_groupSize);
                l_horizontalSettings->addWidget(wm_groupSize);
                wm_groupSize->hide();
            wm_groupResult = new QGroupBox();
                l_groupResult = new QVBoxLayout;
                wm_buttonPreview = new QPushButton(tr("Prévisualiser"));
                    l_groupResult->addWidget(wm_buttonPreview);
                wm_buttonFinish = new QPushButton(tr("Terminer"));
                    l_groupResult->addWidget(wm_buttonFinish);
                wm_buttonAbort = new QPushButton(tr("Annuler"));
                    l_groupResult->addWidget(wm_buttonAbort);
                wm_groupResult->setLayout(l_groupResult);
                l_horizontalSettings->addWidget(wm_groupResult);
            l_horizontalSettings->addStretch();
            l_subLayout->addLayout(l_horizontalSettings);
        layout->addLayout(l_subLayout);
    l_settings = new QVBoxLayout();
        l_settings->addStretch();
        wm_groupDataType = new QGroupBox(tr("Type de données"));
            l_groupDataType = new QHBoxLayout();
            wm_dataType = new QComboBox();
                wm_dataType->addItem(tr("Un tile/sprite"));
                wm_dataType->addItem(tr("Grille de tiles/sprites"));
                wm_dataType->addItem(tr("Une map"));
                l_groupDataType->addWidget(wm_dataType);
            wm_groupDataType->setLayout(l_groupDataType);
            l_settings->addWidget(wm_groupDataType);
        wm_zoom = new QGroupBox("Zoom");
            l_groupZoom = new QVBoxLayout();
            wm_zoomed = new SamplingColorLabel();
                wm_zoomed->setFixedSize(100, 100);
                l_groupZoom->addWidget(wm_zoomed);
            wm_zoom->setLayout(l_groupZoom);
            l_settings->addWidget(wm_zoom);
        wm_groupColors = new QGroupBox(tr("Couleurs :"));
            l_groupColors = new QVBoxLayout();
            wm_colorType = new QComboBox();
                wm_colorType->addItem(tr("Noir et Blanc"));
                wm_colorType->addItem(tr("3 niveaux de Gris"));
                wm_colorType->addItem(tr("4 niveaux de Gris"));
                wm_colorType->setCurrentIndex(2);
                l_groupColors->addWidget(wm_colorType);
            wm_white = new QPushButton(tr("Blanc"));
                wm_white->setCheckable(true);
                wm_white->setChecked(true);
                wm_white->setAutoExclusive(true);
                l_groupColors->addWidget(wm_white);
            wm_black = new QPushButton(tr("Noir"));
                wm_black->setCheckable(true);
                wm_black->setAutoExclusive(true);
                l_groupColors->addWidget(wm_black);
            wm_light = new QPushButton(tr("Gris Clair"));
                wm_light->setCheckable(true);
                wm_light->setAutoExclusive(true);
                l_groupColors->addWidget(wm_light);
            wm_dark = new QPushButton(tr("Gris Foncé"));
                wm_dark->setCheckable(true);
                wm_dark->setAutoExclusive(true);
                l_groupColors->addWidget(wm_dark);
            wm_transType = new QComboBox();
                wm_transType->addItem(tr("Trans. en Blanc"));
                wm_transType->addItem(tr("Trans. auto"));
                wm_transType->addItem(tr("Couleur de trans."));
                l_groupColors->addWidget(wm_transType);
            wm_trans = new QPushButton(tr("Transparent"));
                wm_trans->setCheckable(true);
                wm_trans->setAutoExclusive(true);
                wm_trans->setEnabled(false);
                l_groupColors->addWidget(wm_trans);
            m_buttonGroup = new QButtonGroup();
                m_buttonGroup->addButton(wm_light);
                m_buttonGroup->addButton(wm_dark);
                m_buttonGroup->addButton(wm_white);
                m_buttonGroup->addButton(wm_black);
                m_buttonGroup->addButton(wm_trans);
            wm_groupColors->setLayout(l_groupColors);
            l_settings->addWidget(wm_groupColors);
        l_settings->addStretch();
        layout->addLayout(l_settings);

    //Définition de la GUI de la fenêtre de prévisualisation :
    wm_previewWidget = new QWidget(this);
    wm_previewWidget->setWindowModality(Qt::WindowModal);
    wm_previewWidget->setWindowFlags(Qt::Window);
    wm_previewWidget->setWindowTitle(tr("Prévisualisation"));
    l_previewLayout = new QVBoxLayout();
        wm_previewLabel = new QLabel();
        wm_previewArea = new QScrollArea();
            wm_previewArea->setWidget(wm_previewLabel);
            l_previewLayout->addWidget(wm_previewArea);
        l_buttonBar = new QHBoxLayout();
            wm_buttonPreviousItem = new QPushButton(tr("Item précédent"));
                l_buttonBar->addWidget(wm_buttonPreviousItem);
            wm_buttonNextItem = new QPushButton(tr("Item suivant"));
                l_buttonBar->addWidget(wm_buttonNextItem);
            l_buttonBar->addStretch();
            wm_buttonViewMap = new QPushButton(tr("Map"));
                l_buttonBar->addWidget(wm_buttonViewMap);
            l_buttonBar->addStretch();
            wm_buttonPreviousFile = new QPushButton(tr("Fichier précédent"));
                l_buttonBar->addWidget(wm_buttonPreviousFile);
            wm_buttonNextFile = new QPushButton(tr("Fichier suivant"));
                l_buttonBar->addWidget(wm_buttonNextFile);
            l_previewLayout->addLayout(l_buttonBar);
        l_statusBar = new QHBoxLayout();
            l_statusBar->addStretch();
            wm_previewFileName = new QLineEdit();
                wm_previewFileName->setReadOnly(true);
                l_statusBar->addWidget(wm_previewFileName);
            l_statusBar->addStretch();
            wm_previewItemNumber = new QLabel();
                l_statusBar->addWidget(wm_previewItemNumber);
            l_statusBar->addStretch();
            wm_previewFileNumber = new QLabel();
                l_statusBar->addWidget(wm_previewFileNumber);
            l_statusBar->addStretch();
            l_previewLayout->addLayout(l_statusBar);
    wm_previewWidget->setLayout(l_previewLayout);



    m_colorLight = TC::rgbLightGray;
    m_colorDark = TC::rgbDarkGray;
    m_colorWhite = TC::rgbWhite;
    m_colorBlack = TC::rgbBlack;
    m_colorTrans = QColor(0xFF, 0xFF, 0xFF);
    setButtonStyleSheet(wm_light, TC::rgbLightGray);
    setButtonStyleSheet(wm_dark, TC::rgbDarkGray);
    setButtonStyleSheet(wm_white, TC::rgbWhite);
    setButtonStyleSheet(wm_black, TC::rgbBlack);
    setButtonStyleSheet(wm_trans, m_colorTrans);
    m_colorNumber = 4;

    connect(wm_picture, SIGNAL(samplingColorChanged(QRgb)), this, SLOT(setButtonColor(QRgb)));
    connect(wm_picture, SIGNAL(zoomRequested(QPoint)), this, SLOT(setZoomedContent(QPoint)));
    connect(wm_zoomed, SIGNAL(samplingColorChanged(QRgb)), this, SLOT(setButtonColor(QRgb)));
    connect(wm_colorType, SIGNAL(currentIndexChanged(int)), this, SLOT(setColorType(int)));
    connect(wm_transType, SIGNAL(currentIndexChanged(int)), this, SLOT(setTransType(int)));
    connect(wm_buttonFile, SIGNAL(clicked()), this, SLOT(selectFile()));
    connect(wm_fileName, SIGNAL(textChanged(QString)), this, SLOT(setFileName(QString)));
    connect(wm_buttonPreview, SIGNAL(clicked()), this, SLOT(previewResult()));
    connect(wm_buttonFinish, SIGNAL(clicked()), this, SLOT(finishWizard()));
    connect(wm_buttonAbort, SIGNAL(clicked()), this, SLOT(abortWizard()));
    connect(wm_dataType, SIGNAL(currentIndexChanged(int)), this, SLOT(setDataType(int)));
    connect(wm_boxMultipleFiles, SIGNAL(toggled(bool)), this, SLOT(setUseMultipleFiles(bool)));
    connect(wm_setMultipleFiles, SIGNAL(clicked()), this, SLOT(addFiles()));
    connect(wm_viewMultipleFiles, SIGNAL(clicked()), this, SLOT(viewFileList()));
    connect(wm_buttonClearFiles, SIGNAL(clicked()), this, SLOT(clearFileList()));

    //Connects en rapport avec la fenêtre de prévisualisation :
    connect(wm_buttonNextFile, SIGNAL(clicked()), this, SLOT(nextFile()));
    connect(wm_buttonNextItem, SIGNAL(clicked()), this, SLOT(nextItem()));
    connect(wm_buttonPreviousFile, SIGNAL(clicked()), this, SLOT(previousFile()));
    connect(wm_buttonPreviousItem, SIGNAL(clicked()), this, SLOT(previousItem()));
    connect(wm_buttonViewMap, SIGNAL(clicked()), this, SLOT(viewMapOrOverview()));
}


void PictureColorsSelector::setButtonStyleSheet(QPushButton *button, QColor color) {
    int r = color.red();
    int g = color.green();
    int b = color.blue();
    int a = color.alpha();
    int tR = 0xFF, tG = 0xFF, tB = 0xFF;
    if(r>0x80) tR = 0x00;
    if(g>0x80) tG = 0x00;
    if(b>0x80) tB = 0x00;

    QString tColor;
    tColor.append(to2CharHex(tR)).append(to2CharHex(tG)).append(to2CharHex(tB));

    button->setStyleSheet("QPushButton { border: 3px solid #8f8f91; border-radius: 4px;"
                          "background-color: rgba("+QString::number(r)+", "+QString::number(g)+", "
                          +QString::number(b)+", "+QString::number(a)+"); color: #"+tColor+" }"
                          "QPushButton:checked {border-color: qlineargradient(x1:0, y1:1, x2:1, y2:1,"
                          "stop:0 #B0B020, stop:0.05 #B0B020, stop:0.5 #FF0000, stop:0.95 #B0B020, stop:1 #B0B020) "
                          "#B0B020; }"
                          "QPushButton:pressed {border-color: #0000FF}"
                          "QPushButton:disabled {background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                          "stop:0 #F0F0F0, stop:1 #A0A0A0); color: #808080; "
                          "border: 2px solid #B0B0B0}");
}


QString PictureColorsSelector::to2CharHex(int num) {
    QString ret;
    ret = QString::number(num, 16).toUpper();
    if (ret.size() < 2) ret.insert(0, '0');
    else if (ret.size() > 2) ret = ret.right(2);
    return ret;
}


void PictureColorsSelector::setButtonColor(QRgb rgba) {
    QColor color;
    color.setRgba(rgba);
    QPushButton *button = qobject_cast<QPushButton*>(m_buttonGroup->checkedButton());
    if (button != 0) {
        setButtonStyleSheet(button, color);
        if(button == wm_white) m_colorWhite = color;
        else if(button == wm_black) m_colorBlack = color;
        else if(button == wm_light) m_colorLight = color;
        else if(button == wm_dark) m_colorDark = color;
        else if(button == wm_trans) m_colorTrans = color;
    }
}


void PictureColorsSelector::setZoomedContent(QPoint center) {
    if (wm_picture->pixmap() != 0 && wm_picture->pixmap()->width() > center.x()
            && wm_picture->pixmap()->height() > center.y()) {
        int x = center.x() - 12;
        if (x<0) x=0;
        int y = center.y() - 12;
        if (y<0) y=0;
        wm_zoomed->setPixmap(wm_picture->pixmap()->copy(x, y, 25, 25).scaled(100, 100));
    }
}

void PictureColorsSelector::setColorType(int type) {
    if (type == 0) {
        wm_colorType->setCurrentIndex(0);
        wm_light->setEnabled(false);
        wm_dark->setEnabled(false);
        wm_white->setChecked(true);
        m_colorNumber = 2;
    }
    else if (type == 1) {
        wm_colorType->setCurrentIndex(1);
        wm_light->setEnabled(true);
        wm_dark->setEnabled(false);
        wm_white->setChecked(true);
        m_colorNumber = 3;
    }
    else {
        wm_colorType->setCurrentIndex(2);
        wm_light->setEnabled(true);
        wm_dark->setEnabled(true);
        m_colorNumber = 4;
    }
}


void PictureColorsSelector::setTransType(int type) {
    if (type == 0) {
        wm_transType->setCurrentIndex(0);
        wm_trans->setEnabled(false);
    }
    else if (type == 1) {
        wm_transType->setCurrentIndex(1);
        wm_trans->setEnabled(false);
    }
    else {
        wm_transType->setCurrentIndex(2);
        wm_trans->setEnabled(true);
    }
}


void PictureColorsSelector::selectFile() {
    QString file = QFileDialog::getOpenFileName(this, tr("Ouvrir une image"), QString(),
                   tr("Images (*.bmp *.png *.gif *.jpg *.jpeg *.xbm *.xpm *pbm *ppm *pgm)"));
    if (!file.isEmpty()) {
        wm_fileName->setText(file);
        QPixmap pixmap;
        if (pixmap.load(file)) {
            wm_scroll->takeWidget();
            wm_picture->setPixmap(pixmap);
            wm_picture->resize(pixmap.size());
            wm_scroll->setWidget(wm_picture);
        }
        else {
            wm_picture->setText(tr("Le fichier n'existe pas ou n'est pas une image valide."));
            wm_picture->resize(wm_scroll->size());
        }
    }
}


void PictureColorsSelector::setFileName(QString name) {
    if (name != wm_fileName->text())
        wm_fileName->setText(name);
    if (QFile::exists(name)) {
        QPixmap pixmap;
        if (pixmap.load(name)) {
            wm_scroll->takeWidget();
            wm_picture->setPixmap(pixmap);
            wm_picture->resize(pixmap.size());
            wm_scroll->setWidget(wm_picture);
        }
        else {
            wm_picture->setText(tr("Le fichier n'existe pas ou n'est pas une image valide."));
            wm_picture->resize(wm_scroll->size());
        }
    }
    else {
        wm_picture->setText(tr("Le fichier n'existe pas ou n'est pas une image valide."));
        wm_picture->resize(wm_scroll->size());
    }
}


void PictureColorsSelector::previewResult() {
    m_previewItems.clear();
    if (wm_picture->pixmap() != 0) {
        QPixmap currentPixmap;
        bool errorOccured = false;
        for (int i=-1; i<m_fileNameList.size(); i++) {
            m_previewItems.append(QList<QImage>());
            bool loadOk = true;
            if (i == -1) currentPixmap = wm_picture->pixmap()->copy();
            else if (QFile::exists(m_fileNameList.at(i)))
                if (!currentPixmap.load(m_fileNameList.at(i))) loadOk = false;
            if (loadOk) {
                if (wm_dataType->currentIndex() == 0) {
                    if (currentPixmap.width() <= 128 && currentPixmap.height() <= 64) {
                        m_previewItems.last().append(getConvertedImage(currentPixmap, true));
                    }
                    else {
                        m_previewItems.removeLast();
                        QString tmpName = wm_fileName->text();
                        if (i >= 0) tmpName = m_fileNameList.at(i);
                        QMessageBox::StandardButton b = QMessageBox::information(this, tr("Image trop grande"),
                            tr("L'image contenue dans le fichier \"") + tmpName + tr("\" est trop"
                               " grande pour être utilisée comme tile/sprite.\nCliquez sur OK"
                               " pour ignorer ce fichier ou sur Annuler pour retourner sur"
                               " PictImport."), QMessageBox::Ok, QMessageBox::Cancel);
                        if (b == QMessageBox::Cancel) {
                            errorOccured = true;
                            i = m_fileNameList.size();
                        }
                    }
                }
                else if (wm_dataType->currentIndex() == 1) {
                    int tw = wm_width->value();
                    int th = wm_height->value();
                    int iw = currentPixmap.width();
                    int ih = currentPixmap.height();
                    QImage image(getConvertedImage(currentPixmap, true));
                    for (int k=0; k<ih/th; k++) {
                        for (int j=0; j<iw/tw; j++) {
                            m_previewItems.last().append(image.copy(j*tw, k*th, tw, th));
                        }
                    }
                    m_previewItems.last().append(image);
                }

                else {
                    QVector<QVector<QImage> > map;
                    int tw = wm_width->value();
                    int th = wm_height->value();
                    int iw = currentPixmap.width();
                    int ih = currentPixmap.height();
                    QImage image(getConvertedImage(currentPixmap, true));

                    map.resize(ih/th);
                    for (int k=0; k<ih/th; k++) {
                        map[k].resize(iw/tw);
                        for (int j=0; j<iw/tw; j++) {
                            map[k][j] = image.copy(j*tw, k*th, tw, th);
                        }
                    }

                    for (int k=0; k<ih/th; k++) {
                        for (int j=0; j<iw/tw; j++) {
                            int index = m_previewItems.last().indexOf(map[k][j]);
                            if (index == -1) m_previewItems.last().append(map[k][j]);
                        }
                    }
                    m_previewItems.last().append(image);
                }
            }
            else {
                QMessageBox::information(this, tr("Fichier non valide"), tr("Le fichier \"")
                        + m_fileNameList.at(i) + tr("\" n'est pas un fichier image valide."));
            }
        }

        if (!errorOccured && !m_previewItems.isEmpty()) {
            m_currentItemNumber = 1;
            m_currentFileNumber = 1;
            updatePreviewInformations();
        }
        else m_previewItems.clear();
    }
    else {
        QMessageBox::information(this, tr("Pas d'image..."), tr("Aucune image valide selectionnée."));
    }
}


void PictureColorsSelector::abortWizard() {
    done(QDialog::Rejected);
}


void PictureColorsSelector::finishWizard() {
    if (wm_picture->pixmap() != 0) {
        QPixmap currentPixmap;
        bool errorOccured = false;
        for (int i=-1; i<m_fileNameList.size(); i++) {
            bool loadOk = true;
            if (i == -1) currentPixmap = wm_picture->pixmap()->copy();
            else if (QFile::exists(m_fileNameList.at(i)))
                if (!currentPixmap.load(m_fileNameList.at(i))) loadOk = false;
            if (loadOk) {
                if (wm_dataType->currentIndex() == 0) {
                    if (currentPixmap.width() <= 128 && currentPixmap.height() <= 64) {
                        TcTile tile;
                        tile.name = "Imported Tile " + QString::number(i+1);
                        tile.tile = getConvertedImage(currentPixmap, true);
                        imported.tiles.append(tile);
                    }
                    else {
                        QString tmpName = wm_fileName->text();
                        if (i >= 0) tmpName = m_fileNameList.at(i);
                        QMessageBox::StandardButton b = QMessageBox::information(this, tr("Image trop grande"),
                            tr("L'image contenue dans le fichier \"") + tmpName + tr("\" est trop"
                               " grande pour être utilisée comme tile/sprite.\nCliquez sur OK"
                               " pour ignorer ce fichier ou sur Annuler pour retourner sur"
                               " PictImport."), QMessageBox::Ok, QMessageBox::Cancel);
                        if (b == QMessageBox::Cancel) {
                            errorOccured = true;
                            i = m_fileNameList.size();
                        }
                    }
                }
                else if (wm_dataType->currentIndex() == 1) {
                    int tw = wm_width->value();
                    int th = wm_height->value();
                    int iw = currentPixmap.width();
                    int ih = currentPixmap.height();
                    QImage image(getConvertedImage(currentPixmap, true));
                    for (int k=0; k<ih/th; k++) {
                        for (int j=0; j<iw/tw; j++) {
                            TcTile tile;
                            tile.name = "Imported Tile " + QString::number(i+1) + " - "
                                        + QString::number(k*(iw/tw)+j);
                            tile.tile = image.copy(j*tw, k*th, tw, th);
                            imported.tiles.append(tile);
                        }
                    }
                }

                else {
                    QVector<QVector<QImage> > map;
                    int tw = wm_width->value();
                    int th = wm_height->value();
                    int iw = currentPixmap.width();
                    int ih = currentPixmap.height();
                    QImage image(getConvertedImage(currentPixmap, true));

                    map.resize(ih/th);
                    for (int k=0; k<ih/th; k++) {
                        map[k].resize(iw/tw);
                        for (int j=0; j<iw/tw; j++) {
                            map[k][j] = image.copy(j*tw, k*th, tw, th);
                        }
                    }

                    QList<QImage> tiles;
                    Array2D<unsigned char> mapped;
                    mapped.resize(ih/th, iw/tw);
                    for (int k=0; k<ih/th; k++) {
                        for (int j=0; j<iw/tw; j++) {
                            int index = tiles.indexOf(map[k][j]);
                            if (index == -1) {
                                tiles.append(map[k][j]);
                                mapped(k, j) = tiles.size()-1;
                            }
                            else mapped(k, j) = index;
                        }
                    }
                    TcTileset tileset;
                    tileset.name = "Imported Tileset " + QString::number(i+1);
                    tileset.tilesSize = QSize(tw, th);
                    QVector<unsigned char> properties;
                    properties.resize(tiles.size());
                    tileset.properties = properties.toList();
                    QList<int> tilesId;
                    for (int k=0; k<tiles.size(); k++) {
                        tilesId.append(imported.tiles.size());
                        TcTile tile;
                        tile.name = "Imported Tile " + QString::number(i+1) + " - " + QString::number(k);
                        tile.tile = tiles.at(k);
                        imported.tiles.append(tile);
                    }
                    tileset.tiles = tilesId;
                    imported.tilesets.append(tileset);

                    TcMap tcMap;
                    tcMap.name = "Imported Map " + QString::number(i+1);
                    tcMap.map = mapped;
                    tcMap.tilesetId = imported.tilesets.size()-1;
                    imported.maps.append(tcMap);
                }
            }
            else {
                QMessageBox::information(this, tr("Fichier non valide"), tr("Le fichier \"")
                        + m_fileNameList.at(i) + tr("\" n'est pas un fichier image valide."));
            }
        }
        if (!errorOccured) done(QDialog::Accepted);
    }
    else {
        QMessageBox::information(this, tr("Pas d'image..."), tr("Aucune image valide selectionnée."));
    }
}


QImage PictureColorsSelector::getConvertedImage(QPixmap toConvert, bool showProgressBar) {
    QImage mask = toConvert.mask().toImage();
    QImage image = toConvert.toImage();
    QImage ret = QImage(toConvert.width(), toConvert.height(), QImage::Format_Indexed8);
    QProgressDialog *dialog = new QProgressDialog(tr("Conversion en cours..."), QString(), 0, 100, this);
    dialog->setValue(0);
    dialog->setWindowModality(Qt::WindowModal);
    dialog->setModal(true);
    dialog->setWindowFlags(dialog->windowFlags() & ~(Qt::WindowContextHelpButtonHint));

    TC::setColorsIndexes(&ret);

    QList<QColor> colors;
    colors.append(m_colorBlack);
    colors.append(m_colorWhite);
    if (m_colorNumber > 2) colors.append(m_colorLight);
    if (m_colorNumber > 3) colors.append(m_colorDark);
    if (wm_transType->currentIndex() == 2) colors.append(m_colorTrans);

    for (int i=0; i<toConvert.width(); i++) {
        for (int j=0; j<toConvert.height(); j++) {
            if (toConvert.hasAlpha() && mask.pixelIndex(i, j) == Qt::color0) {
                if (wm_transType->currentIndex() == 1) ret.setPixel(i, j, TC::Transparency);
                else ret.setPixel(i, j, TC::White);
            }
            else {
                QColor c = getNearestColor(colors, QColor().fromRgb(image.pixel(i, j)));
                if (c == m_colorLight) ret.setPixel(i, j, TC::LightGray);
                else if (c == m_colorDark) ret.setPixel(i, j, TC::DarkGray);
                else if (c == m_colorBlack) ret.setPixel(i, j, TC::Black);
                else if (c == m_colorWhite) ret.setPixel(i, j, TC::White);
                else if (wm_transType->currentIndex() == 2) ret.setPixel(i, j, TC::Transparency);
                else ret.setPixel(i, j, TC::White);
            }
        }
        if (showProgressBar) dialog->setValue(i*100/toConvert.width());
    }
    delete(dialog);
    return ret;
}


int PictureColorsSelector::compareColor(QColor c1, QColor c2) {
    return (256-abs(c1.red()-c2.red()))*(256-abs(c1.green()-c2.green()))*(256-abs(c1.blue()-c2.blue()));
}


QColor PictureColorsSelector::getNearestColor(QList<QColor> colors, QColor toCompare) {
    QColor ret = toCompare;
    int currentMax = 0;
    for (int i = 0; i<colors.size(); i++) {
        int tmp = compareColor(colors[i], toCompare);
        if (tmp>currentMax) {
            currentMax = tmp;
            ret = colors[i];
        }
    }
    return ret;
}


void PictureColorsSelector::setDataType(int type) {
    if (type == 0) wm_groupSize->hide();
    else if (type == 1) wm_groupSize->show();
    else wm_groupSize->show();
}


void PictureColorsSelector::setUseMultipleFiles(bool use) {
    wm_viewMultipleFiles->setEnabled(use);
    wm_setMultipleFiles->setEnabled(use);
    wm_buttonClearFiles->setEnabled(use);
}


void PictureColorsSelector::addFiles() {
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Selectionner des images"), QString(),
            tr("Images (*.bmp *.png *.gif *.jpg *.jpeg *.xbm *.xpm *pbm *ppm *pgm)"));
    m_fileNameList.append(files);
}


void PictureColorsSelector::clearFileList() {
    m_fileNameList.clear();
}


void PictureColorsSelector::viewFileList() {
    QListWidget *viewer = new QListWidget(this);
    viewer->setWindowModality(Qt::WindowModal);
    viewer->setWindowFlags(Qt::Window);
    viewer->setSelectionMode(QAbstractItemView::MultiSelection);
    for (int i=0; i<m_fileNameList.size(); i++) {
        QListWidgetItem *item = new QListWidgetItem(m_fileNameList.at(i));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        item->setIcon(QFileIconProvider().icon(QFileInfo(m_fileNameList.at(i))));
        viewer->addItem(item);
    }
    viewer->setResizeMode(QListView::Adjust);
    viewer->show();
    viewer->setAttribute(Qt::WA_DeleteOnClose);
}


void PictureColorsSelector::nextItem() {
    m_currentItemNumber++;
    updatePreviewInformations();
}

void PictureColorsSelector::nextFile() {
    m_currentFileNumber++;
    m_currentItemNumber = 1;
    updatePreviewInformations();
}

void PictureColorsSelector::previousItem() {
    m_currentItemNumber--;
    updatePreviewInformations();
}

void PictureColorsSelector::previousFile() {
    m_currentFileNumber--;
    m_currentItemNumber = 1;
    updatePreviewInformations();
}

void PictureColorsSelector::viewMapOrOverview() {
    m_currentItemNumber = m_previewItems.at(m_currentFileNumber-1).size();
    updatePreviewInformations();
}

void PictureColorsSelector::updatePreviewInformations() {
    QImage image;

    //On met des valeurs par défaut aux widgets :
    wm_buttonPreviousItem->setEnabled(true);
    wm_buttonPreviousFile->setEnabled(true);
    wm_buttonNextFile->setEnabled(true);
    wm_buttonNextItem->setEnabled(true);
    wm_buttonViewMap->setEnabled(true);
    wm_previewFileNumber->setText(tr("Fichier %1/%2").arg(m_currentFileNumber).arg(m_previewItems.size()));
    wm_previewItemNumber->setText(tr("Item %1/%2").arg(m_currentItemNumber).arg(m_previewItems.first().size()));

    //On modifie les informations en fonction du nombre de fichier et du fichier actuel...
    if (m_currentFileNumber == 1) {
        wm_previewFileName->setText(wm_fileName->text());
        wm_buttonPreviousFile->setEnabled(false);
    }
    else wm_previewFileName->setText(m_fileNameList.at(m_currentFileNumber-1));
    if (m_currentFileNumber > m_fileNameList.size()) wm_buttonNextFile->setEnabled(false);

    //...puis en fonction du nombre et du numéro actuel d'item...
    if (m_currentItemNumber == 1) wm_buttonPreviousItem->setEnabled(false);
    if (m_currentItemNumber >= m_previewItems.at(m_currentFileNumber-1).size())
        wm_buttonNextItem->setEnabled(false);

    //...et enfin en fonction du type de donnée :
    if (wm_dataType->currentIndex() == 0) {
        wm_buttonPreviousItem->setEnabled(false);
        wm_buttonNextItem->setEnabled(false);
        wm_buttonViewMap->setEnabled(false);
    }
    else if (wm_dataType->currentIndex() == 1) {
        wm_buttonViewMap->setText(tr("Aperçu"));
    }
    else {
        wm_buttonViewMap->setText(tr("Map"));
    }

    image = m_previewItems.at(m_currentFileNumber-1).at(m_currentItemNumber-1);

    wm_previewArea->takeWidget();
    wm_previewLabel->setPixmap(QPixmap::fromImage(image.scaled(image.size() *= 2)));
    wm_previewLabel->resize(image.size() *= 2);
    wm_previewArea->setWidget(wm_previewLabel);
    wm_previewWidget->show();
}
