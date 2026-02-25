#include <stdio.h>
#include <string.h>

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define BUILD "build/"
#define SOURC "src/"
#define INCLU "include/"

Cmd   cmd   = {0};
Procs procs = {0};

bool compile_src(void) {
    File_Paths files = {0};
    if(!nob_read_entire_dir(SOURC, &files))
        return false;

    const char *filename = files.items[--files.count];
    char file_path[1024] = {0};
    char out_file_path[1024] = {0};

    for(;;) {
        if(*filename == '.') 
            goto END_OF_LOOP;
        nob_cc(&cmd);
        nob_cc_flags(&cmd);

        cmd_append(&cmd, "-c", "-O2", "-ggdb");
        cmd_append(&cmd, "-Iinclude");
        
        sprintf(file_path, SOURC"%s", filename);
        nob_cc_inputs(&cmd, file_path);
        
        sprintf(out_file_path, BUILD"%s", filename);
        out_file_path[strlen(out_file_path)-1] = 'o';
        nob_cc_output(&cmd, out_file_path);

        da_append(&procs, cmd_run_async_and_reset(&cmd));

END_OF_LOOP:
        if(files.count <= 0) 
            break;
        filename = files.items[--files.count];
    }

    da_free(files);

    if(!nob_procs_wait_and_reset(&procs)) 
        return false;

    return true;
}

bool link_objects(const char *app_name) {
    File_Paths files = {0};
    if(!nob_read_entire_dir(BUILD, &files))
        return false;

    const char *filename = files.items[--files.count];
    char file_path[1024] = {0};

    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cc_output(&cmd, app_name);
    cmd_append(&cmd, "-fsanitize=address,undefined", "-ggdb");

    for(;;) {
        if(*filename == '.') 
            goto END_OF_LOOP;

        sprintf(file_path, BUILD"%s", filename);
        nob_cc_inputs(&cmd, file_path);

END_OF_LOOP:
        if(files.count <= 0) 
            break;
        filename = files.items[--files.count];
    }

    da_free(files);

    if(!cmd_run_sync_and_reset(&cmd))
        return false;

    return true;
}

int main(int argc, char *argv[]) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    
    if(!mkdir_if_not_exists(BUILD))
        return 1;

    if(!compile_src())
        return 1;

    if(!link_objects("carcara.elf"))
        return 1;
    
    return 0;
}
