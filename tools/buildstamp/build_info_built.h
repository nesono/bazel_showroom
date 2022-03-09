#ifndef TOOLS_BUILDSTAMP_BUILD_INFO_BUILT_H
#define TOOLS_BUILDSTAMP_BUILD_INFO_BUILT_H

#ifdef __cplusplus
extern "C" {
#endif

extern const char* buildEmbedLabel();
extern const char* buildHost();
extern const char* buildUser();
extern const char* buildDateStamp();
extern const char* buildTimeStamp();
extern const char* buildScmRevision();
extern const char* buildScmBranch();
extern const char* buildScmStatus();

#ifdef __cplusplus
}
#endif
#endif  // TOOLS_BUILDSTAMP_BUILD_INFO_BUILT_H