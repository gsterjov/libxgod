/*
    Copyright 2012 Goran Sterjov
    
    libxgod is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libxgod is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libxgod.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBXGOD_FILE_H_
#define LIBXGOD_FILE_H_


#include <stdio.h>


typedef struct _XGODFile XGODFile;


/**
 * Create a new GOD file structure to load data into.
 */
XGODFile *xgod_file_new();

/**
 * Free the GOD file structure and all its child members.
 */
void xgod_file_free(XGODFile *file);


/**
 * Parse the metadata of a GOD file
 */
int xgod_file_parse(XGODFile *file, FILE *stream);


/**
 * Get the title ID
 */
const char *xgod_file_get_title_id (XGODFile *file);

/**
 * Get the media ID
 */
const char *xgod_file_get_media_id (XGODFile *file);

/**
 * Get the game title
 */
const char *xgod_file_get_title (XGODFile *file);

/**
 * Get the game description
 */
const char *xgod_file_get_description (XGODFile *file);


#endif /* LIBXGOD_FILE_H_ */
