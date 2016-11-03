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
#include "patcher/fs_patcher.h"
#include "patcher/aoc_patcher.h"
#include "patcher/extra_log_patcher.h"
#include "system/exception_handler.h"
#include "system/memory.h"
#include "utils/function_patcher.h"
#include "kernel/kernel_functions.h"
#include "fs/sd_fat_devoptab.h"
#include "utils/logger.h"

/* Entry point */
extern "C" int Menu_Main(void)
{
    //!*******************************************************************
    //!                   Initialize function pointers                   *
    //!*******************************************************************
    //! aquire every rpl we want to patch
	
    InitOSFunctionPointers();
    InitSocketFunctionPointers(); //For logging
	
	log_init("192.168.1.5");
	
    InitSysFunctionPointers(); // For SYSLaunchMenu()
	
	//For patching
	InitFSFunctionPointers();
	InitACPFunctionPointers();
	InitAocFunctionPointers();
	
	log_init("192.168.1.5");
	
	log_printf("Function exports loaded\n");

    //!*******************************************************************
    //!                Initialize our kernel variables                   *
    //!*******************************************************************
    log_printf("Setup kernel variables\n");
    SetupKernelCallback();

    //!*******************************************************************
    //!                        Initialize FS                             *
    //!*******************************************************************
    log_printf("Mount SD partition\n");
    mount_sd_fat("sd");
	
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
		return EXIT_RELAUNCH_ON_LOAD;
    }
	
    if(strlen(cosAppXmlInfoStruct.rpx_name) <= 0){ // First boot back to SysMenu
		log_printf("\nReturning to the Wii U menu.\n");
        SYSLaunchMenu();
        return EXIT_RELAUNCH_ON_LOAD;
    }
	
	log_printf("Unmount SD\n");
    unmount_sd_fat("sd");
	
    deInit();
    return EXIT_SUCCESS;
}

/*
    Patching all the functions!!!
*/
void ApplyPatches(){
    log_print("\nPatching FS functions\n");
    PatchInvidualMethodHooks(method_hooks_fs,                   method_hooks_size_fs,               method_calls_fs);
    log_print("\nPatching functions for AOC support\n");
    PatchInvidualMethodHooks(method_hooks_aoc,                  method_hooks_size_aoc,              method_calls_aoc);
	log_print("\nPatching extra log functions\n");
    PatchInvidualMethodHooks(method_hooks_extra_log,            method_hooks_size_extra_log,        method_calls_extra_log);
}

/*
    Restoring everything!!
*/
void deInit(){
    log_print("\nRestoring FS functions\n");
    RestoreInvidualInstructions(method_hooks_fs,                method_hooks_size_fs);
    log_print("\nRestoring functions for AOC support\n");
    RestoreInvidualInstructions(method_hooks_aoc,               method_hooks_size_aoc);
	log_print("\nRestoring extra log functions\n");
    RestoreInvidualInstructions(method_hooks_extra_log,         method_hooks_size_extra_log);
}

