/*
 * Filename: listio.c
 * Author: Connor Sansom
 * Last Update: 25/10/2016
 * Purpose: listio.c is the implimentation of a library containing
 *          functions that are used to create and work with dataHeaders
 *          (linked lists of strings).
 */

#ifndef LISTIO_C
#define LISTIO_C

#include "listio.h"

/*
 * Helper function for processStrings(-). Processes a single dataString.
 * IN: dString - a dataString
 * OUT: SUCCESS or FAILURE
 * PRE: dString is not NULL
 * POST: dString has been processed in accordences with processStrings(-)
 */
int processDS(dataString *dString);

/*
 * Return TRUE if the givern character is whitespace.
 * IN: c - a character
 * OUT: TRUE if c is a space, tab, newline, or carriage return; FALSE otherwise
 * PRE: c is not NULL
 * POST: c has not been altered
 */
int isWhiteSpace(char c);

/*
 * Clears repeated <P> and <BR> tags between consecutive dataStrings.
 * IN: header - a header
 * OUT: SUCCESS or FAILURE
 * PRE: header is not NULL
 * POST: redundant <P> and <BR> tags have been removed
 */
int cleanTags(dataHeader *header);

/*
 * Return TRUE if the first string ends with "<P>" and the second begins with one.
 * IN: first - the first string
 *     second - the second string
 * OUT: TRUE if there are repeated <P> tags between the 2 strings; FALSE otherwise
 * PRE: first and second are not NULL
 * POST: first and second have not been altered
 */
int repeatedTags(char *first, char *second);

/*
 * Calls "a3.py" to add final HTML tags to processed string
 * IN: header - a header
 * OUT: SUCCESS or FAILURE
 * PRE: header is not NULL
 * POST: header has been fully processed
 */
int pyProg(dataHeader *header);

returnStruct * buildHeader() {
  returnStruct *rs = malloc(sizeof(returnStruct) + sizeof(dataHeader));
  dataHeader *header = malloc(sizeof(dataHeader) + sizeof(dataString));
  header->name = NULL;
  header->length = 0;
  header->next = NULL;
  rs->value = (header == NULL) ? FAILURE : SUCCESS;
  rs->header = header;
  return rs;
}

int setName(dataHeader *header, char *name) {
  if (name == NULL || header == NULL)
    return FAILURE;
  int length = strlen(name);
  if (header->name != NULL)
    free(header->name);
  header->name = malloc(sizeof(char) * (length + 1));
  if (header->name == NULL)
    return FAILURE;
  strcpy(header->name, name);
  strcat(header->name, "\0");
  return SUCCESS;
}

char * getName(dataHeader *header) {
  return (header == NULL || header->name == NULL || strcmp(header->name, "") == 0) ? NULL : header->name;
}

int getLength(dataHeader *header) {
  return (header == NULL) ? FAILURE : header->length;
}

int addString(dataHeader *header, char *string) {
  if (header == NULL || string == NULL)
    return FAILURE;
  dataString *temp = header->next;
  dataString *dString = malloc(sizeof(dataString) + sizeof(char) * (strlen(string) + 1));
  dString->string = malloc(sizeof(char) * (strlen(string) + 1));
  if (dString == NULL || dString->string == NULL)
    return FAILURE;
  strcpy(dString->string, string);
  strcat(dString->string, "\0");
  dString->next = NULL;
  if (temp == NULL)
    header->next = dString;
  else {
    while (temp->next != NULL)
      temp = temp->next;
    temp->next = dString;
  }
  header->length += strlen(string) + 1;
  return SUCCESS;
}

int printString(dataHeader *header) {
  if (header == NULL)
    return FAILURE;
  dataString *temp = header->next;
  while (temp != NULL) {
    printf("%s", temp->string);
    temp = temp->next;
  }
  return SUCCESS;
}

