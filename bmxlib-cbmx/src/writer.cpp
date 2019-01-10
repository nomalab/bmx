
#include <cbmx/writer.h>
#include <cbmx/essence_type.h>
#include <bmx/apps/AppUtils.h>
#include <bmx/clip_writer/ClipWriter.h>
#include <bmx/mxf_op1a/OP1ATrack.h>
#include <bmx/mxf_op1a/OP1AAVCITrack.h>
#include <bmx/wave/WaveFileIO.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>

struct BmxWriter {
    bmx::ClipWriter* clip;
    std::vector<bmx::ClipWriterTrack*> tracks;
};

void* create_as02_writer(const char* filename, struct MxfConfig* config) {
    std::string output_name(filename);
    bmx::Rational frame_rate;
    bmx::DefaultMXFFileFactory file_factory;

    frame_rate.numerator = config->frame_rate_num;
    frame_rate.denominator = config->frame_rate_den;

    bmx::ClipWriter* clip = bmx::ClipWriter::OpenNewAS02Clip(output_name, true, frame_rate, &file_factory, false);
    std::vector<bmx::ClipWriterTrack*> tracks;
    BmxWriter* writer = new BmxWriter();
    writer->clip = clip;
    writer->tracks = tracks;
    return (void*)writer;
}

void* create_op1a_writer(const char* filename, struct MxfConfig* config) {
    std::string output_name(filename);
    bmx::Rational frame_rate;
    bmx::DefaultMXFFileFactory file_factory;
    int flavour = OP1A_DEFAULT_FLAVOUR;
    bmx::Timecode start_timecode;

    frame_rate.numerator = config->frame_rate_num;
    frame_rate.denominator = config->frame_rate_den;

    if(config->single_pass) {
        flavour |= OP1A_SINGLE_PASS_WRITE_FLAVOUR;
    }
    if(config->kag_512) {
        flavour |= OP1A_512_KAG_FLAVOUR;
    }
    if(config->aes3) {
        flavour |= OP1A_AES_FLAVOUR;
    }
    if(config->ard_zdf_hdf) {
        flavour |= OP1A_ARD_ZDF_HDF_PROFILE_FLAVOUR;
    }

    // remaining Op1a options
    // OP1A_MIN_PARTITIONS_FLAVOUR
    // OP1A_377_2004_FLAVOUR
    // OP1A_SINGLE_PASS_WRITE_FLAVOUR
    // OP1A_SINGLE_PASS_MD5_WRITE_FLAVOUR
    // OP1A_NO_BODY_PART_UPDATE_FLAVOUR
    // OP1A_BODY_PARTITIONS_FLAVOUR
    // OP1A_MP_TRACK_NUMBER_FLAVOUR
    // OP1A_AS11_FLAVOUR

    bmx::ClipWriter* clip = bmx::ClipWriter::OpenNewOP1AClip(flavour, file_factory.OpenNew(output_name), frame_rate);
    bmx::OP1AFile *op1a_clip = clip->GetOP1AClip();
    if(config->partition_size_in_frames > 0) {
        op1a_clip->SetPartitionInterval(config->partition_size_in_frames);
    }

    start_timecode.Init(frame_rate, false);
    if (config->timecode) {
        parse_timecode(config->timecode, frame_rate, &start_timecode);
    }
    clip->SetStartTimecode(start_timecode);

    std::vector<bmx::ClipWriterTrack*> tracks;
    BmxWriter* writer = new BmxWriter();
    writer->clip = clip;
    writer->tracks = tracks;
    return (void*)writer;
}

void* create_rdd9_writer(const char* filename, struct MxfConfig* config) {
    std::string output_name(filename);
    bmx::Rational frame_rate;
    bmx::DefaultMXFFileFactory file_factory;
    int flavour = 0;

    frame_rate.numerator = config->frame_rate_num;
    frame_rate.denominator = config->frame_rate_den;

    if(config->single_pass) {
        flavour |= RDD9_SINGLE_PASS_WRITE_FLAVOUR;
    }

    bmx::ClipWriter* clip = bmx::ClipWriter::OpenNewRDD9Clip(flavour, file_factory.OpenNew(output_name), frame_rate);
    std::vector<bmx::ClipWriterTrack*> tracks;
    BmxWriter* writer = new BmxWriter();
    writer->clip = clip;
    writer->tracks = tracks;
    return (void*)writer;
}

void* create_d10_writer(const char* filename, struct MxfConfig* config) {
    std::string output_name(filename);
    bmx::Rational frame_rate;
    bmx::DefaultMXFFileFactory file_factory;
    int flavour = 0;

    frame_rate.numerator = config->frame_rate_num;
    frame_rate.denominator = config->frame_rate_den;

    if(config->single_pass) {
        flavour |= D10_SINGLE_PASS_WRITE_FLAVOUR;
    }

    bmx::ClipWriter* clip = bmx::ClipWriter::OpenNewD10Clip(flavour, file_factory.OpenNew(output_name), frame_rate);
    std::vector<bmx::ClipWriterTrack*> tracks;
    BmxWriter* writer = new BmxWriter();
    writer->clip = clip;
    writer->tracks = tracks;
    return (void*)writer;
}

