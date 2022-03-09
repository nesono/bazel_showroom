#ifndef TOOLS_BUILDSTAMP_BUILD_INFO_LAST_CHANGE_H
#define TOOLS_BUILDSTAMP_BUILD_INFO_LAST_CHANGE_H

#ifdef __cplusplus
extern "C" {
#endif

extern const char* lastChangeBuildRevision();
extern const char* lastChangeBuildStatus();
extern const long long lastChangeBuildTimestamp();
extern const char* lastChangeBuildEmbedLabel();

#ifdef __cplusplus
}
#endif
#endif  // TOOLS_BUILDSTAMP_BUILD_INFO_LAST_CHANGE_H