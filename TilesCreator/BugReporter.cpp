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

#include "BugReporter.h"

BugReporter::BugReporter () : QDialog(){
    QVBoxLayout *layout = new QVBoxLayout;
    QLabel *version = new QLabel (tr("Version : ") + TC_VERSION);
    version->setAlignment(Qt::AlignHCenter);
    text = new QTextEdit;
    text->setToolTip(tr("Entrer une description du bug (ce qui se passe, ce que vous avez fait avant...)."
    "\nEssayez d'être assez clair pour qu'il soit possible de le reproduire."));
    submit = new QPushButton (tr("Soumettre"));
    submit->setToolTip(tr("Envoyer le message au serveur."));
    layout->addWidget(version);
    layout->addWidget(text);
    QHBoxLayout *l_user = new QHBoxLayout;
        QLabel *userLabel = new QLabel(tr("Pseudo (optionnel) "));
        l_user->addWidget(userLabel);
        user = new QLineEdit;
        user->setMaxLength(30);
        l_user->addWidget(user);
        layout->addLayout(l_user);
    QHBoxLayout *l_email = new QHBoxLayout;
        QLabel *emailLabel = new QLabel(tr("Mail (optionnel) "));
        l_email->addWidget(emailLabel);
        email = new QLineEdit;
        email->setMaxLength(30);
        l_email->addWidget(email);
        layout->addLayout(l_email);
    layout->addWidget(submit);
    setLayout (layout);
    connect(submit, SIGNAL(clicked()), this, SLOT(startUpload()));
    setWindowTitle (tr("Signaler un bug"));
}

void BugReporter::startUpload () {
    submit->setEnabled(false);
    url = new QUrl (BUG_BDD_WEBSITE);
    data = new QByteArray (QString("version="TC_VERSION"&user=" + user->text().toLatin1() + "&email=" + email->text().toLatin1()
            + "&message=" + text->toPlainText()).toLatin1());

    header = new QHttpRequestHeader ("POST", url->path());
    header->setValue("Host", url->host());
    header->setContentType("application/x-www-form-urlencoded"); // important
    header->setContentLength(data->length());

    // request
    http = new QHttp (url->host());
    http->request(*header, *data);

    connect (http, SIGNAL(done(bool)), this, SLOT(close()));
}
