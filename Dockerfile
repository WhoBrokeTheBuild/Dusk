FROM ubuntu:20.04 AS builder-minimal

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update \
    && apt install -y \
        wget \
        software-properties-common \
    && add-apt-repository -y ppa:ubuntu-toolchain-r/test \
    && bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)" \
    && apt purge -y \
        software-properties-common \
    && apt autoremove -y --purge \
    && rm -rf /var/lib/apt/lists/*

RUN apt update \
    && apt install -y \
        g++-8 \
        clang \
        git git-lfs \
        make \
        xorg-dev \
        libglu1-mesa-dev \
        libpython3.8-dev \
    && rm -rf /var/lib/apt/lists/*

ARG CMAKE_VERSION=3.18.4

RUN wget -qO cmake-install.sh https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-Linux-x86_64.sh \
    && bash cmake-install.sh --prefix=/usr/ --skip-license --exclude-subdir \
    && rm -f cmake-install.sh

ARG VULKAN_SDK_VERSION=1.2.154.0
ARG VULKAN_SDK_MD5SUM=91c548e851d4c679c072c109aae8a775

RUN wget -qO vulkan-sdk.tar.gz https://sdk.lunarg.com/sdk/download/${VULKAN_SDK_VERSION}/linux/vulkansdk-linux-x86_64-${VULKAN_SDK_VERSION}.tar.gz \
    && echo "${VULKAN_SDK_MD5SUM} vulkan-sdk.tar.gz" > vulkan-sdk.md5 \
    && md5sum --status -c < vulkan-sdk.md5 \
    && mkdir -p /opt/vulkan/ \
    && tar -xf vulkan-sdk.tar.gz -C /opt/vulkan/ \
    && rm -rf /tmp/vulkan-sdk.tar.gz /tmp/vulkan-sdk.md5

ENV VULKAN_SDK=/opt/vulkan/${VULKAN_SDK_VERSION}/x86_64



FROM builder-minimal AS builder

RUN apt update \
    && apt install -y \
        libogg-dev \
        libvorbis-dev \
        libtheora-dev \
        libgtest-dev \
        coreutils \
        unzip \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /tmp

ARG GLM_VERSION=0.9.9.8
ARG GLM_MD5SUM=69895110052f0d711c9c54fbf385f6f5

RUN wget -qO glm.zip https://github.com/g-truc/glm/releases/download/${GLM_VERSION}/glm-${GLM_VERSION}.zip \
    && echo "${GLM_MD5SUM} glm.zip" > glm.md5 \
    && md5sum --status -c < glm.md5 \
    && unzip glm.zip \
    && cp -R glm/glm/ /usr/include/ \
    && cp -R glm/cmake/ /usr/share/ \
    && rm -rf /tmp/glm.zip /tmp/glm.md5 /tmp/glm/

ARG GSL_VERSION=3.1.0
ARG GSL_MD5SUM=b6910c54113f921b03dc06642cf7f11c

RUN wget -qO gsl.tar.gz https://github.com/microsoft/GSL/archive/v${GSL_VERSION}.tar.gz \
    && echo "${GSL_MD5SUM} gsl.tar.gz" > gsl.md5 \
    && md5sum --status -c < gsl.md5 \
    && tar -xf gsl.tar.gz \
    && mkdir -p GSL-${GSL_VERSION}/build \
    && cd GSL-${GSL_VERSION}/build \
    && cmake -DGSL_TEST=OFF .. \
    && make -l install \
    && rm -rf /tmp/gsl.tar.gz /tmp/gsl.md5 /tmp/GSL-${GSL_VERSION}/

ARG JSON_VERSION=3.9.1
ARG JSON_MD5SUM=e386222fb57dd2fcb8a7879fc016d037

RUN wget -qO json.tar.gz https://github.com/nlohmann/json/archive/v${JSON_VERSION}.tar.gz \
    && echo "${JSON_MD5SUM} json.tar.gz" > json.md5 \
    && md5sum --status -c < json.md5 \
    && tar -xf json.tar.gz \
    && mkdir -p json-${JSON_VERSION}/build \
    && cd json-${JSON_VERSION}/build \
    && cmake -DJSON_BuildTests=OFF .. \
    && make -l install \
    && rm -rf /tmp/json.tar.gz /tmp/json.md5 /tmp/json-${JSON_VERSION}

ARG SDL2_VERSION=2.0.12
ARG SDL2_MD5SUM=783b6f2df8ff02b19bb5ce492b99c8ff

RUN wget -qO sdl2.tar.gz https://www.libsdl.org/release/SDL2-${SDL2_VERSION}.tar.gz \
    && echo "${SDL2_MD5SUM} sdl2.tar.gz" > sdl2.md5 \
    && md5sum --status -c < sdl2.md5 \
    && tar -xf sdl2.tar.gz \
    && mkdir -p SDL2-${SDL2_VERSION}/build \
    && cd SDL2-${SDL2_VERSION}/build \
    && cmake -DBUILD_SHARED_LIBS=ON .. \
    && make -l install \
    && rm -rf /tmp/sdl2.tar.gz /tmp/sdl2.md5 /tmp/SDL2-${SDL2_VERSION}

ARG OPENAL_VERSION=1.19.0
ARG OPENAL_MD5SUM=a98737cc8fe65eb9c91b82c719c6465f

RUN wget -qO openal.tar.gz https://github.com/kcat/openal-soft/archive/openal-soft-${OPENAL_VERSION}.tar.gz \
    && echo "${OPENAL_MD5SUM} openal.tar.gz" > openal.md5 \
    && md5sum --status -c < openal.md5 \
    && tar -xf openal.tar.gz \
    && mkdir -p openal-soft-openal-soft-${OPENAL_VERSION}/build \
    && cd openal-soft-openal-soft-${OPENAL_VERSION}/build \
    && cmake -DALSOFT_UTILS=OFF -DALSOFT_EXAMPLES=OFF -DALSOFT_TESTS=OFF .. \
    && make -l install \
    && rm -rf /tmp/openal.tar.gz /tmp/openal.md5 /tmp/openal-soft-openal-soft-${OPENAL_VERSION}

ARG FREETYPE_VERSION=2-10-0
ARG FREETYPE_MD5SUM=33a87b91db6334c02e5b2a6c98604aa8

RUN wget -qO freetype.tar.gz https://github.com/aseprite/freetype2/archive/VER-${FREETYPE_VERSION}.tar.gz \
    && echo "${FREETYPE_MD5SUM} freetype.tar.gz" > freetype.md5 \
    && md5sum --status -c < freetype.md5 \
    && tar -xf freetype.tar.gz \
    && mkdir -p freetype2-VER-${FREETYPE_VERSION}/build \
    && cd freetype2-VER-${FREETYPE_VERSION}/build \
    && cmake .. \
    && make -l install \
    && rm -rf /tmp/freetype.tar.gz /tmp/freetype.md5 /tmp/freetype2-VER-${FREETYPE_VERSION}
