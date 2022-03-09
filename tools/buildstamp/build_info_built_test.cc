#include "tools/buildstamp/build_info_built.h"

#include "gtest/gtest.h"
#include <string>

TEST(BuildInfo, CheckBuildRevision)
{
    auto buildRev = buildScmRevision();
    EXPECT_EQ(strlen(buildRev), 40);
    auto buildTs = buildTimeStamp();
    EXPECT_STRNE(buildTs, "");
    auto buildEmbLab = buildEmbedLabel();
    EXPECT_STREQ("test label", buildEmbLab);
    auto buildBr = buildScmBranch();
    EXPECT_STRNE(buildBr, "");
    auto buildHst = buildHost();
    EXPECT_STRNE(buildHst, "");
    auto buildUsr = buildUser();
    EXPECT_STRNE(buildUsr, "");
    auto builtBuildDateStamp = buildDateStamp();
    EXPECT_STRNE(builtBuildDateStamp, "");
}
