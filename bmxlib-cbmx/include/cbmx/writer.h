
#ifndef __CBMX_WRITER_H__
#define __CBMX_WRITER_H__

#include <cbmx/essence_type.h>

#ifdef __cplusplus
extern "C" {
#endif

void* create_as02_writer(const char* filename, int frame_rate_num, int frame_rate_den);
void* create_op1a_writer(const char* filename, int frame_rate_num, int frame_rate_den);

void* bmx_add_track(void* clip, EssenceType essence_type);

void bmx_set_quantization_bits(void* data_writer, int quantization_bits);
void bmx_channel_count(void* data_writer, int channel_count);

bool bmx_init(void* clip);
bool bmx_uninit(void* clip);

bool bmx_write_sample(void* data_track, const unsigned char *data, int size, int num_samples);

#ifdef __cplusplus
}
#endif

#endif
