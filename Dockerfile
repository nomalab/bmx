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
  make

RUN \
  git clone https://github.com/ffmpeg/ffmpeg && \
  cd ffmpeg && \
  git apply ../FFmpeg-patches/0001-start-to-integrate-BMX-library.patch && \
  ./configure --enable-libcbmx --extra-cflags="-I`pwd`/../bmxlib-cbmx/include" && \
  make && \
  make install

# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/

