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

#ifndef PICTURECOLORSSELECTOR_H
#define PICTURECOLORSSELECTOR_H

#include <QtGui>
#include "samplingcolorlabel.h"
#include <tcproject.h>
#include <array2d.h>

class PictureColorsSelector : public QDialog
{
Q_OBJECT
public:
    PictureColorsSelector(QWidget *parent = 0);
    TcProject getImported() {
        return imported;
    }

protected:
    void setButtonStyleSheet(QPushButton *button, QColor color);
    QString to2CharHex(int num);
    QImage getConvertedImage(QPixmap toConvert, bool showProgressBar = false);
    int compareColor(QColor c1, QColor c2);
    QColor getNearestColor(QList<QColor> colors, QColor toCompare);
    void updatePreviewInformations();

signals:

public slots:
    void setButtonColor(QRgb color);
    void setZoomedContent(QPoint center);
    void selectFile();
    void setColorType(int type);
    void setTransType(int type);
    void setFileName(QString name);
    void previewResult();
    void finishWizard();
    void setDataType(int type);
    void abortWizard();
    void setUseMultipleFiles(bool use);
    void addFiles();
    void clearFileList();
    void viewFileList();

protected slots:
    void nextItem();
    void nextFile();
    void previousItem();
    void previousFile();
    void viewMapOrOverview();

protected:
    QHBoxLayout *layout, *l_scroll, *l_horizontalSettings, *l_groupDataType;
    QVBoxLayout *l_groupColors, *l_settings, *l_groupZoom, *l_subLayout, *l_groupFile, *l_groupResult;
    QVBoxLayout *l_groupSize;
    QScrollArea *wm_scroll;
    QGroupBox *wm_groupColors, *wm_zoom, *wm_groupFile, *wm_groupResult, *wm_groupDataType;
    QGroupBox *wm_groupSize;
    SamplingColorLabel *wm_picture, *wm_zoomed;
    QPushButton *wm_white, *wm_black, *wm_light, *wm_dark, *wm_trans;
    QPushButton *wm_buttonFile, *wm_buttonPreview, *wm_buttonFinish, *wm_buttonAbort;
    QPushButton *wm_viewMultipleFiles, *wm_setMultipleFiles, *wm_buttonClearFiles;
    QLineEdit *wm_fileName;
    QCheckBox *wm_boxMultipleFiles;
    QComboBox *wm_colorType, *wm_transType, *wm_dataType;
    QSpinBox *wm_width, *wm_height;

    QWidget *wm_previewWidget;
    QPushButton *wm_buttonNextFile, *wm_buttonPreviousFile, *wm_buttonNextItem, *wm_buttonPreviousItem;
    QPushButton *wm_buttonViewMap;
    QScrollArea *wm_previewArea;
    QLabel *wm_previewLabel, *wm_previewItemNumber, *wm_previewFileNumber;
    QLineEdit *wm_previewFileName;
    QVBoxLayout *l_previewLayout;
    QHBoxLayout *l_buttonBar, *l_statusBar;

    QButtonGroup *m_buttonGroup;
    QColor m_colorWhite, m_colorBlack, m_colorLight, m_colorDark, m_colorTrans;
    int m_colorNumber;
    QString m_fileName;
    QList<QString> m_fileNameList;
    TcProject imported;
    QList<QList<QImage> >m_previewItems; //Liste des items de chaque fichier en preview.
    int m_currentFileNumber, m_currentItemNumber;
};

#endif // PICTURECOLORSSELECTOR_H
