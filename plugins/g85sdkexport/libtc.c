#include "libtc.h"

void drawGrayTile (unsigned char *lightBuffer, unsigned char *darkBuffer, GrayTile *bitmap, short x, short y) {
        drawBitmap(lightBuffer, bitmap->light, x, y, bitmap->width, bitmap->height);
        drawBitmap(darkBuffer, bitmap->dark, x, y, bitmap->width, bitmap->height);
}


void drawAlphaGrayBitmap (unsigned char *lightBuffer, unsigned char *darkBuffer, AlphaGrayBitmap *bitmap, short x, short y) {
        drawTransBitmap(lightBuffer, bitmap->bitmap.light, bitmap->alphaMap, x, y, bitmap->bitmap.width, bitmap->bitmap.height);
        drawTransBitmap(darkBuffer, bitmap->bitmap.dark, bitmap->alphaMap, x, y, bitmap->bitmap.width, bitmap->bitmap.height);
}


void drawBitmap(unsigned char *buffer, unsigned char *bitmap, short x, short y, unsigned char w, unsigned char h) {
        if (x>-w && y>-h && x<128 && y<64) {
                char shift;
                char eshift = (x+w)%8;
                char lineLenght = (w-1)/8+1;
                short start = y*16+x/8;
                int i;

                if (x < 0) {
                        shift = 8-((-x)%8);
                        if (shift == 8) shift = 0;
                        if (x%8 != 0) start--;
                }
                else shift = x%8;

                for (i=0; i<h; i++) {
                        if (i+y >= 0 && i+y < 64) {
                                int j = 8-shift, k=1;
                                if (x >= 0) buffer[start] = (buffer[start] & ~(0xFF >> shift)) | (bitmap[i*lineLenght] >> shift);
                                while (j+8 <= w) {
                                        if (x+j >= 0 && x+j < 128) buffer[start+k] = (bitmap[i*lineLenght+k-1] << (8-shift)) | (bitmap[i*lineLenght+k] >> shift);
                                        k++;
                                        j+=8;
                                }
                                if (j<w && x+j < 128) {
                                        if ((shift + w%8) > 8) buffer[start+k] = (buffer[start+k] & (0xFF >> eshift)) | ((bitmap[i*lineLenght+k-1] << (8-shift)) & ~(0xFF >> eshift));
                                        else buffer[start+k] = (bitmap[i*lineLenght+k-1] << (8-shift)) | (buffer[start+k] & (0xFF >> eshift)) | ((bitmap[i*lineLenght+k] >> shift) & ~(0xFF >> eshift) & (0xFF >> shift));
                                }
                        }
                        start += 16;
                }
        }
}


void drawTransBitmap(unsigned char *buffer, unsigned char *bitmap, unsigned char *transMapBitmap, short x, short y, unsigned char w, unsigned char h){
        if (x>-w && y>-h && x<128 && y<64) {
                char shift;
                char eshift = (x+w)%8;
                char lineLenght = (w-1)/8+1;
                short start = y*16+x/8;
                int i;

                if (x < 0) {
                        shift = 8-((-x)%8);
                        if (shift == 8) shift = 0;
                        if (x%8 != 0) start--;
                }
                else shift = x%8;

                for (i=0; i<h; i++) {
                        if (i+y >= 0 && i+y < 64) {
                                int j = 8-shift, k=1;
                                if (x >= 0) buffer[start] = (buffer[start] & (~(0xFF >> shift) | (transMapBitmap[i*lineLenght] >> shift))) | ((bitmap[i*lineLenght] >> shift));
                                while (j+8 <= w) {
                                        if (x+j >= 0 && x+j < 128) buffer[start+k] = (buffer[start+k] & ((transMapBitmap[i*lineLenght+k-1] << (8-shift)) | (transMapBitmap[i*lineLenght+k] >> shift))) | (bitmap[i*lineLenght+k-1] << (8-shift)) | (bitmap[i*lineLenght+k] >> shift);
                                        k++;
                                        j+=8;
                                }
                                if (j<w && x+j < 128) {
                                        if ((shift + w%8) > 8) buffer[start+k] = (buffer[start+k] & ((0xFF >> eshift) | ((transMapBitmap[i*lineLenght+k-1] << (8-shift) & ~(0xFF >> eshift))))) | ((bitmap[i*lineLenght+k-1] << (8-shift)) & ~(0xFF >> eshift));
                                        else buffer[start+k] = (buffer[start+k] & ((0xFF >> eshift) | (transMapBitmap[i*lineLenght+k-1] << (8-shift)) | ((transMapBitmap[i*lineLenght+k] >> shift) & ~(0xFF >> eshift) & (0xFF >> shift)))) | (bitmap[i*lineLenght+k-1] << (8-shift)) | ((bitmap[i*lineLenght+k] >> shift) & ~(0xFF >> eshift) & (0xFF >> shift));
                                }
                        }
                        start += 16;
                }
        }
}


