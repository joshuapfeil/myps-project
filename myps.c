/* 
 * Author: Luke Hindman, Joshua Pfeil
 * Date: Fri 13 Nov 2020 12:21:37 PM PST
 * Description: Starter code for final project (myps)
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "ProcEntry.h"

/*
    filters only process directories, which have numbers in their names
*/
static int showOnlyProcesses(const struct dirent* current){
    if(current->d_type != DT_DIR || !isdigit(current->d_name[0])){
        return 0;
    }
    return 1;
}

/*
    sorts based on process id
*/
static int pidSort(const void *a, const void *b)
{
     ProcEntry *f = *(ProcEntry **)a;
     ProcEntry *s = *(ProcEntry **)b;
     int rval = f->process_id - s->process_id;
     return rval;
}

/*
    sorts based on command using strcmp
*/
static int commSort(const void *a, const void *b)
{
     ProcEntry *f = *(ProcEntry **)a;
     ProcEntry *s = *(ProcEntry **)b;
     return strcmp(f->comm,s->comm);
}

int main (int argc, char * argv[]) {
    char opt;
    struct dirent** dir;
    int p = 1;
    int c;
    int z;
    char dirpath[4096] = "/proc";
    while ((opt = getopt(argc, argv, "d:pczh")) != -1)
    {
        switch (opt){
        case 'd':
            strncpy(dirpath,optarg,4096);
            break;
        case 'p':
            p = 1;
            break;
        case 'c':
            c = 1;
            p = 0;
            break;
        case 'z':
            z = 1;
            break;
        case 'h':
        default:
            fprintf(stderr, "Usage: %s [-d <path>] [-p] [-c] [-z] [-h]",argv[0]);
            fprintf(stderr, "        -d <path> Directory containing proc entries (default: /proc)\n");
            fprintf(stderr, "        -p        Display proc entries sorted by pid (default)\n");
            fprintf(stderr, "        -c        Display proc entries sorted by command lexicographically\n");
            fprintf(stderr, "        -z        Display ONLY proc entries in the zombie state\n");
            fprintf(stderr, "        -h        Display this help message\n");
            exit(EXIT_FAILURE);
        }
    }
    int n = scandir (dirpath, &dir, showOnlyProcesses, NULL);

    if (n < 0) {
        free(dir);
        perror("scandir: ");
        exit(1);
    }

    ProcEntry ** myprocs = (ProcEntry **) (malloc(sizeof(ProcEntry *) * n));
   
    fprintf(stdout,"%7s %7s %5s %5s %5s %7s %-25s %-20s\n","PID","PPID","STATE","UTIME","STIME","THREADS","CMD","STAT_FILE");

    for (int i = 0; i < n; i++) {
        char path [4096];
        strcpy(path,dirpath);
        strcat(path, "/");
        strcat(path, dir[i]->d_name);
        strcat(path, "/");
        strcat(path, "stat");
        myprocs[i] = CreateProcEntryFromFile(path);
        free(dir[i]);
    }
    free (dir);
    if(p && !c){
        qsort(myprocs, n, sizeof(ProcEntry*), pidSort);
    }else if(c){
        qsort(myprocs, n, sizeof(ProcEntry*), commSort);
    }
    for (int i = 0; i < n; i++) {
        if(!z){
            PrintProcEntry(myprocs[i]);
        }else if(myprocs[i]->state == 'Z'){
            PrintProcEntry(myprocs[i]);
        }
        DestroyProcEntry(myprocs[i]);
    }
    free(myprocs);
}
