
#include <cbmx/writer.h>
#include <cbmx/essence_type.h>
#include <bmx/clip_writer/ClipWriter.h>
#include <cstdlib>
#include <string>
#include <iostream>

void* create_as02_writer(const char* filename, int frame_rate_num, int frame_rate_den) {
    std::string output_name(filename);
    bmx::Rational frame_rate;
    bmx::DefaultMXFFileFactory file_factory;

    return bmx::ClipWriter::OpenNewAS02Clip(output_name, true, frame_rate, &file_factory, false);
}

void* create_op1a_writer(const char* filename, int frame_rate_num, int frame_rate_den) {
    std::string output_name(filename);
    bmx::Rational frame_rate;
    bmx::DefaultMXFFileFactory file_factory;
    int flavour = OP1A_DEFAULT_FLAVOUR | OP1A_512_KAG_FLAVOUR;

    frame_rate.numerator = frame_rate_num;
    frame_rate.denominator = frame_rate_den;

    return bmx::ClipWriter::OpenNewOP1AClip(flavour, file_factory.OpenNew(output_name), frame_rate);
}

void* create_rdd9_writer(const char* filename) {
    std::string output_name(filename);
    bmx::Rational frame_rate;
    bmx::DefaultMXFFileFactory file_factory;
    int flavour = 0;

    return bmx::ClipWriter::OpenNewRDD9Clip(flavour, file_factory.OpenNew(output_name), frame_rate);
}

void* bmx_add_track(void* data_clip, EssenceType essence_type)
{
    bmx::ClipWriter* clip = (bmx::ClipWriter*)data_clip;
    bmx::ClipWriterTrack* clipWriterTrack = clip->CreateTrack((bmx::EssenceType)essence_type, "video");
    return clipWriterTrack;
}

void bmx_set_quantization_bits(void* data_writer, int quantization_bits)
{
    bmx::ClipWriterTrack* clipWriterTrack = (bmx::ClipWriterTrack*)data_writer;
    clipWriterTrack->SetQuantizationBits(quantization_bits);
}

void bmx_channel_count(void* data_writer, int channel_count)
{
    bmx::ClipWriterTrack* clipWriterTrack = (bmx::ClipWriterTrack*)data_writer;
    clipWriterTrack->SetChannelCount(channel_count);
}

bool bmx_init(void* data_clip)
{
    bmx::ClipWriter* clip = (bmx::ClipWriter*)data_clip;
    try {
        bmx::OP1AFile *op1a_clip = clip->GetOP1AClip();
        op1a_clip->SetRepeatIndexTable(true);
        clip->PrepareHeaderMetadata();
        clip->PrepareWrite();
    }
    catch (const std::exception& e) {
        std::cout << "BMX ERROR " << e.what() << std::endl;
        return false;
    }
    catch (...) {
        std::cout << "BMX ERROR !" << std::endl;
        return false;
    }
    return true;
}

bool bmx_uninit(void* data_clip)
{
    bmx::ClipWriter* clip = (bmx::ClipWriter*)data_clip;
    try {
        clip->CompleteWrite();
    }
    catch (const std::exception& e) {
        std::cout << "ERROR " << e.what() << std::endl;
        return false;
    }
    catch (...) {
        std::cout << "ERROR !" << std::endl;
        return false;
    }
    return true;
}

bool bmx_write_sample(void* data_track, const unsigned char *data, int size, int num_samples)
{
    bmx::ClipWriterTrack* clipWriterTrack = (bmx::ClipWriterTrack*)data_track;
    std::cout << "*** " << num_samples << " samples for " << size << " bytes" << std::endl;
    try {
        clipWriterTrack->WriteSamples(
            data,
            size,
            num_samples);
    }
    catch (const std::exception& e) {
        std::cout << "BMX ERROR " << e.what() << std::endl;
        return false;
    }
    catch (...) {
        std::cout << "BMX ERROR !" << std::endl;
        return false;
    }
    return true;
}
