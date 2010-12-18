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

#include "tileseditor.h"

TilesEditor::TilesEditor(ProjectWidget *project, QWidget *parent) : QWidget(parent)
{
    m_project = project;

    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    //Layout des paramètres :
    QHBoxLayout *l_para = new QHBoxLayout;

    //GroupBox concernant la taille des tiles :
    groupLenght = new QGroupBox(tr("Taille du tile"));
        QVBoxLayout *l_groupLenght = new QVBoxLayout;
        groupLenght->setLayout(l_groupLenght);
        //Layout de la largeur :
        QHBoxLayout *l_width = new QHBoxLayout;
            tileWidth = new QSpinBox;
            tileWidth->setKeyboardTracking(false);
            tileWidth->setMinimum(1);
            tileWidth->setMaximum(128);
            tileWidth->setValue(16);
            tileWidth->setSuffix(tr(" pixel"));
            labelWidth = new QLabel(tr("Largeur : "));
            l_width->addWidget(labelWidth);
            l_width->addWidget(tileWidth);
            l_width->addStretch();
            l_groupLenght->addLayout(l_width);
        //Layout de la hauteur :
        QHBoxLayout *l_height = new QHBoxLayout;
            tileHeight = new QSpinBox;
            tileHeight->setKeyboardTracking(false);
            tileHeight->setMinimum(1);
            tileHeight->setMaximum(64);
            tileHeight->setValue(16);
            tileHeight->setSuffix(tr(" pixel"));
            labelHeight = new QLabel(tr("Hauteur : "));
            l_height->addWidget(labelHeight);
            l_height->addWidget(tileHeight);
            l_height->addStretch();
            l_groupLenght->addLayout(l_height);
        l_para->addWidget(groupLenght);
    //GroupBox concernant les options d'affichage :
    groupAff = new QGroupBox(tr("Affichage"));
        QVBoxLayout *l_groupAff = new QVBoxLayout;
        groupAff->setLayout(l_groupAff);
        //Layout du zoom :
        QHBoxLayout *l_zoom = new QHBoxLayout;
            zoom = new QSpinBox;
            zoom->setMinimum(1);
            zoom->setMaximum(16);
            zoom->setValue(4);
            labelZoom = new QLabel(tr("Zoom : "));
            l_zoom->addWidget(labelZoom);
            l_zoom->addWidget(zoom);
            l_zoom->addStretch();
            l_groupAff->addLayout(l_zoom);
        //Checkbox de la grille :
        grid = new QCheckBox(tr("Grille"));
        grid->setChecked(true);
        l_groupAff->addWidget(grid);
        //Selecteur de couleurs :
        colorsSelecter = new ColorsSelecter;
        //colorsSelecter->setUseAlpha(true);
        l_groupAff->addWidget(colorsSelecter);
        l_para->addWidget(groupAff);
    //GroupBox concernant le tile/sprite :
    groupTile = new QGroupBox(tr("Tile"));
        QVBoxLayout *l_groupTile = new QVBoxLayout;
        groupTile->setLayout(l_groupTile);
        //Label et LineEdit du nom du tile ou du sprite :
        nameLabel = new QLabel(tr("Nom du tile :"));
        l_groupTile->addWidget(nameLabel);
        name = new QLineEdit;
        l_groupTile->addWidget(name);
        //CheckBox du canal Alpha :
        alphaChanel = new QCheckBox(tr("Utiliser un canal Alpha (transparence)"));
        l_groupTile->addWidget(alphaChanel);
        //Bouton d'effacement :
        clear = new QPushButton(tr("Effacer"));
        l_groupTile->addWidget(clear);
        //Bouton d'enregistrement :
        save = new QPushButton(tr("Enregistrer"));
        l_groupTile->addWidget(save);
        l_para->addWidget(groupTile);


    layout->addLayout(l_para);
    layout->addStretch();
    //layout->setSizeConstraint(QLayout::SetFixedSize);

    QHBoxLayout *l_editor = new QHBoxLayout;
    l_editor->addStretch();
    editor = new PixelEditor(tileWidth->value(), tileHeight->value(), zoom->value());
    editor->setGrid(true);
    l_editor->addWidget(editor);
    l_editor->addStretch();
    layout->addLayout(l_editor);
    layout->addStretch();

    saveStatus = SaveEnable;

    connect(tileHeight, SIGNAL(valueChanged(int)), editor, SLOT(changeHeight(int)));
    connect(tileWidth, SIGNAL(valueChanged(int)), editor, SLOT(changeWidth(int)));
    connect(zoom, SIGNAL(valueChanged(int)), editor, SLOT(changeZoom(int)));
    connect(grid, SIGNAL(clicked(bool)), editor, SLOT(setGrid(bool)));
    connect(clear, SIGNAL(clicked()), editor, SLOT(clearTile()));
    connect(save, SIGNAL(clicked()), this, SIGNAL(saveTile()));
    connect(colorsSelecter, SIGNAL(frontChanged(TC::Color)), editor, SLOT(changeFrontColor(TC::Color)));
    connect(colorsSelecter, SIGNAL(backChanged(TC::Color)), editor, SLOT(changeBackColor(TC::Color)));
    connect(alphaChanel, SIGNAL(toggled(bool)), this, SLOT(setAlphaChanel(bool)));
    connect(editor, SIGNAL(pixelsModified(QList<QPoint>,TC::Color)), this, SLOT(pixelsModified(QList<QPoint>,TC::Color)));
    connect(name, SIGNAL(textEdited(QString)), this, SLOT(setTileName(QString)));
}



