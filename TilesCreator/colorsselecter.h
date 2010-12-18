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
  * @file colorsselecter.h
  * @brief Fichier contenant la classe ColorsSelecter.
  * @author Léo Grange
  * @version 1.0
  * @date 24/09/2009
  */
#ifndef COLORSSELECTER_H
#define COLORSSELECTER_H

#include <QtGui>
#include "globaltilescreator.h"

/**
* @brief La classe ColorsSelecter fournit un petit widget de selection de couleurs.
*
* ColorsSelecter est un selecteur de couleur de premier et d'arrière plan. Il permet
* de choisir parmis un nombre limité de couleurs (blanc, gris clair, gris foncé, noir
* ou une couleur reprensentant le "transparent").\n
* Un clic gauche sur ce widget ouvrira un menu pour choisir la couleur de premier plan
* tandis qu'un clic droit permetra de choisir la couleur d'arrière plan.\n
* Il est possible de rendre possible ou non l'utilisation de la couleur "transparente"
* grace à setUseAlpha().
* @warning Pour le moment il est déconseillé d'utiliser depuis l'extérieur les slots
*   setXxx(), car il n'est pas possible d'appliquer la couleur désirée sur un plan
*   choisis.
* @todo Mettre les slots setXxx() en protected.
* @todo Créer une énum Position avec comme valeurs énumérées Front et Back.
* @todo Créer une énum Color avec comme valeurs énumérées les différentes couleurs traitées.
* @todo Créer une méthode publique setColor(Position pos, Color color).
*/
class ColorsSelecter : public QLabel
{
    Q_OBJECT

    public:
        /**
        * @brief Constructeur par défaut.
        * @param parent QWidget parent du ColorsSelecter, ou 0 si il ne doit pas
        * posséder de parent.
        */
        ColorsSelecter(QWidget *parent = 0);

        /**
        * @brief active ou désactive l'utilisation de la couleur "transparente".
        * @param use true si on veut utiliser la couler "transparente", false sinon.
        */
        void setUseAlpha(bool use);

    protected:
        void reprintImage();
        QImage colorIcon(TC::Color c);
        void mousePressEvent(QMouseEvent *event);
        void setColor(TC::Color c);

    public slots:
        void setWhite(); /**< @brief Règle la couleur à blanc. */
        void setLightGray(); /**< @brief Règle la couleur à gris clair. */
        void setDarkGray(); /**< @brief Règle la couleur à gris foncé. */
        void setBlack(); /**< @brief Règle la couleur à noir. */
        void setTransparency(); /**< @brief Règle la couleur à "transparent". */

    signals:
        /**
        * @brief Signal émis lorsque la couleur de premiier plan change.
        * @param c nouvelle couleur de premier plan.
        * @see PixelEditor::color   @see backChanged()
        */
        void frontChanged (TC::Color c);

        /**
        * @brief Signal émis lorsque la couleur d'arrière plan change.
        * @param c nouvelle couleur d'arrière plan.
        * @see PixelEditor::color   @see frontChanged()
        */
        void backChanged (TC::Color c);

    protected:
        QImage *image;
        TC::Color front, back;
        QAction *actionWhite, *actionLightGray, *actionDarkGray, *actionBlack, *actionTransp;
        Qt::MouseButton lastButton;
        bool m_useAlpha;
};

#endif // COLORSSELECTER_H
