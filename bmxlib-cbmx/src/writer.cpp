
#include <cbmx/writer.h>
#include <cbmx/essence_type.h>
#include <bmx/apps/AppUtils.h>
#include <bmx/as10/AS10RDD9Validator.h>
#include <bmx/clip_writer/ClipWriter.h>
#include <bmx/apps/AS10Helper.h>
#include <bmx/apps/AS11Helper.h>
#include <bmx/mxf_op1a/OP1ATrack.h>
#include <bmx/mxf_op1a/OP1AAVCITrack.h>
#include <bmx/wave/WaveFileIO.h>
#include <bmx/Utils.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>

struct BmxWriter {
    bmx::ClipWriter* clip;
    std::vector<bmx::ClipWriterTrack*> tracks;
};

typedef struct {
    ShimName shim;
    const char *name;
} ShimNamesMap;

static const ShimNamesMap SHIM_NAMES_MAP[] =
    {
        {AS10_HIGH_HD_2014,         "HIGH_HD_2014"},
        {AS10_CNN_HD_2012,          "CNN_HD_2012"},
        {AS10_NRK_HD_2012,          "NRK_HD_2012"},
        {AS10_JVC_HD_35_VBR_2012,   "JVC_HD_35_VBR_2012"},
        {AS10_JVC_HD_25_CBR_2012,   "JVC_HD_25_CBR_2012"},
    };

const char* get_as10_shim_name(ShimName shim) {
    size_t i;
    for (i = 0; i < BMX_ARRAY_SIZE(SHIM_NAMES_MAP); i++) {
        if (shim == SHIM_NAMES_MAP[i].shim)
            return SHIM_NAMES_MAP[i].name;
    }
    return NULL;
}

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

void* create_as10_writer(const char* filename, struct MxfConfig* config) {
    std::string output_name(filename);
    bmx::Rational frame_rate;
    bmx::DefaultMXFFileFactory file_factory;

    frame_rate.numerator = config->frame_rate_num;
    frame_rate.denominator = config->frame_rate_den;

    if (get_as10_shim_name(config->shim_name) == NULL) {
        std::cerr << "BMX AS-10 ERROR : UNABLE TO GET SHIM NAME" << std::endl;
        return NULL;
    }

    bmx::AS10Shim as10_shim = bmx::get_as10_shim(get_as10_shim_name(config->shim_name));

    bmx::ClipWriter* clip = bmx::ClipWriter::OpenNewRDD9Clip(RDD9_AS10_FLAVOUR, file_factory.OpenNew(output_name), frame_rate);

    bmx::RDD9File *rdd9_clip = clip->GetRDD9Clip();
    rdd9_clip->SetValidator(new bmx::AS10RDD9Validator(as10_shim, config->loose_checks));

    std::vector<bmx::ClipWriterTrack*> tracks;
    BmxWriter* writer = new BmxWriter();
    writer->clip = clip;
    writer->tracks = tracks;
    return (void*)writer;
}


void* create_as11_op1a_writer(const char* filename, struct MxfConfig* config) {
    std::string output_name(filename);
    bmx::Rational frame_rate;
    bmx::DefaultMXFFileFactory file_factory;

    frame_rate.numerator = config->frame_rate_num;
    frame_rate.denominator = config->frame_rate_den;

    int flavour = OP1A_AS11_FLAVOUR;
    if (config->as11_core) {
        flavour |= OP1A_MP_TRACK_NUMBER_FLAVOUR;
    }
    if(config->single_pass) {
        flavour |= OP1A_SINGLE_PASS_WRITE_FLAVOUR;
    }
    if(config->kag_512) {
        flavour |= OP1A_512_KAG_FLAVOUR;
    }

    bmx::ClipWriter* clip = bmx::ClipWriter::OpenNewOP1AClip(flavour, file_factory.OpenNew(output_name), frame_rate);

    bmx::OP1AFile *op1a_clip = clip->GetOP1AClip();

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
    if(config->smpte_st377m2004) {
        flavour |= OP1A_377_2004_FLAVOUR;
    }

    // remaining Op1a options
    // OP1A_MIN_PARTITIONS_FLAVOUR
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
    if(config->smpte_st377m2004) {
        flavour |= RDD9_SMPTE_377_2004_FLAVOUR;
    }

    // remaining rdd9 options
    // RDD9_SINGLE_PASS_MD5_WRITE_FLAVOUR
    // RDD9_NO_BODY_PART_UPDATE_FLAVOUR
    // RDD9_ARD_ZDF_HDF_PROFILE_FLAVOUR
    // RDD9_AS11_FLAVOUR

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

    // remaining d10 options
    // D10_SINGLE_PASS_MD5_WRITE_FLAVOUR
    // D10_AS11_FLAVOUR

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
        case CLIP_TYPE_AS10:
            return create_as10_writer(filename, config);
        case CLIP_TYPE_AS11_OP1A:
            return create_as11_op1a_writer(filename, config);
        case CLIP_TYPE_WAVE:
            return create_wave_writer(filename, config);
        default: return NULL;
    }
}

