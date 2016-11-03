#include <stdio.h>
#include "aoc_patcher.h"
#include "utils/logger.h"
#include "kernel/kernel_functions.h"

DECL(int, AOC_ListTitle, u32 * num_titles, void * titles, u32 max_titles, void * buffer, u32 buffer_size)
{
	int result = real_AOC_ListTitle(num_titles, titles, max_titles, buffer, buffer_size);

	*num_titles = 1;
	result = 0;
		
    return result;
}

DECL(int, AOC_OpenTitle, char * path, void * target, void * buffer, unsigned int buffer_size)
{
    int result  = real_AOC_OpenTitle(path, target, buffer, buffer_size);	
                       
		sprintf(path, "/vol/aoc0005000c%08x", (u32)(cosAppXmlInfoStruct.title_id & 0xffffffff));
		result = 0;
	
	
    return result;
}

DECL(int, AOC_GetPurchaseInfo, u32 * bResult, u64 title_id, u16 contentIndexes[], u32 numberOfContent, void * buffer, u32 buffer_size)
{
    int result  = real_AOC_GetPurchaseInfo(bResult, title_id, contentIndexes, numberOfContent, buffer, buffer_size );
	
		for (u32 i = 0; i < numberOfContent; i++)
 		{
 			if (bResult != NULL)
				*bResult++ = 1;
 		}
		result = 0;
    
    return result;
}

/* *****************************************************************************
 * Creates function pointer array
 * ****************************************************************************/

hooks_magic_t method_hooks_aoc[] __attribute__((section(".data"))) = {
	MAKE_MAGIC(AOC_ListTitle,               LIB_AOC,DYNAMIC_FUNCTION),
    MAKE_MAGIC(AOC_OpenTitle,               LIB_AOC,DYNAMIC_FUNCTION),
	MAKE_MAGIC(AOC_GetPurchaseInfo,         LIB_AOC,DYNAMIC_FUNCTION),
};


u32 method_hooks_size_aoc __attribute__((section(".data"))) = sizeof(method_hooks_aoc) / sizeof(hooks_magic_t);

//! buffer to store our instructions needed for our replacements
volatile unsigned int method_calls_aoc[sizeof(method_hooks_aoc) / sizeof(hooks_magic_t) * FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));
