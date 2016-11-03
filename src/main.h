#ifndef _MAIN_H_
#define _MAIN_H_

#include "common/types.h"
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/fs_defs.h"

/* Main */
#ifdef __cplusplus
extern "C" {
#endif

//! C wrapper for our C++ functions
int Menu_Main(void);
void ApplyPatches(void);
void deInit(void);

#ifdef __cplusplus
}
#endif

#endif
