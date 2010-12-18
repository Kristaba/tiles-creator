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


/**
  * @file BugReporter.h
  * @brief Fichier contenant la classe BugReporter.
  * @author Léo Grange
  * @version 1.0
  * @date 21/09/2009
  */

#ifndef BUGREPORTER_H
#define BUGREPORTER_H

#include <QtGui>
#include <QtNetwork>

#define TC_VERSION "1.1b"  /**< @brief Version indicative de Tiles Creator dans son ensemble.*/
/**
  * @brief URL de la page d'enregistrement des reports de bug.
  */
#define BUG_BDD_WEBSITE "http://hem-atom.homelinux.com/tc/bugreport.php"

/**
  * @brief La classe BugReporter fournit un moyen simple de report des bugs par l'utilisateur.
  *
  * Pour utiliser ce dialogue, il suffit d'en faire une nouvelle instance.\n
  * Pour le moment il est impossible de changer l'URL de destination.\n
  * La classe QHttp étant obsolette depuis depuis Qt 4.6, une prochaine version vera le jour,
  * utilisant QNetworkAccessManager.
  * @todo Réécrire la classe en utilisant QNetworkAccessManager à la place de QHttp.
  * @todo Pouvoir changer l'URL de destination lors de la création d'une instance.
  */
class BugReporter : public QDialog {
    Q_OBJECT

    public:
        /**
          * @brief Constructeur par défaut.
          */
        BugReporter ();

    protected slots:
        void startUpload();

    protected:
        QUrl *url;
        QByteArray *data;
        QHttpRequestHeader *header;
        QHttp *http;

        QTextEdit *text;
        QLineEdit *user, *email;
        QPushButton *submit;
};

#endif // BUGREPORTER_H
