#include "fxlib.h"
#include "revolution.h"
#include "stdlib.h" //Afin d'utiliser calloc/malloc/realoc/free
#include "export.h" //Le fichier exporté par Tiles Creator


int AddIn_main(int isAppli, unsigned short OptionNum)
{
	int xMap, yMap, newXMap, newYMap;
	unsigned char *light_buffer, *dark_buffer, *light_swap, *dark_swap, *tmp_swap;
	Map mapTest;

	//On initialise les buffer par calloc (plus propre que par allocation automatique!) : 
	light_buffer = calloc(1024, sizeof(unsigned char)); 
	dark_buffer = calloc(1024, sizeof(unsigned char));
	//Les deux buffers ci-dessous sont les "sawp" qui permettent de faire du double buffering
	// afin d'obtenir une meilleure fluiditée.
	light_swap = calloc(1024, sizeof(unsigned char));
	dark_swap = calloc(1024, sizeof(unsigned char));
	tmp_swap = 0;
	
    Bdisp_AllClr_DDVRAM();
	Change_Contrast (166);
	
	//On initialise le grayscale de revolution-fx :
	GrayLinkBuffers(light_buffer, dark_buffer);
	GrayInit(6987, 3269);
	
	//Allocation de la map :
	mapTest = aMap(m_votre_map);

	//Boucle principale d'execution :
    while(IsKeyUp (KEY_CTRL_EXIT)){
		if (IsKeyDown (KEY_CTRL_LEFT)) xMap-=2;
		if (IsKeyDown (KEY_CTRL_RIGHT)) xMap+=2;
		if (IsKeyDown (KEY_CTRL_UP)) yMap-=2;
		if (IsKeyDown (KEY_CTRL_DOWN)) yMap+=2;
		
		//On vérifie si on ne sort pas de la map :
		if (xMap<0) xMap=0;
		else if (xMap > (mapTest.width-128/mapTest.tileset.tileWidth)*mapTest.tileset.tileWidth) 
			xMap = (mapTest.width-128/mapTest.tileset.tileWidth)*mapTest.tileset.tileWidth;
			
		if (yMap<0) yMap=0;
		else if (yMap > (mapTest.height-64/mapTest.tileset.tileHeight)*mapTest.tileset.tileHeight) 
			yMap = (mapTest.height-64/mapTest.tileset.tileHeight)*mapTest.tileset.tileHeight;
		
		//On dessine la map sur les buffer de swap :
		drawMap (mapTest,
			xMap/mapTest.tileset.tileWidth,
			yMap/mapTest.tileset.tileHeight, 
			128/mapTest.tileset.tileWidth +1, 
			64/mapTest.tileset.tileWidth +1, 
			-(xMap % mapTest.tileset.tileWidth),
			-(yMap % mapTest.tileset.tileHeight),
			light_swap,
			dark_swap);
		
		//On inverse les buffer de swap avec les buffer principaux :
		GrayLinkBuffers(light_swap, dark_swap);
		tmp_swap = light_swap;
		light_swap = light_buffer;
		light_buffer = tmp_swap;
		tmp_swap = dark_swap;
		dark_swap = dark_buffer;
		dark_buffer = tmp_swap;
		//On efface les nouveaux buffer de swap :
		memset(light_swap, 0, 1024);
		memset(dark_swap, 0, 1024);
    }
	
	CPUSpeedNormal();
    Reset_Calc();
	return 1;
}


#pragma section _BR_Size
unsigned long BR_Size;
#pragma section


#pragma section _TOP


int InitializeSystem(int isAppli, unsigned short OptionNum)
{
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}

#pragma section
