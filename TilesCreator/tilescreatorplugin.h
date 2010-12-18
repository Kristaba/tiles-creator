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
  * @file tilescreatorplugin.h
  * @brief Fichier contenant la classe TilesCratorPlugin.
  * @author Léo Grange
  * @version 1.0
  * @date Créé le 06/01/2009\n
  *     Dernière modification le 06/01/2009
  */

#ifndef TILESCREATORPLUGIN_H
#define TILESCREATORPLUGIN_H

#include <QtCore>


/**
  * @brief La classe TilesCreatorPlugin est la classe mère de tout plugin de TC.
  *
  * TilesCreatorPlugin contient les méthodes de base permettant de renseigner Tiles Creator
  * sur le plugin (nom, version, détails...).
  */
class TilesCreatorPlugin
{    
public:
    /**
      * @brief Enumération des différents types de plugins.
      */
    enum PluginType { Other = 0, /**< Type non supporté... */
                      Import, /**< Plugin d'import. */
                      Export, /**< Plugin d'export. */
                  };

    /**
      * @brief Renvoie le nom du plugin (nom affiché dans les menus).
      * @return le nom du plugin
      */
    virtual QString pluginName() = 0;

    /**
      * @brief Renvoie le type du plugin.
      * @return le type du plugin
      */
    virtual PluginType pluginType() = 0;

    /**
      * @brief Renvoie la version du plugin.
      *
      * La version du plugin doit être numérotée de cette façon : xx.yy (avec xx et yy des entiers positifs
      * ). xx est le numéro majeur de version et yy le numéro mineur. \n
      * Exemples : 1.0 ; 0.012 ; 12.6 ; 2010.0601
      * @return la version du plugin
      */
    virtual QString pluginVersion() = 0;

    /**
      * @brief Renvoie la version de Tiles Creator pour laquelle a été conçu le plugin.
      * @see pluginVersion()
      * @return version la de Tiles Creator pour laquelle a été conçu le plugin.
      */
    virtual QString tcVersion() = 0;

    /**
      * @brief Renvoie des détails sur le plugin, qui peuvent être enrichis en HTML.
      * @return les détails du plugin
      */
    virtual QString pluginDetails() {
        return QString();
    }

    /**
      * @brief Renvoie la liste des auteurs du plugin.
      * @return une liste contenant les noms complets de tous les auteurs du plugin
      */
    virtual QList<QString> pluginAuthors() {
        return QList<QString>();
    }

    /**
      * @brief Renvoie une URL de site web en relation avec le plugin (site de l'auteur, etc...).
      * @return une URL vers un site web en rapport avec le plugin
      */
    virtual QString relatedWebsite() {
        return QString();
    }

    virtual ~TilesCreatorPlugin() {}

};

//Q_DECLARE_INTERFACE(TilesCreatorPlugin, "com.planet-casio.TilesCreator.TilesCreatorPlugin/1.0")