QImage TilesEditor::createNewImage() {
    QImage tmp(tileWidth->value(), tileHeight->value(), QImage::Format_Indexed8);
    tmp.setColor(TC::White, qRgb(255,255,255));
    tmp.setColor(TC::Black, qRgb(0,0,0));
    tmp.fill(TC::White);
    return tmp;
}



void TilesEditor::drawTile(QString n, QImage tile) {
    if (name->text() != n) name->setText(n);
    QImage *pix = new QImage(tile);
    editor->setImage(pix);
    disconnect(tileHeight, SIGNAL(valueChanged(int)), editor, SLOT(changeHeight(int)));
    disconnect(tileWidth, SIGNAL(valueChanged(int)), editor, SLOT(changeWidth(int)));
    tileWidth->setValue(pix->width());
    tileHeight->setValue(pix->height());
    connect(tileHeight, SIGNAL(valueChanged(int)), editor, SLOT(changeHeight(int)));
    connect(tileWidth, SIGNAL(valueChanged(int)), editor, SLOT(changeWidth(int)));
    editor->setWidthHeight(pix->width(), pix->height());
}


void TilesEditor::changeSaveStatus(SaveButtonStatus s) {
    if (s == SaveEnable && saveStatus != SaveEnable) {
        disconnect(save, SIGNAL(clicked()), this, SIGNAL(addToProject()));
        connect(save, SIGNAL(clicked()), this, SIGNAL(saveTile()));
        save->setText(tr("Enregistrer"));
        save->setEnabled(true);
    }

    if (s == SaveDisable && saveStatus != SaveDisable) {
        disconnect(save, SIGNAL(clicked()), this, SIGNAL(addToProject()));
        disconnect(save, SIGNAL(clicked()), this, SIGNAL(saveTile()));
        save->setText(tr("Enregistrer"));
        save->setEnabled(false);
    }

    if (s == AddToProject && saveStatus != AddToProject) {
        disconnect(save, SIGNAL(clicked()), this, SIGNAL(saveTile()));
        connect(save, SIGNAL(clicked()), this, SIGNAL(addToProject()));
        save->setText(tr("Ajouter au projet"));
        save->setEnabled(true);
    }
    saveStatus = s;
}


void TilesEditor::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        groupLenght->setTitle(tr("Taille du tile"));
        labelWidth->setText(tr("Largeur : "));
        labelHeight->setText(tr("Hauteur : "));
        groupAff->setTitle(tr("Affichage"));
        labelZoom->setText(tr("Zoom : "));
        grid->setText(tr("Grille"));
        groupTile->setTitle(tr("Tile"));
        nameLabel->setText(tr("Nom du tile :"));
        clear->setText(tr("Effacer"));
        save->setText(tr("Enregistrer"));
        alphaChanel->setText(tr("Utiliser un canal Alpha (transparence)"));
        tileWidth->setSuffix(tr(" pixel"));
        tileHeight->setSuffix(tr(" pixel"));
    }
    QWidget::changeEvent(event);
}


void TilesEditor::setAlphaChanel(bool alpha) {
    if (alphaChanel->isChecked() != alpha) alphaChanel->setChecked(alpha);
    else {
        colorsSelecter->setUseAlpha(alpha);
        if (!alpha) {
            QImage *tmpImage = editor->image();
            for (int i=0; i < tmpImage->width(); i++)
                for (int j=0; j < tmpImage->height(); j++)
                    if (tmpImage->pixelIndex(i, j) == TC::Transparency)
                        tmpImage->setPixel(i, j, TC::White);
        }
    }
}


void TilesEditor::pixelsModified(QList<QPoint> points, TC::Color newColor) {
    if (m_index.isValid()) {
        m_project->undoGroup()->activeStack()->push(
            new TilePixmapModifiedCommand(points, newColor, m_index, m_project));
    }
}


void TilesEditor::setTileIndex(const QPersistentModelIndex &index) {
    m_index = index;
}


void TilesEditor::setTileName(QString newName) {
    QUndoCommand *cmd = new SetItemNameCommand(newName, m_index, m_project);
    //if (name->setText(newName);
    m_project->undoGroup()->activeStack()->push(cmd);
}
