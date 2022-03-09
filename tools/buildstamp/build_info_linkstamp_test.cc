#include "tools/buildstamp/build_info_linkstamp.h"

#include "gtest/gtest.h"
#include <string>

TEST(BuildInfo, CheckBuildRevision)
{
    auto buildRev = buildRevision();
    EXPECT_EQ(strlen(buildRev), 40);
    auto buildStat = buildStatus();
    EXPECT_STRNE("", buildStat);
    auto buildTs = buildTimestamp();
    EXPECT_GT(buildTs, 665);
    auto buildEmbLabel = buildEmbedLabel();
    EXPECT_STREQ("", buildEmbLabel);
    auto buildBr = buildBranch();
    EXPECT_STRNE(buildBr, "");
}
