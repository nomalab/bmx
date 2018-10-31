
#include <cbmx/version.h>
#include <bmx/Version.h>
#include <cstdlib>

void get_bmx_library_name(char* library_name) {
    std::string data = bmx::get_bmx_library_name();
    std::strcpy (library_name, data.c_str());
}

void get_bmx_version_string(char* version) {
    std::string data = bmx::get_bmx_version_string();
    std::strcpy (version, data.c_str());
}

void get_bmx_scm_version_string(char* scm_version) {
    std::string data = bmx::get_bmx_scm_version_string();
    std::strcpy (scm_version, data.c_str());
}

void get_bmx_build_string(char* build) {
    std::string data = bmx::get_bmx_build_string();
    std::strcpy (build, data.c_str());
}

// Timestamp get_bmx_build_timestamp();

void get_bmx_company_name(char* company_name) {
    std::string data = bmx::get_bmx_company_name();
    std::strcpy (company_name, data.c_str());
}

// UUID get_bmx_product_uid();
// mxfProductVersion get_bmx_mxf_product_version();

void get_bmx_mxf_version_string(char* mxf_version) {
    std::string data = bmx::get_bmx_mxf_version_string();
    std::strcpy (mxf_version, data.c_str());
}
