#include <stdio.h>
#include "aoc_patcher.h"
#include "kernel/kernel_functions.h"

int titleNum = 0;

enum
{
	NO_DLC,
	HYRULE_WARRIORS,
	MARIO_KART_8,
	MINECRAFT,
	PIKMIN_3,
	SONIC_LOST_WORLD,
	SUPER_SMASH_BROS,
	TOKYO_MIRAGE_SESSIONS,
	WII_SPORTS_CLUB
};

u32 getGame(void)
{
	u32 ret = NO_DLC;
	unsigned int uniqueID = (cosAppXmlInfoStruct.title_id >> 8) & 0xffff;
	switch(uniqueID)
	{
		// Hyrule Warriors
		case 0x017d8 :	// USA
		case 0x017D9 :	// EUR
		case 0x017CD :	// JAP
			ret = HYRULE_WARRIORS;
			break;

		// Mario Kart 8
		case 0x010EC :	// USA
		case 0x010ED :	// EUR
		case 0x010EB :	// JAP
			ret = MARIO_KART_8;
			break;

		// Minecraft: Wii U Edition
		case 0x01D9D :	// USA
		case 0x01D75 :	// EUR
		case 0x01DBE :	// JAP
			ret = MINECRAFT;
			break;

		//Pikmin 3
		case 0x012BD :	// USA
		case 0x012BE :	// EUR
		case 0x012BC :	// JAP
			ret = PIKMIN_3;
			break;

		// Sonic Lost World
		case 0x0128F :	// USA
		case 0x012B1 :	// EUR
		case 0x01357 :	// JAP
			ret = SONIC_LOST_WORLD;
			break;

		//Super Smash Bros. for Wii U 
		case 0x0144F :	// USA
		case 0x01450 :	// EUR
		case 0x0110E :	// JAP
			ret = SUPER_SMASH_BROS;
			break;

		//Tokyo Mirage Sessions ♯FE
		case 0x01ED8 :	// USA
		case 0x01ED7 :	// EUR
		//case 0x????? :	// JAP
			ret = TOKYO_MIRAGE_SESSIONS;
			break;

		// Wii Sports Club
		case 0x0144D :	// USA
		case 0x0144E :	// EUR
		case 0x012F1 :	// JAP
			ret = WII_SPORTS_CLUB;
			break;
	}
	return ret;
}

DECL(int, AOC_ListTitle, u32 * num_titles, AOC_TitleListType * titles, u32 max_titles, void * buffer, u32 buffer_size)
{
	int result = real_AOC_ListTitle(num_titles, titles, max_titles, buffer, buffer_size);

	u32 game = getGame();
	result = 0;

	// num_titles is the number of aoc folders.  
	// Each folder inside a title folder is the content. AOC_GetPurchaseInfo will pass this as numberOfContent
	switch(game)
	{
		case HYRULE_WARRIORS :
			// content 1 each
			*num_titles = 46;
			break;

		case MINECRAFT :
			// content 27
		case SUPER_SMASH_BROS :
			// content 51
		case WII_SPORTS_CLUB :
			// content 5
		//case MARIO_KART_8 :
			// content 10
		//case PIKMIN_3 :
			// content 17
		//case SONIC_LOST_WORLD :
			// content 8
			*num_titles = 1;
			break;

		//case TOKYO_MIRAGE_SESSIONS :
		//	*num_titles = 8;
		//	break;
			
		default :
			*num_titles = 0;
		result = -16;	// AOC_STATUS_NOT_FOUND_TITLE
	}

	// AOC_TitleListType->path is depreciated and I don't believe the rest of the data is used by much either
		
    return result;
}

DECL(int, AOC_OpenTitle, char * path, AOC_TitleListType * target, void * buffer, unsigned int buffer_size)
{
    int result  = real_AOC_OpenTitle(path, target, buffer, buffer_size);	

	result = 0;
	u32 game = getGame();
	switch(game)
	{
		case HYRULE_WARRIORS :
			sprintf(path, "/vol/save/8000000%1x/aoc/aoc0005000c%06x%02x", USER_ID, (u32)((cosAppXmlInfoStruct.title_id >> 8) & 0x00ffffff), 16 + titleNum++);
			break;

		case SUPER_SMASH_BROS :
#if SSB_ON_SD
				sprintf(path, "/vol/aoc0005000c%08x", (u32)(cosAppXmlInfoStruct.title_id & 0xffffffff));
				break;
#endif	//else use common save

		case WII_SPORTS_CLUB :
			sprintf(path, "/vol/save/common/aoc/aoc0005000c%08x", (u32)(cosAppXmlInfoStruct.title_id & 0xffffffff));
			break;

		case MINECRAFT :
			sprintf(path, "/vol/save/8000000%1x/aoc/aoc0005000c%08x", USER_ID, (u32)(cosAppXmlInfoStruct.title_id & 0xffffffff));
		break;
			
		default :
		result = -17;	// AOC_STATUS_NOT_FOUND_CONTENT
	}
	
    return result;
}

DECL(int, AOC_GetPurchaseInfo, u32 * bResult, u64 title_id, u16 contentIndexes[], u32 numberOfContent, void * buffer, u32 buffer_size)
{
    int result  = real_AOC_GetPurchaseInfo(bResult, title_id, contentIndexes, numberOfContent, buffer, buffer_size );
	
		if (bResult != NULL)
			for (u32 i = 0; i < numberOfContent; i++)
				*bResult++ = 1;
 		result = 0;
    
    return result;
}

/* *****************************************************************************
 * Creates function pointer array
 * ****************************************************************************/

hooks_magic_t method_hooks_aoc[] __attribute__((section(".data"))) = {
	MAKE_MAGIC(AOC_ListTitle,               LIB_NN_AOC,DYNAMIC_FUNCTION),
    MAKE_MAGIC(AOC_OpenTitle,               LIB_NN_AOC,DYNAMIC_FUNCTION),
	MAKE_MAGIC(AOC_GetPurchaseInfo,         LIB_NN_AOC,DYNAMIC_FUNCTION),
};


u32 method_hooks_size_aoc __attribute__((section(".data"))) = sizeof(method_hooks_aoc) / sizeof(hooks_magic_t);

//! buffer to store our instructions needed for our replacements
volatile unsigned int method_calls_aoc[sizeof(method_hooks_aoc) / sizeof(hooks_magic_t) * FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));
