
#ifndef __CBMX_VERSION_H__
#define __CBMX_VERSION_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CBMX_LIBRARY_NAME "cbmx"

void get_bmx_library_name(char* library_name);
void get_bmx_version_string(char* version);
void get_bmx_scm_version_string(char* scm_version);
void get_bmx_build_string(char* build);

void get_bmx_company_name(char* company_name);
void get_bmx_mxf_version_string(char* mxf_version);

#ifdef __cplusplus
}
#endif

#endif