void freeTile(GrayTile *tile) {
	// WARNING : si le pointeur est < 0x08000000, pointe vers la ROM -> PAS DE FREE!
        if (/*tile->light != 0 &&*/ (int)tile->light >= 0x08000000) free(tile->light);
        if (/*tile->dark != 0 &&*/ (int)tile->dark >= 0x08000000) free(tile->dark);
        tile->light = 0;
        tile->dark = 0;
        tile->width = 0;
        tile->height = 0;
}


void freeTileset(Tileset *tileset) {
        int i;
        for (i=0; i < tileset->size; i++) {
                if (tileset->tiles[i] != 0) {
			freeTile(tileset->tiles[i]);
			free(tileset->tiles[i]);
		}
	}
        if (tileset->tiles != 0) free(tileset->tiles);
        if (tileset->properties != 0) free(tileset->properties);
        tileset->size = 0;
        tileset->tileWidth = 0;
        tileset->tileHeight = 0;
        tileset->tiles = 0;
        tileset->properties = 0;
}


void freeMap(Map *map) {
        if ((int)map->map >= 0x08000000) free(map->map);
        freeTileset(&(map->tileset));
        map->map = 0;
        map->width = 0;
        map->height = 0;
}


void drawMap (Map *map, int xStart, int yStart, int width, int height, int posX, int posY, unsigned char *lightBuffer, unsigned char *darkBuffer){
        int tileWidth, tileHeight;
        int x, y, tmp, tileX, tileY;
        GrayTile tmpTile;

        tileWidth = map->tileset.tileWidth;
        tileHeight = map->tileset.tileHeight;

        //On boucle pour afficher chaque tile du morceau de la map à afficher :
        for (y=0; y<height; y++) {
                for (x=0; x<width; x++) {
                        //Si le tile actuel ne dépasse pas de la map :
                        if (x+xStart < map->width && y+yStart < map->height) {
                                tileX = posX + tileWidth*x;
                                tileY = posY + tileHeight*y;
                                //Si au moins un pixel du tile est visible, on l'affiche.
                                if ((tileX+tileWidth >= 0  &&  tileX < 128) && (tileY+tileHeight >= 0  &&  tileY < 64)) {
                                        tmp = map->width*yStart + map->width*y + xStart + x;
                                        drawGrayTile (lightBuffer, darkBuffer, map->tileset.tiles[map->map[tmp]], tileX, tileY);
                                }
                        }
                }
        }
}



BOOL testTilesetProperty (Tileset *tileset, unsigned char propertyId, unsigned char tileId) {
        BOOL ret = FALSE;

        if ((propertyId < 8*tileset->propertiesSize) && (tileId < tileset->size)) {
                if (tileset->properties[(tileId*tileset->propertiesSize)+(propertyId%8)] & (0x01 << (propertyId%8))) ret = TRUE;
        }

        return ret;
}



BOOL testMapProperty (Map *map, unsigned char propertyId, unsigned short x, unsigned short y) {
        BOOL ret = FALSE;

        if (x < map->width && y < map->height) {
                ret = testTilesetProperty (&(map->tileset), propertyId, map->map[x + y*map->width]);
        }

        return ret;
}


