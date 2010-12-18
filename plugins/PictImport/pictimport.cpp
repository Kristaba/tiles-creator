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

#include "pictimport.h"


TcProject PictImport::import() {
    PictureColorsSelector *selector = new PictureColorsSelector(qApp->activeWindow());
    selector->setWindowTitle("PictImport");
    if(selector->exec() == QDialog::Accepted)
        return selector->getImported();
    else return TcProject();
}

Q_EXPORT_PLUGIN2(pict_import, PictImport)
