/*  Copyright 2009, 2010 Léo Grange

This file is part of G85SDKExport, a plugin for Tiles Creator.

G85SDKExport is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

G85SDKExport is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with G85SDKExport.  If not, see <http://www.gnu.org/licenses/>. */

#include "g85sdkexport.h"

void G85SDKExport::exportProject(TcProject project) {
    bool ok;
    QString exportType = QInputDialog::getItem(qApp->activeWindow(), tr("Exporter un projet pour G85"),
            tr("Comment voulez-vous exporter les données?"), QStringList() <<
            tr("Export total utilisant libtc.") << tr("Export des bitmaps des tiles...")
            << tr ("Export total utilisant libtc, version Assembleur GCC (experimental)"), 0
            , false, &ok);
    if (ok) {
        if (exportType == tr("Export total utilisant libtc.")) totalExport(project, CSDK);
        else if (exportType == tr("Export des bitmaps des tiles...")) tilesExport(project.tiles);
        else if (exportType == tr ("Export total utilisant libtc, version Assembleur GCC (experimental)"))
            totalExport(project, GCCASM);
    }
}


void G85SDKExport::totalExport(const TcProject& project, ExportType eType) {
    QString path = QFileDialog::getExistingDirectory(qApp->activeWindow(), tr("Exporter un projet pour G85"));
    if (!path.isEmpty()) {
        QString fileName = QInputDialog::getText(qApp->activeWindow(), tr("Exporter un projet pour G85"),
                tr("Nom du header et du fichier source"));
        if (!fileName.isEmpty()) {
            QFile file(QDir(path).filePath(fileName+".h"));
            if (file.exists()) file.remove();
            file.open(QIODevice::WriteOnly);

            QString hName = nameToValidString(fileName).toUpper()+"_H";
            file.write(QString("#ifndef "+hName+"\n#define "+hName+"\n\n#include \"libtc.h\"\n\n\n").toAscii());

            //Enum des tiles :
            if (!project.tiles.isEmpty()) {
                file.write(QString("enum Tiles {").toAscii());
                for (int i=0; i<project.tiles.size(); i++) {
                    QString name = project.tiles.at(i).name;
                    file.write(QString("t_"+nameToValidString(name)).toAscii());
                    if (i < project.tiles.size()-1) file.write(QString(", ").toAscii());
                }
                file.write(QString("};\n\n").toAscii());
            }

            //Enum des tilesets :
            if (!project.tilesets.isEmpty()) {
                file.write(QString("enum Tilesets {").toAscii());
                for (int i=0; i<project.tilesets.size(); i++) {
                    QString name = project.tilesets.at(i).name;
                    file.write(QString("ts_"+nameToValidString(name)).toAscii());
                    if (i < project.tiles.size()-1) file.write(QString(", ").toAscii());
                }
                file.write(QString("};\n\n").toAscii());
            }

            //Enum des maps :
            if (!project.maps.isEmpty()) {
            file.write(QString("enum Maps {").toAscii());
                for (int i=0; i<project.maps.size(); i++) {
                    QString name = project.maps.at(i).name;
                    file.write(QString("m_"+nameToValidString(name)).toAscii());
                    if (i < project.tiles.size()-1) file.write(QString(", ").toAscii());
                }
                file.write(QString("};\n\n").toAscii());
            }

            file.write(QString("#endif //"+hName+"\n").toAscii());
            file.close();

            /*#
              # Dans le cas d'un export en C :
              #*/
            if (eType == CSDK) {
                //Ecriture du fichier .c
                file.setFileName(QDir(path).filePath(fileName+".c"));
                if (file.exists()) file.remove();
                file.open(QIODevice::WriteOnly);


                file.write(QString("#include \"" + fileName + ".h\"\n\n\nGrayTile aTile(int tileId){\n    GrayTile ret = {0,0,0,0};\n"
                                   "    unsigned char *l=0, *d=0;\n    switch (tileId) {").toAscii());

                //Code de la fonction permettant d'initialiser un tile :
                for (int i=0; i<project.tiles.size(); i++) {
                    QString name = project.tiles.at(i).name;
                    QImage image = project.tiles.at(i).tile;
                    int w = image.width();
                    int h = image.height();
                    file.write(QString("\n    case t_"+nameToValidString(name)+" :\n        l = (unsigned char*) "
                                       "malloc("+QString::number(((w-1)/8+1)*h)+");\n        d = (unsigned char*) malloc("
                                       +QString::number(((w-1)/8+1)*h)+");\n        ret.width = "+QString::number(image.width())
                                       +";\n        ret.height = "+QString::number(image.height())+"; \n        {\n            "
                                       "unsigned char l_tmp[] = {").toAscii());

                    QByteArray bitmap = bitmapFromGrayImage(image, TC::LightGray);
                    for (int j=0; j<bitmap.size(); j++) {
                        file.write(getHexa2((unsigned char)bitmap.at(j)).toAscii());
                        if (j+1 < bitmap.size()) file.write(QString(", ").toAscii());
                    }

                    file.write(QString("};\n            unsigned char d_tmp[] = {").toAscii());

                    bitmap = bitmapFromGrayImage(image, TC::DarkGray);
                    for (int j=0; j<bitmap.size(); j++) {
                        file.write(getHexa2((unsigned char)bitmap.at(j)).toAscii());
                        if (j+1 < bitmap.size()) file.write(QString(", ").toAscii());
                    }

                    file.write(QString("};\n            ret.light = (unsigned char*)memcpy((void*)l, (void*)l_tmp, "+QString::number(((w-1)/8+1)*h)+
                                       ");\n            ret.dark = (unsigned char*)memcpy((void*)d, (void*)d_tmp, "+QString::number(((w-1)/8+1)*h)+");\n        }\n"
                                       "        break;").toAscii());
                }

                file.write(QString("}\n    return ret;\n}\n\nTileset aTileset(int tilesetId){\n    Tileset ret;\n"
                                   "    unsigned char *p;    switch (tilesetId) {").toAscii());

                //Code de la fonction permettant d'initialiser un tileset :
                for (int i=0; i<project.tilesets.size(); i++) {
                    QVector<int> t = project.tilesets.at(i).tiles.toVector();
                    QVector<unsigned char> p = project.tilesets.at(i).properties.toVector();
                    QString name = project.tilesets.at(i).name;
                    int tileWidth = project.tilesets.at(i).tilesSize.width();
                    int tileHeight = project.tilesets.at(i).tilesSize.height();

                    file.write(QString("\n    case ts_"+nameToValidString(name)+" :\n        ret.size = "
                                       +QString::number(t.size())+";\n        ret.tileWidth = "+QString::number(tileWidth)
                                       +";\n        ret.tileHeight = "+QString::number(tileHeight)+";\n        ret.tiles ="
                                       " calloc (" + QString::number(t.size()) + ", sizeof(GrayTile*));\n        p = "
                                       "(unsigned char*)malloc(" + QString::number(t.size()) + ");").toAscii());

                    for (int j=0; j<t.size(); j++) {
                        if (t.at(j) >= 0) {
                            file.write(QString("\n        *(ret.tiles["+QString::number(j)+"] = malloc(sizeof(GrayTile)))"
                                                   " = aTile(t_"+nameToValidString(project.tiles.at(t.at(j)).name)
                                                   +");").toAscii());
                        }
                    }
                    file.write(QString("\n        {\n            unsigned char p_tmp[] = {").toAscii());

                    for (int j=0; j<p.size(); j++) {
                        file.write(getHexa2((unsigned char)p.at(j)).toAscii());
                        if (j+1 < p.size()) file.write(QString(", ").toAscii());
                    }

                    file.write(QString("};\n            ret.properties = (unsigned char*)memcpy((void*)p, (void*)p_tmp, " +
                                       QString::number(t.size()) + ");\n        }\n        break;").toAscii());
                }

                file.write(QString("}\n    return ret;\n}\n\nMap aMap(int mapId){\n    Map ret;\n"
                                   "    switch (mapId) {").toAscii());


                //Code de la fonction permettant d'initialiser une map :
                for (int i=0; i<project.maps.size(); i++) {
                    QVector<unsigned char> m = QVector<unsigned char>::fromStdVector(project.maps.at(i).map.toVector());
                    QString name = project.maps.at(i).name;
                    int tilesetId = project.maps.at(i).tilesetId;
                    QString tilesetName = project.tilesets.at(tilesetId).name;
                    int mapWidth = project.maps.at(i).map.columnCount();
                    int mapHeight = project.maps.at(i).map.rowCount();

                    file.write(QString("\n    case m_"+nameToValidString(name)+" :\n        ret.tileset = aTileset(ts_"
                                       +nameToValidString(tilesetName)+");\n        ret.width = "+QString::number(mapWidth)
                                       +";\n        ret.height = "+QString::number(mapHeight)+";\n        ret.map = malloc("
                                       +QString::number(m.size())+" * sizeof(unsigned char));\n        {\n            "
                                       "unsigned char tmp[] = {").toAscii());

                    for (int j=0; j<mapHeight; j++) {
                        for (int k=0; k<mapWidth; k++) {
                            file.write(QString("0x"+QString::number((unsigned char)m.at(j*mapWidth+k), 16).toUpper()).toAscii());
                            if (j*mapWidth+k+1 < m.size()) file.write(QString(", ").toAscii());
                        }
                        if (j+1 < mapHeight) file.write(QString("\n                                   ").toAscii());
                    }

                    file.write(QString("};\n            memcpy((void*)ret.map, (void*)tmp, "+QString::number(m.size())
                                       +");\n        }\n        break;").toAscii());
                }
                file.write(QString("}\n    return ret;\n}\n\n").toAscii());
                file.close();
            }


            /*#
              # Dans le cas d'un export en Assembleur (GCC ou Renesas) :
              #*/
            if (eType == GCCASM || eType == SDKASM) {
                QDir(path).mkdir("rsc");
                QDir rscPath(path);
                rscPath.cd("rsc");

                //Ecriture des fichiers assembleur :
                if (eType == GCCASM) file.setFileName(QDir(path).filePath(fileName+".S"));
                else file.setFileName(QDir(path).filePath(fileName+".src"));
                if (!file.exists()) {
                    file.open(QIODevice::WriteOnly);

                    // Fichier à compiler contenant tous les autres :
                    file.write(QString("! Generated by Tiles Creator, the graphics resources editor for Casio programming.\n"
                                       "! This file contain the tiles, tileset and map allocating functions.\n\n"
                                       "! Comment this line make aTile don't copy tile data on RAM and use pointers to Program Memory (section .rodata)\n"
                                       "#define COPY_TO_RAM\n\n"
                                       "#include \"rsc/tile.s\"\n"
                                       "#include \"rsc/tiledata.s\"\n"
                                       "#include \"rsc/tileset.s\"\n"
                                       "#include \"rsc/tilesetdata.s\"\n"
                                       "#include \"rsc/map.s\"\n"
                                       "#include \"rsc/mapdata.s\"\n\n"
                                       "\t.section \".text\"\n"
                                       "\t.align 4\n"
                                       "SyscallAddress :\n"
                                       "\t.long 0x80010070\n"
                                       "MallocSyscallID:\n"
                                       "\t.long 0xacd\n").toAscii());
                    file.close();
                }


                // Fichier contenant les données des tiles :
                file.setFileName(rscPath.filePath("tiledata.s"));
                if (file.exists()) file.remove();
                file.open(QIODevice::WriteOnly);

                file.write(QString("TileNumber:\n"
                                   "\t.long\t"+QString::number(project.tiles.size())+
                                   "\n\n\t.section \".rodata\"\n"
                                   "\t.align 4\n"
                                   "Tile_JTable:\n").toAscii());
                for (int i=0; i<project.tiles.size(); i++)
                    file.write(QString("\t.long\tTile_Data_"+nameToValidString(project.tiles.at(i).name)+"\n").toAscii());

                for (int i=0; i<project.tiles.size(); i++) {
                    file.write(QString("\n\nTile_Data_"+nameToValidString(project.tiles.at(i).name)+":\n"
                                       "\t.word\t"+QString::number(project.tiles.at(i).tile.width())+"\n"
                                       "\t.word\t"+QString::number(project.tiles.at(i).tile.height())+"\n").toAscii());
                    QByteArray bitmap = bitmapFromGrayImage(project.tiles.at(i).tile, TC::LightGray);
                    for (int j=0; j<bitmap.size(); j++)
                        file.write(QString("\t.byte\t"+getHexa2(bitmap.at(j))+"\n").toAscii());
                    file.write(QString("\n").toAscii());
                    bitmap = bitmapFromGrayImage(project.tiles.at(i).tile, TC::DarkGray);
                    for (int j=0; j<bitmap.size(); j++)
                        file.write(QString("\t.byte\t"+getHexa2(bitmap.at(j))+"\n").toAscii());
                }
                file.close();


                // Fichier contenant les données des tilesets :
                file.setFileName(rscPath.filePath("tilesetdata.s"));
                if (file.exists()) file.remove();
                file.open(QIODevice::WriteOnly);

                file.write(QString("TilesetNumber:\n"
                                   "\t.long\t"+QString::number(project.tilesets.size())+
                                   "\n\n\t.section \".rodata\"\n"
                                   "\t.align 4\n"
                                   "Tileset_JTable:\n").toAscii());
                for (int i=0; i<project.tilesets.size(); i++)
                    file.write(QString("\t.long\tTileset_Data_"+nameToValidString(project.tilesets.at(i).name)+"\n")
                               .toAscii());

                for (int i=0; i<project.tilesets.size(); i++) {
                    file.write(QString("\n\n\t.align 4\n"
                                       "Tileset_Data_"+nameToValidString(project.tilesets.at(i).name)+":\n"
                                       "\t.word\t"+QString::number(project.tilesets.at(i).tiles.size())+"\n"
/* /!\ Penser à modifier! */           "\t.word\t"+QString::number(1)+"\n"
                                       "\t.word\t"+QString::number(project.tilesets.at(i).tilesSize.width())+"\n"
                                       "\t.word\t"+QString::number(project.tilesets.at(i).tilesSize.height())+"\n")
                               .toAscii());
                    for (int j=0; j<project.tilesets.at(i).tiles.size(); j++)
                        file.write(QString("\t.long\t"+QString::number(project.tilesets.at(i).tiles.at(j))
                                           +"\n").toAscii());
                    file.write(QString("\n").toAscii());
                    for (int j=0; j<project.tilesets.at(i).tiles.size(); j++)
                        file.write(QString("\t.byte\t"+getHexa2(project.tilesets.at(i).properties.at(j))
                                           +"\n").toAscii());
                }
                file.close();

                // Fichier contenant les données des maps :
                file.setFileName(rscPath.filePath("mapdata.s"));
                if (file.exists()) file.remove();
                file.open(QIODevice::WriteOnly);

                file.write(QString("MapNumber:\n"
                                   "\t.long\t"+QString::number(project.maps.size())+
                                   "\n\n\t.section \".rodata\"\n"
                                   "\t.align 4\n"
                                   "Map_JTable:\n").toAscii());
                for (int i=0; i<project.maps.size(); i++)
                    file.write(QString("\t.long\tMap_Data_"+nameToValidString(project.maps.at(i).name)+"\n")
                               .toAscii());

                for (int i=0; i<project.maps.size(); i++) {
                    file.write(QString("\n\n\t.align 4\n"
                                       "Map_Data_"+nameToValidString(project.maps.at(i).name)+":\n"
                                       "\t.long\t"+QString::number(project.maps.at(i).tilesetId)+"\n"
                                       "\t.word\t"+QString::number(project.maps.at(i).map.columnCount())+"\n"
                                       "\t.word\t"+QString::number(project.maps.at(i).map.rowCount())+"\n")
                               .toAscii());
                    for (int j=0; j<project.maps.at(i).map.rowCount(); j++)
                        for (int k=0; k<project.maps.at(i).map.columnCount(); k++)
                            file.write(QString("\t.byte\t"+getHexa2(project.maps.at(i).map.at(j,k))
                                            +"\n").toAscii());
                    file.write(QString("\n").toAscii());
                }
                file.close();

                //Copie de tile.s, tileset.s et map.s :
                if (QFile::exists(rscPath.filePath("tile.s"))) QFile::remove(rscPath.filePath("tile.s"));
                QFile::copy(":/lib/tile.s", rscPath.filePath("tile.s"));
                if (QFile::exists(rscPath.filePath("tileset.s"))) QFile::remove(rscPath.filePath("tileset.s"));
                QFile::copy(":/lib/tileset.s", rscPath.filePath("tileset.s"));
                if (QFile::exists(rscPath.filePath("map.s"))) QFile::remove(rscPath.filePath("map.s"));
                QFile::copy(":/lib/map.s", rscPath.filePath("map.s"));
            }


            //Copie de libtc.h et libtc.c :
            if (QFile::exists(QDir(path).filePath("libtc.h"))) QFile::remove(QDir(path).filePath("libtc.h"));
            QFile::copy(":/lib/libtc.h", QDir(path).filePath("libtc.h"));
            if (QFile::exists(QDir(path).filePath("libtc.c"))) QFile::remove(QDir(path).filePath("libtc.c"));
            QFile::copy(":/lib/libtc.c", QDir(path).filePath("libtc.c"));

            QMessageBox::information(qApp->activeWindow(), tr("Export terminé!"),
                tr("Export des données terminé avec succès!"));
        }
    }
}



