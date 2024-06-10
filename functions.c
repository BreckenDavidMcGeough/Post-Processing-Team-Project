#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "functions.h"
#include "globals.h"
#include "structures.h"



struct ParsedFile *parseFile(char* filelocation){
     struct ParsedFile *temp = (struct ParsedFile*)malloc(sizeof(struct ParsedFile));;
     temp->head = NULL;
     temp->count = 0;

     FILE *fileptr = fopen(filelocation,"r");

     if(fileptr == NULL){
          printf("BAD FILE");
          return NULL;
     }

     char data[256];
     char totalData[10000];
     while(fgets(data,256,fileptr) != NULL){
          strcat(totalData,data);
     }

     parseLines(totalData,NULL,temp,2);
     return temp;
}

struct ParsedLine *parseLines(char *lines, struct ParsedLine *attachTo,struct ParsedFile *mainFile,int which){

     if(attachTo == NULL && which != 2){
          return NULL;
     }
     if(strcmp(lines,"") == 0){
          return NULL;
     }
     char *label = obtainLabelName(lines);
     if(strcmp(label,"") == 0){
          return NULL;
     }
     mainFile->count += 1;
     char *totalData = obtainInnerData(lines);
     if(which == 0){
          attachTo->next = createLine(label,"",totalData);
     }
     else if(which == 1){
          attachTo->InnerTextParsed = createLine(label,"",totalData);
     }
     else{
          mainFile->head = createLine(label,"",totalData);
     }
     int totalBlockSize = 2*strlen(label) + 5 + strlen(totalData);
     free(label);

     //Obtain the Inner Text 
     if(which == 2){
          parseLines(totalData,mainFile->head,mainFile,1);
          parseLines(strchr(lines,'<') + totalBlockSize,mainFile->head,mainFile,0);
     }
     else if(which == 1){
          parseLines(totalData,attachTo->InnerTextParsed,mainFile,1);
          //Obtain the *Next statements
          parseLines(strchr(lines,'<') + totalBlockSize,attachTo->InnerTextParsed,mainFile,0);
     }
     else{
          parseLines(totalData,attachTo->next,mainFile,1);
          //Obtain the *Next statements
          parseLines(strchr(lines,'<') + totalBlockSize,attachTo->next,mainFile,0);
     }
     free(totalData);
     return NULL;
}

struct ParsedLine *createLine(char* Label, char* Tag, char* InnerText){
     struct ParsedLine *temp = (struct ParsedLine *)malloc(sizeof(struct ParsedLine));
     memset(temp->Label,'\0',100);
     memset(temp->Tag,'\0',100);
     memset(temp->InnerText,'\0',10000);
     strcpy(temp->Label,Label);
     strcpy(temp->Tag,Tag);
     strcpy(temp->InnerText,InnerText);
     temp->count = 0;
     temp->next = NULL;
     temp->prev = NULL;
     temp->InnerTextParsed = NULL;

     return temp;
}



void saveFileState(char* outputFileName){
     FILE *fileptr;
     FILE *outputFilePtr;
     fileptr = fopen(GlobalUserInputtedFileName,"r");

     if(fileptr == NULL){
          strcpy(GlobalUserInputtedFileName,"");
          return;
     }
     outputFilePtr = fopen(outputFileName,"w");
     char data[256];
     while(fgets(data,256,fileptr) != NULL){
          fprintf(outputFilePtr,"%s",data);
     }
     fclose(fileptr);
     fclose(outputFilePtr);
}

//!Given string of label, convert to closing blocks
char* obtainClosingBlock(char* label){
     char *closingBlock = (char*)malloc(1000);
     closingBlock[0] = '<';
     closingBlock[1] = '/';
     strcat(closingBlock,label);
     strcat(closingBlock,">");
     return closingBlock;
};


//! Given string, obtain nearest label (does not consider tag)
char* obtainLabelName(char* string){
     char duplicate[10000];
     char *before;
     if(strcmp(string,"") == 0){
          return "";
     }
     if(strchr(string,'>') == NULL){
          return  "";
     }

     strcpy(duplicate,string);
     before = strtok(duplicate,">");
     if(before == NULL){
          return "";
     }
     before = strchr(before,'<')+ 1;
     char *returnStatement = (char *)malloc(10000);
     strcpy(returnStatement,before);
     return returnStatement;
}

