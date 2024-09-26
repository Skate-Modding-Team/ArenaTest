// made by tuukkas 2023-24

#include "texturehandling.h"
#include "DDS.h"
#include <vector>
#include <cassert>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <unordered_map>
#include <bitset>

const std::unordered_map<uint32_t, DirectX::DDS_PIXELFORMAT> pixelFormatsPS3 = {
    {0x86, DirectX::DDSPF_DXT1},
    {0x87, DirectX::DDSPF_DXT3},
    {0x88, DirectX::DDSPF_DXT5},
    {0xA5, DirectX::DDSPF_A8R8G8B8},
    {0x81, DirectX::DDSPF_DXT1},
    // Add more entries as needed
};

void printBinary(DWORD value) {
    std::bitset<32> bits(value);
    std::cout << bits << std::endl;
}

unsigned int reverseBits(unsigned int value) {
    unsigned int result = 0;
    for (int i = 0; i < 32; ++i) {
        result = (result << 1) | (value & 1);
        value >>= 1;
    }
    return result;
}

unsigned int reverseBytes(unsigned int value) {
    return ((value & 0xFF) << 24) | (((value >> 8) & 0xFF) << 16) | (((value >> 16) & 0xFF) << 8) | ((value >> 24) & 0xFF);
}

void reverseBytesInArray(unsigned char* array, size_t size) {
    for (size_t i = 0; i < size; i += 4) {
        uint32_t* value = reinterpret_cast<uint32_t*>(&array[i]);
        *value = reverseBytes(*value);
        *value = reverseBits(*value);
    }
}

void printBinaryArray(unsigned char* array, size_t size) {
    for (size_t i = 0; i < size; i += 4) {
        uint32_t* value = reinterpret_cast<uint32_t*>(&array[i]);
        printBinary(*value);
    }
}

int breverse(int num, int numBits) {
    int result = 0;
    for (int i = 0; i < numBits; ++i) {
        result = (result << 1) | (num & 1);
        num >>= 1;
    }
    return result;
}

int extractDwordValue(DWORD dwordValue, int size, int offset) {
    return (dwordValue >> offset) & ((1 << size) - 1);
}

int extractValue(unsigned char* GPUTEXTURE_FETCH_CONSTANT, int size, int offset) {
    DWORD dwordValue = *(reinterpret_cast<const DWORD*>(GPUTEXTURE_FETCH_CONSTANT));
    return breverse(extractDwordValue(dwordValue, size, offset), size);
}

DWORD XGAddress2DTiledOffset(DWORD x, DWORD y, DWORD w, DWORD texelPitch)
{
    DWORD alignedWidth = (w + 31) & ~31;
    DWORD logBpp = (texelPitch >> 2) + ((texelPitch >> 1) >> (texelPitch >> 2));
    DWORD Macro = (((x >> 5) + (y >> 5) * (alignedWidth >> 5)) << (logBpp + 7));
    DWORD Micro = (((x & 7) + ((y & 6) << 2)) << logBpp);
    DWORD Offset = Macro + ((Micro & ~15) << 1) + (Micro & 15) + ((y & 8) << (3 + logBpp)) + ((y & 1) << 4);

    return ((((Offset & ~511) << 3) + ((Offset & 448) << 2) + (Offset & 63) + ((y & 16) << 7) + (((((y & 8) >> 2) + (x >> 3)) & 3) << 6)) >> logBpp);
}

std::string GetGPUENDIAN(DWORD dwEndian)
{
    switch (dwEndian)
    {
    case 0:
        return "GPUENDIAN_NONE";
    case 1:
        return "GPUENDIAN_8IN16";
    case 2:
        return "GPUENDIAN_8IN32";
    case 3:
        return "GPUENDIAN_16IN32";
    default:
        return "-invalid-endian-";
    }
}

