
FFmpeg with BMX
===

# Get FFmpeg
```
git clone https://github.com/ffmpeg/ffmpeg
```

if master not works to apply these patch, the lastest validated commit is:
323c2cfd384a06ad1e5233306156eb931185193f

# Apply patches

```
git apply *.patch
```

# Build
You can and `--enable-libcbmx`, `--extra-cflags` and `--extra-ldflags` to build with BMX to build FFmpeg.
The example is provided in the `Dockerfile` at the root of this repository.
