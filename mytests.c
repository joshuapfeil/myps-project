/*
 * Author: Joshua Pfeil
 * Date: Sat Apr 29 12:35:48 AM MDT 2023
 * Description: tests ProcEntry struct fucntions
*/
#include<stdio.h>
#include <string.h>
#include "ProcEntry.h"

int main(void) {

   ProcEntry* proc = CreateProcEntry();
   PrintProcEntry(proc);
   DestroyProcEntry(proc);
   ProcEntry* proc2 = CreateProcEntryFromFile("./test_data/onyx_proc/1/stat");
   PrintProcEntry(proc2);
   DestroyProcEntry(proc2);
   ProcEntry* proc3 = CreateProcEntryFromFile("./test_data/onyx_proc/1368/stat");
   PrintProcEntry(proc3);
   DestroyProcEntry(proc3);
}