void bmx_add_shim_metadata(void* bmx_writer, ShimName shim_name)
{
    // For AS-10 only
    bmx::AS10Helper as10_helper;
    BmxWriter* writer = (BmxWriter*)bmx_writer;

    as10_helper.SetFrameworkProperty("as10", "ShimName", get_as10_shim_name(shim_name));
    as10_helper.AddMetadata(writer->clip);
    as10_helper.Complete();
}

int bmx_set_descriptive_metadata(void* bmx_writer, const char* core_filename, const char* dpp_filename, const char* segmentation_filename)
{
    bmx::AS11Helper as11_helper;
    BmxWriter* writer = (BmxWriter*)bmx_writer;
    bmx::Rational frame_rate = writer->clip->GetFrameRate();

    if (core_filename != NULL && !as11_helper.ParseFrameworkFile("as11", core_filename)) {
        std::cerr << "BMX AS-11 ERROR : UNABLE TO PARSE CORE FILE " << core_filename << std::endl;
        return -1;
    }
    if (dpp_filename != NULL && !as11_helper.ParseFrameworkFile("dpp", dpp_filename)) {
        std::cerr << "BMX AS-11 ERROR : UNABLE TO PARSE DPP FILE " << dpp_filename << std::endl;
        return -1;
    }
    if (segmentation_filename != NULL && !as11_helper.ParseSegmentationFile(segmentation_filename, frame_rate)) {
        std::cerr << "BMX AS-11 ERROR : UNABLE TO PARSE SEGMENTATION FILE " << segmentation_filename << std::endl;
        return -1;
    }

    as11_helper.AddMetadata(writer->clip);
    as11_helper.Complete();

    return 0;
}


void bmx_add_track(void* bmx_writer, EssenceType essence_type)
{
    BmxWriter* writer = (BmxWriter*)bmx_writer;
    bmx::ClipWriterTrack* clipWriterTrack = writer->clip->CreateTrack((bmx::EssenceType)essence_type, "");
    writer->tracks.push_back(clipWriterTrack);
}

void bmx_set_aspect_ratio(void* bmx_writer, int track_index, int display_aspect_ratio_num, int display_aspect_ratio_den)
{
    BmxWriter* writer = (BmxWriter*)bmx_writer;
    bmx::Rational aspect_ratio;
    aspect_ratio.numerator = display_aspect_ratio_num;
    aspect_ratio.denominator = display_aspect_ratio_den;
    writer->tracks[track_index]->SetAspectRatio(aspect_ratio);
}

void bmx_set_timecode(void* bmx_writer, struct MxfConfig* config) {
    bmx::Timecode start_timecode;
    bmx::Rational frame_rate;
    BmxWriter* writer = (BmxWriter*)bmx_writer;

    frame_rate.numerator = config->frame_rate_num;
    frame_rate.denominator = config->frame_rate_den;
    start_timecode.Init(frame_rate, false); //non drop frame

    if (config->timecode) {
        parse_timecode(config->timecode, frame_rate, &start_timecode);
    }
    writer->clip->SetStartTimecode(start_timecode);
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
        if (writer->clip->GetType() == bmx::CW_OP1A_CLIP_TYPE) {
            bmx::OP1AFile *op1a_clip = writer->clip->GetOP1AClip();
            op1a_clip->SetRepeatIndexTable(true);
        }
        writer->clip->PrepareHeaderMetadata();
        writer->clip->PrepareWrite();
    }
    catch (const std::exception& e) {
        std::cerr << "BMX EXCEPTION (init) " << e.what() << std::endl;
        return -1;
    }
    catch (...) {
        std::cerr << "BMX ERROR (init) !" << std::endl;
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
        std::cerr << "BMX EXCEPTION (finish) " << e.what() << std::endl;
        return -1;
    }
    catch (...) {
        std::cerr << "BMX ERROR (finish) !" << std::endl;
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
        std::cerr << "BMX EXCEPTION (uninit) " << e.what() << std::endl;
        return -1;
    }
    catch (...) {
        std::cerr << "BMX ERROR (uninit) !" << std::endl;
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
        std::cerr << "BMX EXCEPTION (write sample) " << e.what() << std::endl;
        return -1;
    }
    catch (...) {
        std::cerr << "BMX ERROR (write sample) !" << std::endl;
        return -1;
    }
    return 0;
}