DirectX::DDS_PIXELFORMAT GetDDSTEXTUREFORMAT(DWORD dwTextureType)
{
    switch (dwTextureType)
    {
    case 0: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_1_REVERSE";
    case 1: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_1";
    case 2: return DirectX::DDSPF_A8;
    case 3: return DirectX::DDSPF_A1R5G5B5;
    case 4: return DirectX::DDSPF_R5G6B5;
    case 5: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_6_5_5";
    case 6: return DirectX::DDSPF_A8R8G8B8;
    case 7: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_2_10_10_10";
    case 8: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_8_A";
    case 9: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_8_B";
    case 10: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_8_8";
    case 11: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_Cr_Y1_Cb_Y0_REP";
    case 12: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_Y1_Cr_Y0_Cb_REP";
    case 13: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_16_16_EDRAM";
    case 14: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_8_8_8_8_A";
    case 15: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_4_4_4_4";
    case 16: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_10_11_11";
    case 17: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_11_11_10";
    case 18: return DirectX::DDSPF_DXT1;
    case 19: return DirectX::DDSPF_DXT3;
    case 20: return DirectX::DDSPF_DXT5;
    case 21: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_16_16_16_16_EDRAM";
    case 22: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_24_8";
    case 23: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_24_8_FLOAT";
    case 24: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_16";
    case 25: return DirectX::DDSPF_G16R16; //16_16
    case 26: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_16_16_16_16";
    case 27: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_16_EXPAND";
    case 28: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_16_16_EXPAND";
    case 29: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_16_16_16_16_EXPAND";
    case 30: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_16_FLOAT";
    case 31: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_16_16_FLOAT";
    case 32: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_16_16_16_16_FLOAT";
    case 33: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_32";
    case 34: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_32_32";
    case 35: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_32_32_32_32";
    case 36: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_32_FLOAT";
    case 37: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_32_32_FLOAT";
    case 38: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_32_32_32_32_FLOAT";
    case 39: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_32_AS_8";
    case 40: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_32_AS_8_8";
    case 41: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_16_MPEG";
    case 42: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_16_16_MPEG";
    case 43: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_8_INTERLACED";
    case 44: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_32_AS_8_INTERLACED";
    case 45: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_32_AS_8_8_INTERLACED";
    case 46: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_16_INTERLACED";
    case 47: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_16_MPEG_INTERLACED";
    case 48: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_16_16_MPEG_INTERLACED";
    case 49: return DirectX::DDSPF_BC5_SNORM; //DXN
    case 50: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_8_8_8_8_AS_16_16_16_16";
    case 51: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_DXT1_AS_16_16_16_16";
    case 52: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_DXT2_3_AS_16_16_16_16";
    case 53: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_DXT4_5_AS_16_16_16_16";
    case 54: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_2_10_10_10_AS_16_16_16_16";
    case 55: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_10_11_11_AS_16_16_16_16";
    case 56: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_11_11_10_AS_16_16_16_16";
    case 57: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_32_32_32_FLOAT";
    case 58: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_DXT3A";
    case 59: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_DXT5A";
    case 60: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_CTX1";
    case 61: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_DXT3A_AS_1_1_1_1";
    case 62: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_8_8_8_8_GAMMA_EDRAM";
    case 63: return DirectX::DDSPF_A8R8G8B8; //"GPUTEXTUREFORMAT_2_10_10_10_FLOAT_EDRAM";
    default: return DirectX::DDSPF_A8R8G8B8; //"-unknown-";
    ;}
}

