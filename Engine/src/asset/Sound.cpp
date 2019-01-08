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
    

    ov_clear(&vf);

	DuskLogLoad("Loaded Sound from '%s", fullPath);

    SetLoaded(true);

    DuskBenchEnd("Sound::LoadFromFile");
    return true;
}

} // namespace dusk
