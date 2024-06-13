#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

bool TryConvertToInt(const char* Input_str, int* value);
bool TryConvertToDouble(const char* Input_str, double* value);
void TryConvertDataFromCSVtoXML(FILE* stream);
char** SplitString(const char* Input_str, char delimiter);
