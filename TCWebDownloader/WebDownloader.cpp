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

#include "WebDownloader.h"

WebDownloader::WebDownloader(QString webLink) : QDialog(){
    m_webLink = webLink;
    layout = new QVBoxLayout;
    state = 0;
    changed = false;
    wdUpdated = false;
    setMaximumHeight (148);
    setWindowTitle (tr("Mise à jour Tiles Creator"));

    doing = new QLabel (tr("Vérification des mises à jour depuis ") + m_webLink
                           + "/" + PATH_OS + "/newversion.ref");
    layout->addWidget(doing);

    fileProgress = new QProgressBar;
    fileProgress->setRange (0, 100);
    layout->addWidget(fileProgress);
    fileProgress->setValue(-1);

    totalProgress = new QProgressBar;
    totalProgress->setRange (0, 0);
    totalProgress->setFormat("%v/%m");
    layout->addWidget(totalProgress);
    totalProgress->setValue(-1);

    buttonsLayout = new QHBoxLayout;
    buttonDownload = new QPushButton (tr("Télécharger"));
    buttonDownload->setEnabled(false);
    buttonDownload->setMaximumWidth(120);
    buttonClose = new QPushButton (tr("Fermer"));
    buttonClose->setEnabled(false);
    buttonClose->setMaximumWidth(120);
    buttonsLayout->addWidget(buttonDownload);
    buttonsLayout->addWidget(buttonClose);
    layout->addLayout(buttonsLayout);

    down = new HttpDown (m_webLink + "/" + PATH_OS + "/newversion.ref", "newversion.ref");

    QObject::connect(down->getHttp(), SIGNAL(dataReadProgress(int, int)), this, SLOT(setFileBar(int, int)));
    QObject::connect(down, SIGNAL(finish()), this, SLOT(checkRefFile()));
    QObject::connect(buttonDownload, SIGNAL(clicked()), this, SLOT(startDownload()));
    QObject::connect(buttonClose, SIGNAL(clicked()), this, SLOT(close()));

    setLayout(layout);
}



void WebDownloader::checkRefFile() {
    if (down != 0) delete down;
    //Lire les versions actuelles :
    QFile version ("tilescreator.version");
    version.open(QIODevice::ReadOnly | QIODevice::Text);
    while (!version.atEnd()) {
        QString line (version.readLine());
        if (line != "\n") {
            actualFiles.append (line.section(' ', 0, 0));
            QString tmp (line.section (' ', -1, 1));
            actualFilesVersion.append (tmp.section('\n', 0, 0));
        }
    }
    version.close();
    //Lire les nouvelles versions :
    QFile newVersion ("newversion.ref");
    newVersion.open(QIODevice::ReadOnly | QIODevice::Text);
    if (newVersion.readLine() == "TILESCREATOR_NEW_VERSION_REFERENCE_FILE\n") {
        while (!newVersion.atEnd()) {
            QString line (newVersion.readLine());
            if (line != "\n") {
                newFiles.append (line.section(' ', 0, 0));
                newFilesVersion.append (line.section (' ', 1, 1));
                QString tmp (line.section (' ', -1, 2));
                newHttpLink.append(tmp.section('\n', 0, 0));
            }
        }
    }
    newVersion.close();
    //Faire une liste des N° de fichiers à remplacer :
    for (int i=0; i<newFiles.size(); i++) {
        bool remplace = true;
        for (int b = 0; b<actualFiles.size(); b++) {
            if (actualFiles.at(b) == newFiles.at(i)) {
                bool Ok1, Ok2;
                float val1, val2;
                val1 = actualFilesVersion.at(b).toFloat(&Ok1);
                val2 = newFilesVersion.at(i).toFloat(&Ok2);
                if (Ok1 && Ok2) {
                    if (val1 >= val2) remplace = false;
                }
                else if (actualFilesVersion.at(b) == newFilesVersion.at(i)) remplace = false;
            }
        }
        if (remplace) filesToDownload.append(i);
    }
    if (filesToDownload.size() == 0) {
        buttonDownload->setEnabled(false);
        buttonClose->setEnabled(true);
        doing->setText(tr("Aucune nouvelle mise à jour détectée..."));
    }
    else {
        buttonDownload->setEnabled(true);
        buttonClose->setEnabled(true);
        doing->setText(tr("Prêt à télécharger les mises à jour...\nNombre de fichiers à remplacer : ") + QString::number(filesToDownload.size()));
    }
    setFileBar(-1, 0);
}

void WebDownloader::startDownload() {
    buttonDownload->setEnabled(false);
    buttonClose->setEnabled(false);
    totalProgress->setRange(0, filesToDownload.size());
    nextFile();
}


void WebDownloader::nextFile() {
    totalProgress->setValue(state);
    state++;
    //Vérifier si il reste des fichiers à télécharger :
    if (filesToDownload.size() < state) {
        //Plus aucun fichier à télécharger...
        doing->setText(tr("Tiles Creator est bien mis à jour!\nFermez cette fenêtre pour quitter l'application."));

        QFile changeVersion ("tilescreator.version");
        changeVersion.open(QIODevice::ReadWrite | QIODevice::Text);

        //Réécrire la version des fichiers téléchargés :
        for (int i=0; i<filesToDownload.size(); i++) {
            QString tmp;
            tmp = newFiles.at(filesToDownload.at(i)) + " " + newFilesVersion.at(filesToDownload.at(i)) + "\n";
            changeVersion.write(tmp.toLocal8Bit());
        }
        //Réécrire la version des fichiers non modifiés :
        for (int i=0; i<actualFiles.size(); i++) {
            QString tmp;
            bool newF = false;
            for (int b=0; b<filesToDownload.size(); b++) {
                if (actualFiles.at(i) == newFiles.at(filesToDownload.at(b))) newF = true;
            }
            if (!newF) tmp = actualFiles.at(i) + " " + actualFilesVersion.at(i) + "\n";
            changeVersion.write(tmp.toLocal8Bit());
        }
        changeVersion.close();
        buttonClose->setEnabled(true);
        changed = true;
    }
    else {
        //Télécharger le fichier demandé...
        int i = filesToDownload.at(state-1);

        QFile actualFile (newFiles.at(i));
        QString path = actualFile.fileName().section('/', 0, -2);
        if (!path.isEmpty()) {
            QDir dir("./");
            dir.mkpath(path);
        }
        actualFile.remove();

        if (newFiles.at(i) == "wd_update") wdUpdated = true;

        down = new HttpDown (m_webLink + newHttpLink.at(i), newFiles.at(i));
        doing->setText(tr("Téléchargement depuis ") + m_webLink + newHttpLink.at(i));
        QObject::connect(down->getHttp(), SIGNAL(dataReadProgress(int, int)), this, SLOT(setFileBar(int, int)));
        QObject::connect(down, SIGNAL(finish()), this, SLOT(nextFile()));
    }
}
