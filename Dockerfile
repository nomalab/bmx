FROM ubuntu

ADD . /src
WORKDIR /src

RUN apt update
RUN apt install -y \
  automake \
  build-essential \
  cmake \
  git \
  libexpat-dev \
  liburiparser-dev \
  libtool \
  m4 \
  nasm \
  pkg-config \
  uuid-dev

RUN \
  cd bmxlib-libmxf && \
  ./autogen.sh && \
  ./configure && \
  make && \
  make install

RUN \
  cd bmxlib-libmxfpp && \
  ./autogen.sh && \
  ./configure && \
  make && \
  make install

RUN \
  cd bmxlib-bmx && \
  ./autogen.sh && \
  ./configure && \
  make && \
  make install

RUN \
  cd bmxlib-cbmx && \
  mkdir -p build && \
  cd build && \
  cmake .. && \
  make && \
  make install

RUN \
  git clone https://github.com/ffmpeg/ffmpeg && \
  cd ffmpeg && \
  git apply ../FFmpeg-patches/0001_declare_BMX_wrapper.patch && \
  git apply ../FFmpeg-patches/0002_add_bmx_files.patch && \
  ./configure --enable-libcbmx && \
  make && \
  make install

# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/