int processStrings(dataHeader *header) {
  if (header == NULL)
    return FAILURE;
  int newLength = 0;
  dataString *temp = header->next;
  while (temp != NULL) {
    if (temp->string == NULL)
      return FAILURE;
    if (strcmp(temp->string, "") != 0) {
      if (processDS(temp) == FAILURE)
        return FAILURE;
      newLength += strlen(temp->string) + 1;
    }
    temp = temp->next;
  }
  header->length = newLength;
  if (cleanTags(header) == FAILURE)
    return FAILURE;
  if (header->next != NULL && header->next->next == NULL)
    return pyProg(header);
  return SUCCESS;
}

int freeStructure(dataHeader *header) {
  if (header == NULL)
    return FAILURE;
  dataString *current = header->next;
  dataString *next;
  while (current != NULL) {
    next = current->next;
    if (current->string == NULL)
      return FAILURE;
    free(current->string);
    free(current);
    current = next;
    if (current != NULL)
      next = current->next;
  }
  free(header->name);
  free(header);
  return SUCCESS;
}

int writeStrings(char *filename, dataHeader *header) {
  int status;
  FILE *file;
  file = fopen(filename, "wb");
  if (file != NULL && header != NULL) {
    int length = strlen(header->name) + 1;
    fwrite(&length, sizeof(int), 1, file);
    fwrite(header->name, sizeof(char) * length, 1, file);
    fwrite(&header->length, sizeof(int), 1, file);
    dataString *temp = header->next;
    while (temp != NULL) {
      if (temp->string != NULL && strcmp(temp->string, "") != 0) {
        length = strlen(temp->string) + 1;
        fwrite(&length, sizeof(int), 1, file);
        fwrite(temp->string, sizeof(char) * length, 1, file);
      }
      temp = temp->next;
    }
    status = SUCCESS;
  } else
    status = FAILURE;
  fclose(file);
  return status;
}

returnStruct * readStrings(char *filename) {
  returnStruct *rs = malloc(sizeof(returnStruct) + sizeof(dataHeader));
  dataHeader *header = NULL;
  FILE *file;
  int totalLength;
  int charCounter = 0;
  file = fopen(filename, "rb");
  if (file != NULL) {
    int length;
    fread(&length, sizeof(int), 1, file);
    returnStruct * temp = buildHeader();
    header = temp->header;
    free(temp);
    char *str = malloc(sizeof(char) * (length + 1));
    fread(str, sizeof(char) * length, 1, file);
    if (str == NULL || strlen(str) != (length - 1)) {
      free(header);
      free(str);
      rs->value = FAILURE;
      rs->header = NULL;
      return rs;
    }
    str[length] = '\0';
    setName(header, str);
    fread(&totalLength, sizeof(int), 1, file);
    free(str);
    while (charCounter < totalLength) {
      fread(&length, sizeof(int), 1, file);
      str =  malloc(sizeof(char) * (length + 1));
      fread(str, sizeof(char) * length, 1, file);
      if (str == NULL || strlen(str) != (length - 1)) {
       freeStructure(header);
       free(str);
       rs->value = FAILURE;
       rs->header = NULL;
       return rs;
      }
      str[length] = '\0';
      addString(header, str);
      charCounter += length;
      free(str);
    }
    fclose(file);
    rs->value = SUCCESS;
  } else
    rs->value = FAILURE;
  rs->header = header;
  return rs;
}