//! Will obtain all data in between nearest opening and closing blocks, fails to free pointers
char* obtainInnerData(char * string){
     char* Label = obtainLabelName(string);
     char *ClosingBlock = obtainClosingBlock(Label);
     char *dupli;
     dupli = (char *)malloc(10000);
     string = strchr(string,'<');
     memcpy(dupli,string + strlen(Label) + 2,strlen(string)-strlen(Label)-2);
     char *cur = dupli;
     while(strcmp(cur,"\0") != 0){
          if(strncmp(cur,ClosingBlock,strlen(ClosingBlock)) == 0){
               memset(cur,'\0',1);
               free(Label);
               free(ClosingBlock);
               return dupli;
          }
          cur += 1;
     }
     free(Label);
     free(ClosingBlock);
     return "";
}

void decomposeFilter(char* filter, char* label, char* innerText, int* inNotIn){
     int i = 0;
     while (filter[i] != '\0' && i != strlen(filter)-1){
          if (filter[i] == '\"' || filter[i] == '"'){
               i++;
               int j = 0;
               while (filter[i] != '\"' && filter[i] != '"'){
                    innerText[j] = filter[i];
                    j++;
                    i++;
               }
          }else if (filter[i] == 'i' && filter[i+1] == 'n'){
               if (filter[i-1] == ' '){
                    *inNotIn = 0; //0 for in
               }else{
                    *inNotIn = 1; //0 for notin
               }
               i += 3;
               int j = 0;
               while (filter[i] != ' ' && filter[i] != '\0'){
                    label[j] = filter[i];
                    j++;
                    i++;
               }
          }
          i++;
     }
}

int breakLogicalOp(char* filter, char* subFilter, int start){
     int i = start;
     int j = 0;
     while (filter[i] != '\0' && filter[i] != strlen(filter)){
          if (i < strlen(filter)-4){
               if (filter[i+2] == 'O' && filter[i+3] == 'R'){
                    return i+1;
               }
          }
          subFilter[j] = filter[i];
          i++;
          j++;
     }
     return -1;
}

//recursively search for the label and text in the dataset, recursively search innerText and next for each node in ParsedLine struct
void recursiveFilter(struct ParsedLine* cur, char* label, char* innerText,int* found,struct ParsedLine* obtain){
     if (cur == NULL){
          printf("NULL\n");
     }else if (strcmp(cur->Label,label) == 0){
          if (strstr(innerText,cur->InnerText) != NULL){
               printf("found\n");
               *found = 1;
               obtain = cur;
          }
     }else{
          printf("%s\n",cur->Label);
          recursiveFilter(cur->next,label,innerText,found,obtain);
          recursiveFilter(cur->InnerTextParsed,label,innerText,found,obtain);
     }
}

//parse label and text from user inputed filter, then recursively search for the label and text in the dataset
int filterRecordsHelper(struct ParsedFile* file, char* filter,char* filename){
     char label[100];
     char innerText[10000];
     memset(label,'\0',100);
     memset(innerText,'\0',10000);
     int type;

     decomposeFilter(filter,label,innerText, &type);
     printf("%s\n",label);
     printf("%s\n",innerText);

     int found = 0;
     struct ParsedLine* cur = file->head;
     struct ParsedLine* obtain = NULL;
     recursiveFilter(cur,label,innerText,&found,obtain);

     if (type == 1){
          found = 1-found;
     }

     //FILE *fileptr;
     //fileptr = fopen(filename,"w");
     //fputs(obtain->InnerText,fileptr);
     //fclose(fileptr);

     return found;
}

int filterRecords(struct ParsedFile* file, char* filter){
     char subFilter[10000];
     memset(subFilter,'\0',10000);
     int start = 0;

     int next = breakLogicalOp(filter,subFilter,start);
     char subFilter2[10000];
     int found2 = -1;
     if (next != -1){
          memset(subFilter2,'\0',10000);
          breakLogicalOp(filter,subFilter2,next);
          found2 = filterRecordsHelper(file,subFilter2,"temp.txt");
     }
     fopen("temp.txt","w");
     int found1 = filterRecordsHelper(file,subFilter,"temp.txt");
     if (found2 == -1){
          return found1;
     }
     return found1 | found2;
}

void save(char* filename){
     rename("temp.txt",filename);
}

void help() {
     printf("Available commands to use:\n");
     printf("filter <FILTER> - Change the current dataset based on the given filter\n");
     printf("reset - Reset the current dataset to its original state\n");
     printf("save <filename> - Save current dataset to the given filename\n");
     printf("overwrite <filename> - Overwrite an existing file with the current dataset\n");
     printf("count - Print the number of records in the current dataset\n");
     printf("print - Print the records in the current dataset\n");
     printf("help - Display this help message\n");
     printf("quit - Exit the program\n");
     return;
}