#pragma once

#include <vector>
#include <string>
#include "DDS.h"

namespace DirectX {
    struct DDS_PIXELFORMAT;
}

template<class T> inline T Align(const T ptr, int alignment);

void getTextureInformationAndUntile(std::string name, std::vector<uint8_t>& buffer, unsigned char* GPUTEXTURE_FETCH_CONSTANT, int arraySize);

void swapEndianArray(std::vector<unsigned char>& data, size_t elementSize);

void untile_xbox_textures_and_write_to_DDS(std::string filename, std::vector<uint8_t> src, int width, int height, int mipMapLevels, int format);

void write_ps3_textures_to_DDS(std::string filename, std::vector<uint8_t> buffer, int curwidth, int curheight, int mipMapLevels, int format, uint32_t storeType);

void unswizzle_wii_textures_and_write_to_DDS(std::string filename, const std::vector<uint8_t>& data, int width, int height, int mipMapLevels, DirectX::DDS_PIXELFORMAT pixelFormat);