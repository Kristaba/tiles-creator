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

#include <QtGui>
#include "tilescreator.h"

#ifdef __linux__
#define TC_WD_NAME "webdownloader"
#else
#define TC_WD_NAME "webdownloader.exe"
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    TilesCreator w;

    QSettings settingsFile("settings.ini", QSettings::IniFormat);
    QString local = settingsFile.value("config/language", "fr").toString();
    if (local == "fr") w.changeFrench();
    else if (local == "en") w.changeEnglish();

    QString instruction;
    if (argc > 1) {
        for (int i=0; argv[1][i] != 0; i++) instruction.append(argv[1][i]);
    }
    if (QFile::exists("wd_update")) {
        if (QFile::exists(TC_WD_NAME)) {
            QFile::remove(TC_WD_NAME);
        }
        QFile::rename("wd_update", TC_WD_NAME);
        QMessageBox::information(0, QObject::tr("Mise à jour de l'updater"), QObject::tr("Le logiciel de mises à jour à correctement été mis à jour."));
    }

    w.show();
    return app.exec();
}
