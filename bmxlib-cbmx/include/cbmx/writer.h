
#ifndef __CBMX_WRITER_H__
#define __CBMX_WRITER_H__

#include <cbmx/essence_type.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CLIP_TYPE_ASO2 = 0,
    CLIP_TYPE_OP1A,
    CLIP_TYPE_AVID,
    CLIP_TYPE_D10,
    CLIP_TYPE_RDD9,
    CLIP_TYPE_WAVE,
    CLIP_TYPE_NB
} ClipType;

typedef enum {
    AS10_UNKNOWN_SHIM = 0,
    AS10_HIGH_HD_2014,
    AS10_CNN_HD_2012,
    AS10_NRK_HD_2012,
    AS10_JVC_HD_35_VBR_2012,
    AS10_JVC_HD_25_CBR_2012,
} ShimName;

struct MxfConfig
{
    ClipType clip_type;
    ShimName shim_name;
    int ard_zdf_hdf;
    int frame_rate_num;
    int frame_rate_den;
    int partition_size_in_frames;
    int aes3;
    int kag_512;
    int single_pass;
    const char* timecode;
} MxfConfig;

void* create_writer(const char* filename, struct MxfConfig* config);

void bmx_add_track(void* bmx_writer, EssenceType essence_type);

void bmx_set_quantization_bits(void* bmx_writer, int track_index, int quantization_bits);
void bmx_channel_count(void* bmx_writer, int track_index, int channel_count);
int bmx_avci_header(void* bmx_writer, int track_index, int ps_avcihead, EssenceType essence_type);
void bmx_afd(void* bmx_writer, int track_index, int afd);

int bmx_init(void* bmx_writer);
int bmx_finish(void* bmx_writer);
int bmx_uninit(void* bmx_writer);

int bmx_write_sample(void* bmx_writer, int track_index, const unsigned char *data, int size, int num_samples);

#ifdef __cplusplus
}
#endif

#endif
