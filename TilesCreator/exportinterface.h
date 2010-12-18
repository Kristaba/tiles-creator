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
  * @file exportinterface.h
  * @brief Fichier contenant l'interface ExportInterface.
  * @author L�o Grange
  * @version 1.0
  * @date 10/12/2009
  */

#ifndef EXPORTINTERFACE_H
#define EXPORTINTERFACE_H

#include "tcproject.h"
#include "tilescreatorplugin.h"


/**
  * @interface ExportInterface
  * @brief Fournit une interface d'export des projets Tiles Creator.
  *
  * La classe ExportInterface est une interface Qt permettant de construire des classes
  * d'export de projets Tiles Creator, et plus particuli�rement des plugins Qt.
  * @todo Compl�ter l'interface afin de permettre de renseigner des champs compl�mentaires
  *     sur le plugin (auteur, version, date, r�f�rence internet, aide, description...).
  */
class ExportInterface : public TilesCreatorPlugin
{
    public:
        virtual ~ExportInterface() {};

        /**
        * @brief Exporte un projet Tiles Creator.
        *
        * Exporte un projet d�cris par un TcProject.
        * @param project projet � exporter.
        */
        virtual void exportProject(TcProject project) = 0;

        PluginType pluginType() {
            return Export;
        }
};

Q_DECLARE_INTERFACE(ExportInterface, "com.planet-casio.TilesCreator.ExportInterface/1.0")

#endif // EXPORTINTERFACE_H
