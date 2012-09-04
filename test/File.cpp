
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
}

/* test file opening */
TEST_F (FileTest, OpenFile)
{
	FILE* stream = fopen ("testfile", "r");
	
	ASSERT_EQ (0, xgod_file_parse (mFile, stream));
	
	fclose (stream);
}
