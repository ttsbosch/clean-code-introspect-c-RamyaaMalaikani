#include "CSVtoXML.h"

char** SplitString(const char* Input_str, char delimiter) {
    int count = 0;
    const char* ptr = Input_str;
    while (*ptr != '\0') {
        if (*ptr++ == delimiter) {
            count++;
        }
    }

    char** Duplicates = (char**)malloc(sizeof(char*) * (count + 2));
    int DuplicateIterator = 0; 
    ptr = Given_str;
    char* token = (char*)malloc(strlen(Given_str) + 1);
    int tokenIterator = 0;
    while (*ptr != '\0') {
        if (*ptr == delimiter) {
            token[tokenIterator] = '\0';
            Duplicates[DuplicateIterator++] = strdup(token);
            tokenIterator = 0;
        } else {
            token[tokenIterator++] = *ptr;
        }
        ptr++;
    }
    token[tokenIterator] = '\0';
    Duplicates[DuplicateIterator++] = strdup(token);
    Duplicates[DuplicateIterator] = NULL;
    free(token);
    return Duplicates;
}

int TryConvertToInt(const char* Input_str, int* value) { 
    char* endptr;
    *value = strtol(Input_str, &endptr, 10);
    if (endptr == Input_str) {
        return 0;
    }
    return 1;
}

int TryConvertToDouble(const char* Input_str, double* value) { 
    char* endptr;
    *value = strtod(Input_str, &endptr);
    if (endptr == Input_str) {
        return 0;
    }
    return 1;
}

void TryConvertDataFromCSVtoXML(FILE* stream) { 
    char line[1024];
    TradeRecord Tradeobjects[1024];
    int lineCount = 0;
    int objectCount = 0;

    while (fgets(line, sizeof(line), stream)) {
        char* fields[3];
        int fieldCount = 0;
        char* token = strtok(line, ",");
        while (token != NULL) {
            fields[fieldCount++] = token;
            token = strtok(NULL, ",");
        }

        if (fieldCount != 3) {
            fprintf(stderr, "WARN: Line %d malformed. Only %d field(s) found.\n", lineCount + 1, fieldCount);
            continue;
        }

        if (strlen(fields[0]) != 6) {
            fprintf(stderr, "WARN: Trade currencies on line %d malformed: '%s'\n", lineCount + 1, fields[0]);
            continue;
        }

        int TradeAmount;
        if (!TryConvertToInt(fields[1], &TradeAmount)) {
            fprintf(stderr, "WARN: Trade amount on line %d not a valid integer: '%s'\n", lineCount + 1, fields[1]);
        }

        double TradePrice;
        if (!TryConvertToDouble(fields[2], &TradePrice)) {
            fprintf(stderr, "WARN: Trade price on line %d not a valid decimal: '%s'\n", lineCount + 1, fields[2]);
        }

        strncpy(Tradeobjects[objectCount].SourceCurrency, fields[0], 3);
        strncpy(Tradeobjects[objectCount].DestinationCurrency, fields[0] + 3, 3);
        Tradeobjects[objectCount].Lots = TradeAmount / LotSize;
        Tradeobjects[objectCount].Price = TradePrice;
        objectCount++;
        lineCount++;
    }

    FILE* outFile = fopen("output.xml", "w");
    fprintf(outFile, "<TradeRecords>\n");
    for (int i = 0; i < objectCount; i++) {
        fprintf(outFile, "\t<TradeRecord>\n");
        fprintf(outFile, "\t\t<SourceCurrency>%s</SourceCurrency>\n", objects[i].SourceCurrency);
        fprintf(outFile, "\t\t<DestinationCurrency>%s</DestinationCurrency>\n", objects[i].DestinationCurrency);
        fprintf(outFile, "\t\t<Lots>%d</Lots>\n", objects[i].Lots);
        fprintf(outFile, "\t\t<Price>%f</Price>\n", objects[i].Price);
        fprintf(outFile, "\t</TradeRecord>\n");
    }
    fprintf(outFile, "</TradeRecords>");
    fclose(outFile);
    printf("INFO: %d trades processed\n", objectCount);
}

