#include "tools/buildstamp/build_info_last_change.h"

#include "gtest/gtest.h"
#include <string>

TEST(BuildInfo, CheckBuildRevision)
{
    auto buildRev = lastChangeBuildRevision();
    EXPECT_EQ(strlen(buildRev), 40);
    auto buildStat = lastChangeBuildStatus();
    EXPECT_STRNE("", buildStat);
    auto buildTs = lastChangeBuildTimestamp();
    EXPECT_GT(buildTs, 665);
    auto buildLabel = lastChangeBuildEmbedLabel();
    EXPECT_STREQ(buildLabel, "test label");
}
