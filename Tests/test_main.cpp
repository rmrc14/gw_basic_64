#include <gtest/gtest.h>
#include <string>

bool hasBasExtension(const std::string& filename) {
    return filename.size() >= 4 &&
        filename.substr(filename.size() - 4) == ".bas";
}

TEST(MainHelpersTest, DetectsBasExtension) {
    EXPECT_TRUE(hasBasExtension("program.bas"));
    EXPECT_FALSE(hasBasExtension("program.txt"));
    EXPECT_TRUE(hasBasExtension(".bas"));
    EXPECT_FALSE(hasBasExtension("file.basx"));
    EXPECT_FALSE(hasBasExtension("file.bas.txt"));

}
