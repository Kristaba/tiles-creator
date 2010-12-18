/**
  * @file libtc.h
  * @brief Fichier ne faisant PAS partie de Tiles Creator contenant un ensemble de fonctions
  *	et de définitions pour utiliser des export de Tiles Creator avec le SDK G85.
  * @author Léo Grange
  * @version 0.1
  * @date Créé le 08/10/2009
  * Dernière modification le 30/01/2010
  * 
  * LibTc est une library de gestion de l'affichage conçue pour manipuler des exportations de
  * Tiles Creator. Elle contient plusieurs define utiles, la déclaration de plusieurs structures,
  * ainsi que de nombreuses fonctions permettant de manipuler ces structures avec un fort niveau
  * d'abstraction.
  */

#ifndef USE_LIBTC
#define USE_LIBTC

#include <stdlib.h>
#include <revolution.h>

/**
  * Id de la propriété BLOCKING.
  */
#define BLOCKING 0

#ifndef BOOL
/**
  * Simple définition d'un booléen...
  */
#define BOOL unsigned char
#endif

#ifndef FALSE
/**
  * Simple définition de la valeur de vrai/true.
  */
#define TRUE 1
/**
  * Simple définition de la valeur de faux/false.
  */
#define FALSE 0
#endif

/**********************************************************************
 * LibTC est un ensemble de définitions et de fonctions de base pour  *
 *    la gestion des fichiers exportés de Tiles Creator pour Graph 85.*
 **********************************************************************/

 //Définitions de quelques structures utiles :
 /**
   * @brief GrayTile est une structure representant un tile en niveaux de gris.
   */
typedef struct {
    unsigned short width; /**< Largeur du tile. */
    unsigned short height; /**< Hauteur du tile. */
    unsigned char *light; /**< Tableau contenant le bitmap de poids faible du tile. */
    unsigned char *dark; /**< Tableau contenant le bitmap de poids fort du tile. */
} GrayTile;

/**
  * Permet l'utilisation de GrayBitmap à la place de GrayTile.
  */
typedef GrayTile GrayBitmap;

 /**
   * @brief AlphaGrayTile est une structure representant un tile en niveaux de gris avec une
   * map de transparence (alpha map).
   */
typedef struct {
	GrayTile bitmap; /**< Bitmap contenant la partie niveaux de gris de l'image. */
	unsigned char *alphaMap; /**< Tableau contenant le bitmap de transparence. */
} AlphaGrayTile;

/**
  * Permet l'utilisation de AlphaGrayBitmap à la place de AlphaGrayTile.
  */
typedef AlphaGrayTile AlphaGrayBitmap;


 /**
   * @brief Tileset est une structure représentant un tileset.
   */
typedef struct {
    unsigned short size; /**< Nombre de tiles dans le tileset. */
    unsigned short propertiesSize; /**< Taille des propriétés de chaque tile (en byte). */
    unsigned short tileWidth; /**< Largeur des tiles du tileset. */
    unsigned short tileHeight; /**< Hauteur des tiles du tileset. */
    GrayTile **tiles; /**< Liste des pointeur correspondant aux tiles du tileset. */
    unsigned char *properties; /**< Liste des propriétés correspondant à chaque tile. */
} Tileset;


 /**
   * @brief Map est une structure représentant une map.
   */
typedef struct {
    Tileset tileset; /**< Tileset utilisé par la map. */
    unsigned short width; /**< Largeur de la map (en tiles). */
    unsigned short height; /**< Hauteur de la map (en tiles). */
    unsigned char *map; /**< Tableau représentant la map. */
} Map;



//Quelques fonctions utiles :

/**
  * @brief Dessine un GrayTile sur des buffers.
  * @param lightBuffer buffer clair (de poids faible).
  * @param darkBuffer buffer foncé (de poids fort).
  * @param bitmap tile ou bitmap à dessiner.
  * @param x abscisse du coin haut gauche du tile.
  * @param y ordonnée du coin haut gauche du tile.
  * @see drawAlphaGrayBitmap()
  * @see drawBitmap()
  * @see drawMap()
  */
void drawGrayTile (unsigned char *lightBuffer, unsigned char *darkBuffer,
                   GrayTile *bitmap, short x, short y);


/**
  * @brief Dessine un AlphaGrayBitmap sur des buffers.
  * @param lightBuffer buffer clair (de poids faible).
  * @param darkBuffer buffer foncé (de poids fort).
  * @param bitmap tile ou bitmap avec alpha map à dessiner.
  * @param x abscisse du coin haut gauche du tile.
  * @param y ordonnée du coin haut gauche du tile.
  * @see drawGrayTile()
  * @see drawTransBitmap()
  */
void drawAlphaGrayBitmap (unsigned char *lightBuffer, unsigned char *darkBuffer,
                          AlphaGrayBitmap *bitmap, short x, short y);


/**
  * @brief Dessine un bitmap sur un buffer.
  * @param buffer buffer sur lequel dessiner.
  * @param bitmap bitmap comme décris dans la documentation du SDK G85.
  * @param x abscisse du coin haut gauche du bitmap.
  * @param y ordonnée du coin haut gauche du bitmap.
  * @param w largeur du bitmap.
  * @param h hauteur du bitmap.
  * @see drawGrayTile()
  * @see drawTransBitmap()
  */