std::string GetGPUTEXTUREFORMAT(DWORD dwTextureType)
{
    switch (dwTextureType)
    {
    case 0: return "GPUTEXTUREFORMAT_1_REVERSE";
    case 1: return "GPUTEXTUREFORMAT_1";
    case 2: return "GPUTEXTUREFORMAT_8";
    case 3: return "GPUTEXTUREFORMAT_1_5_5_5";
    case 4: return "GPUTEXTUREFORMAT_5_6_5";
    case 5: return "GPUTEXTUREFORMAT_6_5_5";
    case 6: return "GPUTEXTUREFORMAT_8_8_8_8";
    case 7: return "GPUTEXTUREFORMAT_2_10_10_10";
    case 8: return "GPUTEXTUREFORMAT_8_A";
    case 9: return "GPUTEXTUREFORMAT_8_B";
    case 10: return "GPUTEXTUREFORMAT_8_8";
    case 11: return "GPUTEXTUREFORMAT_Cr_Y1_Cb_Y0_REP";
    case 12: return "GPUTEXTUREFORMAT_Y1_Cr_Y0_Cb_REP";
    case 13: return "GPUTEXTUREFORMAT_16_16_EDRAM";
    case 14: return "GPUTEXTUREFORMAT_8_8_8_8_A";
    case 15: return "GPUTEXTUREFORMAT_4_4_4_4";
    case 16: return "GPUTEXTUREFORMAT_10_11_11";
    case 17: return "GPUTEXTUREFORMAT_11_11_10";
    case 18: return "GPUTEXTUREFORMAT_DXT1";
    case 19: return "GPUTEXTUREFORMAT_DXT2_3";
    case 20: return "GPUTEXTUREFORMAT_DXT4_5";
    case 21: return "GPUTEXTUREFORMAT_16_16_16_16_EDRAM";
    case 22: return "GPUTEXTUREFORMAT_24_8";
    case 23: return "GPUTEXTUREFORMAT_24_8_FLOAT";
    case 24: return "GPUTEXTUREFORMAT_16";
    case 25: return "GPUTEXTUREFORMAT_16_16";
    case 26: return "GPUTEXTUREFORMAT_16_16_16_16";
    case 27: return "GPUTEXTUREFORMAT_16_EXPAND";
    case 28: return "GPUTEXTUREFORMAT_16_16_EXPAND";
    case 29: return "GPUTEXTUREFORMAT_16_16_16_16_EXPAND";
    case 30: return "GPUTEXTUREFORMAT_16_FLOAT";
    case 31: return "GPUTEXTUREFORMAT_16_16_FLOAT";
    case 32: return "GPUTEXTUREFORMAT_16_16_16_16_FLOAT";
    case 33: return "GPUTEXTUREFORMAT_32";
    case 34: return "GPUTEXTUREFORMAT_32_32";
    case 35: return "GPUTEXTUREFORMAT_32_32_32_32";
    case 36: return "GPUTEXTUREFORMAT_32_FLOAT";
    case 37: return "GPUTEXTUREFORMAT_32_32_FLOAT";
    case 38: return "GPUTEXTUREFORMAT_32_32_32_32_FLOAT";
    case 39: return "GPUTEXTUREFORMAT_32_AS_8";
    case 40: return "GPUTEXTUREFORMAT_32_AS_8_8";
    case 41: return "GPUTEXTUREFORMAT_16_MPEG";
    case 42: return "GPUTEXTUREFORMAT_16_16_MPEG";
    case 43: return "GPUTEXTUREFORMAT_8_INTERLACED";
    case 44: return "GPUTEXTUREFORMAT_32_AS_8_INTERLACED";
    case 45: return "GPUTEXTUREFORMAT_32_AS_8_8_INTERLACED";
    case 46: return "GPUTEXTUREFORMAT_16_INTERLACED";
    case 47: return "GPUTEXTUREFORMAT_16_MPEG_INTERLACED";
    case 48: return "GPUTEXTUREFORMAT_16_16_MPEG_INTERLACED";
    case 49: return "GPUTEXTUREFORMAT_DXN";
    case 50: return "GPUTEXTUREFORMAT_8_8_8_8_AS_16_16_16_16";
    case 51: return "GPUTEXTUREFORMAT_DXT1_AS_16_16_16_16";
    case 52: return "GPUTEXTUREFORMAT_DXT2_3_AS_16_16_16_16";
    case 53: return "GPUTEXTUREFORMAT_DXT4_5_AS_16_16_16_16";
    case 54: return "GPUTEXTUREFORMAT_2_10_10_10_AS_16_16_16_16";
    case 55: return "GPUTEXTUREFORMAT_10_11_11_AS_16_16_16_16";
    case 56: return "GPUTEXTUREFORMAT_11_11_10_AS_16_16_16_16";
    case 57: return "GPUTEXTUREFORMAT_32_32_32_FLOAT";
    case 58: return "GPUTEXTUREFORMAT_DXT3A";
    case 59: return "GPUTEXTUREFORMAT_DXT5A";
    case 60: return "GPUTEXTUREFORMAT_CTX1";
    case 61: return "GPUTEXTUREFORMAT_DXT3A_AS_1_1_1_1";
    case 62: return "GPUTEXTUREFORMAT_8_8_8_8_GAMMA_EDRAM";
    case 63: return "GPUTEXTUREFORMAT_2_10_10_10_FLOAT_EDRAM";
    default: return "-unknown-";
    }
}

void swapEndianArray(std::vector<unsigned char>& data, size_t elementSize)
{
    size_t dataSize = data.size();
    if (dataSize % elementSize != 0)
        return;

    size_t numElements = dataSize / elementSize;
    unsigned char* pData = data.data();
    for (size_t i = 0; i < numElements; i++)
    {
        unsigned char* pElement = pData + i * elementSize;
        for (size_t j = 0; j < elementSize / 2; j++)
        {
            std::swap(pElement[j], pElement[elementSize - 1 - j]);
        }
    }
}

template<class T> inline T Align(const T ptr, int alignment)
{
    return (T)(((size_t)ptr + alignment - 1) & ~(alignment - 1));
}

inline int appLog2(int n)
{
    int r;
    for (r = -1; n; n >>= 1, r++)
    { /*empty*/
    }
    return r;
}