int processDS(dataString *dString) {
  int pTag = FALSE;
  int length = strlen(dString->string);
  char *newString = malloc(sizeof(char) * (length + 1));
  if (newString == NULL)
    return FAILURE;
  int index = 0;
  int newLength = 0;
  for (int i = 0; i < length; i++) {
    newLength++;
    if (dString->string[i] == SPACE) {
      while (dString->string[i+1] == SPACE)
        i++;
      if (!isWhiteSpace(dString->string[i+1]))
        newString[index++] = SPACE;
    } else if (dString->string[i] == TAB) {
      while (dString->string[i+1] == TAB)
        i++;
      if (!isWhiteSpace(dString->string[i+1]))
        newString[index++] = TAB;
    } else if (dString->string[i] == NEWLINE || dString->string[i] == CARRIAGERETURN) {
      newLength += 4;
      newString = realloc(newString, sizeof(char) * (length + newLength + 4));
      if (dString->string[i+1] == NEWLINE || dString->string[i+1] == CARRIAGERETURN) {
        if (newString == NULL)
          return FAILURE;
        newString[index++] = '<';
        newString[index++] = 'P';
        newString[index++] = '>';
        while (isWhiteSpace(dString->string[i+1]))
          i++;
      } else {
        if (newString == NULL)
          return FAILURE;
        while (isWhiteSpace(dString->string[i+1])) {
          if (dString->string[i+1] == NEWLINE || dString->string[i+1] == CARRIAGERETURN)
            pTag = TRUE;
          i++;
        }
        if (pTag) {
          newString[index++] = '<';
	  newString[index++] = 'P';
	  newString[index++] = '>';
        } else {
          newString[index++] = '<';
	  newString[index++] = 'B';
          newString[index++] = 'R';
          newString[index++] = '>';
        }
        if (dString->string[i] == SPACE || dString->string[i] == TAB)
          newString[index++] = dString->string[i];
        pTag = FALSE;
      }
    } else
      newString[index++] = dString->string[i];
  }
  newString[index] = '\0';
  newLength = strlen(newString);
  dString->string = realloc(dString->string, sizeof(char) * (newLength + 1));
  if (dString->string == NULL) {
    free(newString);
    return FAILURE;
  }
  strcpy(dString->string, newString);
  memset(newString, 0, strlen(newString));
  free(newString);
  return SUCCESS;
}

int isWhiteSpace(char c) {
  return (c == SPACE || c == TAB || c == NEWLINE || c == CARRIAGERETURN) ? TRUE : FALSE;
}

int cleanTags(dataHeader *header) {
  dataString *temp = header->next;
  dataString *next;
  int length;
  while (temp->next != NULL) {
    next = temp->next;
    length = strlen(temp->string);
    while (next != NULL && strcmp(next->string, "") == 0)
      next = next->next;
    if (next != NULL) {
      if (repeatedTags(temp->string, next->string)) {
        temp->string[length-3] = '\0';
        header->length -= 3;
      }
      temp = next;
      next = temp->next;
    }
  }
  return SUCCESS;
}

int repeatedTags(char *first, char *second) {
 int fLen = strlen(first);
 return ((first[fLen - 3] == '<' && first[fLen - 2] == 'P' && first[fLen - 1] == '>')
          && (second[0] == '<' && second[1] == 'P' && second[2] == '>'));
}

int pyProg(dataHeader *header) {
  FILE *q1, *q2, *py;
  if (header == NULL)
    return FAILURE;
  dataString *temp = header->next;
  q1 = fopen("q1", "w");
  if (q1 == NULL || header->name == NULL)
    return FAILURE;
  fprintf(q1, "%s\n", header->name);
  fprintf(q1, "%d\n", header->length);
  while (temp != NULL) {
    fprintf(q1, "%s\n", temp->string);
    temp = temp->next;
  }
  fclose(q1);
  mkfifo("q2", 0777);
  py = popen("./a4.py", "r");
  q2 = fopen("q2", "r");
  if (q2 == NULL)
    return FAILURE;
  header->length = 0;
  char *sizeString = malloc(sizeof(long));
  if (sizeString == NULL)
    return FAILURE;
  int size = 0;
  temp = header->next;
  while (temp != NULL) {
    fgets(sizeString, sizeof(long), q2);
    size = atoi(sizeString) + 1;
    header->length += size;
    free(temp->string);
    temp->string = malloc(sizeof(char) * (size + 1));
    if (temp->string == NULL)
      return FAILURE;
    fgets(temp->string, size, q2);
    temp->string[size] = '\0';
    temp = temp->next;
  }
  free(sizeString);
  fclose(q2);
  remove("./q2");
  pclose(py);
  return SUCCESS;
}

#endif
