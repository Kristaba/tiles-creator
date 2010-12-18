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
  * @file importinterface.h
  * @brief Fichier contenant l'interface ImportInterface
  * @author L�o Grange
  * @version 1.0
  * @date Cr�� le 18/12/2009\n
  *     Derni�re modification le 19/12/2009
  */

#ifndef IMPORTINTERFACE_H
#define IMPORTINTERFACE_H

#include "tcproject.h"
#include "tilescreatorplugin.h"


/**
  * @interface ImportInterface
  * @brief Fournit une interface d'import de tiles, tilesets et maps pour Tiles Creator.
  *
  * La classe ImportTilesInterface est une interface Qt permettant de construire des classes
  * d'import de tiles, de tilesets et de maps Tiles Creator, en vue de cr�er des plugins Qt.
  */
class ImportInterface : public TilesCreatorPlugin
{

public:
    virtual ~ImportInterface() {};

    /**
      * @brief Importe des �l�ments dans Tiles Creator.
      *
      * Le TcProject revoy� n'est pas pris en compte lui-m�me. Seul son contenu est ajout� au
      * projet en cours.
      * @return un projet contenant tous les �l�ments import�s.
      */
    virtual TcProject import() = 0;

    PluginType pluginType() {
        return Import;
    }
};

Q_DECLARE_INTERFACE(ImportInterface, "com.planet-casio.TilesCreator.ImportInterface/1.0")

#endif // IMPORTINTERFACE_H
