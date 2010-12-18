#include "basicasioexport.h"

void BasiCasioExport::exportProject(TcProject project){
    this->totalExport(project);
}

void BasiCasioExport::totalExport(const TcProject& project){
    if( !project.tiles.isEmpty() ){
        for( int i = 0 ; i < project.tiles.size() ; i++ ){

        }
    }
}

void BasiCasioExport::tilesExport(const QList<TcTile> &tiles){

}

QString BasiCasioExport::getHexa2(unsigned char num){
    QString ret;
    ret = "0x"+QString::number(num, 16).toUpper();
    if (ret.size() < 4) ret.insert(2, '0');
    return ret;
}

QString BasiCasioExport::nameToValidString(const QString &s) {
    QString ret;
    for (int i=0; i<s.size(); i++) {
        QChar c = s.at(i);
        if (c.isLower() || c.isDigit()) ret.append(c);
        else if (c.isUpper()) ret.append(c.toLower());
        else ret.append('_');
    }
    return ret;
}

Q_EXPORT_PLUGIN2(BasiCasioExporter, BasiCasioExport)
