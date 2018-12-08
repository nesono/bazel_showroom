#include "build_info.h"

const char* buildRevision()
{
    return BUILD_SCM_REVISION;
}

const char* buildStatus()
{
    return BUILD_SCM_STATUS;
}

const long long buildTimestamp()
{
    return BUILD_TIMESTAMP;
}

const char* buildEmbedLabel()
{
    return BUILD_EMBED_LABEL;
}

const char* buildBranch()
{
    return "currently unsupported by bazel" /*BUILD_SCM_BRANCH*/;
}
