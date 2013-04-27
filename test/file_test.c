
#include "seatest/seatest.h"

#include <stdlib.h>
#include <string.h>
#include <xgod/file.h>


static XGODFile *god_file = NULL;


void setup()
{
	god_file = xgod_file_new();
}


void teardown()
{
	xgod_file_free (god_file);
}


/* test file opening */
void test_open_file()
{
	FILE *stream = fopen ("testfile", "rb");

	assert_true (xgod_file_parse (god_file, stream));

	char *media_id = strdup(xgod_file_get_media_id (god_file));
	char *title_id = strdup(xgod_file_get_title_id (god_file));

	char *description = strdup(xgod_file_get_description (god_file));
	char *title = strdup(xgod_file_get_title (god_file));

	assert_string_equal ("01234567", media_id);
	assert_string_equal ("76543210", title_id);
	assert_string_equal ("libxgod test description", description);
	assert_string_equal ("libxgod test title", title);

	assert_int_equal (1, xgod_file_get_disc_number (god_file));
	assert_int_equal (1, xgod_file_get_disc_count (god_file));

	fclose (stream);
}


/* test thumbnails */
void test_thumbnails()
{
	FILE *stream = fopen ("testfile", "rb");

	assert_true (xgod_file_parse (god_file, stream));

	const XGODImage *thumb1 = xgod_file_get_thumbnail1 (god_file);
	const XGODImage *thumb2 = xgod_file_get_thumbnail2 (god_file);

	FILE *out = fopen ("thumb1.png", "wb");
	fwrite (thumb1->data, 1, thumb1->length, out);
	fclose (out);

	FILE *out2 = fopen ("thumb2.png", "wb");
	fwrite (thumb2->data, 1, thumb2->length, out2);
	fclose (out2);

	fclose (stream);
}


/* write a sample file */
void test_write_file()
{
	// FILE* stream = fopen ("sample", "wb");

	// ASSERT_EQ (0, xgod_file_write (mFile, stream));

	// fclose (stream);
}


/* check default values */
void test_defaults()
{
	// EXPECT_EQ (NULL, xgod_file_get_title_id (mFile));
	// EXPECT_EQ (NULL, xgod_file_get_media_id (mFile));
	// EXPECT_EQ (NULL, xgod_file_get_title (mFile));
	// EXPECT_EQ (NULL, xgod_file_get_description (mFile));
	
	// EXPECT_EQ (0, xgod_file_get_disc_number (mFile));
	// EXPECT_EQ (0, xgod_file_get_disc_count (mFile));
	
	// const XGODImage *image1 = xgod_file_get_thumbnail1 (mFile);
	// const XGODImage *image2 = xgod_file_get_thumbnail2 (mFile);
	
	// EXPECT_EQ (0, image1->length);
	// EXPECT_EQ (0, image2->length);
	// EXPECT_EQ (NULL, image1->data);
	// EXPECT_EQ (NULL, image2->data);
}


void test_fixture_file()
{
	test_fixture_start();

	fixture_setup (setup);
	fixture_teardown (teardown);

	run_test (test_open_file);
	run_test (test_thumbnails);
	run_test (test_write_file);
	run_test (test_defaults);

	test_fixture_end();
}