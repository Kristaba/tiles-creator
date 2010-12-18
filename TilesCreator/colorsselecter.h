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
  * @file colorsselecter.h
  * @brief Fichier contenant la classe ColorsSelecter.
  * @author L�o Grange
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
* ColorsSelecter est un selecteur de couleur de premier et d'arri�re plan. Il permet
* de choisir parmis un nombre limit� de couleurs (blanc, gris clair, gris fonc�, noir
* ou une couleur reprensentant le "transparent").\n
* Un clic gauche sur ce widget ouvrira un menu pour choisir la couleur de premier plan
* tandis qu'un clic droit permetra de choisir la couleur d'arri�re plan.\n
* Il est possible de rendre possible ou non l'utilisation de la couleur "transparente"
* grace � setUseAlpha().
* @warning Pour le moment il est d�conseill� d'utiliser depuis l'ext�rieur les slots
*   setXxx(), car il n'est pas possible d'appliquer la couleur d�sir�e sur un plan
*   choisis.
* @todo Mettre les slots setXxx() en protected.
* @todo Cr�er une �num Position avec comme valeurs �num�r�es Front et Back.
* @todo Cr�er une �num Color avec comme valeurs �num�r�es les diff�rentes couleurs trait�es.
* @todo Cr�er une m�thode publique setColor(Position pos, Color color).
*/
class ColorsSelecter : public QLabel
{
    Q_OBJECT

    public:
        /**
        * @brief Constructeur par d�faut.
        * @param parent QWidget parent du ColorsSelecter, ou 0 si il ne doit pas
        * poss�der de parent.
        */
        ColorsSelecter(QWidget *parent = 0);

        /**
        * @brief active ou d�sactive l'utilisation de la couleur "transparente".
        * @param use true si on veut utiliser la couler "transparente", false sinon.
        */
        void setUseAlpha(bool use);

    protected:
        void reprintImage();
        QImage colorIcon(TC::Color c);
        void mousePressEvent(QMouseEvent *event);
        void setColor(TC::Color c);

    public slots:
        void setWhite(); /**< @brief R�gle la couleur � blanc. */
        void setLightGray(); /**< @brief R�gle la couleur � gris clair. */
        void setDarkGray(); /**< @brief R�gle la couleur � gris fonc�. */
        void setBlack(); /**< @brief R�gle la couleur � noir. */
        void setTransparency(); /**< @brief R�gle la couleur � "transparent". */

    signals:
        /**
        * @brief Signal �mis lorsque la couleur de premiier plan change.
        * @param c nouvelle couleur de premier plan.
        * @see PixelEditor::color   @see backChanged()
        */
        void frontChanged (TC::Color c);

        /**
        * @brief Signal �mis lorsque la couleur d'arri�re plan change.
        * @param c nouvelle couleur d'arri�re plan.
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
