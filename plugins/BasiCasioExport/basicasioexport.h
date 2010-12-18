#ifndef BASICASIOEXPORT_H
#define BASICASIOEXPORT_H

#include <QtGui>
#include <exportinterface.h>
#include <pixeleditor.h>

class BasiCasioExport : public QObject, public ExportInterface
{
    Q_OBJECT
    Q_INTERFACES(ExportInterface)

public:
    void exportProject(TcProject project);
    static QString nameToValidString(const QString &s);

    //Si bitmap == TC::LightGray renvoit le buffer light, si = TC::DarkGray le buffer dark,
    // et si TC::Black ou TC::White le buffer unique noir/blanc.
    static QByteArray bitmapFromGrayImage(const QImage &i, TC::Color bitmap);

    QString pluginName() {
        return QString("BasiCasioExporter");
    }

    QString pluginVersion() {
        return "1.0";
    }

    QString tcVersion() {
        return "1.10";
    }

    QString pluginDetails() {
        return "Ce plugin permet d'exporter un projet Tiles Creator en code Basic Casio."
                "Il suffit de recopier le code.";
    }

    QList<QString> pluginAuthors() {
        QList<QString> ret;
        ret.append("Adrien Pirault (light_spirit)");
        return ret;
    }

    QString relatedWebsite() {
        return "http://www.planet-casio.com/Fr/";
    }

private:
    void totalExport(const TcProject& project);
    void tilesExport(const QList<TcTile> &tiles);
    QString getHexa2(unsigned char num);
};

#endif // BASICASIOEXPORT_H
