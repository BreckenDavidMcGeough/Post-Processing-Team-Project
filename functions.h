
struct ParsedFile *parseFile(char* filelocation);
struct ParsedLine *createLine(char* Label, char* Tag, char* InnerText);
struct ParsedLine *parseLines(char *lines, struct ParsedLine *attachTo,struct ParsedFile *mainFile,int which);
void saveFileState(char* outputFileName);
char *obtainInnerData(char* string);
char *obtainLabelName(char* string);
char *obtainClosingBlock(char* label);
void decomposeFilter(char* filter, char* label, char* innerText, int* inNotIn);
int breakLogicalOp(char* filter, char* subFilter, int start);
void recursiveFilter(struct ParsedLine* cur, char* label, char* innerText,int* found,struct ParsedLine* obtain);
int filterRecordsHelper(struct ParsedFile* file, char* filter,char* filename);
int filterRecords(struct ParsedFile* file, char* filter);
void save(char* filename);
void help();