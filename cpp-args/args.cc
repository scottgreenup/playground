#include "args.h"

#include <string>
#include <map>

#include <gtest/gtest.h>

struct Args::Rep {
    std::map<std::string, std::vector<std::string>> values;
};

/**
 * Types of arguments
 *
 * -a
 * -a=value
 * --argument
 * --argument=value
 * -a value
 * --argument value
 */

inline bool IsAlphanumeric(char c) {
    return (c >= 'a' && c <= 'b') || (c >= 'A' && c <= 'Z');
}

static std::string GetShortArg(std::string str) {
    if (str.empty() || str.at(0) != '-' || !IsAlphanumeric(str.at(1))) {
        return "";
    }

    // replace the equal
    std::string::size_type pos = str.find("=");
    if (pos != std::string::npos) {
        return std::string(str.begin()+1, str.begin()+pos);
    }
    return std::string(str.begin()+1, str.end()-1);
}

static std::string GetLongArg(std::string str) {

    return "";
}


Args::Args(int argc, char* argv[]) {
    for (int i = 0; i < argc; ++i) {
        if (argv[i][0] == '-') {

        }

        if (argv[i][1] == '-') {

        }
    }
}

Args::~Args() {

}

/******************************************************************************/
#include <gtest/gtest.h>
#include "args.h"

TEST(ArgsTest, TestGetShortArg) {
    EXPECT_EQ("",    GetShortArg("word"));
    EXPECT_EQ("",    GetShortArg(" word"));
    EXPECT_EQ("",    GetShortArg("word "));
    EXPECT_EQ("arg", GetShortArg("-arg=value"));
    EXPECT_EQ("",    GetShortArg("--arg=value"));
    EXPECT_EQ("",    GetShortArg("-=value"));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

