#pragma once

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#define MAX_TEXT_BUFFER_LENGTH              1024      

#define GLSL(src) "#version 430\n" #src


const char *strprbrk(const char *s, const char *charset);

bool SaveFileData(const char *fileName, void *data, unsigned int bytesToWrite);
char* LoadFileText(const char *fileName);

unsigned char *LoadFileData(const char *fileName, unsigned int *bytesRead);
 bool SaveFileText(const char *fileName, char *text);

void LogError( const char *msg, ... );
void LogInfo( const char *msg, ... );
void LogWarning( const char *msg, ... );
