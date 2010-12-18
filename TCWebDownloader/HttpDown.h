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

/**
  * @file HttpDown.h
  * @brief Fichier contenant la classe HttpDown.
  * @author Léo Grange
  * @version 1.0
  * @date 28/01/2009
  */

#ifndef HTTPTEST_H_INCLUDED
#define HTTPTEST_H_INCLUDED

#include <QObject>
#include <QtNetwork>


/**
* @brief HttpDown fournit une classe simple d'enregistrement du contenu d'une URL sur le
* système de fichiers.
*
* Pour utiliser HttpDown, il suffit de construire une instance en précisant l'URL et le nom
* du fichier qui enregistrera le contenu de cette dernière.
* @todo Changer le QHttp par un QNetworkAccessManager.
*/
class HttpDown : public QObject{

    Q_OBJECT

protected:
    QHttp *http;
    QFile *file;
    int m_id;

public:
    /**
    * @brief Constructeur.
    * @param adresse URL du contenu.
    * @param newName nom du fichier sous lequel sera sauvé le contenu.
    */
    HttpDown(QString adresse, QString newName);

    /**
    * @brief Renvoie le QHttp utilisé.
    * @return un pointeur vers le QHttp utilisé.
    */
    QHttp *getHttp () {
        return http;
    }

signals:
    void finish(); /**< Signal émis lorsque l'enregistrement est terminé. */

public slots:
    /**
    * @brief Arrête une connexion dont on connait l'id.
    * @param id id de la connexion à arrêter.
    */
    void stopConnection(int id);
};


#endif // HTTPTEST_H_INCLUDED

