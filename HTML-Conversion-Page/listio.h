/*
 * Filename: listio.h
 * Author: Connor Sansom
 * Last Update: 07/10/2016
 * Purpose: listio.h is the header file for a library containing
 *          functions that are used to create and work with dataHeaders
 *          (linked lists of strings).
 */

#ifndef LISTIO_H
#define LISTIO_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SUCCESS 1
#define FAILURE 0

#define TRUE 1
#define FALSE 0

#define SPACE ' '
#define TAB '\t'
#define NEWLINE '\n'
#define CARRIAGERETURN '\r'

/*
 * The header for a linked list of strings.
 * 
 * name - the name of the list
 * length - the total length of all strings
 * next - the first string node
 */
typedef struct dataHeader dataHeader;
typedef struct dataHeader {
  char *name;
  int length;
  struct dataString *next;
} dataHeader;

/*
 * A node in a linked list of strings.
 *
 * string - a string
 * next - the next string node
 */
typedef struct dataString dataString;
typedef struct dataString {
  char *string;
  struct dataString *next;
} dataString;

/*
 * An error checking struct that indicates success or failure when creating a dataHeader.
 *
 * value - intdicates success (1) or failure (0) of a function
 * header - the dataHeader returned by a function on success, NULL on failure
 */
typedef struct returnStruct returnStruct;
typedef struct returnStruct {
  int value;
  struct dataHeader *header;
} returnStruct;

/*
 * Allocates and returns an empty dataHeader.
 * IN: void
 * OUT: an empty dataHeader on SUCCESS
 * PRE: none
 * POST: header length is 0; name and next are NULL
 */
returnStruct * buildHeader();

/*
 * Allocates and sets the header name.
 * IN: header - the header
 *     string - the header name
 * OUT: SUCCESS or FAILURE
 * PRE: header is initialized
 * POST: header name has been changed to string
 */
int setName(dataHeader *header, char *string);

/*
 * Returns a pointer to the header name.
 * IN: header - the header
 * OUT: the header name
 * PRE: header is initialized
 * POST: header has not been altered
 */
char * getName(dataHeader *header);

/*
 * Returns the header length.
 * IN: header - the header
 * OUT: the header name
 * PRE: header is initialized
 * POST: header has not been altered
 */
int getLength(dataHeader *header);

/*
 * Allocates and adds a dataString to the end of header.
 * IN: header - the header
 *     string - the string to be added to the list
 * OUT: SUCCESS or FAILURE
 * PRE: header is initialized
 * POST: a dataString containing string is at the end of the list
 */
int addString(dataHeader *header, char *string);

/*
 * Print each element of header in sequential order.
 * IN: header - the header
 * OUT: SUCCESS or FAILURE
 * PRE: header is initialized
 * POST: header has not been altered
 */
int printString(dataHeader *header);

/*
 * Traverses the list and processes each string such that all
 * repeated spaces and tabs are reduced to one, and new line
 * and carriage return characters are replaced by "<BR>" and
 * when isolated and "<P>" when repeated.
 * IN: header - the header
 * OUT: SUCCESS or FAILURE
 * PRE: header is initialized
 * POST: each string has been processed as stated above
 */
int processStrings(dataHeader *header);

/*
 * Free memory allocated for header and its elements
 * IN: header - the header
 * OUT: SUCCESS or FAILURE
 * PRE: header is initialized
 * POST: all memory allocated for list is freed; header is NULL
 */
int freeStructure(dataHeader *header);

/*
 * Writes header and all of its strings to a binary file.
 * IN: filename - the file name
 *     header - the header
 * OUT: SUCCESS or FAILURE
 * PRE: header is initialized
 * POST: header has been written to a binary file in the current directory
 */
int writeStrings(char *filename, dataHeader *header);

/*
 * Creates a dataHeader based on information from a binary file.
 * IN: filename - the filename
 * OUT: a dataHeader containing strings from the binary file on SUCCESS
 * PRE: binary file must exist and be formatted correctly
 * POST: a new dataHeader has been allocated and built
 */
returnStruct * readStrings(char *filename);

#endif
