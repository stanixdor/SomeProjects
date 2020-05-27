#pragma once
// ----------------------------------------------------------------------------
// Project Name		:	Game State Manager
// File Name		:	Utils.h
// Author			:	Thomas Komair
// Creation Date	:	January 20, 2013
// Purpose			:	Declares general purpose utility functions.
// ----------------------------------------------------------------------------
#ifndef UTILITIES__H__
#define UTILITIES__H__

#include <cstdio>	// FILE

//																			 //
//---------------------------FILE I/O----------------------------------------//
//																			 //

/*
Writes an int to the file.
*/
int FIO_WriteInt(FILE * pCurrFile, int towrite);

/*
Writes a float to the file.
*/
int FIO_WriteFloat(FILE * pCurrFile, float towrite);

/*
Writes a string to a file and appends a null character after the string.
*/
int FIO_WriteString(FILE * pCurrFile, const char * towrite);

/*
Writes a string to a file and adds a new line after the file.
*/
int FIO_WriteLine(FILE * pCurrFile, const char * towrite);

/*
Writes a null character to the file.
*/
int FIO_WriteNULL(FILE * pCurrFile);

/*
Reads an int from a file
*/
int FIO_ReadInt(FILE * pCurrFile, int * toread);

/*
Reads a float from a file
*/
int FIO_ReadFloat(FILE * pCurrFile, float * toread);

/*
Reads a string of character from a file.

This function reads a line of text until it encounters a new line character '\n' OR a NULL character '\0'
and stores the result in toread.
*/
int FIO_ReadString(FILE* pCurrFile, char * toread, int size);

/*
Read a line from a file

This function reads a line of text until it encounters a new line character '\n' and stores the result in toread.
*/
int FIO_ReadLine(FILE * pCurrFile, char * toread, int size);

#endif