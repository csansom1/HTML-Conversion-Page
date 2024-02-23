/*
 * Filename: a4.c
 * Author: Connor Sansom
 * Last Update: 14/11/2016
 * Purpose: The main function for A4 reads in the given file name.
 *          If there exists an HTML file with that name its contents
 *          are printed to stdout, if not it looks for a plain text
 *          file of the same name and converts it to HTML before saving
 *          it to an HTML file. If the text file is not found a 404 error is
 *          printed to stdout.
 */

#ifndef A4_C
#define A4_C

#include <string.h>
#include <mysql/mysql.h>
#include "listio.h"

#define HOSTNAME "dursley.socs.uoguelph.ca"
#define USERNAME "csansom"
#define PASSWORD "0871933"
#define DATABASE "csansom"

int main(int argc, char * * argv) {

  FILE *file1, *file2;

  MYSQL sql;

  if (argc < 2) {
    printf("Please select a file.\n");
    return 0;
  }
  
  char staticBuffer[1000];
  char *dynamicBuffer;
  int i, c, maxLen, index;
  returnStruct * rs = buildHeader();

  char *filename = argv[1];
  int nameLen = strlen(filename);
  char dotHTML[nameLen + 6];
  strcpy(dotHTML, filename);
  strcat(dotHTML, ".html\0");

  index = strrchr(filename, '/') - filename;
  setName(rs->header, (index < strlen(filename)) ? &filename[index+1] : filename);

  file1 = fopen(dotHTML, "r");
  if (file1 != NULL && argc > 2) {
    while (!feof(file1)) {
      fgets(staticBuffer, 1000, file1);
      strcat(staticBuffer, "\0");
      addString(rs->header, staticBuffer);
    }
    fclose(file1);
  } else {
    if (file1)
      fclose(file1);
    file2 = fopen(filename, "r");
    if (file2 != NULL) {
      i = 0;
      maxLen = 1000;
      dynamicBuffer = malloc(sizeof(char) * maxLen);
      while ((c = fgetc(file2)) != EOF) {
        dynamicBuffer[i++] = c;
        if (i == maxLen) {
          maxLen *= 2;
          dynamicBuffer = realloc(dynamicBuffer, sizeof(char) * maxLen);
        }
      }
      fclose(file2);
      dynamicBuffer[i] = '\0';
      addString(rs->header, dynamicBuffer);
      free(dynamicBuffer);
      processStrings(rs->header);
      if (argc > 2) {
        file1 = fopen(dotHTML, "w");
        fwrite(rs->header->next->string, sizeof(char) * rs->header->length, 1, file1);
	fclose(file1);
      } else {
        char *dbName = malloc(sizeof(char) * 100);
	char *query = malloc(sizeof(char) * rs->header->length * 2);
	mysql_init(&sql);
        mysql_options(&sql, MYSQL_READ_DEFAULT_GROUP, "a4");
	mysql_real_connect(&sql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0);
	if (strchr(rs->header->name, '.')) {
	  index = strchr(rs->header->name, '.') - rs->header->name;
	  strncpy(dbName, rs->header->name, index);
	} else
	  strcpy(dbName, rs->header->name);
	dbName[index] = '\0';
	sprintf(query, "create table if not exists files (text varchar(900000), length int, name varchar(100), addDate timestamp not null, primary key(name));");
	mysql_query(&sql, query);
	sprintf(query, "insert into files values (\'%s\', %d, \'%s\', NOW());", rs->header->next->string, rs->header->length, dbName);
        printf("%s.html\n", dbName);
	printf("%s\n", rs->header->next->string);
	mysql_query(&sql, query);
	mysql_close(&sql);
	free(query);
	free(dbName);
	mysql_library_end();

      }
    } else
      printf("Error 404: %s not found\n", filename);
  }

  freeStructure(rs->header);
  free(rs);

  return 0;

}
#endif
