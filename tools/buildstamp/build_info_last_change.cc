#include "tools/buildstamp/build_info_last_change.h"

const char* lastChangeBuildRevision()
{
    return BUILD_SCM_REVISION;
}

const char* lastChangeBuildStatus()
{
    return BUILD_SCM_STATUS;
}

const long long lastChangeBuildTimestamp()
{
    return BUILD_TIMESTAMP;
}

const char* lastChangeBuildEmbedLabel()
{
    return BUILD_EMBED_LABEL;
}