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

#include "HttpDown.h"


HttpDown::HttpDown(QString adresseS, QString newName) {

     QUrl adresse (adresseS);
     http = new QHttp(adresse.host(),80);
     QObject::connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(stopConnection(int)));
     file = new QFile(newName);
     file->open(QIODevice::WriteOnly);

     m_id = http->get(adresse.toString(), file);
}



void HttpDown::stopConnection(int id) {
     if (id == m_id) {
          http->close();
          file->close();
          emit finish();
     }
}

