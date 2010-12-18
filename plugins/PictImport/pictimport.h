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

#ifndef PICTIMPORT
#define PICTIMPORT

#include <QtGui>
#include <importinterface.h>
#include "picturecolorsselector.h"

class PictImport : public QObject, public ImportInterface
{
    Q_OBJECT
    Q_INTERFACES(ImportInterface)

public:
    TcProject import();

    QString pluginName() {
        return tr("Picture Import");
    }

    QString pluginVersion() {
        return "0.17";
    }

    QString pluginDetails() {
        return "PictImport est un plugin permettant d'importer des ressources pour Tiles Creator à"
               " partir de fichiers images.";
    }

    QList<QString> pluginAuthors() {
        QList<QString> ret;
        ret.append("Léo Grange (Kristaba)");
        return ret;
    }

    QString relatedWebsite() {
        return "http://www.planet-casio.com/Fr/";
    }

    QString tcVersion() {
        return "1.10";
    }
};

#endif //PICTIMPORT
