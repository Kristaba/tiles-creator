/*  Copyright 2009, 2010 Léo Grange

This file is part of TCWebDownloader.

TCWebDownloader is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

TCWebDownloader is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with TCWebDownloader.  If not, see <http://www.gnu.org/licenses/>. */

#include <QtGui/QApplication>
#include "WebDownloader.h"

int main(int argc, char *argv[])
{
    QString url("http://tilescreator.calctools.fr/updates");

    if (argc > 1) {
        url.clear();
        for (int i=0; argv[1][i] != 0; i++) {
            url.append(argv[1][i]);
        }
    }

    QApplication a(argc, argv);
    WebDownloader w(url);
    w.show();
    return a.exec();
}
