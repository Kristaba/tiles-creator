#include "fxlib.h"
#include "revolution.h"
#include "stdlib.h" //Afin d'utiliser calloc/malloc/realoc/free
#include "export.h" //Le fichier exporté par Tiles Creator


int AddIn_main(int isAppli, unsigned short OptionNum)
{
	unsigned char *light_buffer, *dark_buffer;
	Map mapTest;
	GrayTile tileTest;

	//On initialise les buffer par calloc (plus propre que par allocation automatique!) : 
	light_buffer = calloc(1024, sizeof(unsigned char)); 
	dark_buffer = calloc(1024, sizeof(unsigned char));
	
    Bdisp_AllClr_DDVRAM();
	Change_Contrast (166);
	
	//On initialise le grayscale de revolution-fx :
	GrayLinkBuffers(light_buffer, dark_buffer);
	GrayInit(6987, 3269);
	
	//Allocation de la map :
	mapTest = aMap(m_votre_map);
	//Allocation du tile :
	tileTest = aTile(t_votre_tile);
	
	//On dessine la map :
	drawMap (mapTest, 0, 0, 8, 4, 0, 0, light_buffer, dark_buffer);
	//On dessine le tile :
	drawGrayTile (light_buffer, dark_buffer, tileTest, 27, 41); 
	
    while(IsKeyUp (KEY_CTRL_EXIT));
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