void* create_avid_writer(const char* filename, struct MxfConfig* config) {
    std::string output_name(filename);
    bmx::Rational frame_rate;
    bmx::DefaultMXFFileFactory file_factory;
    int flavour = 0;

    frame_rate.numerator = config->frame_rate_num;
    frame_rate.denominator = config->frame_rate_den;

    bmx::ClipWriter* clip = bmx::ClipWriter::OpenNewAvidClip(flavour, frame_rate, &file_factory, false);
    std::vector<bmx::ClipWriterTrack*> tracks;
    BmxWriter* writer = new BmxWriter();
    writer->clip = clip;
    writer->tracks = tracks;
    return (void*)writer;
}

void* create_wave_writer(const char* filename, struct MxfConfig* config) {
    std::string output_name(filename);

    bmx::ClipWriter* clip = bmx::ClipWriter::OpenNewWaveClip(bmx::WaveFileIO::OpenNew(output_name));
    std::vector<bmx::ClipWriterTrack*> tracks;
    BmxWriter* writer = new BmxWriter();
    writer->clip = clip;
    writer->tracks = tracks;
    return (void*)writer;
}

void* create_writer(const char* filename, struct MxfConfig* config)
{
    switch(config->clip_type) {
        case CLIP_TYPE_ASO2:
            return create_as02_writer(filename, config);
        case CLIP_TYPE_OP1A:
            return create_op1a_writer(filename, config);
        case CLIP_TYPE_AVID:
            return create_avid_writer(filename, config);
        case CLIP_TYPE_D10:
            return create_d10_writer(filename, config);
        case CLIP_TYPE_RDD9:
            return create_rdd9_writer(filename, config);
        case CLIP_TYPE_WAVE:
            return create_wave_writer(filename, config);
        default: return NULL;
    }
}

void bmx_add_track(void* bmx_writer, EssenceType essence_type)
{
    BmxWriter* writer = (BmxWriter*)bmx_writer;
    bmx::ClipWriterTrack* clipWriterTrack = writer->clip->CreateTrack((bmx::EssenceType)essence_type, "");
    writer->tracks.push_back(clipWriterTrack);
}

void bmx_set_quantization_bits(void* bmx_writer, int track_index, int quantization_bits)
{
    BmxWriter* writer = (BmxWriter*)bmx_writer;
    writer->tracks[track_index]->SetQuantizationBits(quantization_bits);
}

void bmx_channel_count(void* bmx_writer, int track_index, int channel_count)
{
    BmxWriter* writer = (BmxWriter*)bmx_writer;
    writer->tracks[track_index]->SetChannelCount(channel_count);
}

int bmx_avci_header(void* bmx_writer, int track_index, int ps_avcihead, EssenceType essence_type)
{
    unsigned char avci_header_data[AVCI_HEADER_SIZE];
    BmxWriter* writer = (BmxWriter*)bmx_writer;
    bmx::Rational frame_rate = writer->clip->GetFrameRate();
    switch(essence_type) {
        case AVCI200_1080I:
        case AVCI200_1080P:
        case AVCI200_720P:
        case AVCI100_1080I:
        case AVCI100_1080P:
        case AVCI100_720P:
        case AVCI50_1080I:
        case AVCI50_1080P:
        case AVCI50_720P:
            if (ps_avcihead && bmx::get_ps_avci_header_data((bmx::EssenceType)essence_type, frame_rate,
                                                    avci_header_data, sizeof(avci_header_data)))
            {
                writer->tracks[track_index]->SetAVCIHeader(avci_header_data, sizeof(avci_header_data));
                return 0;
            }
        default: return -1;
    }
}

void bmx_afd(void* bmx_writer, int track_index, int afd)
{
    BmxWriter* writer = (BmxWriter*)bmx_writer;
    writer->tracks[track_index]->SetAFD(afd);
}

int bmx_init(void* bmx_writer)
{
    BmxWriter* writer = (BmxWriter*)bmx_writer;
    try {
        bmx::OP1AFile *op1a_clip = writer->clip->GetOP1AClip();
        op1a_clip->SetRepeatIndexTable(true);
        writer->clip->PrepareHeaderMetadata();
        writer->clip->PrepareWrite();
    }
    catch (const std::exception& e) {
        std::cout << "BMX ERROR " << e.what() << std::endl;
        return -1;
    }
    catch (...) {
        std::cout << "BMX ERROR !" << std::endl;
        return -1;
    }
    return 0;
}

int bmx_finish(void* bmx_writer)
{
    BmxWriter* writer = (BmxWriter*)bmx_writer;
    try {
        writer->clip->CompleteWrite();
    }
    catch (const std::exception& e) {
        std::cout << "ERROR " << e.what() << std::endl;
        return -1;
    }
    catch (...) {
        std::cout << "ERROR !" << std::endl;
        return -1;
    }
    return 0;
}

int bmx_uninit(void* bmx_writer)
{
    BmxWriter* writer = (BmxWriter*)bmx_writer;
    try {
        delete writer->clip;
        delete writer;
    }
    catch (const std::exception& e) {
        std::cout << "ERROR " << e.what() << std::endl;
        return -1;
    }
    catch (...) {
        std::cout << "ERROR !" << std::endl;
        return -1;
    }
    return 0;
}

int bmx_write_sample(void* bmx_writer, int track_index, const unsigned char *data, int size, int num_samples)
{
    BmxWriter* writer = (BmxWriter*)bmx_writer;
    try {
        assert(track_index < writer->tracks.size());
        writer->tracks[track_index]->WriteSamples(data, size, num_samples);
    }
    catch (const std::exception& e) {
        std::cout << "BMX ERROR " << e.what() << std::endl;
        return -1;
    }
    catch (...) {
        std::cout << "BMX ERROR !" << std::endl;
        return -1;
    }
    return 0;
}
