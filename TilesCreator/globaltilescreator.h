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
  * @file globaltilescreator.h
  * @brief Fichier contenant des fonctions, énumérations et constantes utilisées dans Tiles Creator.
  *
  * Ces fichier contient certaines énumérations et constantes, qui peuvent être utiles
  * même en faisant abstraction des classes spécifiques de Tiles Creator, ainsi que quelques
  * fonctions directement en rapport avec ces données globales.\n
  * On y trouve nottament des constantes en rapport aux couleurs utilisées, ou des valeurs de
  * \p key à passer à des fonctions de type setData()/data() de stockage générique d'informations.\n
  * Tout le contenu de ce fichier est inclus dans l'espace de nom "TC".
  * @author Léo Grange
  * @version 1.0
  * @date Créé le 15/01/2010
  * Modifié pour la dernière fois le 16/01/2010
  */

#ifndef GLOBALTILESCREATOR_H
#define GLOBALTILESCREATOR_H

#include <QRgb>
#include <QImage>

namespace TC {
    /**
      * @brief Enumération des index de couleur utilisés dans les QImage représentant des tiles
      * dans Tiles Creator.
      */
    enum Color {White=0 /**< Index du blanc. */,
                Black /**< Index du noir. */,
                LightGray /**< Index du gris clair. */,
                DarkGray /**< Index du gris foncé. */,
                Transparency /**< Index du transparent. */,
                GridColor /**< Index de la couleur des éventuelles grilles. */
            };

    const QRgb rgbWhite = qRgb(255,255,255); /**< Couleur du blanc. */
    const QRgb rgbBlack = qRgb(0,0,0); /**< Couleur du noir. */
    const QRgb rgbGridColor = qRgb(80,80,80); /**< Couleur de grille. */
    const QRgb rgbLightGray = qRgb(180,180,180); /**< Couleur du gris clair. */
    const QRgb rgbDarkGray = qRgb(100,100,100); /**< Couleur du gris foncé. */
    const QRgb rgbTransparency = qRgb(167, 231, 227); /**< Couleur du "transparent". */

    void setColorsIndexes(QImage *image);
}

#endif // GLOBALTILESCREATOR_H
