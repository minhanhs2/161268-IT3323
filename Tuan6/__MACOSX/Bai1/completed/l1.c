#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define MAX_STOP_WORDS 500
#define INITIAL_TABLE_CAP 1000
#define INITIAL_LINE_STR_CAP 32

typedef struct {
    char *word;
    int count;
    char *lines;
    int line_capacity;
    int last_line_added;
} IndexEntry;

typedef struct {
    IndexEntry *entries;
    int count;
    int capacity;
} IndexTable;

char* g_stopWords[MAX_STOP_WORDS];
int g_stopWordsCount = 0;

int compareStrings(const void* a, const void* b);
int compareIndexEntries(const void* a, const void* b);

void loadStopWords(const char* filename);
int isStopWord(const char* word);
void freeStopWords();

void initIndexTable(IndexTable* table);
IndexEntry* findWord(IndexTable* table, const char* word);
IndexEntry* addWord(IndexTable* table, const char* word);
void addLineToString(IndexEntry* entry, int lineNumber);
void freeIndexTable(IndexTable* table);

void processFile(const char* filename, IndexTable* table);
void printIndexTable(const IndexTable* table, const char* outputFilename);

int main() {
    
    const char* stopWordFile = "stopw.txt";
    
    const char* inputFile = "alice30.txt";
    
    const char* outputFile = "output.txt";

    loadStopWords(stopWordFile);

    IndexTable table;
    initIndexTable(&table);

    processFile(inputFile, &table);

    qsort(table.entries, table.count, sizeof(IndexEntry), compareIndexEntries);

    printIndexTable(&table, outputFile);
    
    printf("Da xu ly xong. Kiem tra tep '%s' de xem ket qua.\n", outputFile);

    freeIndexTable(&table);
    freeStopWords();

    return 0;
}

int compareStrings(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

int compareIndexEntries(const void* a, const void* b) {
    const IndexEntry* entryA = (const IndexEntry*)a;
    const IndexEntry* entryB = (const IndexEntry*)b;
    return strcmp(entryA->word, entryB->word);
}

void loadStopWords(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Loi: Khong mo duoc tep %s\n", filename);
        return;
    }

    char buffer[MAX_WORD_LEN];
    while (g_stopWordsCount < MAX_STOP_WORDS && fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) > 0) {
            for (int i = 0; buffer[i]; i++) {
                buffer[i] = tolower(buffer[i]);
            }
            g_stopWords[g_stopWordsCount++] = strdup(buffer);
        }
    }
    fclose(file);

    qsort(g_stopWords, g_stopWordsCount, sizeof(char*), compareStrings);
}

int isStopWord(const char* word) {
    return bsearch(&word, g_stopWords, g_stopWordsCount, sizeof(char*), compareStrings) != NULL;
}

void freeStopWords() {
    for (int i = 0; i < g_stopWordsCount; i++) {
        free(g_stopWords[i]);
    }
}

void initIndexTable(IndexTable* table) {
    table->entries = (IndexEntry*)malloc(INITIAL_TABLE_CAP * sizeof(IndexEntry));
    if (!table->entries) {
        perror("Loi cap phat bo nho cho bang chi muc");
        exit(1);
    }
    table->count = 0;
    table->capacity = INITIAL_TABLE_CAP;
}

IndexEntry* findWord(IndexTable* table, const char* word) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->entries[i].word, word) == 0) {
            return &table->entries[i];
        }
    }
    return NULL;
}

IndexEntry* addWord(IndexTable* table, const char* word) {
    if (table->count == table->capacity) {
        table->capacity *= 2;
        table->entries = (IndexEntry*)realloc(table->entries, table->capacity * sizeof(IndexEntry));
        if (!table->entries) {
            perror("Loi cap phat lai bo nho cho bang chi muc");
            exit(1);
        }
    }

    IndexEntry* newEntry = &table->entries[table->count];
    newEntry->word = strdup(word);
    newEntry->count = 0;
    newEntry->line_capacity = INITIAL_LINE_STR_CAP;
    newEntry->lines = (char*)malloc(newEntry->line_capacity);
    newEntry->lines[0] = '\0';
    newEntry->last_line_added = -1;

    table->count++;
    return newEntry;
}

