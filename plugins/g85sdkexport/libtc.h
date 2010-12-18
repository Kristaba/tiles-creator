/**
  * @file libtc.h
  * @brief Fichier ne faisant PAS partie de Tiles Creator contenant un ensemble de fonctions
  *	et de d�finitions pour utiliser des export de Tiles Creator avec le SDK G85.
  * @author L�o Grange
  * @version 0.1
  * @date Cr�� le 08/10/2009
  * Derni�re modification le 30/01/2010
  * 
  * LibTc est une library de gestion de l'affichage con�ue pour manipuler des exportations de
  * Tiles Creator. Elle contient plusieurs define utiles, la d�claration de plusieurs structures,
  * ainsi que de nombreuses fonctions permettant de manipuler ces structures avec un fort niveau
  * d'abstraction.
  */

#ifndef USE_LIBTC
#define USE_LIBTC

#include <stdlib.h>
#include <revolution.h>

/**
  * Id de la propri�t� BLOCKING.
  */
#define BLOCKING 0

#ifndef BOOL
/**
  * Simple d�finition d'un bool�en...
  */
#define BOOL unsigned char
#endif

#ifndef FALSE
/**
  * Simple d�finition de la valeur de vrai/true.
  */
#define TRUE 1
/**
  * Simple d�finition de la valeur de faux/false.
  */
#define FALSE 0
#endif

/**********************************************************************
 * LibTC est un ensemble de d�finitions et de fonctions de base pour  *
 *    la gestion des fichiers export�s de Tiles Creator pour Graph 85.*
 **********************************************************************/

 //D�finitions de quelques structures utiles :
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
  * Permet l'utilisation de GrayBitmap � la place de GrayTile.
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
  * Permet l'utilisation de AlphaGrayBitmap � la place de AlphaGrayTile.
  */
typedef AlphaGrayTile AlphaGrayBitmap;


 /**
   * @brief Tileset est une structure repr�sentant un tileset.
   */
typedef struct {
    unsigned short size; /**< Nombre de tiles dans le tileset. */
    unsigned short propertiesSize; /**< Taille des propri�t�s de chaque tile (en byte). */
    unsigned short tileWidth; /**< Largeur des tiles du tileset. */
    unsigned short tileHeight; /**< Hauteur des tiles du tileset. */
    GrayTile **tiles; /**< Liste des pointeur correspondant aux tiles du tileset. */
    unsigned char *properties; /**< Liste des propri�t�s correspondant � chaque tile. */
} Tileset;


 /**
   * @brief Map est une structure repr�sentant une map.
   */
typedef struct {
    Tileset tileset; /**< Tileset utilis� par la map. */
    unsigned short width; /**< Largeur de la map (en tiles). */
    unsigned short height; /**< Hauteur de la map (en tiles). */
    unsigned char *map; /**< Tableau repr�sentant la map. */
} Map;



//Quelques fonctions utiles :

/**
  * @brief Dessine un GrayTile sur des buffers.
  * @param lightBuffer buffer clair (de poids faible).
  * @param darkBuffer buffer fonc� (de poids fort).
  * @param bitmap tile ou bitmap � dessiner.
  * @param x abscisse du coin haut gauche du tile.
  * @param y ordonn�e du coin haut gauche du tile.
  * @see drawAlphaGrayBitmap()
  * @see drawBitmap()
  * @see drawMap()
  */
void drawGrayTile (unsigned char *lightBuffer, unsigned char *darkBuffer,
                   GrayTile *bitmap, short x, short y);


/**
  * @brief Dessine un AlphaGrayBitmap sur des buffers.
  * @param lightBuffer buffer clair (de poids faible).
  * @param darkBuffer buffer fonc� (de poids fort).
  * @param bitmap tile ou bitmap avec alpha map � dessiner.
  * @param x abscisse du coin haut gauche du tile.
  * @param y ordonn�e du coin haut gauche du tile.
  * @see drawGrayTile()
  * @see drawTransBitmap()
  */
void drawAlphaGrayBitmap (unsigned char *lightBuffer, unsigned char *darkBuffer,
                          AlphaGrayBitmap *bitmap, short x, short y);


/**
  * @brief Dessine un bitmap sur un buffer.
  * @param buffer buffer sur lequel dessiner.
  * @param bitmap bitmap comme d�cris dans la documentation du SDK G85.
  * @param x abscisse du coin haut gauche du bitmap.
  * @param y ordonn�e du coin haut gauche du bitmap.
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
  * @param bitmap bitmap � dessiner comme d�cris dans la documentation du SDK G85.
  * @param transMapBitmap bitmap contenant l'alpha map (bit � 1 = transparent, bit � 0 = opaque).
  * @param x abscisse du coin haut gauche du bitmap.
  * @param y ordonn�e du coin haut gauche du bitmap.
  * @param w largeur du bitmap.
  * @param h hauteur du bitmap.
  * @see drawAlphaGrayBitmap()
  * @see drawBitmap()
  */
