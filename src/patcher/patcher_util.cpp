#include "patcher_util.h"
#include "utils/logger.h"

/* Client functions */
int client_num_alloc(void *pClient) {
    int i;

    for (i = 0; i < MAX_CLIENT; i++)
        if (bss.pClient_fs[i] == pClient) {
            return i;
        }

    for (i = 0; i < MAX_CLIENT; i++)
        if (bss.pClient_fs[i] == 0) {
            bss.pClient_fs[i] = pClient;
            return i;
        }
    return -1;
}

int client_num(void *pClient) {
    int i;
    for (i = 0; i < MAX_CLIENT; i++)
        if (bss.pClient_fs[i] == pClient)
            return i;
    return -1;
}

void client_num_free(int client) {
    bss.pClient_fs[client] = 0;
}

int getPathType(const char *path) {
    // In case the path starts by "//" and not "/" (some games do that ... ...)
    if (path[0] == '/' && path[1] == '/')
        path = &path[1];

    // In case the path does not start with "/" (some games do that too ...)
    int len = 0;
    char new_path[20];
    if(path[0] != '/') {
        new_path[0] = '/';
        len++;
    }

    while(*path && len < (int)(sizeof(new_path) - 1)) {
        new_path[len++] = *path++;
    }
    new_path[len++] = 0;

    if (strncasecmp(new_path, "/vol/aoc", 8) == 0)
        return PATH_TYPE_AOC;

    return 0;
}

void compute_new_path(char* new_path, const char* path, int len, int pathType) {

    int n, path_offset = 0;

    // In case the path starts by "//" and not "/" (some games do that ... ...)
    if (path[0] == '/' && path[1] == '/')
        path = &path[1];

    // In case the path does not start with "/" set an offset for all the accesses
	if(path[0] != '/')
		path_offset = -1;

    // some games are doing /vol/content/aoc/./....
    if(path[13 + path_offset] == '.' && path[14 + path_offset] == '/') {
        path_offset += 2;
    }

    if(pathType == PATH_TYPE_AOC)
    {
		char * pathfoo = (char*)path + 4 + path_offset;
        if(pathfoo[0] == '/') pathfoo++; //Skip double slash

        n = strlcpy(new_path, bss.mount_base, sizeof(bss.mount_base));
        n += strlcpy(new_path + n, "/", sizeof(bss.mount_base) - n);
        n += strlcpy(new_path + n, pathfoo, sizeof(bss.mount_base) - n);
        new_path[n++] = '\0';
    }
}

int GetCurClient(void *pClient) {
    if ((int)bss_ptr != 0x0a000000) {
        int client = client_num(pClient);
        if (client >= 0) {
            return client;
        }
    }
    return -1;
}

int getNewPathLen(int pathType){

    int len_base = 0;
	if(pathType == PATH_TYPE_AOC)
    {
        len_base += strlen(bss.mount_base) + 23;
    }
    

    return len_base;
}
