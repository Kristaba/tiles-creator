/*  Copyright 2009, 2010 L�o Grange

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

/**
  * @file g85sdkexport.h
  * @brief Fichier contenant G85SDKExport, une classe plugin impl�mentant ExportInterface.
  * @author L�o Grange
  * @version 1.1
  * @date 08/12/2009
  *     Last edit : 13/04/2010
  */

#ifndef G85SDKEXPORT_H
#define G85SDKEXPORT_H

#include <QtGui>
#include <exportinterface.h>
#include <pixeleditor.h>

/**
  * @brief Plugin d'export vers un code pour le SDK Graph 85.
  */
class G85SDKExport : public QObject, public ExportInterface
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
        return tr("SDK G85 (C/asm)");
    }

    QString pluginVersion() {
        return "1.1";
    }

    QString tcVersion() {
        return "1.20";
    }

    QString pluginDetails() {
        return "Ce plugin permet d'exporter un projet Tiles Creator en code C utilisable"
                " avec le SDK pour Graph 85/75/95 fournit par Casio.\n Il associe au code"
                " g�n�r� un header (libtc.h) contenant de nombreuses fonctions utiles"
                " permettant d'utiliser simplement et avec puissance les ressources g�n�r�es.";
    }

    QList<QString> pluginAuthors() {
        QList<QString> ret;
        ret.append("L�o Grange (Kristaba)");
        return ret;
    }

    QString relatedWebsite() {
        return "http://www.planet-casio.com/Fr/";
    }

private:
    enum ExportType {CSDK, GCCASM, SDKASM};
    void totalExport(const TcProject& project, ExportType type);
    void tilesExport(const QList<TcTile> &tiles);
    QString getHexa2(unsigned char num);
};

/**
  * @page g85export Documentation de l'export Graph 85 SDK
  * Le plugin G85SDKExport permet d'ajouter � Tiles Creator un module d'exportation d'un projet
  * afin de l'utiliser avec le SDK G85 (disponible entre autre <a href="http://www.planet-casio.com/Fr/logiciels/voir_un_logiciel_casio.php?cat=3">
  * ici </a>) et la library Revolution-Fx de Kucalc afin d'assurer le support des niveaux de gris
  * (disponible <a href="http://sourceforge.net/projects/revolution-fx/">sur SourceForge</a>).\n
  * Cette page explique comment utiliser les fichiers export�s afin de cr�er des add-in pour
  * Graph 85(SD)/75/95 utilisant des ressources cr��es � l'aide de Tiles Creator.\n\n
  * <b>Sommaire :</b>\n @ref generalOverview\n @ref usingInProject \n\n
  * @section generalOverview Aper�u g�n�ral
  * Pour utiliser facilement les exports de Tiles Creator, une library a �t� con�u, qui apporte une
  * gestion haut niveau de l'affichage. Bien qu'�crite en C, <b>libtc</b> utilise de nombreuses
  * structures de mani�re "objet", afin de rendre plus simple la manipulation de tiles, tilesets
  * et maps, repr�sent�es respectivement par @ref GrayTile (ou @ref AlphaGrayTile pour des tiles
  * avec un canal alpha -transparence-), @ref Tileset et @ref Map. La documentation de ce fichier
  * est disponible ici : @ref libtc.h.\n\n
  * @section usingInProject Utiliser l'export dans un projet
  * Tout d'abord, ajoutez votrefichier.c et libtc.c � votre projet SDK, puis incluez le header
  * n�cessaire dans vos sources :
  * @code #include "votrefichier.h" //votrefichier.h inclus libtc.h donc pas besoin de l'inclure ici. @endcode
  * Il faut savoir que libtc.h est enti�rement dynamique, la m�moire est allou�e selon les besoins.
  * Chaque ressource que vous souhaitez utiliser doit donc �tre allou�e grace aux m�thodes apropri�es.
  * \n Ces fonctions d'allocation sont aTile(), aTileset() et aMap(), qui prennent chacune un int en
  * param�tre et renvoient une structure de type respectivement GrayTile, Tileset ou Map. \n
  * Le int � envoyer est l'identifiant interne de la ressource. Une enum�ration pour chaque type de
  * ressource est pr�sent au d�but de votrefichier.h, et correspond � une transformation du nom donn�
  * � chaque ressource dans Tiles Creator :\n
  * Chaque valeur �num�r�e commence pat t_ pour les tiles, ts_ pour les tilesets et m_ pour les maps.
  * Ceci est suivi du nom donn� � la ressource dans lequel tous les caract�res non alphanum�riques
  * sont remplac�s par des underscore (caract�re '_') et les majuscules deviennent des minuscules.
  * \n Par exemple, un tile nomm� "Maison super belle!" aura pour identifiant "t_maison_super_belle_".
  * \n \n Pour allouer un tile "t_perso01" et une map "m_ville_port" le code sera :
  * @code GrayTile perso;
  * Map map;
  * perso = aTile(t_perso01);
  * map = aMap(m_ville_port); @endcode
  * Afin de lib�rer la m�moire allou�e de cette fa�on, libtc.h contient les fonctions freeTile(),
  * freeTileset() et freeMap() qui prennent respectivement en argument GrayTile*, Tileset* ou Map*.
  * \n Pensez � bien lib�rer la m�moire que vous allouez sans quoi vous risquez d'avoir des "fuites
  * de m�moire" qui peuvent conduire, � terme, � un plantage de votre programme!\n\n
  * Afin d'utiliser facilement les ressources, libtc.h contient un certain nombre de fonctions
  * d'affichage. Nous n'�voquerons ici que deux d'entre elles : drawGrayTile() qui permet de dessiner
  * un tile, et drawMap() qui permet de dessiner une map. R�f�rez vous � leur documentation pour
  * plus d'information.\n\n
  * Enfin, deux fonctions permettent d'avoir des informations sur les propri�t�s des tiles (pas
  * totalement implant� pour l'instant, seul une propri�t�, BLOCKING, est accessible) :
  * testTilesetProperty() qui renvoit TRUE ou FALSE selon l'�tat du flag de la propri�t� du tile
  * indiqu� d'un tileset, et testMapProperty, qui renvoit l'�tat du flag de la propri�t� du tile
  * d'une map, indiqu� par ses coordon�es sur celle-ci.\n\n
  * Voici un exemple complet de gestion d'une map avec scrolling pour vous donner une id�e du code.
  * \n Il peut para�tre un compliqu�, car il g�re les d�placements de la map, utilise le
  * double buffering, et s'adapte � toutes les tailles de tiles automatiquement.
  * @include g85example.c
  * Ce second exemple est un code minimaliste, qui affiche simplement la partie en haut � droite
  * d'une map dont les tiles font 16*16 pixels, et place un tile en position (27, 41).
  * @include g85_minimalist_example.c
  */

/* @dot
* digraph example {
*     graph [ rankdir="LR", smoothing="none", dpi=96.0, spline="true"];
*     node [shape=box3d, fontname=Helvetica, fontsize=10];
*     edge [ arrowhead="empty", style="full", color="#0000FF", weight=3.0 ];
*     exIn [ label="Interface d'Export" URL="\ref ExportInterface"];
*     plugIn [ label="Plugin" shape=ellipse URL="\ref G85SDKExport"];
*     aTile [ label="aTile" shape=box fillcolor="#FFAAAA"];
*     plugIn -> exIn [ arrowhead="empty", style="full", color="#0000FF" ];
*     plugIn -> aTile;
*     exIn -> aTile;
* }
* @enddot*/

#endif // G85SDKEXPORT_H