void drawTransBitmap(unsigned char *buffer, unsigned char *bitmap,
                     unsigned char *transMapBitmap, short x, short y, unsigned char w, unsigned char h);


/**
  * @brief Lib�re la m�moire allou�e par un tile cr�� avec aTile et initialise la structure � 0.
  * @param tile pointeur vers le GrayTile � lib�rer.
  */
void freeTile(GrayTile *tile);


/**
  * @brief Lib�re la m�moire allou�e par un tileset cr�� avec aTileset et initialise la structure � 0.
  * @param tileset pointeur vers le Tileset � lib�rer.
  */
void freeTileset(Tileset *tileset);


/**
  * @brief Lib�re la m�moire allou�e par une map cr�� avec aMap et initialise la structure � 0.
  * @param map pointeur vers la Map � lib�rer.
  */
void freeMap(Map *map);


/**
  * @brief Dessine une Map sur des buffers.
  * @param map map � dessiner.
  * @param xStart abscisse du tile au coin haut gauche de la map (en tile).
  * @param yStart ordonn�e du tile au coin haut gauche de la map (en tile).
  * @param width nombre de tiles � afficher au maximum en largeur.
  * @param height nombre de tiles � afficher au maximum en hauteur.
  * @param posX abscisse du coin haut gauche o� afficher la map sur l'�cran (en pixel). Ce nombre peut �tre n�gatif.
  * @param posY ordonn�e du coin haut gauche o� afficher la map sur l'�cran (en pixel). Ce nombre peut �tre n�gatif.
  * @param lightBuffer buffer clair (de poids faible).
  * @param darkBuffer buffer fonc� (de poids fort).
  * @see drawGrayTile()
  */
void drawMap (Map *map, int xStart, int yStart, int width, int height, int posX, int posY,
              unsigned char *lightBuffer, unsigned char *darkBuffer);



/**
  * @brief Renvoie l'�tat du flag de la propri�t� d'un tile d'un tileset.
  *
  * Permet de v�rifier l'�tat (TRUE ou FALSE) d'une propri�t� \p propertyId dans le tile num�ro \p tileId du tileset \p tileset. \n
  * Si \p propertyId est sup�rieur au nombre de propri�t�s des tiles du tileset ou que \p tileId est sup�rieur au nombre de tiles
  * du tileset, renvoie 0.
  * @param tileset Tileset dans lequel se trouve le tile demand�.
  * @param propertyId num�ro de la propri�t� � v�rifier.
  * @param tileId num�ro de tile dans le tileset.
  * @return l'�tat du flag de la propri�t� � v�rifier sur le tile.
  * @see testMapProperty()
  */
BOOL testTilesetProperty (Tileset *tileset, unsigned char propertyId, unsigned char tileId);


/**
  * @brief Renvoie l'�tat du flag de la propri�t� d'un tile d'une map.
  *
  * Permet de v�rifier l'�tat (TRUE ou FALSE) d'une propri�t� \p propertyId dans le tile aux coordon�es \p x , \p y de la map \p map.
  * \n Si \p propertyId est sup�rieur au nombre de propri�t�s des tiles du tileset ou que les coordon�es d�passent de la map, renvoie 0.
  * @param map Map dans laquelle se trouve le tile demand�.
  * @param propertyId num�ro de la propri�t� � v�rifier.
  * @param x abscisse du tile dans la map.
  * @param y ordonn�e du tile dans la map.
  * @return l'�tat du flag de la propri�t� � v�rifier sur le tile.
  */
BOOL testMapProperty (Map *map, unsigned char propertyId, unsigned short x, unsigned short y);

/**
  * @brief Alloue un tile � partir de son ID.
  * @param tileId ID du tile � allouer, indiqu� par l'�num�ration Tiles dans le header de sortie de
  * Tiles Creator.
  * @return une structure GrayTile contenant le tile indiqu� si il existe, ou un tile vide sinon.
  * @see freeTile()
  * @see aTileset()
  * @see aMap()
  */
GrayTile aTile(int tileId);


/**
  * @brief Alloue un tileset � partir de son ID.
  * @param tilesetId ID du tileset � allouer, indiqu� par l'�num�ration Tilesets dans le header de
  * sortie de Tiles Creator.
  * @return une structure Tileset contenant le tileset indiqu� si il existe, ou un tileset vide sinon.
  * @see freeTileset()
  * @see aTile()
  * @see aMap()
  */
Tileset aTileset(int tilesetId);


/**
  * @brief Alloue une � partir de son ID.
  * @param mapId ID de la map � allouer, indiqu� par l'�num�ration Maps dans le header de sortie de
  * Tiles Creator.
  * @return une structure Map contenant la map indiqu�e si elle existe, ou une map vide sinon.
  * @see freeMap()
  * @see aTileset()
  * @see aTile()
  */
Map aMap(int mapId);
 
#endif