void G85SDKExport::tilesExport(const QList<TcTile> &tiles) {
    bool ok;
    QString exportType = QInputDialog::getItem(qApp->activeWindow(), tr("Exporter un projet pour G85"),
            tr("Quelles couleurs utiliser?"), QStringList() << tr("Noir et blanc uniquement (1 buffer)")
             << tr("4 niveaux de gris (2 buffer)"), 0, false, &ok);

    if (ok) {
        int type;
        if (exportType == tr("Noir et blanc uniquement (1 buffer)")) type = 0;
        else type = 1;

        QString path = QFileDialog::getSaveFileName(qApp->activeWindow(),
            tr("Choisissez le nom du fichier exporté"));
        if (!path.isEmpty()) {
            QFile file(path);
            if (file.exists()) file.remove();
            file.open(QIODevice::WriteOnly);

            for (int i=0; i<tiles.size(); i++) {
                if (type == 0) {
                    QByteArray array = bitmapFromGrayImage(tiles.at(i).tile, TC::Black);
                    file.write(QString("unsigned char " + nameToValidString(tiles.at(i).name) +
                        "[] = {").toAscii());
                    for (int j=0; j<array.size(); j++) {
                        file.write(QString(getHexa2((unsigned char)array.at(j))).toAscii());
                        if (j+1 < array.size()) {
                            file.write(QString(", ").toAscii());
                            /*if ((j != 0) && (j%((tiles.at(i).tile.width()-1)/8+1) == 0))
                                file.write(QString("\n").toAscii());*/
                        }
                    }
                    file.write(QString("};\n\n").toAscii());
                }

                else {
                    QByteArray array = bitmapFromGrayImage(tiles.at(i).tile, TC::LightGray);
                    file.write(QString("unsigned char l_" + nameToValidString(tiles.at(i).name) +
                        "[] = {").toAscii());
                    for (int j=0; j<array.size(); j++) {
                        file.write(QString(getHexa2((unsigned char)array.at(j))).toAscii());
                        if (j+1 < array.size()) file.write(QString(", ").toAscii());
                    }
                    file.write(QString("};\n").toAscii());

                    array = bitmapFromGrayImage(tiles.at(i).tile, TC::DarkGray);
                    file.write(QString("unsigned char d_" + nameToValidString(tiles.at(i).name) +
                        "[] = {").toAscii());
                    for (int j=0; j<array.size(); j++) {
                        file.write(QString(getHexa2((unsigned char)array.at(j))).toAscii());
                        if (j+1 < array.size()) file.write(QString(", ").toAscii());
                    }
                    file.write(QString("};\n\n").toAscii());
                }
            }
            file.close();
        }
    }
}



