#include <criterion/criterion.h>
#include <stdio.h>
#include "functions.h"
#include "structures.h"
#include "globals.h"

void TestLabelName(char* inputtedString, char* expectedLabel){
      char* actual = obtainLabelName(inputtedString);
      cr_assert(strcmp(expectedLabel,actual)==0,"Obtained: %s\tExpected: %s",actual,expectedLabel);
      free(actual);
}

void TestClosingBlock(char* inputtedString, char* expectedLabel){
      char* actual = obtainClosingBlock(inputtedString);
      cr_assert(strcmp(expectedLabel,actual)==0,"Obtained: %s\tExpected: %s",actual,expectedLabel);
      free(actual);
}

void TestInnerData(char* inputtedString, char* expectedLabel){
      char* actual = obtainInnerData(inputtedString);
      cr_assert(strcmp(expectedLabel,actual)==0,"Obtained: %s\tExpected: %s",actual,expectedLabel);
      free(actual);
}

void TestSaveFile(char* inputtedFileData,char*outputFileName){
     FILE *fileptr;
     strcpy(GlobalUserInputtedFileName,"testingFile.txt");
     fileptr = fopen(GlobalUserInputtedFileName,"w");
     fprintf(fileptr,"%s",inputtedFileData);
     fclose(fileptr);

     saveFileState(outputFileName);

     fileptr = fopen(outputFileName,"r");
     char data[100000];
     char newdata[256];
     while(fgets(newdata,256,fileptr) != NULL){
          strcat(data,newdata);
     }

     cr_assert(strcmp(data,inputtedFileData) == 0,"Obtained: %s\tExpected: %s",data,inputtedFileData);
}

Test(post,opaqueFileSave1){
     TestSaveFile("<ITEM>Apple Bottom Jeans Boots With The Fur<ITEM>","screaming.txt");
}

Test(post, transparentFileSave1){
     strcpy(GlobalUserInputtedFileName,"failTestingFile.txt");
     saveFileState("apple.txt");
     cr_assert(strcmp(GlobalUserInputtedFileName,"") == 0);
}

Test(post,opaqueLabelName1){
     TestLabelName("<ENDING>Hello</ENDING>","ENDING");
}

Test(post,opaqueLabelName2){
     TestLabelName("<ENDING>Hello World</ENDING>","ENDING");
}

Test(post,opaqueClosingBlock1){
     TestClosingBlock("ENDING","</ENDING>");
}

Test(post,opaqueInnerData1){
     TestInnerData("<TEST>I am a test</TEST>","I am a test");
}

Test(post,transInnerData1){
     TestInnerData("<TEST1><TESTTWO>apple</TESTTWO></TEST1>","<TESTTWO>apple</TESTTWO>");
}


Test(post,opaqueParseFile){
     struct ParsedFile *annoying = parseFile("./TestXMLfile.xml");
     cr_assert(annoying->count == 6);
     cr_assert(strcmp(annoying->head->Label,"OVERALL") == 0);
     cr_assert(strcmp(annoying->head->InnerTextParsed->Label,"OBTUSE") == 0);
     cr_assert(strcmp(annoying->head->InnerTextParsed->InnerText," Rubber goose ") == 0);
     cr_assert(strcmp(annoying->head->InnerTextParsed->next->Label,"APPLE") == 0);
     cr_assert(strcmp(annoying->head->InnerTextParsed->next->next->InnerTextParsed->Label,"FRUIT1") == 0);
}

Test(post, opaqueCreateLine){
     struct ParsedLine *testLine = createLine("Banana","fruit","wee woo wee woo");
     cr_assert(strcmp(testLine->Label,"Banana") == 0);
     cr_assert(strcmp(testLine->Tag,"fruit") == 0);
     cr_assert(strcmp(testLine->InnerText,"wee woo wee woo") == 0);
}

Test(post, opaqueParseLines1){
     struct ParsedFile *lines = (struct ParsedFile*)malloc(sizeof(struct ParsedFile));
     lines->head = NULL;
     lines->count = 0;
     parseLines("<TEST1><TEST2>apple</TEST2></TEST1>",NULL,lines,2);
     cr_assert(lines->count == 2);
     cr_assert(strcmp(lines->head->Label,"TEST1") == 0);
     cr_assert(strcmp(lines->head->InnerText,"<TEST2>apple</TEST2>") == 0);
     cr_assert(strcmp(lines->head->InnerTextParsed->Label,"TEST2") == 0);
     cr_assert(strcmp(lines->head->InnerTextParsed->InnerText,"apple") == 0);
     cr_assert(lines->head->InnerTextParsed->count == 0);

}

Test(post, transParseFile2){
     struct ParsedFile *result = parseFile("./notARealFile");
     cr_assert(result == NULL);
}

Test(post, transLabelName1){
     char *result = obtainLabelName("");
     cr_assert(strcmp(result,"") == 0);
}

Test(post, transLabelName2){
     char *result = obtainLabelName("apple");
     cr_assert(strcmp(result,"") == 0);
}

Test(post, transInnerData2){
     char* result = obtainInnerData("<APPLE>adadaddasdas</das>");
     cr_assert(strcmp(result,"") == 0);
}

Test(post, transInnerData3){
     char* result = obtainInnerData("<APPLE>adadaddasdas</APPLE>");
     cr_assert(strcmp(result,"adadaddasdas") == 0);
}

Test(post, transFilter1){
     struct ParsedFile* annoying = parseFile("./TestXMLfile.xml");
     int filtered = filterRecords(annoying,"filter \"pear\" in FRUIT1");
     cr_assert(filtered == 1);
}

Test(post, transFilter2){
     struct ParsedFile* annoying = parseFile("./TestXMLfile.xml");
     int filtered = filterRecords(annoying,"filter \"apple\" in FRUIT1");
     cr_assert(filtered == 0);
}

Test(post, transFilter3){
     struct ParsedFile* annoying = parseFile("./TestXMLfile.xml");
     int filtered = filterRecords(annoying,"filter \"pear\" in APPLE");
     cr_assert(filtered == 0);
}

Test(post, transFilter4){
     struct ParsedFile* annoying = parseFile("./TestXMLfile.xml");
     int filtered = filterRecords(annoying,"filter \"pear\" notin FRUIT1");
     cr_assert(filtered == 0);
}

Test(post, transFilter5){
     struct ParsedFile* annoying = parseFile("./TestXMLfile.xml");
     int filtered = filterRecords(annoying,"filter \"pear\" notin APPLE");
     cr_assert(filtered == 1);
}

Test(post, transFilter6){
     struct ParsedFile* annoying = parseFile("./TestXMLfile.xml");
     int filtered = filterRecords(annoying,"filter \"pear\" notin APPLE OR \"pear\" in FRUIT1");
     cr_assert(filtered == 1);
}

Test(post, transFilter7){
     struct ParsedFile* annoying = parseFile("./TestXMLfile.xml");
     int filtered = filterRecords(annoying,"filter \"pear\" in APPLE OR \"apple\" in FRUIT1");
     cr_assert(filtered == 0);
}

Test(post, transFilter8){
     struct ParsedFile* annoying = parseFile("./TestXMLfile.xml");
     int filtered = filterRecords(annoying,"filter \"pear\" notin APPLE OR \"apple\" in FRUIT1");
     cr_assert(filtered == 1);
}
