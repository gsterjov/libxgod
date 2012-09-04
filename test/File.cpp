
#include <gtest/gtest.h>

#include <xgod/file.h>


/* test fixture */
class FileTest : public testing::Test
{
public:
	FileTest ()
	{
		mFile = xgod_file_new ();
	}
	
	~FileTest()
	{
		xgod_file_free (mFile);
	}
	
protected:
	XGODFile* mFile;
};


/* check default values */
TEST_F (FileTest, CheckDefaults)
{
	EXPECT_EQ (NULL, xgod_file_get_title_id (mFile));
	EXPECT_EQ (NULL, xgod_file_get_media_id (mFile));
	EXPECT_EQ (NULL, xgod_file_get_title (mFile));
	EXPECT_EQ (NULL, xgod_file_get_description (mFile));
	
	EXPECT_EQ (0, xgod_file_get_disc_number (mFile));
	EXPECT_EQ (0, xgod_file_get_disc_count (mFile));
	
	const XGODImage *image1 = xgod_file_get_thumbnail1 (mFile);
	const XGODImage *image2 = xgod_file_get_thumbnail2 (mFile);
	
	EXPECT_EQ (0, image1->length);
	EXPECT_EQ (0, image2->length);
	EXPECT_EQ (NULL, image1->data);
	EXPECT_EQ (NULL, image2->data);
}


/* write a sample file */
TEST_F (FileTest, WriteFile)
{
	FILE* stream = fopen ("sample", "wb");
	
	ASSERT_EQ (0, xgod_file_write (mFile, stream));
	
	fclose (stream);
}


/* test file opening */
TEST_F (FileTest, OpenFile)
{
	FILE* stream = fopen ("testfile", "rb");
	
	ASSERT_EQ (0, xgod_file_parse (mFile, stream));
	
	fclose (stream);
}