static unsigned GetXbox360TiledOffset(int x, int y, int width, int logBpb)
{
    assert(width <= 8192);

    int alignedWidth = Align(width, 32);
    // top bits of coordinates
    int macro = ((x >> 5) + (y >> 5) * (alignedWidth >> 5)) << (logBpb + 7);
    // lower bits of coordinates (result is 6-bit value)
    int micro = ((x & 7) + ((y & 0xE) << 2)) << logBpb;
    // mix micro/macro + add few remaining x/y bits
    int offset = macro + ((micro & ~0xF) << 1) + (micro & 0xF) + ((y & 1) << 4);
    // mix bits again
    return (((offset & ~0x1FF) << 3) +				// upper bits (offset bits [*-9])
        ((y & 16) << 7) +							// next 1 bit
        ((offset & 0x1C0) << 2) +					// next 3 bits (offset bits [8-6])
        (((((y & 8) >> 2) + (x >> 3)) & 3) << 6) +	// next 2 bits
        (offset & 0x3F)								// lower 6 bits (offset bits [5-0])
        ) >> logBpb;
}

std::vector<uint8_t> UntileCompressedXbox360Texture(const std::vector<uint8_t>& src,
    int tiledBlockWidth, int originalBlockWidth,
    int tiledBlockHeight, int originalBlockHeight, int texelPitch, int sxOffset, int syOffset) {

    std::vector<uint8_t> dst(static_cast<size_t>(originalBlockHeight) * static_cast<size_t>(originalBlockWidth) + static_cast<size_t>(originalBlockWidth) * static_cast<size_t>(texelPitch) * 2);
    const int logBpp = appLog2(texelPitch);
    const int numImageBlocks = static_cast<size_t>(tiledBlockWidth * tiledBlockHeight);

    for (int dy = 0; dy < originalBlockHeight; dy++) {
        for (int dx = 0; dx < originalBlockWidth; dx++) {
            unsigned swzAddr = GetXbox360TiledOffset(dx + sxOffset, dy + syOffset, tiledBlockWidth, logBpp);
            
            int sy = swzAddr / tiledBlockWidth;
            int sx = swzAddr % tiledBlockWidth;

            size_t srcIndex = (static_cast<size_t>(sy) * static_cast<size_t>(tiledBlockWidth) + static_cast<size_t>(sx)) * static_cast<size_t>(texelPitch);
            size_t dstIndex = (static_cast<size_t>(dy) * static_cast<size_t>(originalBlockWidth) + static_cast<size_t>(dx)) * static_cast<size_t>(texelPitch);

            // Emplace back the elements into the vector
            dst.insert(dst.begin() + dstIndex, src.begin() + srcIndex, src.begin() + srcIndex + texelPitch);
        }
    }

    return dst;
}

bool readDDS(const std::string& filename, std::vector<uint8_t>& textureArray, DirectX::DDS_HEADER& header) {
    std::ifstream infile(filename, std::ios::in | std::ios::binary);

    if (!infile.is_open()) {
        // cannot open
    }
    char magic[4];
    infile.read(magic, 4);
    if (strncmp(magic, "DDS ", 4) != 0) {
        // not dds file
    }

    // Read DDS header
    infile.read(reinterpret_cast<char*>(&header), sizeof(DirectX::DDS_HEADER));

    // Validate DDS header and perform additional checks if necessary
    // ...

    // Read texture data
    textureArray.resize(header.size - sizeof(DirectX::DDS_HEADER));
    infile.read(reinterpret_cast<char*>(textureArray.data()), textureArray.size());

    infile.close();
    return true;
}