/**
  * @page makeplugin Faire un plugin pour Tiles Creator
  * Depuis sa version 1.10, Tiles Creator embarque le code nécessaire à la gestion de plugins
  * d'import et d'export des données, permettant ainsi à la fois une plus grande modularité,
  * et la possibilité à chaque dévelopeur indépendant d'ajouter des fonctionnalités sans avoir
  * à prendre connaissance ou à modifier le code de Tiles Creator.\n\n
  * <b>Sommaire :</b>\n @ref pluginGeneralities \n @ref pluginInTC \n
  * @section pluginGeneralities Généralités
  * Tiles Creator ayant été développé à l'aide du framework <a HREF="http://qt.nokia.com/">Qt</a>,
  * il tire partie de la gestion des plugins par celui-ci. Cela à pour conséquence de limiter
  * les plugins à utiliser Qt eux aussi, au moins de façon superficielle (théoriquement, rien
  * n'empêche d'utiliser d'autres GUI et framework que Qt dans les classes "internes" du plugin).
  * \n La création d'un plugin Qt est une chose très simple que nous aborderons succintement ici,
  * mais je vous invite à vous rendre, si vous voulez plus de précision ou que vous ne comprenez
  * pas bien, sur <a HREF="http://doc.trolltech.com/4.6/plugins-howto.html">la page consacrée</a>
  * dans la documentation de Qt (en) ou sur <a HREF="http://doc.qtfr.org/post/2007/02/18/
  * Creation-de-plugin-avec-Qt">un tuto</a> du site qtfr (fr).\n\n
  * Pour créer un plugin avec Qt, il faut avant tout créer un fichier .pro indiquant à qmake que
  * le projet est un plugin. La base de ce fichier .pro devrait ressembler à ceci :
  * @code
  * TEMPLATE = lib
  * CONFIG += release \
  *     plugin
  * HEADERS = votreplugin.h \
  *     autre_classe.h
  * SOURCES = votreplugin.cpp \
  *     autre_classe.cpp
  * TARGET = nom_du_plugin
  * @endcode
  * Pour qu'une classe soit l'entrée d'un plugin Qt, celle-ci doit hériter obligatoirement de QObject
  * (ou d'une classe fille comme QWidget) ET de la ou des classes qui font office d'interface Qt.\n
  * N'oubliez pas de mettre la macro Q_OBJECT dans la section privée de la classe. Après Q_OBJECT
  * , une autre macro est à insérer : Q_INTERFACES(NomDeLaClasseDinterface). Celle-ci permet à Qt
  * de savoir que votre classe implémente NomDeLaClasseDinterface.\n
  * Dans le fichier votreplugin.cpp, ajoutez en dehors de tout espace de nom (dans l'espace global) :
  * Q_EXPORT_PLUGIN2(nom_du_plugin, VotrePlugin)\n
  * @warning nom_du_plugin doit être le même que celui indiqué dans le TARGET du .pro!
  *
  * Enfin, il faut maintenant redefinir toutes les fonctions virtual pure des interfaces implémentées
  * dans le plugin.\n\n
  * La compilation de ce projet donnera une library dynamique binaire (.so ou .dll selon le système)
  * ainsi qu'un fichier .a .\n
  * @section pluginInTC Pour Tiles Creator
  * Dans Tiles Creator, deux interfaces sont utilisables pour le moment : ImportInterface qui permet
  * d'ajouter une fonction d'import, et ExportInterface qui permet d'ajouter une fonction d'export.\n
  * Toutes les interfaces héritent de TilesCreatorPlugin qui contient essentiellement des méthodes
  * permettant d'avoir des informations sur la classe (nom, auteurs, version, etc...).\n\n
  * ImportInterface et ExportInterface ont toutes deux une méthode principale, respectivement import()
  * et exportProject() qui nécéssitent l'utilisation des classes-structures TcProject, TcTile, TcTileset,
  * TcMap. Il est donc préférable de regarder leur documentation avant de commencer un plugin.\n\n
  * Pour faire votre propre plugin pour Tiles Creator, il vous faudra télécharger les sources
  * nécessaires (au minimum tilescreatorplugin.h, importinterface.h, exportinterface.h et array2d.h).
  * Il existe deux manières de les rendres accécibles depuis votre projet : ou bien encopiant les
  * sources avec les sources du plugin et en faisant \#include "...", ou bien en ajoutant un paramètre
  * dans le .pro : INCLUDEPATH += C:\Repertoire\des\sources\de\TilesCreator\n
  * Le repertoire peut être absolu ou relatif, et les fichiers contenus à l'intérieur peuvent être
  * inclus en faisant \#include <...>.\n\n
  * Une fois votre plugin compilé, il suffira d'ajouter le binaire .so ou .dll dans le repertoire
  * plugins de Tiles Creator pour que celui-ci les prenne en compte dès le prochain lancement!
  */

#endif // TILESCREATORPLUGIN_H