void addLineToString(IndexEntry* entry, int lineNumber) {
    if (entry->last_line_added == lineNumber) {
        return;
    }

    char line_buffer[16];
    sprintf(line_buffer, ",%d", lineNumber);

    int needed_capacity = strlen(entry->lines) + strlen(line_buffer) + 1;
    if (needed_capacity > entry->line_capacity) {
        while (needed_capacity > entry->line_capacity) {
            entry->line_capacity *= 2;
        }
        entry->lines = (char*)realloc(entry->lines, entry->line_capacity);
        if (!entry->lines) {
            perror("Loi cap phat lai bo nho cho chuoi dong");
            exit(1);
        }
    }

    strcat(entry->lines, line_buffer);
    entry->last_line_added = lineNumber;
}

void freeIndexTable(IndexTable* table) {
    for (int i = 0; i < table->count; i++) {
        free(table->entries[i].word);
        free(table->entries[i].lines);
    }
    free(table->entries);
}

void processFile(const char* filename, IndexTable* table) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Loi: Khong mo duoc tep van ban: %s\n", filename);
        exit(1);
    }

    char originalWord[MAX_WORD_LEN];
    char lowercaseWord[MAX_WORD_LEN];
    int wordIndex = 0;
    int lineNumber = 1;
    int afterPunctuation = 1;

    int c;
    while ((c = fgetc(file)) != EOF) {
        if (isalpha(c)) {
            if (wordIndex < MAX_WORD_LEN - 1) {
                originalWord[wordIndex] = c;
                lowercaseWord[wordIndex] = tolower(c);
                wordIndex++;
            }
        }
        else {
            if (wordIndex > 0) {
                originalWord[wordIndex] = '\0';
                lowercaseWord[wordIndex] = '\0';

                int isProper = isupper(originalWord[0]) && !afterPunctuation;
                int isStop = isStopWord(lowercaseWord);

                if (!isProper && !isStop) {
                    IndexEntry* entry = findWord(table, lowercaseWord);
                    if (entry == NULL) {
                        entry = addWord(table, lowercaseWord);
                    }
                    
                    entry->count++;
                    addLineToString(entry, lineNumber);
                }

                wordIndex = 0;
                afterPunctuation = 0;
            }

            if (c == '.' || c == '?' || c == '!') {
                afterPunctuation = 1;
            } else if (c == '\n') {
                lineNumber++;
                afterPunctuation = 1;
            } else if (isspace(c)) {
                
            } else {
                afterPunctuation = 0;
            }
        }
    }

    if (wordIndex > 0) {
        originalWord[wordIndex] = '\0';
        lowercaseWord[wordIndex] = '\0';
        int isProper = isupper(originalWord[0]) && !afterPunctuation;
        int isStop = isStopWord(lowercaseWord);
        if (!isProper && !isStop) {
            IndexEntry* entry = findWord(table, lowercaseWord);
            if (entry == NULL) {
                entry = addWord(table, lowercaseWord);
            }
            entry->count++;
            addLineToString(entry, lineNumber);
        }
    }

    fclose(file);
}

void printIndexTable(const IndexTable* table, const char* outputFilename) {
    FILE* file = fopen(outputFilename, "w");
    if (!file) {
        fprintf(stderr, "Loi: Khong the mo tep dau ra %s\n", outputFilename);
        return;
    }

    for (int i = 0; i < table->count; i++) {
        const IndexEntry* entry = &table->entries[i];
        
        fprintf(file, "%s %d%s\n", entry->word, entry->count, entry->lines);
    }

    fclose(file);
}