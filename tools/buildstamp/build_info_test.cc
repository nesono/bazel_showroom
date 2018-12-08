#include "tools/buildstamp/build_info.h"

#include "gtest/gtest.h"
#include <cstring>

TEST(BuildInfo, CheckBuildRevision)
{
    auto buildRev = buildRevision();
    EXPECT_EQ(strlen(buildRev), 40);
    auto buildStat = buildStatus();
    EXPECT_STRNE("", buildStat);
    auto buildTs = buildTimestamp();
    EXPECT_GT(buildTs, 665);
    auto buildEmbLab = buildEmbedLabel();
    EXPECT_STREQ("", buildEmbLab);
    auto buildBr = buildBranch();
    EXPECT_STRNE(buildBr, "");
}
