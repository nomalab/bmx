
#include <cbmx/writer.h>
#include <cbmx/essence_type.h>
#include <bmx/clip_writer/ClipWriter.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>

struct BmxWriter {
    bmx::ClipWriter* clip;
    std::vector<bmx::ClipWriterTrack*> tracks;
};

void* create_as02_writer(const char* filename, int frame_rate_num, int frame_rate_den) {
    std::string output_name(filename);
    bmx::Rational frame_rate;
    bmx::DefaultMXFFileFactory file_factory;

    bmx::ClipWriter* clip = bmx::ClipWriter::OpenNewAS02Clip(output_name, true, frame_rate, &file_factory, false);
    std::vector<bmx::ClipWriterTrack*> tracks;
    BmxWriter* w = new BmxWriter();
    w->clip = clip;
    w->tracks = tracks;
    return (void*)w;
}

void* create_op1a_writer(const char* filename, int frame_rate_num, int frame_rate_den) {
    std::string output_name(filename);
    bmx::Rational frame_rate;
    bmx::DefaultMXFFileFactory file_factory;
    int flavour = OP1A_DEFAULT_FLAVOUR | OP1A_512_KAG_FLAVOUR;

    frame_rate.numerator = frame_rate_num;
    frame_rate.denominator = frame_rate_den;

    flavour |= OP1A_SINGLE_PASS_WRITE_FLAVOUR;

    bmx::ClipWriter* clip = bmx::ClipWriter::OpenNewOP1AClip(flavour, file_factory.OpenNew(output_name), frame_rate);
    std::vector<bmx::ClipWriterTrack*> tracks;
    BmxWriter* w = new BmxWriter();
    w->clip = clip;
    w->tracks = tracks;
    return (void*)w;
}

void* create_rdd9_writer(const char* filename, int frame_rate_num, int frame_rate_den) {
    std::string output_name(filename);
    bmx::Rational frame_rate;
    bmx::DefaultMXFFileFactory file_factory;
    int flavour = 0;

    frame_rate.numerator = frame_rate_num;
    frame_rate.denominator = frame_rate_den;

    flavour |= RDD9_SINGLE_PASS_WRITE_FLAVOUR;

    bmx::ClipWriter* clip = bmx::ClipWriter::OpenNewRDD9Clip(flavour, file_factory.OpenNew(output_name), frame_rate);
    std::vector<bmx::ClipWriterTrack*> tracks;
    BmxWriter* w = new BmxWriter();
    w->clip = clip;
    w->tracks = tracks;
    return (void*)w;
}

void* create_d10_writer(const char* filename, int frame_rate_num, int frame_rate_den) {
    std::string output_name(filename);
    bmx::Rational frame_rate;
    bmx::DefaultMXFFileFactory file_factory;
    int flavour = 0;

    frame_rate.numerator = frame_rate_num;
    frame_rate.denominator = frame_rate_den;

    flavour |= D10_SINGLE_PASS_WRITE_FLAVOUR;

    bmx::ClipWriter* clip = bmx::ClipWriter::OpenNewRDD9Clip(flavour, file_factory.OpenNew(output_name), frame_rate);
    std::vector<bmx::ClipWriterTrack*> tracks;
    BmxWriter* w = new BmxWriter();
    w->clip = clip;
    w->tracks = tracks;
    return (void*)w;
}

void* create_avid_writer(const char* filename, int frame_rate_num, int frame_rate_den) {
    std::string output_name(filename);
    bmx::Rational frame_rate;
    bmx::DefaultMXFFileFactory file_factory;
    int flavour = 0;

    frame_rate.numerator = frame_rate_num;
    frame_rate.denominator = frame_rate_den;

    bmx::ClipWriter* clip = bmx::ClipWriter::OpenNewRDD9Clip(flavour, file_factory.OpenNew(output_name), frame_rate);
    std::vector<bmx::ClipWriterTrack*> tracks;
    BmxWriter* w = new BmxWriter();
    w->clip = clip;
    w->tracks = tracks;
    return (void*)w;
}

void* create_wave_writer(const char* filename, int frame_rate_num, int frame_rate_den) {
    std::string output_name(filename);
    bmx::Rational frame_rate;
    bmx::DefaultMXFFileFactory file_factory;
    int flavour = 0;

    frame_rate.numerator = frame_rate_num;
    frame_rate.denominator = frame_rate_den;

    bmx::ClipWriter* clip = bmx::ClipWriter::OpenNewRDD9Clip(flavour, file_factory.OpenNew(output_name), frame_rate);
    std::vector<bmx::ClipWriterTrack*> tracks;
    BmxWriter* w = new BmxWriter();
    w->clip = clip;
    w->tracks = tracks;
    return (void*)w;
}

void bmx_add_track(void* bmx_writer, EssenceType essence_type)
{
    BmxWriter* w = (BmxWriter*)bmx_writer;
    bmx::ClipWriterTrack* clipWriterTrack = w->clip->CreateTrack((bmx::EssenceType)essence_type, "");
    w->tracks.push_back(clipWriterTrack);
}

void bmx_set_quantization_bits(void* bmx_writer, int track_index, int quantization_bits)
{
    BmxWriter* w = (BmxWriter*)bmx_writer;
    w->tracks[track_index]->SetQuantizationBits(quantization_bits);
}

void bmx_channel_count(void* bmx_writer, int track_index, int channel_count)
{
    BmxWriter* w = (BmxWriter*)bmx_writer;
    w->tracks[track_index]->SetChannelCount(channel_count);
}

bool bmx_init(void* bmx_writer)
{
    BmxWriter* w = (BmxWriter*)bmx_writer;
    try {
        bmx::OP1AFile *op1a_clip = w->clip->GetOP1AClip();
        op1a_clip->SetRepeatIndexTable(true);
        w->clip->PrepareHeaderMetadata();
        w->clip->PrepareWrite();
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

bool bmx_finish(void* bmx_writer)
{
    BmxWriter* w = (BmxWriter*)bmx_writer;
    try {
        w->clip->CompleteWrite();
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

bool bmx_uninit(void* bmx_writer)
{
    BmxWriter* w = (BmxWriter*)bmx_writer;
    try {
        delete w->clip;
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

bool bmx_write_sample(void* bmx_writer, int track_index, const unsigned char *data, int size, int num_samples)
{
    BmxWriter* w = (BmxWriter*)bmx_writer;
    try {
        assert(track_index < w->tracks.size());
        w->tracks[track_index]->WriteSamples(data, size, num_samples);
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
