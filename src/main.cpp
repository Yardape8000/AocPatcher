#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "main.h"
#include "common/common.h"

#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/gx2_functions.h"
#include "dynamic_libs/socket_functions.h"
#include "dynamic_libs/sys_functions.h"
#include "patcher/coreinit_patcher.h"
#if SSB_ON_SD
#include "patcher/fs_patcher.h"
#endif
#include "patcher/aoc_patcher.h"
#include "patcher/extra_log_patcher.h"
#include "system/exception_handler.h"
#include "system/memory.h"
#include "utils/function_patcher.h"
#include "kernel/kernel_functions.h"
//#include "fs/sd_fat_devoptab.h"
#include "utils/logger.h"

/* Entry point */
extern "C" int Menu_Main(void)
{
    //!*******************************************************************
    //!                   Initialize function pointers                   *
    //!*******************************************************************
    //! acquire every rpl we want to patch
	
    InitOSFunctionPointers();
    InitSocketFunctionPointers(); //For logging
	
	log_init(LOGGER_IP);
	
    InitSysFunctionPointers(); // For SYSLaunchMenu()
	
	//For patching
#if SSB_ON_SD
	InitFSFunctionPointers();
#endif
	InitACPFunctionPointers();
	InitAocFunctionPointers();
	
	log_init(LOGGER_IP);
	
	log_printf("Function exports loaded\n");

    //!*******************************************************************
    //!                Initialize our kernel variables                   *
    //!*******************************************************************
    log_printf("Setup kernel variables\n");
    SetupKernelCallback();
	
	log_printf("AocPatcher started (rpx = '%s')\n", cosAppXmlInfoStruct.rpx_name);
    //Reset everything when were going back to the Mii Maker
    if(strlen(cosAppXmlInfoStruct.rpx_name) > 0 && strcasecmp("ffl_app.rpx", cosAppXmlInfoStruct.rpx_name) == 0){
        log_print("\nReturing to the Homebrew Launcher!\n");
		deInit();
        return EXIT_SUCCESS;
    }
	
	//!*******************************************************************
	//!                        Patching functions                        *
	//!*******************************************************************
	log_print("\nPatching functions\n");
	ApplyPatches();
		
    if(strlen(cosAppXmlInfoStruct.rpx_name) > 0 && strcasecmp("ffl_app.rpx", cosAppXmlInfoStruct.rpx_name) != 0) //Starting the application
    {
        log_printf("\nReturning to application.\n");
		log_deinit();	
		return EXIT_RELAUNCH_ON_LOAD;
    }
	
    if(strlen(cosAppXmlInfoStruct.rpx_name) <= 0){ // First boot back to SysMenu
		log_printf("\nReturning to the Wii U menu.\n");
		log_deinit();	
        SYSLaunchMenu();
        return EXIT_RELAUNCH_ON_LOAD;
    }
	
    deInit();
    return EXIT_SUCCESS;
}

/*
    Patching all the functions!!!
*/
void ApplyPatches(){
#if SSB_ON_SD
    log_print("\nPatching FS functions\n");
    PatchInvidualMethodHooks(method_hooks_fs,                   method_hooks_size_fs,               method_calls_fs);
#endif
    log_print("\nPatching functions for AOC support\n");
    PatchInvidualMethodHooks(method_hooks_aoc,                  method_hooks_size_aoc,              method_calls_aoc);
	log_print("\nPatching extra log functions\n");
    PatchInvidualMethodHooks(method_hooks_extra_log,            method_hooks_size_extra_log,        method_calls_extra_log);
}

/*
    Restoring everything!!
*/
void deInit(){
#if SSB_ON_SD
    log_print("\nRestoring FS functions\n");
    RestoreInvidualInstructions(method_hooks_fs,                method_hooks_size_fs);
#endif
    log_print("\nRestoring functions for AOC support\n");
    RestoreInvidualInstructions(method_hooks_aoc,               method_hooks_size_aoc);
	log_print("\nRestoring extra log functions\n");
    RestoreInvidualInstructions(method_hooks_extra_log,         method_hooks_size_extra_log);
	log_deinit();	
}
