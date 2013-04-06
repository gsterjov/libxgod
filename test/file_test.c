
#include "seatest/seatest.h"

#include <xgod/file.h>


void setup()
{

}


void teardown()
{

}


/* test file opening */
void test_open_file()
{
	// FILE* stream = fopen ("testfile", "rb");

	// ASSERT_EQ (0, xgod_file_parse (mFile, stream));

	// fclose (stream);
}


/* write a sample file */
void test_write_file()
{
	FILE* stream = fopen ("sample", "wb");

	// ASSERT_EQ (0, xgod_file_write (mFile, stream));

	fclose (stream);
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

	test_open_file();
	test_write_file();
	test_defaults();

	test_fixture_end();
}