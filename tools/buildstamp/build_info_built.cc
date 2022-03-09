#include "tools/buildstamp/build_info_stable.h"

#include "tools/buildstamp/build_info_built.h"

extern const char* buildEmbedLabel()
{
    return BUILD_EMBED_LABEL;
}

extern const char* buildHost()
{
    return BUILD_HOST;
}

extern const char* buildUser()
{
    return BUILD_USER;
}

extern const char* buildDateStamp()
{
    return BUILD_DATE_STAMP;
}

extern const char* buildScmBranch()
{
    return BUILD_SCM_BRANCH;
}

extern const char* buildTimeStamp()
{
    return BUILD_TIME_STAMP;
}

extern const char* buildScmRevision()
{
    return BUILD_SCM_REVISION;
}

extern const char* buildScmStatus()
{
    return BUILD_SCM_STATUS;
}