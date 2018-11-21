
#ifndef __CBMX_WRITER_H__
#define __CBMX_WRITER_H__

#include <cbmx/essence_type.h>

#ifdef __cplusplus
extern "C" {
#endif

void* create_as02_writer(const char* filename, int frame_rate_num, int frame_rate_den);
void* create_op1a_writer(const char* filename, int frame_rate_num, int frame_rate_den);
void* create_rdd9_writer(const char* filename, int frame_rate_num, int frame_rate_den);
void* create_d10_writer(const char* filename, int frame_rate_num, int frame_rate_den);
void* create_avid_writer(const char* filename, int frame_rate_num, int frame_rate_den);
void* create_wave_writer(const char* filename, int frame_rate_num, int frame_rate_den);

void bmx_add_track(void* bmx_writer, EssenceType essence_type);

void bmx_set_quantization_bits(void* bmx_writer, int track_index, int quantization_bits);
void bmx_channel_count(void* bmx_writer, int track_index, int channel_count);

bool bmx_init(void* bmx_writer);
bool bmx_finish(void* bmx_writer);
bool bmx_uninit(void* bmx_writer);

bool bmx_write_sample(void* bmx_writer, int track_index, const unsigned char *data, int size, int num_samples);

#ifdef __cplusplus
}
#endif

#endif