QString G85SDKExport::nameToValidString(const QString &s) {
    QString ret;
    for (int i=0; i<s.size(); i++) {
        QChar c = s.at(i);
        if (c.isLower() || c.isDigit()) ret.append(c);
        else if (c.isUpper()) ret.append(c.toLower());
        else ret.append('_');
    }
    return ret;
}


QByteArray G85SDKExport::bitmapFromGrayImage(const QImage &image, TC::Color wichBitmap) {
    QByteArray ret;
    if (wichBitmap == TC::White || wichBitmap == TC::Black || wichBitmap == TC::LightGray ||
            wichBitmap == TC::DarkGray) {
        QByteArray array = PixelEditor::toGrayTileArray(image);
        int count = 0;
        for (int h=0; h<image.height(); h++) {
            for (int j=0; j<(image.width()-1)/8+1; j++) {
                unsigned char c = 0;
                for (int k=0; k<8; k++) {
                    if (count%image.width() != 0 || count/image.width() <= h) {
                        if (wichBitmap == TC::LightGray) {
                            if (array.at(count) == TC::LightGray || array.at(count) == TC::Black)
                                c += 1 << (7-k);
                        }
                        else if (wichBitmap == TC::DarkGray){
                            if (array.at(count) == TC::DarkGray || array.at(count) == TC::Black)
                                c += 1 << (7-k);
                        }
                        else if (wichBitmap == TC::White || wichBitmap == TC::Black) {
                            if (array.at(count) == TC::Black)
                                c += 1 << (7-k);
                        }
                        count ++;
                    }
                    else k=8;
                }
                ret.append(c);
            }
        }
    }
    return ret;
}


QString G85SDKExport::getHexa2(unsigned char num) {
    QString ret;
    ret = "0x"+QString::number(num, 16).toUpper();
    if (ret.size() < 4) ret.insert(2, '0');
    return ret;
}


Q_EXPORT_PLUGIN2(g85sdk_export, G85SDKExport)