void drawBitmap(unsigned char *buffer, unsigned char *bitmap,
                short x, short y, unsigned char w, unsigned char h);


/**
  * @brief Dessine un bitmap avec alpha map sur un buffer.
  * @param buffer buffer sur lequel dessiner.
  * @param bitmap bitmap à dessiner comme décris dans la documentation du SDK G85.
  * @param transMapBitmap bitmap contenant l'alpha map (bit à 1 = transparent, bit à 0 = opaque).
  * @param x abscisse du coin haut gauche du bitmap.
  * @param y ordonnée du coin haut gauche du bitmap.
  * @param w largeur du bitmap.
  * @param h hauteur du bitmap.
  * @see drawAlphaGrayBitmap()
  * @see drawBitmap()
  */
void drawTransBitmap(unsigned char *buffer, unsigned char *bitmap,
                     unsigned char *transMapBitmap, short x, short y, unsigned char w, unsigned char h);


/**
  * @brief Libère la mémoire allouée par un tile créé avec aTile et initialise la structure à 0.
  * @param tile pointeur vers le GrayTile à libérer.
  */
void freeTile(GrayTile *tile);


/**
  * @brief Libère la mémoire allouée par un tileset créé avec aTileset et initialise la structure à 0.
  * @param tileset pointeur vers le Tileset à libérer.
  */
void freeTileset(Tileset *tileset);


/**
  * @brief Libère la mémoire allouée par une map créé avec aMap et initialise la structure à 0.
  * @param map pointeur vers la Map à libérer.
  */
void freeMap(Map *map);


/**
  * @brief Dessine une Map sur des buffers.
  * @param map map à dessiner.
  * @param xStart abscisse du tile au coin haut gauche de la map (en tile).
  * @param yStart ordonnée du tile au coin haut gauche de la map (en tile).
  * @param width nombre de tiles à afficher au maximum en largeur.
  * @param height nombre de tiles à afficher au maximum en hauteur.
  * @param posX abscisse du coin haut gauche où afficher la map sur l'écran (en pixel). Ce nombre peut être négatif.
  * @param posY ordonnée du coin haut gauche où afficher la map sur l'écran (en pixel). Ce nombre peut être négatif.
  * @param lightBuffer buffer clair (de poids faible).
  * @param darkBuffer buffer foncé (de poids fort).
  * @see drawGrayTile()
  */
void drawMap (Map *map, int xStart, int yStart, int width, int height, int posX, int posY,
              unsigned char *lightBuffer, unsigned char *darkBuffer);



/**
  * @brief Renvoie l'état du flag de la propriété d'un tile d'un tileset.
  *
  * Permet de vérifier l'état (TRUE ou FALSE) d'une propriété \p propertyId dans le tile numéro \p tileId du tileset \p tileset. \n
  * Si \p propertyId est supérieur au nombre de propriétés des tiles du tileset ou que \p tileId est supérieur au nombre de tiles
  * du tileset, renvoie 0.
  * @param tileset Tileset dans lequel se trouve le tile demandé.
  * @param propertyId numéro de la propriété à vérifier.
  * @param tileId numéro de tile dans le tileset.
  * @return l'état du flag de la propriété à vérifier sur le tile.
  * @see testMapProperty()
  */
BOOL testTilesetProperty (Tileset *tileset, unsigned char propertyId, unsigned char tileId);


/**
  * @brief Renvoie l'état du flag de la propriété d'un tile d'une map.
  *
  * Permet de vérifier l'état (TRUE ou FALSE) d'une propriété \p propertyId dans le tile aux coordonées \p x , \p y de la map \p map.
  * \n Si \p propertyId est supérieur au nombre de propriétés des tiles du tileset ou que les coordonées dépassent de la map, renvoie 0.
  * @param map Map dans laquelle se trouve le tile demandé.
  * @param propertyId numéro de la propriété à vérifier.
  * @param x abscisse du tile dans la map.
  * @param y ordonnée du tile dans la map.
  * @return l'état du flag de la propriété à vérifier sur le tile.
  */
BOOL testMapProperty (Map *map, unsigned char propertyId, unsigned short x, unsigned short y);

/**
  * @brief Alloue un tile à partir de son ID.
  * @param tileId ID du tile à allouer, indiqué par l'énumération Tiles dans le header de sortie de
  * Tiles Creator.
  * @return une structure GrayTile contenant le tile indiqué si il existe, ou un tile vide sinon.
  * @see freeTile()
  * @see aTileset()
  * @see aMap()
  */
GrayTile aTile(int tileId);


/**
  * @brief Alloue un tileset à partir de son ID.
  * @param tilesetId ID du tileset à allouer, indiqué par l'énumération Tilesets dans le header de
  * sortie de Tiles Creator.
  * @return une structure Tileset contenant le tileset indiqué si il existe, ou un tileset vide sinon.
  * @see freeTileset()
  * @see aTile()
  * @see aMap()
  */
Tileset aTileset(int tilesetId);


/**
  * @brief Alloue une à partir de son ID.
  * @param mapId ID de la map à allouer, indiqué par l'énumération Maps dans le header de sortie de
  * Tiles Creator.
  * @return une structure Map contenant la map indiquée si elle existe, ou une map vide sinon.
  * @see freeMap()
  * @see aTileset()
  * @see aTile()
  */
Map aMap(int mapId);
 
#endif
