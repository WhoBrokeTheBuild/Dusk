#include "dusk/asset/Sound.hpp"

#include <dusk/core/Benchmark.hpp>
#include <dusk/core/Log.hpp>

#include <cstdio>

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

namespace dusk {

Sound::Sound(const std::string& filename) 
{
    LoadFromFile(filename);
}

Sound::~Sound()
{
    if (_alID > 0) {
        alDeleteBuffers(1, &_alID);
        _alID = 0;
    }
}

bool Sound::LoadFromFile(const std::string& filename)
{
    DuskBenchStart();

    const auto& paths = GetAssetPaths();

    FILE * fp = nullptr;

    std::string fullPath;
    for (auto& p : paths) {
        fullPath = p + filename;

        DuskLogVerbose("Checking '%s'", fullPath);
        fp = fopen(fullPath.c_str(), "rb");

        if (fp) break;
    }

    if (!fp) {
        DuskLogError("Failed to open file '%s'", filename);
        return false;
    }

    OggVorbis_File vf;
    if (ov_open(fp, &vf, nullptr, 0) < 0) {
        DuskLogError("Invalid file format");
        return false;
    }

    char ** ptr = ov_comment(&vf, -1)->user_comments;
    for (; *ptr; ++ptr) {
        DuskLogVerbose("%s", *ptr);
    }
    
    vorbis_info * vi = ov_info(&vf, -1);
    DuskLogVerbose("Bitstream is %d channel, %ldHz", vi->channels, vi->rate);
    DuskLogVerbose("Decoded length is %ld samples", (long)ov_pcm_total(&vf, -1));
    DuskLogVerbose("Encoded by %s", ov_comment(&vf, -1)->vendor);
    
    ALenum format = (vi->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16);
    size_t length = (size_t)ov_pcm_total(&vf, -1) * vi->channels * 2;
    std::vector<uint16_t *> data(length);

    size_t size = 0;
    size_t offset = 0;
    size_t sel = 0;

    do {
        offset += size;
        size = ov_read(&vf, (char *)data.data() + offset, 4096, 0, 2, 1, (int *)&sel);
    } while (size > 0);

    if (size < 0) {
        DuskLogError("Failed to read data from file '%s'", filename);
    }
    
    fclose(fp);

    alGenBuffers(1, &_alID);
    alBufferData(_alID, format, data.data(), data.size(), vi->rate);
    ov_clear(&vf);

    if (alGetError() != AL_NO_ERROR) {
        DuskLogError("Failed to upload to OpenAL buffer: %d", alGetError());
        return false;
    }

    DuskLogLoad("Loaded Sound from '%s", fullPath);

    SetLoaded(true);

    DuskBenchEnd("Sound::LoadFromFile");
    return true;
}

} // namespace dusk
