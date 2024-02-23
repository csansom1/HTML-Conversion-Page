/*
 * Filename: db.c
 * Author: Connor Sansom
 * Last Update: 15/11/2016
 * Purpose: db.c manages the database for A4. Excicution flags are used to
 *          display the contents of the table, delete all table rows, and reset
 *          the database. Use the -help flag for more information.
 */

#ifndef DB_C
#define DB_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define HOSTNAME "dursley.socs.uoguelph.ca"
#define USERNAME "csansom"
#define PASSWORD "0871933"
#define DATABASE "csansom"

int main(int argc, char * * argv) {

  MYSQL sql;
  MYSQL_RES *res;
  MYSQL_ROW row;

  if (argc > 1) {
    mysql_init(&sql);
    mysql_options(&sql, MYSQL_READ_DEFAULT_GROUP, "a4");
    mysql_real_connect(&sql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0);
    if (strcmp(argv[1], "-clear") == 0) {
      if (mysql_query(&sql, "delete from files;")) {
	printf("Table not found.\n");
      } else
	printf("Table clear successful.\n");
    } else if (strcmp(argv[1], "-reset") == 0) {
      if (mysql_query(&sql, "drop table files;")) {
        printf("Table not found.\n");
      } else
	printf("Database reset successful.\n");
    } else if (strcmp(argv[1], "-show") == 0) {
      if (mysql_query(&sql, "select distinct files.name, files.length, files.addDate from files;")) {
	printf("Table not found.\n");
      } else if (!(res = mysql_store_result(&sql))) {
        printf("Could not execute -show.\n");
      } else {
        printf("csansom.files:\n");
	while ((row = mysql_fetch_row(res))) {
          for (int i = 0; i < mysql_num_fields(res); i++)
            printf("  %s ", row[i]);
	  printf("\n");
	}
      }
    } else if (strcmp(argv[1], "-help") == 0) {
      printf("db flags:\n -clear : delete all rows from db table\n -reset : delete db table\n -show : display name, length, and upload date of each file stored in the database\n -help : display all db flags\n");
    } else
      printf("Please enter a flag.\nUse the -help flag to display all db flags.\n");
    mysql_close(&sql);
    mysql_library_end();
  } else {
    printf("Please enter a flag.\nUse the -help flag to display all db flags.\n");
  }

  return 0;

}
#endif
