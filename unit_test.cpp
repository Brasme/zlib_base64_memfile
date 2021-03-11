#include <gtest/gtest.h>
#include "mem_file.h"

TEST(mem_file_test,create)
{
    MemFile memFile;

    EXPECT_EQ(memFile.FileName(),"");
    EXPECT_EQ(memFile.Encrypted(),false);
    EXPECT_EQ(memFile.Size(),0);
}