void writeDDS(std::string filename, std::vector<uint8_t> texturearray, int width, int height, int mipMapLevels, DirectX::DDS_PIXELFORMAT pixelFormat)
{
    // Define DDS header using DDS.h structures
    DirectX::DDS_HEADER header{};
    header.size = sizeof(DirectX::DDS_HEADER);
    header.flags = DDS_HEADER_FLAGS_TEXTURE | DDS_HEADER_FLAGS_MIPMAP;
    header.height = static_cast<uint32_t>(height);
    header.width = static_cast<uint32_t>(width);
    header.pitchOrLinearSize = 0;
    header.depth = 1;
    header.mipMapCount = static_cast<uint32_t>(mipMapLevels);
    header.ddspf = pixelFormat;
    header.caps = 0x00401008; //enable mips

    // Fix the name
    filename = filename.substr(0, filename.find('\0'));

    // Create directory to save the files
    std::string dirPath = "DDS";
    DIR* dir = opendir(dirPath.c_str());
    if (dir) {
        closedir(dir);
    }
    else if (ENOENT == errno) {
        CreateDirectoryA(dirPath.c_str(), NULL);
    }

    // Check if filename contains backslashes
    size_t pos = filename.find_first_of('\\');
    if (pos != std::string::npos) {
        std::string remainingPath = filename;

        // Iterate over each segment of the path, separated by backslashes
        while (pos != std::string::npos) {
            // Extract the current directory name
            std::string dirName = remainingPath.substr(0, pos);

            // Append the current directory to the directory path
            dirPath += "\\" + dirName;

            // Create the directory if it does not exist
            if (!CreateDirectoryA(dirPath.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
                std::cerr << "Failed to create directory: " << GetLastError() << std::endl;
                return;
            }

            // Update the remaining path and find the next backslash
            remainingPath = remainingPath.substr(pos + 1);
            pos = remainingPath.find_first_of('\\');
        }
    }

    // Write DDS file
    std::string fullFilePath = "DDS/" + filename + ".dds";
    std::ofstream outfile(fullFilePath, std::ios::out | std::ios::binary);

    // Write DDS magic number
    outfile.write("DDS ", 4);

    // Write DDS header
    outfile.write(reinterpret_cast<const char*>(&header), sizeof(DirectX::DDS_HEADER));

    // Write texture data
    outfile.write(reinterpret_cast<const char*>(texturearray.data()), texturearray.size());

    outfile.close();
}

std::vector<uint8_t> UnswizzleCMPR(const std::vector<uint8_t>& data, int width, int height) {
    const int Tile_Width = 2;
    const int Tile_Height = 2;
    const int DxtBlock_Width = 4;
    const int DxtBlockSize = 8;

    int numBlockWidth = width / 8;
    int numBlockHeight = height / 8;

    int tileSize = Tile_Width * Tile_Height * DxtBlockSize;
    int lineSize = Tile_Width * DxtBlockSize;

    std::vector<uint8_t> untileData(data.size());

    for (int y = 0; y < numBlockHeight; ++y) {
        for (int x = 0; x < numBlockWidth; ++x) {
            int dataPtr = (y * numBlockWidth + x) * tileSize;
            for (int ty = 0; ty < Tile_Height; ++ty) {
                int curHeight = y * Tile_Height + ty;
                int dstIndex = (curHeight * numBlockWidth + x) * lineSize;
                int srcIndex = dataPtr + ty * lineSize;
                for (int p = 0; p < Tile_Width; ++p) {
                    untileData[dstIndex + p * DxtBlockSize] = data[srcIndex + p * DxtBlockSize + 1];
                    untileData[dstIndex + p * DxtBlockSize + 1] = data[srcIndex + p * DxtBlockSize];
                    untileData[dstIndex + p * DxtBlockSize + 2] = data[srcIndex + p * DxtBlockSize + 3];
                    untileData[dstIndex + p * DxtBlockSize + 3] = data[srcIndex + p * DxtBlockSize + 2];
                    for (int i = 4; i < 8; ++i) {
                        int index = data[srcIndex + p * DxtBlockSize + i];
                        int swapIndex = (((index >> 6) & 0x3) | (((index >> 4) & 0x3) << 2) | (((index >> 2) & 0x3) << 4) | ((index & 0x3) << 6));
                        untileData[dstIndex + p * DxtBlockSize + i] = swapIndex;
                    }
                }
            }
        }
    }

    return untileData;
}

void untile_xbox_textures_and_write_to_DDS(std::string filename, std::vector<uint8_t> src, int width, int height, int mipMapLevels, int PackedMips, int format, int test) {
    //untiles mips. :)
    DirectX::DDS_PIXELFORMAT pixelFormat{};
    std::string gpuFormat = GetGPUTEXTUREFORMAT(format);
    pixelFormat = GetDDSTEXTUREFORMAT(format);
    std::vector<uint8_t> dst;
    int blockSize = 0;
    int texelPitch = 0;
    int mipWidth = width;
    int mipHeight = height;
    int mipLevelOffset = 0;
    int tiledWidth = 0;
    int tiledHeight = 0;
    int initialmipSize;
    int tiledmipSize;
    int mipSize = 0;
    int syOffset = 0;
    bool firsttime = true;
    bool compressed = true;
    if (gpuFormat == "GPUTEXTUREFORMAT_DXT1") {//DXT1
        blockSize = 4;
        swapEndianArray(src, 2);
        texelPitch = 8;
    }
    else if (gpuFormat == "GPUTEXTUREFORMAT_DXT2_3" or gpuFormat == "GPUTEXTUREFORMAT_DXT4_5" or gpuFormat == "GPUTEXTUREFORMAT_DXN") {//DXT3, DXT5, DXN, 
        blockSize = 4;
        swapEndianArray(src, 2);
        texelPitch = 16;
    }
    else if (gpuFormat == "GPUTEXTUREFORMAT_8_8_8_8") {//A8R8G8B8
        blockSize = 1;
        compressed = false;
        swapEndianArray(src, 4);
        texelPitch = 4;
    }
    else if (gpuFormat == "GPUTEXTUREFORMAT_5_6_5") {//R5G6B5
        blockSize = 1;
        compressed = false;
        swapEndianArray(src, 2);
        texelPitch = 2;
    }
    else if (gpuFormat == "GPUTEXTUREFORMAT_8") {//A8
        blockSize = 1;
        compressed = false;
        texelPitch = 1;
    }
    int chunksize = 4096 * texelPitch / blockSize;
    int smallestmipsize;
    if (compressed) {
        smallestmipsize = 256 * texelPitch / blockSize;
    }
    else {
        if ((height / width >= 4 or width / height >= 4) and gpuFormat == "GPUTEXTUREFORMAT_8_8_8_8") { //this kind of works, but calculate better by aligning to 4096 and using width, height, texelPitch.
            smallestmipsize = 8192;
        }
        else {
            smallestmipsize = 4096;
        }
    }
    std::cout << filename << "\n";
    std::cout << "test: " << test << " pitch: " << texelPitch << " smm: " << smallestmipsize << "\n";
    std::cout << "w: " << width << " h: " << height << " f: " << format << " " << gpuFormat << " m: " << mipMapLevels << "\n";
    if (PackedMips != 0 and mipMapLevels != 0) {
        for (int level = 0; level < mipMapLevels; level++) {
            int sxOffset = 0;
            if (compressed) {
                tiledWidth = Align(mipWidth, 128);
                tiledHeight = Align(mipHeight, 128);
                initialmipSize = max(1, ((mipWidth + 3) / 4)) * max(1, ((mipHeight + 3) / 4)) * texelPitch;
                tiledmipSize = max(1, ((tiledWidth + 3) / 4)) * max(1, ((tiledHeight + 3) / 4)) * texelPitch;
            }
            else {
                tiledWidth = mipWidth;
                tiledHeight = mipHeight;
                initialmipSize = max(1, mipWidth) * max(1, mipHeight) * texelPitch;
                tiledmipSize = Align(initialmipSize, smallestmipsize);
            }
            int endoffsetforsrc = mipLevelOffset + tiledmipSize;

            std::vector<uint8_t> mipsrc(src.begin() + mipLevelOffset, src.begin() + endoffsetforsrc);

            int tiledBlockWidth = tiledWidth / blockSize;
            int originalBlockWidth = max(1, mipWidth / blockSize);
            int tiledBlockHeight = tiledHeight / blockSize;
            int originalBlockHeight = max(1, mipHeight / blockSize);

            if (initialmipSize < (32 * 64 * texelPitch) and mipMapLevels > 1) {
                //processing smallest mips
                if (mipWidth <= 16 or mipHeight <= 16) {
                    if (width > height) {
                        // 16 * width/height * 16
                        //example of this type of scenario
                        //sxOffset
                        //    4   8       16              32
                        //    #   ##      ####            ########                         syOffset
                        //                                ########                        
                        //                                                                
                        //                                                                
                        //################                                                 4
                        //################                                                
                        //################                                                
                        //################                                                
                        //################################                                 8
                        //################################                                
                        //################################                                
                        //################################                                
                        //################################                                
                        //################################                                
                        //################################                                
                        //################################
                        //                                 
                        //################################################################ 16\/
                        if (mipHeight > 2) {
                            syOffset = originalBlockHeight;
                        }
                        else {
                            syOffset = 0;
                            sxOffset = 4 * mipWidth / blockSize;
                        }
                    }
                    else {
                        //example of this type of scenario
                        //    4	  8	      >16
                        //    ############ ################
                        //    ############ ################
                        //    ############ ################
                        //    ############ ################
                        //#       ######## ################ 4
                        //        ######## ################
                        //        ######## ################
                        //        ######## ################
                        //##               ################ 8
                        //##               ################
                        //                 ################
                        //                 ################
                        //                 ################
                        //                 ################
                        //                 ################
                        //                 ################
                        if (mipWidth > 2) {
                            sxOffset = originalBlockWidth;
                        }
                        else {
                            sxOffset = 0;
                            syOffset = 4 * mipHeight / blockSize;
                        }
                    }
                }
            }

            std::vector<uint8_t> mipdst = UntileCompressedXbox360Texture(mipsrc, tiledBlockWidth, originalBlockWidth, tiledBlockHeight,
                originalBlockHeight, texelPitch, sxOffset, syOffset);
            if (mipdst.size() < 0x80) {
                mipdst.resize(0x80);
            }

            dst.insert(dst.end(), mipdst.begin(), mipdst.begin() + initialmipSize);
            mipdst.clear();

            mipWidth /= 2;
            mipHeight /= 2;
            mipWidth = max(1, mipWidth);
            mipHeight = max(1, mipHeight);

            if (src.size() > mipLevelOffset + tiledmipSize) {
                mipLevelOffset += tiledmipSize;
            }

            mipsrc.clear();
        }
    }
    else {
        int sxOffset = 0;
        if (compressed) {
            tiledWidth = Align(mipWidth, 128);
            tiledHeight = Align(mipHeight, 128);
            initialmipSize = max(1, ((mipWidth + 3) / 4)) * max(1, ((mipHeight + 3) / 4)) * texelPitch;
            tiledmipSize = max(1, ((tiledWidth + 3) / 4)) * max(1, ((tiledHeight + 3) / 4)) * texelPitch;
        }
        else {
            tiledWidth = mipWidth;
            tiledHeight = mipHeight;
            initialmipSize = max(1, mipWidth) * max(1, mipHeight) * texelPitch;
            tiledmipSize = Align(initialmipSize, smallestmipsize);
        }
        int endoffsetforsrc = mipLevelOffset + tiledmipSize;

        std::vector<uint8_t> mipsrc(src.begin() + mipLevelOffset, src.begin() + endoffsetforsrc);

        int tiledBlockWidth = tiledWidth / blockSize;
        int originalBlockWidth = mipWidth / blockSize;
        int tiledBlockHeight = tiledHeight / blockSize;
        int originalBlockHeight = mipHeight / blockSize;

        std::vector<uint8_t> mipdst = UntileCompressedXbox360Texture(mipsrc, tiledBlockWidth, originalBlockWidth, tiledBlockHeight,
            originalBlockHeight, texelPitch, sxOffset, syOffset);
        if (mipdst.size() < 0x80) {
            mipdst.resize(0x80);
        }

        dst.insert(dst.end(), mipdst.begin(), mipdst.begin() + initialmipSize);
        mipdst.clear();
        mipsrc.clear();
    }

    writeDDS(filename, dst, width, height, mipMapLevels, pixelFormat);
    dst.clear();
}

void getTextureInformationAndUntile(std::string name, std::vector<uint8_t>& buffer, unsigned char* GPUTEXTURE, int arraySize) {
    reverseBytesInArray(GPUTEXTURE, arraySize);

    // DWORD 0
    bool Tiled = extractValue(GPUTEXTURE, 1, 0);
    int Pitch = extractValue(GPUTEXTURE, 9, 1);
    bool Padding = extractValue(GPUTEXTURE, 1, 10);
    int MultiSample = extractValue(GPUTEXTURE, 2, 11);
    int ClampZ = extractValue(GPUTEXTURE, 3, 13);
    int ClampY = extractValue(GPUTEXTURE, 3, 16);
    int ClampX = extractValue(GPUTEXTURE, 3, 19);
    int SignW = extractValue(GPUTEXTURE, 2, 22);
    int SignZ = extractValue(GPUTEXTURE, 2, 24);
    int SignY = extractValue(GPUTEXTURE, 2, 26);
    int SignX = extractValue(GPUTEXTURE, 2, 28);
    int Type = extractValue(GPUTEXTURE, 2, 30);

    // DWORD 1
    int BaseAddress = extractValue(GPUTEXTURE + 4, 20, 0);
    bool ClampPolicy = extractValue(GPUTEXTURE + 4, 1, 20);
    bool Stacked = extractValue(GPUTEXTURE + 4, 1, 21);
    int RequestSize = extractValue(GPUTEXTURE + 4, 2, 22);
    int Endian = extractValue(GPUTEXTURE + 4, 2, 24);
    int DataFormat = extractValue(GPUTEXTURE + 4, 6, 26);

    // DWORD 2
    int Size = extractValue(GPUTEXTURE + 8, 32, 0);
    int Width = 0;
    int Height = 0;
    int Depth = 0;

    // DWORD 3
    int BorderSize = extractValue(GPUTEXTURE + 12, 1, 0);
    int PaddingDword3 = extractValue(GPUTEXTURE + 12, 3, 1);
    int AnisoFilter = extractValue(GPUTEXTURE + 12, 3, 4);
    int MipFilter = extractValue(GPUTEXTURE + 12, 2, 7);
    int MinFilter = extractValue(GPUTEXTURE + 12, 2, 9);
    int MagFilter = extractValue(GPUTEXTURE + 12, 2, 11);
    int ExpAdjust = extractValue(GPUTEXTURE + 12, 6, 13);
    int SwizzleW = extractValue(GPUTEXTURE + 12, 3, 16);
    int SwizzleZ = extractValue(GPUTEXTURE + 12, 3, 19);
    int SwizzleY = extractValue(GPUTEXTURE + 12, 3, 22);
    int SwizzleX = extractValue(GPUTEXTURE + 12, 3, 25);
    bool NumFormat = extractValue(GPUTEXTURE + 12, 1, 28);

    // DWORD 4
    int GradExpAdjustV = extractValue(GPUTEXTURE + 16, 5, 0);
    int GradExpAdjustH = extractValue(GPUTEXTURE + 16, 5, 5);
    int LODBias = extractValue(GPUTEXTURE + 16, 10, 10);
    int MinAnisoWalk = extractValue(GPUTEXTURE + 16, 1, 20);
    int MagAnisoWalk = extractValue(GPUTEXTURE + 16, 1, 21);
    int MaxMipLevel = extractValue(GPUTEXTURE + 16, 4, 22);
    int MinMipLevel = extractValue(GPUTEXTURE + 16, 4, 26);
    int VolMinFilter = extractValue(GPUTEXTURE + 16, 1, 30);
    int VolMagFilter = extractValue(GPUTEXTURE + 16, 1, 31);

    // DWORD 5
    int MipAddress = extractValue(GPUTEXTURE + 20, 20, 0);
    bool PackedMips = extractValue(GPUTEXTURE + 20, 1, 20);
    int Dimension = extractValue(GPUTEXTURE + 20, 2, 21);
    int AnisoBias = extractValue(GPUTEXTURE + 20, 4, 23);
    int TriClamp = extractValue(GPUTEXTURE + 20, 2, 27);
    bool ForceBCWtoMax = extractValue(GPUTEXTURE + 20, 1, 29);
    int BorderColor = extractValue(GPUTEXTURE + 20, 2, 30);

    if (Dimension == 1) { // GPUDIMENSION_2D, TwoD
        Width = extractDwordValue(Size, 13, 0) + 1;
        Height = extractDwordValue(Size, 13, 13) + 1;
    }
    else if (Dimension == 0) { // GPUDIMENSION_1D, OneD
        Width = extractDwordValue(Size, 24, 0) + 1;
    }
    else if (Dimension == 2) { // GPUDIMENSION_3D, ThreeD
        Width = extractDwordValue(Size, 11, 0) + 1;
        Height = extractDwordValue(Size, 11, 11) + 1;
        Depth = extractDwordValue(Size, 10, 21) + 1;
    }
    else if (Dimension == 3) { // GPUDIMENSION_CUBEMAP, Stack
        Width = extractDwordValue(Size, 13, 0) + 1;
        Height = extractDwordValue(Size, 13, 13) + 1;
        Depth = extractDwordValue(Size, 6, 19) + 1;
    }

    int mipMapLevels = MaxMipLevel - MinMipLevel;
    untile_xbox_textures_and_write_to_DDS(name, buffer, Width, Height, mipMapLevels, PackedMips, DataFormat, Pitch);
}

void write_ps3_textures_to_DDS(std::string filename, std::vector<uint8_t> buffer, int curwidth, int curheight, int mipMapLevels, int format) {
    DirectX::DDS_PIXELFORMAT pixelFormat{};
    auto ps3It = pixelFormatsPS3.find(format);
    if (ps3It != pixelFormatsPS3.end()) {
        pixelFormat = ps3It->second;
    }
    writeDDS(filename, buffer, curwidth, curheight, mipMapLevels, pixelFormat);
}

void unswizzle_wii_textures_and_write_to_DDS(std::string filename, const std::vector<uint8_t>& data, int width, int height, int mipMapLevels, DirectX::DDS_PIXELFORMAT pixelFormat) {
    std::vector<uint8_t> unswizzledData = UnswizzleCMPR(data, width, height);
    writeDDS(filename, unswizzledData, width, height, mipMapLevels, pixelFormat);
}
