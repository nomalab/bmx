
FFmpeg with BMX
===

# Get FFmpeg
```bash
git clone https://github.com/ffmpeg/ffmpeg
```

If master doesn't work to apply these patches, the last valid commit is :
caaf4633117f77a545975dac18e85b8fcdbc9ce7

# Apply patches

```bash
cd ffmpeg
git apply /path/to/FFmpeg-patches/*.patch
```

# Build

## BMX

Firstly, build bmx libraries `libMXF`, `libMXF++` and `bmx`. The instructions are [here](https://sourceforge.net/p/bmxlib/home/Home/).

Build `cbmx` library :

```bash
git clone -b cbmx https://github.com/nomalab/bmx --depth 1 bmxlib
cd bmxlib/bmxlib-cbmx
mkdir build
cd build
cmake ..
make
sudo make install
```

## FFmpeg

Configure and build FFmpeg with needed libraries. You can find information in FFmpeg Readme and [here](https://trac.ffmpeg.org/wiki/CompilationGuide).

Add `--enable-libcbmx` to build with bmx.

Use `-f bmx` in FFmpeg commands for MXF format.
