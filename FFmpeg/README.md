
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
You can and `--enable-libcbmx` to build with BMX to build FFmpeg.

--extra-cflags="-I/path/to/extra/includes"

add_cflags -I`pwd`/../include/bmx-0.1
add_cflags -I`pwd`/../include/libMXF++-1.0
add_cflags -I`pwd`/../include/libMXF-1.0
