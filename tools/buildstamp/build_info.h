#ifndef TOOLS_BUILDSTAMP_BUILD_INFO_H
#define TOOLS_BUILDSTAMP_BUILD_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

extern const char* buildRevision();
extern const char* buildStatus();
extern const long long buildTimestamp();
extern const char* buildEmbedLabel();
extern const char* buildBranch();

#ifdef __cplusplus
}
#endif
#endif  // TOOLS_BUILDSTAMP_BUILD_INFO_H

