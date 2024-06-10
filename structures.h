
struct ParsedFile{
     struct ParsedLine *head;
     int count;
};

struct ParsedLine{
     char Label[100];
     char Tag[100];
     char InnerText[10000];
     struct ParsedLine *next;
     struct ParsedLine *prev;
     int count;
     struct ParsedLine *InnerTextParsed;
};

