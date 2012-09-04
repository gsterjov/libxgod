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


/**
	magic bytes - LIVE
	
	0x354 - media id 			(uint 32 BE)
	0x360 - title id			(uint 32 BE)
	0x412 - game title			(UTF-16 max 2304 bytes)
	0xD12 - game description	(UTF-16 max 2432 bytes)
	
	-- type flags --
	0x364 - platform			(byte)
	0x365 - executable type		(byte)
	0x366 - disc number			(byte)
	0x367 - disc count			(byte)
	
	-- thumbnails --
	0x1712 - thumbnail 1 length	(uint 32 BE)
	0x1716 - thumbnail 2 length	(uint 32 BE)
	0x171a - thumbnail 1 data		(PNG)
	0x571a - thumbnail 2 data		(PNG)
 */
enum HEADER_TYPES
{
	LIVE_HEADER_MEDIA_ID = 0x354,
	LIVE_HEADER_TITLE_ID = 0x360,
	LIVE_HEADER_TITLE = 0x412,
	LIVE_HEADER_DESCRIPTION = 0xD12,
	
	LIVE_HEADER_TYPE_FLAGS = 0x364,
	
	/* thumbnails */
	LIVE_HEADER_THUMB_LENGTHS = 0x1712,
	LIVE_HEADER_THUMB1 = 0x171a,
	LIVE_HEADER_THUMB2 = 0x571a,
};


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


char *
read_id (FILE *stream, size_t offset)
{
	if (fseek (stream, offset, SEEK_SET) == 0)
	{
		uint32_t id = 0;
		int read = fread (&id, 4, 1, stream);
		endian_swap (&id);
		
		return hex_str (id);
	}
	
	return NULL;
}


char *
read_string (FILE *stream, size_t offset, size_t len)
{
	if (fseek (stream, offset, SEEK_SET) == 0)
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
	
	return NULL;
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
	free (file->thumb2.data);
	
	free (file);
}


int
xgod_file_parse (XGODFile *file, FILE *stream)
{
	file->media_id = read_id (stream, LIVE_HEADER_MEDIA_ID);
	file->title_id = read_id (stream, LIVE_HEADER_TITLE_ID);
	file->title = read_string (stream, LIVE_HEADER_TITLE, 2304);
	file->description = read_string (stream, LIVE_HEADER_DESCRIPTION, 2432);
	
	if (fseek (stream, LIVE_HEADER_TYPE_FLAGS, SEEK_SET) == 0)
	{
		file->platform = fgetc (stream);
		file->exec_type = fgetc (stream);
		file->disc_number = fgetc (stream);
		file->disc_count = fgetc (stream);
	}
	
	if (fseek (stream, LIVE_HEADER_THUMB_LENGTHS, SEEK_SET) == 0)
	{
		uint32_t len1 = 0;
		uint32_t len2 = 0;
		
		int read = fread (&len1, 4, 1, stream);
		read = fread (&len2, 4, 1, stream);
		endian_swap (&len1);
		endian_swap (&len2);
		
		file->thumb1.length = len1;
		file->thumb2.length = len2;
		
		file->thumb1.data = malloc (len1);
		file->thumb2.data = malloc (len2);
		
		read = fread (file->thumb1.data, 1, len1, stream);
		
		if (fseek (stream, LIVE_HEADER_THUMB2, SEEK_SET) == 0)
			read = fread (file->thumb2.data, 1, len2, stream);
	}
	
	return 0;
}


int xgod_file_write (const XGODFile *file, FILE *stream)
{
	/* create blank header */
	uint8_t *header = malloc (0xb000);
	memset (header, 0x0, 0xb000);
	
	/* magic bytes */
	memcpy (header, "LIVE", 4);
	
	/* add padding bytes */
	memset (header + 0x22c, 0xff, 8);
	
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
