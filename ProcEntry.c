/*
 * Author: Joshua Pfeil
 * Date: Sat Apr 29 12:37:48 AM MDT 2023
 * Description: implements functions in ProcEntry.h
*/
#include "ProcEntry.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


void PrintProcEntry(ProcEntry *entry) {
     unsigned long int utime = entry->utime / sysconf(_SC_CLK_TCK);
     unsigned long int stime = entry->stime / sysconf(_SC_CLK_TCK);
     fprintf(stdout, "%7d %7d %5c %5lu %5lu %7ld %-25s %-20s\n",
             entry->process_id,
             entry->parent_process_id,
             entry->state,
             utime,
             stime,
             entry->number_of_threads,
             entry->comm,
             entry->path);
}

ProcEntry * CreateProcEntry(void){
    ProcEntry* proc = (ProcEntry*)malloc(sizeof(ProcEntry));
    proc->process_id = 0;
    proc->parent_process_id = 0;
    proc->comm = NULL;
    proc->state = 0;
    proc->utime = 0;
    proc->stime = 0;
    proc->number_of_threads = 0;
    proc->path = NULL;
    return proc;
}


ProcEntry * CreateProcEntryFromFile(const char statFile[]){
    int pid;
    int ppid;
    char* comm;
    char* path;
    char state;
    unsigned long int utime;
    unsigned long int stime;
    long numThreads;
    char details [36][50];
    FILE* file = fopen(statFile, "r");
    for(int i = 0; i < 36; i++){
        if(fscanf(file, "%s ", details[i]) < 1){
            
            return NULL;
        }
        
    }
    pid = atoi(details[0]);
    char tempComm[30] = "";
    int i = 1;
    while(details[i][0] == '('){
        // strncpy(tempComm, details[i] + 1, strlen(details[i]) - 2);
        strcat(tempComm, " ");
        strcat(tempComm, details[i]);
        i++;
    }

    comm = (char*)malloc(sizeof(char)*(strlen(tempComm) + 1));
    strcpy(comm, tempComm);
    path = (char*)malloc(sizeof(char)*(strlen(statFile) + 1));
    strcpy(path, statFile);
    state = *details[0 + i];
    ppid = atoi(details[1 + i]);
    utime = strtoul(details[11 + i], NULL, 0);
    stime = strtoul(details[12 + i], NULL, 0);
    numThreads = strtol(details[17 + i], NULL, 0);
    
    ProcEntry* proc = (ProcEntry*)malloc(sizeof(ProcEntry));
    proc->process_id = pid;
    proc->parent_process_id = ppid;
    proc->comm = comm;
    proc->state = state;
    proc->utime = utime;
    proc->stime = stime;
    proc->number_of_threads = numThreads;
    proc->path = path ;
    fclose(file);
    return proc;
}


void DestroyProcEntry(ProcEntry * entry){
    if(entry != NULL){
        if(entry->path != NULL){
            free(entry->path);
        }
        if(entry->comm != NULL){
            free(entry->comm);
        }
        free(entry);
    }
    
}