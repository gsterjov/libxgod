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
#include <stdint.h>
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
read_id(FILE *stream, size_t offset)
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
read_string(FILE *stream, size_t offset, size_t len)
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
	return malloc (sizeof(XGODFile));
}


void
xgod_file_free (XGODFile *file)
{
	free (file->media_id);
	free (file->title_id);
	free (file->title);
	free (file->description);
	free (file);
}


/**
	magic bytes - LIVE
	
	0x354 - media id 			(uint 32 BE)
	0x360 - title id			(uint 32 BE)
	0x412 - game title			(UTF-16 max 2304 bytes)
	0xD12 - game description	(UTF-16 max 2432 bytes)
	
	0x364 - platform			(byte)
	0x365 - executable type		(byte)
	0x366 - disc number			(byte)
	0x367 - disc count			(byte)
 */
int
xgod_file_parse (XGODFile *file, FILE *stream)
{
	file->media_id = read_id (stream, 0x354);
	file->title_id = read_id (stream, 0x360);
	file->title = read_string (stream, 0x412, 2304);
	file->description = read_string (stream, 0xd12, 2432);
	
	if (fseek (stream, 0x364, SEEK_SET) == 0)
	{
		file->platform = fgetc (stream);
		file->exec_type = fgetc (stream);
		file->disc_number = fgetc (stream);
		file->disc_count = fgetc (stream);
	}
	
	return 0;
}


const char *
xgod_file_get_title_id (XGODFile *file)
{
	return file->title_id;
}

const char *
xgod_file_get_media_id (XGODFile *file)
{
	return file->media_id;
}

const char *
xgod_file_get_title (XGODFile *file)
{
	return file->title;
}

const char *
xgod_file_get_description (XGODFile *file)
{
	return file->description;
}
