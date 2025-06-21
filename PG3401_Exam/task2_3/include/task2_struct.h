#ifndef TASK2_STRUCT_H
#define TASK2_STRUCT_H

struct TASK2_FILE_METADATA {
   char szFileName[32];
   int iFileSize;
   char byHash[4];
   int iSumOfChars;
   char aAlphaCount[26];
}

#endif
