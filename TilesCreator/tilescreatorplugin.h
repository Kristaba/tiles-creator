/*  Copyright 2009, 2010 L�o Grange

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
  * @author L�o Grange
  * @version 1.0
  * @date Cr�� le 06/01/2009\n
  *     Derni�re modification le 06/01/2009
  */

#ifndef TILESCREATORPLUGIN_H
#define TILESCREATORPLUGIN_H

#include <QtCore>


/**
  * @brief La classe TilesCreatorPlugin est la classe m�re de tout plugin de TC.
  *
  * TilesCreatorPlugin contient les m�thodes de base permettant de renseigner Tiles Creator
  * sur le plugin (nom, version, d�tails...).
  */
class TilesCreatorPlugin
{    
public:
    /**
      * @brief Enum�ration des diff�rents types de plugins.
      */
    enum PluginType { Other = 0, /**< Type non support�... */
                      Import, /**< Plugin d'import. */
                      Export, /**< Plugin d'export. */
                  };

    /**
      * @brief Renvoie le nom du plugin (nom affich� dans les menus).
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
      * La version du plugin doit �tre num�rot�e de cette fa�on : xx.yy (avec xx et yy des entiers positifs
      * ). xx est le num�ro majeur de version et yy le num�ro mineur. \n
      * Exemples : 1.0 ; 0.012 ; 12.6 ; 2010.0601
      * @return la version du plugin
      */
    virtual QString pluginVersion() = 0;

    /**
      * @brief Renvoie la version de Tiles Creator pour laquelle a �t� con�u le plugin.
      * @see pluginVersion()
      * @return version la de Tiles Creator pour laquelle a �t� con�u le plugin.
      */
    virtual QString tcVersion() = 0;

    /**
      * @brief Renvoie des d�tails sur le plugin, qui peuvent �tre enrichis en HTML.
      * @return les d�tails du plugin
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
  * Depuis sa version 1.10, Tiles Creator embarque le code n�cessaire � la gestion de plugins
  * d'import et d'export des donn�es, permettant ainsi � la fois une plus grande modularit�,
  * et la possibilit� � chaque d�velopeur ind�pendant d'ajouter des fonctionnalit�s sans avoir
  * � prendre connaissance ou � modifier le code de Tiles Creator.\n\n
  * <b>Sommaire :</b>\n @ref pluginGeneralities \n @ref pluginInTC \n
  * @section pluginGeneralities G�n�ralit�s
  * Tiles Creator ayant �t� d�velopp� � l'aide du framework <a HREF="http://qt.nokia.com/">Qt</a>,
  * il tire partie de la gestion des plugins par celui-ci. Cela � pour cons�quence de limiter
  * les plugins � utiliser Qt eux aussi, au moins de fa�on superficielle (th�oriquement, rien
  * n'emp�che d'utiliser d'autres GUI et framework que Qt dans les classes "internes" du plugin).
  * \n La cr�ation d'un plugin Qt est une chose tr�s simple que nous aborderons succintement ici,
  * mais je vous invite � vous rendre, si vous voulez plus de pr�cision ou que vous ne comprenez
  * pas bien, sur <a HREF="http://doc.trolltech.com/4.6/plugins-howto.html">la page consacr�e</a>
  * dans la documentation de Qt (en) ou sur <a HREF="http://doc.qtfr.org/post/2007/02/18/
  * Creation-de-plugin-avec-Qt">un tuto</a> du site qtfr (fr).\n\n
  * Pour cr�er un plugin avec Qt, il faut avant tout cr�er un fichier .pro indiquant � qmake que
  * le projet est un plugin. La base de ce fichier .pro devrait ressembler � ceci :
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
  * Pour qu'une classe soit l'entr�e d'un plugin Qt, celle-ci doit h�riter obligatoirement de QObject
  * (ou d'une classe fille comme QWidget) ET de la ou des classes qui font office d'interface Qt.\n
  * N'oubliez pas de mettre la macro Q_OBJECT dans la section priv�e de la classe. Apr�s Q_OBJECT
  * , une autre macro est � ins�rer : Q_INTERFACES(NomDeLaClasseDinterface). Celle-ci permet � Qt
  * de savoir que votre classe impl�mente NomDeLaClasseDinterface.\n
  * Dans le fichier votreplugin.cpp, ajoutez en dehors de tout espace de nom (dans l'espace global) :
  * Q_EXPORT_PLUGIN2(nom_du_plugin, VotrePlugin)\n
  * @warning nom_du_plugin doit �tre le m�me que celui indiqu� dans le TARGET du .pro!
  *
  * Enfin, il faut maintenant redefinir toutes les fonctions virtual pure des interfaces impl�ment�es
  * dans le plugin.\n\n
  * La compilation de ce projet donnera une library dynamique binaire (.so ou .dll selon le syst�me)
  * ainsi qu'un fichier .a .\n
  * @section pluginInTC Pour Tiles Creator
  * Dans Tiles Creator, deux interfaces sont utilisables pour le moment : ImportInterface qui permet
  * d'ajouter une fonction d'import, et ExportInterface qui permet d'ajouter une fonction d'export.\n
  * Toutes les interfaces h�ritent de TilesCreatorPlugin qui contient essentiellement des m�thodes
  * permettant d'avoir des informations sur la classe (nom, auteurs, version, etc...).\n\n
  * ImportInterface et ExportInterface ont toutes deux une m�thode principale, respectivement import()
  * et exportProject() qui n�c�ssitent l'utilisation des classes-structures TcProject, TcTile, TcTileset,
  * TcMap. Il est donc pr�f�rable de regarder leur documentation avant de commencer un plugin.\n\n
  * Pour faire votre propre plugin pour Tiles Creator, il vous faudra t�l�charger les sources
  * n�cessaires (au minimum tilescreatorplugin.h, importinterface.h, exportinterface.h et array2d.h).
  * Il existe deux mani�res de les rendres acc�cibles depuis votre projet : ou bien encopiant les
  * sources avec les sources du plugin et en faisant \#include "...", ou bien en ajoutant un param�tre
  * dans le .pro : INCLUDEPATH += C:\Repertoire\des\sources\de\TilesCreator\n
  * Le repertoire peut �tre absolu ou relatif, et les fichiers contenus � l'int�rieur peuvent �tre
  * inclus en faisant \#include <...>.\n\n
  * Une fois votre plugin compil�, il suffira d'ajouter le binaire .so ou .dll dans le repertoire
  * plugins de Tiles Creator pour que celui-ci les prenne en compte d�s le prochain lancement!
  */

#endif // TILESCREATORPLUGIN_H
