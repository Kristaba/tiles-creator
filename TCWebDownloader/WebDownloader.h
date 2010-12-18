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
  * @file WebDownloader.h
  * @brief Fichier contenant la classe WebDownloader.
  * @author Léo Grange
  * @version 1.2
  * @date 28/01/2009 - 13/12/2009
  */


#ifndef WEBDOWNLOADER_H
#define WEBDOWNLOADER_H

#include <QtGui>
#include "HttpDown.h"

#ifdef __linux__
#define PATH_OS "linux"
#define UPDATE_WD "./TilesCreator -u &"
#else
#define PATH_OS "win32"
#define UPDATE_WD "start TilesCreator.exe -u"
#endif


/**
* @brief La classe WebDownloader fournit une interface simple pour la mise à jour d'un programme.
*
* WebDownloader permet de mettre à jour facilement un programme. Pour l'utiliser il suffit de
* créer une nouvelle instance en donnant l'URL du site de mise à jour.
* @todo Générifier la classe en mettant des get/set permettant de modifier le chemin du système
* d'expoitation ainsi que le nom du programme et la commande permettant de le lancer en mode
* mise à jour de l'updater, afin de la rendre utilisable dans d'autres projets.
* @todo Créer un système de mises à jour des sources, basé sur l'empreinte MD5 de chaque fichier.
* @todo Ajouter un système de vérifiaction discrète des mises à jour, qui se ferait en tâche de
* fond, et qui ouvrirait une fenêtre uniquement si des mises à jours sont détectées.
* @todo Ajouter une possibilité de visualiser les détails des fichiers à remplacer.
*/
class WebDownloader : public QDialog {

    Q_OBJECT

public:
    /**
      * @brief Constructeur.
      * @param webLink adresse de la section de mise à jour du site de mises à jour.
      */
    WebDownloader(QString webLink);


protected slots:
    /**
    * @brief Démarre le téléchargement des mises à jour.
    */
    void startDownload();

    void checkRefFile();
    void nextFile();
    void setFileBar(int done, int total) {
        fileProgress->setMaximum (total);
        fileProgress->setValue (done);
    }
    void closeEvent (QCloseEvent *event) {
        if (wdUpdated) system(UPDATE_WD);
        if (changed) {
            qApp->quit();
        }
        if (state == 0) QDialog::closeEvent (event);
    }

protected:
    QString m_webLink;
    QVBoxLayout *layout;
    QHBoxLayout *buttonsLayout;
    QLabel *doing;
    QProgressBar *fileProgress, *totalProgress;
    QPushButton *buttonDownload, *buttonClose;

    HttpDown *down;
    bool changed, wdUpdated;
    int state;

    QVector<QString> actualFiles, actualFilesVersion, newFiles, newFilesVersion, newHttpLink;
    QVector<int> filesToDownload;
};

#endif // WEBDOWNLOADER_H
