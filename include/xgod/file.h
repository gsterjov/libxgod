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
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * The GOD container file
 */
typedef struct _XGODFile XGODFile;


/**
 * Simple image structure representing embedded images in the GOD container
 */
typedef struct _XGODImage
{
	/**
	 * The length of the image data array
	 */
	uint32_t length;
	
	/**
	 * The raw image data
	 */
	uint8_t *data;
} XGODImage;



/**
 * Create a new GOD file structure to load data into.
 */
XGODFile *xgod_file_new ();

/**
 * Free the GOD file structure and all its child members.
 */
void xgod_file_free (XGODFile *file);


/**
 * Parse the GOD container file
 */
int xgod_file_parse (XGODFile *file, FILE *stream);

/**
 * Write the GOD container file
 */
int xgod_file_write (const XGODFile *file, FILE *stream);


/**
 * Get the title ID
 */
const char *xgod_file_get_title_id (const XGODFile *file);

/**
 * Get the media ID
 */
const char *xgod_file_get_media_id (const XGODFile *file);

/**
 * Get the game title
 */
const char *xgod_file_get_title (const XGODFile *file);

/**
 * Get the game description
 */
const char *xgod_file_get_description (const XGODFile *file);


/**
 * Get the disc number
 */
int xgod_file_get_disc_number (const XGODFile *file);

/**
 * Get the disc count
 */
int xgod_file_get_disc_count (const XGODFile *file);


/**
 * Get the disc count
 */
const XGODImage *xgod_file_get_thumbnail1 (const XGODFile *file);

/**
 * Get the disc count
 */
const XGODImage *xgod_file_get_thumbnail2 (const XGODFile *file);



#ifdef __cplusplus
}
#endif

#endif /* LIBXGOD_FILE_H_ */
