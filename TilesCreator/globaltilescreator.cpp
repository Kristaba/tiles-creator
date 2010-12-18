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


#include "globaltilescreator.h"

void TC::setColorsIndexes(QImage *image) {
    image->setColor(White, rgbWhite);
    image->setColor(Black, rgbBlack);
    image->setColor(GridColor, rgbGridColor);
    image->setColor(LightGray, rgbLightGray);
    image->setColor(DarkGray, rgbDarkGray);
    image->setColor(Transparency, rgbTransparency);
}
