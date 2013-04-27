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

#include "file.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <iconv.h>



struct _XGODFile
{
	char *title_id;
	char *media_id;
	
	char *title;
	char *description;
	
	int platform;
	int exec_type;
	
	int disc_number;
	int disc_count;
	
	XGODImage thumb1;
	XGODImage thumb2;
};


static void
endian_swap (uint32_t *value)
{
	/* AA BB CC DD -> DD CC BB AA */
	*value = (*value >> 24) |
			((*value & 0x00ff0000) >> 8) |
			((*value & 0x0000ff00) << 8) |
			(*value << 24);
}


static char *
hex_str (uint32_t val)
{
	char *str = malloc (9);
	
	int i = 0;
	for (i=0; i<8; ++i)
	{
		uint8_t byte = (uint8_t)(val & 0xf);
		char offset = byte < 0xa ? '0' : 'A'-10;
		
		str[7-i] = (uint8_t)(byte + offset);
		val >>= 4;
	}
	
	str[8] = '\0';
	return str;
}


void
read_skip (FILE *stream, size_t size)
{
	fseek (stream, size, SEEK_CUR);
}


char *
read_id (FILE *stream)
{
	uint32_t id = 0;
	int read = fread (&id, 4, 1, stream);
	endian_swap (&id);
	
	return hex_str (id);
}


char *
read_string (FILE *stream, size_t len)
{
	char utf16[len];
	char utf8[len];
	
	char *utf16_ptr = &utf16[0];
	char *utf8_ptr = &utf8[0];
	
	size_t read = fread (utf16, len, 1, stream);
	
	size_t utf16_len = len;
	size_t utf8_len = len;
	
	iconv_t conv = iconv_open ("UTF-8", "UTF-16");
	int ret = iconv (conv, &utf16_ptr, &utf16_len, &utf8_ptr, &utf8_len);
	iconv_close (conv);
	
	return strdup (utf8);
}



XGODFile *
xgod_file_new ()
{
	XGODFile *file = malloc (sizeof(XGODFile));
	memset (file, 0, sizeof(XGODFile));
	
	return file;
}


void
xgod_file_free (XGODFile *file)
{
	free (file->media_id);
	free (file->title_id);
	free (file->title);
	free (file->description);
	
	free (file->thumb1.data);
	// free (file->thumb2.data);
	
	free (file);
}


int
xgod_file_parse (XGODFile *file, FILE *stream)
{
	file->media_id = strdup("1234");
	// read LIVE magic bytes and padding
	read_skip (stream, 4);
	read_skip (stream, 848);

	// read ids
	file->media_id = read_id (stream);
	read_skip (stream, 8);
	file->title_id = read_id (stream);

	// read flags
	file->platform = fgetc (stream);
	file->exec_type = fgetc (stream);
	file->disc_number = fgetc (stream);
	file->disc_count = fgetc (stream);

	// read info
	read_skip (stream, 2474);
	file->description = read_string (stream, 2432);
	file->title = read_string (stream, 128);


	// read thumbnails
	uint32_t len1 = 0;
	uint32_t len2 = 0;

	fread (&len1, 4, 1, stream);
	fread (&len2, 4, 1, stream);
	endian_swap (&len1);
	endian_swap (&len2);

	file->thumb1.length = len1;
	file->thumb2.length = len2;

	file->thumb1.data = malloc (len1);
	file->thumb2.data = malloc (len2);

	fread (file->thumb1.data, 1, len1, stream);

	// 	if (fseek (stream, LIVE_HEADER_THUMB2, SEEK_SET) == 0)
	// 		read = fread (file->thumb2.data, 1, len2, stream);
	// }
	
	return 1;
}


int xgod_file_write (const XGODFile *file, FILE *stream)
{
	/* create blank header */
	uint8_t *header = malloc (0xb000);
	memset (header, 0x0, 0xb000);
	
	/* magic bytes */
	memcpy (header, "LIVE", 4);
	
	/* content metadata */
	//memcpy (header + LIVE_HEADER_TITLE, file->title, strlen(file->title));
	//memcpy (header + LIVE_HEADER_DESCRIPTION, file->description, strlen(file->description));
	
	/* write the header file */
	fwrite (header, 1, 0xb000, stream);
	return 0;
}


const char *
xgod_file_get_title_id (const XGODFile *file)
{
	return file->title_id;
}

const char *
xgod_file_get_media_id (const XGODFile *file)
{
	return file->media_id;
}

const char *
xgod_file_get_title (const XGODFile *file)
{
	return file->title;
}

const char *
xgod_file_get_description (const XGODFile *file)
{
	return file->description;
}


int
xgod_file_get_disc_number (const XGODFile *file)
{
	return file->disc_number;
}

int
xgod_file_get_disc_count (const XGODFile *file)
{
	return file->disc_count;
}


const XGODImage *
xgod_file_get_thumbnail1 (const XGODFile *file)
{
	return &file->thumb1;
}

const XGODImage *
xgod_file_get_thumbnail2 (const XGODFile *file)
{
	return &file->thumb2;
}
