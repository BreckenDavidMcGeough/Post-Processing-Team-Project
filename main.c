#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "functions.h"
#include "globals.h"

int main(int argc, char *argv[]){
     //Saving the File Name into global variable in globals.h
     char temp[256];
     if(argc < 2){
          printf("Please Enter a filename: ");
          fgets(temp,256,stdin);
          int templen = strlen(temp);
          temp[templen-1] = '\0';
     }
     else{
          strcpy(temp,argv[1]);
     }

     strcpy(GlobalUserInputtedFileName,temp);

     //Saving the File State
     printf("%s\n",GlobalUserInputtedFileName);
     saveFileState("./BackupFile.txt");

      if(strcmp(GlobalUserInputtedFileName,"") == 0){
          printf("INVALID FILE ENDING SESSION\n");
          exit(0);
     }

     //Parse the File Name
     struct ParsedFile *globalLinked = parseFile("./TestXMLfile.xml");

     char userInput[50] = "";
     while(1){
               memset(userInput,'\0',50);
               printf("? ");
               fflush(stdout);
               fgets(userInput,50,stdin);
               char *token = strtok(userInput, " "); // check the first word of the given command
               if (strcmp(token,"\n")==0){
                    printf("Please Enter a function\n");
                    continue;
               } else if (strcmp(token,"help")==0) {
                    help();
                    continue;
               } else if (strcmp(token,"filter")==0) {
                    filterRecords(globalLinked,userInput);
                    continue;
               } else if (strcmp(token,"reset")==0) {

                    continue;
               } else if (strcmp(token,"save")==0) {
                    save();
                    continue;
               } else if (strcmp(token,"overwrite")==0) {

                    continue;
               } else if (strcmp(token,"count")==0) {

                    continue;
               } else if (strcmp(token,"print")==0) {

                    continue;
               } else if (strcmp(token,"quit\n")==0) {

                    exit(0);
               }
               
     }
     
}