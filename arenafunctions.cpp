// made by tuukkas 2023-24

#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include "texturehandling.h"

namespace fs = std::filesystem;

std::unordered_map<uint32_t, std::string> elementTypes = {
    {0x00000000, "ELEMENTTYPE_NA"},
    {0x00000001, "ELEMENTTYPE_XYZ"},
    {0x00000002, "ELEMENTTYPE_XYZRHW"},
    {0x00000003, "ELEMENTTYPE_NORMAL"},
    {0x00000004, "ELEMENTTYPE_VERTEXCOLOR"},
    {0x00000005, "ELEMENTTYPE_PRELIT"},
    {0x00000006, "ELEMENTTYPE_TEX0"},
    {0x00000007, "ELEMENTTYPE_TEX1"},
    {0x00000008, "ELEMENTTYPE_TEX2"},
    {0x00000009, "ELEMENTTYPE_TEX3"},
    {0x0000000A, "ELEMENTTYPE_TEX4"},
    {0x0000000B, "ELEMENTTYPE_TEX5"},
    {0x0000000C, "ELEMENTTYPE_TEX6"},
    {0x0000000D, "ELEMENTTYPE_TEX7"},
    {0x0000000E, "ELEMENTTYPE_INDICES"},
    {0x0000000F, "ELEMENTTYPE_WEIGHTS"},
    {0x00000010, "ELEMENTTYPE_XYZ2"},
    {0x00000011, "ELEMENTTYPE_NORMAL2"},
    {0x00000012, "ELEMENTTYPE_XZ"},
    {0x00000013, "ELEMENTTYPE_Y"},
    {0x00000014, "ELEMENTTYPE_Y2"},
    {0x00000015, "ELEMENTTYPE_TANGENT"},
    {0x00000016, "ELEMENTTYPE_BINORMAL"},
    {0x00000017, "ELEMENTTYPE_SPECULAR"},
    {0x00000018, "ELEMENTTYPE_FOG"},
    {0x00000019, "ELEMENTTYPE_PSIZE"},
    {0x0000001A, "ELEMENTTYPE_INDICES2"},
    {0x0000001B, "ELEMENTTYPE_WEIGHTS2"},
    {0x0000001C, "ELEMENTTYPE_MAX"},
    {0x7FFFFFFF, "ELEMENTTYPE_NUM"},
    {0xFFFFFFFF, "VERTEXFORMAT_UNUSED"},
    {0x002C83A4, "VERTEXFORMAT_FLOAT1"},
    {0x002C23A5, "VERTEXFORMAT_FLOAT2"},
    {0x002A23B9, "VERTEXFORMAT_FLOAT3"},
    {0x001A23A6, "VERTEXFORMAT_FLOAT4"},
    {0x00182886, "VERTEXFORMAT_D3DCOLOR"},
    {0x001A2286, "VERTEXFORMAT_UBYTE4"},
    {0x001A2086, "VERTEXFORMAT_UBYTE4N"},
    {0x002C2359, "VERTEXFORMAT_SHORT2"},
    {0x001A235A, "VERTEXFORMAT_SHORT4"},
    {0x002C2159, "VERTEXFORMAT_SHORT2N"},
    {0x001A215A, "VERTEXFORMAT_SHORT4N"},
    {0x002C2059, "VERTEXFORMAT_USHORT2N"},
    {0x001A205A, "VERTEXFORMAT_USHORT4N"},
    {0x002A2287, "VERTEXFORMAT_UDEC3"},
    {0x002A2187, "VERTEXFORMAT_DEC3N"},
    {0x002A2190, "VERTEXFORMAT_HEND3N"},
    {0x002C235F, "VERTEXFORMAT_FLOAT16_2"},
    {0x001A2360, "VERTEXFORMAT_FLOAT16_4"},
};

std::unordered_map<uint32_t, std::string> objectTypes = {
    {0x00000000, "RWOBJECTTYPE_NULL"},
    {0x00010000, "RWOBJECTTYPE_NA"},
    {0x00010001, "RWOBJECTTYPE_ARENA"},
    {0x00010002, "RWOBJECTTYPE_RAW"},
    {0x00010003, "RWOBJECTTYPE_SUBREFERENCE"},
    {0x00010004, "RWOBJECTTYPE_SECTIONMANIFEST"},
    {0x00010005, "RWOBJECTTYPE_SECTIONTYPES"},
    {0x00010006, "RWOBJECTTYPE_SECTIONEXTERNALARENAS"},
    {0x00010007, "RWOBJECTTYPE_SECTIONSUBREFERENCES"},
    {0x00010008, "RWOBJECTTYPE_SECTIONATOMS"},
    {0x00010009, "RWOBJECTTYPE_DEFARENAIMPORTS"},
    {0x0001000A, "RWOBJECTTYPE_DEFARENAEXPORTS"},
    {0x0001000B, "RWOBJECTTYPE_DEFARENATYPES"},
    {0x0001000C, "RWOBJECTTYPE_DEFARENADEFINEDARENAID"},
    {0x0001000D, "RWOBJECTTYPE_ATTRIBUTEPACKET"},
    {0x0001000E, "RWOBJECTTYPE_ATTRIBUTEPACKET_DELEGATE"},
    {0x0001000F, "RWOBJECTTYPE_BITTABLE"},
    {0x00010010, "RWOBJECTTYPE_ARENALOCALATOMTABLE"},
    {0x00010030, "RWOBJECTTYPE_BASERESOURCE_START"},
    {0x00010031, "RWOBJECTTYPE_BASERESOURCE"},
    {0x00010032, "RWOBJECTTYPE_BASERESOURCE"},
    {0x00010033, "RWOBJECTTYPE_BASERESOURCE"},
    {0x00010034, "RWOBJECTTYPE_BASERESOURCE"},
    {0x00010035, "RWOBJECTTYPE_BASERESOURCE"},
    {0x00010036, "RWOBJECTTYPE_BASERESOURCE"},
    {0x00010037, "RWOBJECTTYPE_BASERESOURCE"},
    {0x00010038, "RWOBJECTTYPE_BASERESOURCE"},
    {0x00010039, "RWOBJECTTYPE_BASERESOURCE"},
    {0x0001003A, "RWOBJECTTYPE_BASERESOURCE"},
    {0x0001003B, "RWOBJECTTYPE_BASERESOURCE"},
    {0x0001003C, "RWOBJECTTYPE_BASERESOURCE"},
    {0x0001003D, "RWOBJECTTYPE_BASERESOURCE"},
    {0x0001003E, "RWOBJECTTYPE_BASERESOURCE"},
    {0x0001003F, "RWOBJECTTYPE_BASERESOURCE_RESERVEDTO"},
    {0x00020000, "RWGOBJECTTYPE_NA"},
    {0x00020001, "RWGOBJECTTYPE_CAMERA"},
    {0x00020002, "RWGOBJECTTYPE_PALETTE"},
    {0x00020003, "RWGOBJECTTYPE_RASTER"},
    {0x00020004, "RWGOBJECTTYPE_VDES"},
    {0x00020005, "RWGOBJECTTYPE_VBUF"},
    {0x00020006, "RWGOBJECTTYPE_IDES"},
    {0x00020007, "RWGOBJECTTYPE_IBUF"},
    {0x00020008, "RWGOBJECTTYPE_LIGHT"},
    {0x00020009, "RWGOBJECTTYPE_MESH"},
    {0x0002000A, "RWGOBJECTTYPE_SHADER"},
    {0x0002000B, "RWGOBJECTTYPE_COMPILEDSTATE"},
    {0x0002000C, "RWGOBJECTTYPE_RENDEROBJECT"},
    {0x0002000D, "RWGOBJECTTYPE_GSDATA"},
    {0x0002000E, "RWGOBJECTTYPE_VERTEXSHADER"},
    {0x0002000F, "RWGOBJECTTYPE_VERTEXDATA"},
    {0x00020010, "RWGOBJECTTYPE_INDEXDATA"},
    {0x00020011, "RWGOBJECTTYPE_RASTERDATA"},
    {0x00020012, "RWGOBJECTTYPE_BUILDSTATE"},
    {0x00020013, "RWGOBJECTTYPE_PIXELSHADER"},
    {0x00020015, "RWGOBJECTTYPE_PROFILEMESH"},
    {0x00020016, "RWGOBJECTTYPE_DESIGNVIEWOBJECT"},
    {0x00020017, "RWGOBJECTTYPE_PROFILERENDEROBJECT"},
    {0x00020018, "RWGOBJECTTYPE_IMAGE"},
    {0x00020019, "RWGOBJECTTYPE_RENDEROBJECTCONTAINER"},
    {0x0002001A, "RWGOBJECTTYPE_MESHCOMPILEDSTATELINK"},
    {0x0002001B, "RWGOBJECTTYPE_SHADERCODE"},
    {0x00020020, "RWGOBJECTTYPE_FONT"},
    {0x00020021, "RWGOBJECTTYPE_GLYPHTABLE"},
    {0x00020022, "RWGOBJECTTYPE_KERNTABLE"},
    {0x00020023, "RWGOBJECTTYPE_PAGETABLE"},
    {0x00020024, "RWGOBJECTTYPE_RASTERTEXTURE"},
    {0x00020025, "RWGOBJECTTYPE_FACENAME"},
    {0x0002007F, "RWGOBJECTTYPE_UPDATELOCATOR"},
    {0x00020081, "RWGOBJECTTYPE_MESHHELPER"},
    {0x000200E0, "RWGOBJECTTYPE_NA"},
    {0x000200E1, "RWGOBJECTTYPE_VIEWPORTPARAMETERS"},
    {0x000200E2, "RWGOBJECTTYPE_SCISSORRECTPARAMETERS"},
    {0x000200E3, "RWGOBJECTTYPE_CLEARCOLORPARAMETERS"},
    {0x000200E4, "RWGOBJECTTYPE_CLEARDEPTHSTENCILPARAMETERS"},
    {0x000200E5, "RWGOBJECTTYPE_DRAWINDEXEDPARAMETERS"},
    {0x000200E6, "RWGOBJECTTYPE_DRAWPARAMETERS"},
    {0x000200E7, "RWGOBJECTTYPE_PIXELBUFFER"},
    {0x000200E8, "RWGOBJECTTYPE_TEXTURE"},
    {0x000200E9, "RWGOBJECTTYPE_VERTEXDESCRIPTOR"},
    {0x000200EA, "RWGOBJECTTYPE_VERTEXBUFFER"},
    {0x000200EB, "RWGOBJECTTYPE_INDEXBUFFER"},
    {0x000200EC, "RWGOBJECTTYPE_PROGRAMBUFFER"},
    {0x000200ED, "RWGOBJECTTYPE_STATEBUFFER"},
    {0x000200EE, "RWGOBJECTTYPE_PROGRAMSTATEBUFFER" },
    {0x000200EF, "RWGOBJECTTYPE_PROGRAMSTATES" },
    {0x000200F1, "RWGOBJECTTYPE_SAMPLERSTATE" },
    {0x000200F3, "RWGOBJECTTYPE_RENDERTARGETSTATE" },
    {0x000200F4, "RWGOBJECTTYPE_BLENDSTATE" },
    {0x000200F5, "RWGOBJECTTYPE_DEPTHSTENCILSTATE" },
    {0x000200F6, "RWGOBJECTTYPE_RASTERIZERSTATE" },
    {0x000200F7, "RWGOBJECTTYPE_VERTEXPROGRAMSTATE" },
    {0x000200F8, "RWGOBJECTTYPE_PROGRAMSTATEHANDLE" },
    {0x000200F9, "RWGOBJECTTYPE_DRAWINSTANCEDPARAMETERS" },
    {0x000200FA, "RWGOBJECTTYPE_DRAWINDEXEDINSTANCEDPARAMETERS" },
    {0x00040000, "OBJECTTYPE_NA" },
    {0x00040001, "OBJECTTYPE_PART" },
    {0x00040002, "OBJECTTYPE_PARTDEFINITION" },
    {0x00040003, "OBJECTTYPE_JOINTEDPAIR" },
    {0x00040004, "OBJECTTYPE_JOINTLIMIT" },
    {0x00040005, "OBJECTTYPE_JOINTSKELETON" },
    {0x00040006, "OBJECTTYPE_ASSEMBLY" },
    {0x00040007, "OBJECTTYPE_ASSEMBLYDEFINITION" },
    {0x00070000, "OBJECTTYPE_NA" },
    {0x00070001, "OBJECTTYPE_KEYFRAMEANIM" },
    {0x00070002, "OBJECTTYPE_SKELETON" },
    {0x00070003, "OBJECTTYPE_ANIMATIONSKIN" },
    {0x00070004, "OBJECTTYPE_INTERPOLATOR" },
    {0x00070005, "OBJECTTYPE_FEATHERINTERPOLATOR" },
    {0x00070006, "OBJECTTYPE_ONEBONEIK" },
    {0x00070007, "OBJECTTYPE_TWOBONEIK" },
    {0x00070008, "OBJECTTYPE_BLENDER" },
    {0x00070009, "OBJECTTYPE_WEIGHTEDBLENDER" },
    {0x0007000A, "OBJECTTYPE_REMAPPER" },
    {0x0007000B, "OBJECTTYPE_SKELETONSINK" },
    {0x0007000C, "OBJECTTYPE_SKINSINK" },
    {0x0007000D, "OBJECTTYPE_LIGHTSINK" },
    {0x0007000E, "OBJECTTYPE_CAMERASINK" },
    {0x0007000F, "OBJECTTYPE_SKINMATRIXBUFFER" },
    {0x00070010, "OBJECTTYPE_TWEAKCONTROLLER" },
    {0x00070011, "OBJECTTYPE_SHADERSINK" },
    {0x00080000, "RWCOBJECTTYPE_NA" },
    {0x00080001, "RWCOBJECTTYPE_VOLUME" },
    {0x00080002, "RWCOBJECTTYPE_SIMPLEMAPPEDARRAY" },
    {0x00080003, "RWCOBJECTTYPE_TRIANGLEKDTREEPROCEDURAL" },
    {0x00080004, "RWCOBJECTTYPE_KDTREEMAPPEDARRAY" },
    {0x00080005, "RWCOBJECTTYPE_BBOX" },
    {0x00080006, "RWCOBJECTTYPE_CLUSTEREDMESH" },
    {0x00080008, "RWCOBJECTTYPE_OCTREE" },
    {0x00EB0000, "RWOBJECTTYPE_RENDERMESHDATA" },
    {0x00EB0001, "RWOBJECTTYPE_RENDERMODELDATA" },
    {0x00EB0003, "RWOBJECTTYPE_SIMPLETRIMESHDATA"},
    {0x00EB0004, "RWOBJECTTYPE_SPLINEDATA" },
    {0x00EB0005, "RWOBJECTTYPE_RENDERMATERIALDATA" },
    {0x00EB0006, "RWOBJECTTYPE_COLLISIONMATERIALDATA" },
    {0x00EB0007, "RWOBJECTTYPE_ROLLERDESCDATA" },
    {0x00EB0008, "RWOBJECTTYPE_VERSIONDATA" },
    {0x00EB0009, "RWOBJECTTYPE_LOCATIONDESCDATA" },
    {0x00EB000A, "RWOBJECTTYPE_COLLISIONMODELDATA" },
    {0x00EB000B, "RWOBJECTTYPE_TABLEOFCONTENTS" },
    {0x00EB000C, "RWOBJECTTYPE_COLLISIONBEZIERDATA" },
    {0x00EB000D, "RWOBJECTTYPE_INSTANCEDATA" },
    {0x00EB000E, "RWOBJECTTYPE_RENDERBLENDSHAPEEDATA" },
    {0x00EB000F, "RWOBJECTTYPE_WORLDPAINTERLAYERDATA" },
    {0x00EB0010, "RWOBJECTTYPE_WORLDPAINTERQUADTREEDATA" },
    {0x00EB0011, "RWOBJECTTYPE_WORLDPAINTERDICTIONARYDATA" },
    {0x00EB0012, "RWOBJECTTYPE_NAVMESHDATA" },
    {0x00EB0013, "RWOBJECTTYPE_RAINDATA" },
    {0x00EB0014, "RWOBJECTTYPE_AIPATHDATA" },
    {0x00EB0015, "RWOBJECTTYPE_STATSDATA" },
    {0x00EB0016, "RWOBJECTTYPE_MASSIVEDATA" },
    {0x00EB0017, "RWOBJECTTYPE_DEPTHMAPDATA" },
    {0x00EB0018, "RWOBJECTTYPE_LIONDATA" },
    {0x00EB0019, "RWOBJECTTYPE_TRIGGERINSTANCEDATA" },
    {0x00EB001A, "RWOBJECTTYPE_WAYPOINTDATA" },
    {0x00EB001B, "RWOBJECTTYPE_EMBEDDEDDATA" },
    {0x00EB001C, "RWOBJECTTYPE_EMITTERWAYPOINTDATA" },
    {0x00EB001D, "RWOBJECTTYPE_DMODATA" },
    {0x00EB001E, "RWOBJECTTYPE_HOTPOINTDATA" },
    {0x00EB001F, "RWOBJECTTYPE_GRABDATA" },
    {0x00EB0020, "RWOBJECTTYPE_SPATIALMAP" },
    {0x00EB0021, "RWOBJECTTYPE_VISUALINDICATORDATA" },
    {0x00EB0022, "RWOBJECTTYPE_NAVMESH2DATA" },
    {0x00EB0023, "RWOBJECTTYPE_RENDEROPTIMESHDATA" },
    {0x00EB0024, "RWOBJECTTYPE_IRRADIANCEDATA" },
    {0x00EB0025, "RWOBJECTTYPE_ANTIFRUSTUMDATA" },
    {0x00EB0026, "RWOBJECTTYPE_BLOBDATA" },
    {0x00EB0027, "RWOBJECTTYPE_NAVPOWERDATA" },
    {0x00EB0028, "RWOBJECTTYPE_TEAMBRANDDATA" },
    {0x00EB0029, "RWOBJECTTYPE_WORLDSCRIPTINSTANCEDATA" },
    {0x00EB0064, "RWOBJECTTYPE_SPLINESUBREF" },
    {0x00EB0065, "RWOBJECTTYPE_ROLLERDESCSUBREF" },
    {0x00EB0066, "RWOBJECTTYPE_RENDERMATERIALSUBREF" },
    {0x00EB0067, "RWOBJECTTYPE_COLLISIONMATERIALSUBREF" },
    {0x00EB0068, "RWOBJECTTYPE_LOCATIONDESCSUBREF" },
    {0x00EB0069, "RWOBJECTTYPE_INSTANCESUBREF" },
    {0x00EB006A, "RWOBJECTTYPE_WAYPOINTSUBREF" },
    {0x00EB006B, "RWOBJECTTYPE_TRIGGERINSTANCESUBREF" },
    {0x00EB006C, "RWOBJECTTYPE_EMITTERWAYPOINTSUBREF" },
    {0x00EB006D, "RWOBJECTTYPE_DMOSUBREF" },
    {0x00EB006E, "RWOBJECTTYPE_HOTPOINTSUBREF" },
    {0x00EB006F, "RWOBJECTTYPE_GRABSUBREF" },
    {0x00EB0070, "RWOBJECTTYPE_VISUALINDICATORSUBREF" },
    {0x00EC0010, "ARENADICTIONARY" },
    {0x7FFFFFFF, "RWCOBJECTTYPE_FORCENUMSIZEINT" }
};

struct Arena_Magic_Number
{
    uint32_t prefix;   //RW4
    uint32_t body;     //(platform specification: ps3 + 0x00, xb2 + 0x00, rev + 0x00, win + 0x00)
    uint32_t suffix;   //0x0D0A1A0A
};

struct Arena_Header {
    uint8_t  isBigEndian;
    uint8_t  pointerSizeInBits; //0x20 (32)
    uint8_t  pointerAlignment;  //0x04 (4)
    uint8_t  unused; 			//0x00 
    uint32_t majorVersion; 	    //0x34353400 (454 + 0x00)
    uint32_t minorVersion;	    //0x30303000
    uint32_t buildNo; 		    //0x00000000
    uint32_t id;
};

struct Arena {
    uint32_t numEntries;
    uint32_t numUsed;
    uint32_t alignment; //(16 or 32)
    uint32_t virt;
    uint32_t DictionaryStart; //offset
    uint32_t Arena_Section_Manifest; //offset
    uint32_t base; //offset
    uint32_t m_unfixContext; //offset
    uint32_t m_fixContext; //offset
};

struct BaseResourceDescriptor {
    uint32_t size;
    uint32_t alignment; // alignment will be 1 if unused
};

//resource descriptors (m_resourceDescriptor) (amount ps3 = 6, xbox = 5, wii = 4)

//resource descriptors (m_resourcesUsed) (amount ps3 = 7, xbox = 6, wii=5)

struct TargetResource {
    uint32_t m_baseresource; //offset
};

//Arena_Sections //Astart right after the above header or where defined by the section manifest offset in the header. Typically occur in the order listed
struct Manifest
{
    uint32_t typeID; 		//0x00010004
    uint32_t numEntries;	//usually 4 entries
    uint32_t dictionary; 	//offset from start of this section
};

struct Dictionary_Entry
{
    uint32_t offset; //offset to section from start of sections
};

struct Types
{
    uint32_t typeID; 		//0x00010005
    uint32_t numEntries;	//number of types
    uint32_t dictionary; 	//offset from start of this section
};

//entries typically start with a blank zero entry
struct Type_Entry
{
    uint32_t RW_Object_ID;
};

struct External_Arenas
{
    uint32_t typeID; 		//0x00010006
    uint32_t numEntries;
    uint32_t dictionary; 	//offset from start of this section
};

struct External_Arena
{
    uint32_t ID;            //this arena's id
    uint32_t unknown;       //0xFFB00000
    uint32_t ID1;           //this arena's id
};

struct External_Arena_Entry
{
    uint32_t unknown; //always zero
};

struct Subreferences
{
    uint32_t typeID; 				//0x00010007
    uint32_t numEntries;
    uint32_t m_dictAfterRefix; 	    //offset
    uint32_t m_recordsAfterRefix;   //offset
    uint32_t dictionary; 			//offset
    uint32_t records; 				//offset
    uint32_t numUsed; 				//offset
};

struct Atoms
{
    uint32_t typeID; 		//0x00010008
    uint32_t numEntries;	    //never seen this not be zero
    uint32_t Atom_Table; 	//never seen this not be zero
};

struct Arena_Subreference_Record
{
    uint32_t objectID;
    uint32_t offset; //offset within the item
};

// Model Management
struct ModelHeader
{
    uint32_t uiTypeCount;
    uint32_t uiItemsCount;
    uint32_t alignment; // 00 00 00 14
    uint32_t headerSize; //offset to the start of the data
    uint32_t pNames; //names start offset
};

struct ModelEntry
{
    uint32_t ItemsCount;
    uint32_t unknown; // 00 00 00 00
    uint32_t Offset;
};

struct IDEntry
{
    uint32_t nameStart;
    uint32_t index;
    uint64_t padding; // DE AD BE EF DE AD C0 DE
    uint64_t objectID;
    uint32_t nameEnd;
    uint32_t unknown2; // 00 00 00 00
};

// Collision Management
//TODO: everything

// Texture Management
struct TextureInformationPS3
{
    uint8_t format; //0x81, 0x82, 0x83, 0x84, 0x85, 0x86(DXT1), 0x87(DXT3), 0x88(DXT5), 0x8B, 0x8D, 0x8E, 0x8F, 0x91(bandw), 0x92, 0x93(gray), 0x94, 0x95, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E(A5 but swizzled?), 0x9F, 0xA1(gray), 0xA2, 0xA3, 0xA4, 0xA5(argb8888), A6, A7, A8
    uint8_t mipmap; //Number of mipmap levels (1-13)
    uint8_t dimension; //0x02 Texture dimension
    uint8_t cubemap; //Enable or disable cube mapping
    uint32_t remap; //Method of selecting each element of the color value
    uint16_t width; //Width of texture (1-4096)
    uint16_t height; //Height of texture (1-4096)
    uint16_t depth; //0x0001 Depth of texture (1-512)
    uint8_t location; //Location of texture data (main memory or local memory)
    uint8_t padding;
    uint32_t pitch; //pitch = floor((width+3)/4)*block_size     Where block_size is 8 for DXT1 and 16 for DXT2 through DXT5.
    uint32_t offset; //	Offset value from the base address of the texture data location
    uint32_t buffer;
    uint32_t storeType; //0x00000002, TYPE_NA = -1, TYPE_1D = 1, TYPE_2D = 2, TYPE_3D = 3, TYPE_CUBE = 0x10002, TYPE_FORCEENUMSIZEINT = 0x7FFFFFFF
    uint32_t storeFlags; //0x00000000 Seemingly unused
    uint16_t unknown; //seems to always be 0x5572
    uint8_t padding2; //00000000
    uint8_t format2; //format written another time.
};

struct TextureInformationX360
{
    uint32_t Common; //00 00 00 03
    uint32_t ReferenceCount; //00 00 00 01
    uint32_t Fence; //00 00 00 00
    uint32_t ReadFence; //00 00 00 00
    uint32_t Identifier; //00 00 00 00
    uint32_t BaseFlush; //FF FF 00 00
    uint32_t MipFlush; //FF FF 00 00
    unsigned char GPUTEXTURE_FETCH_CONSTANT[0x18];
};

struct TextureInformationX360old
{
    uint32_t resourcetype;      //0x00000003
    uint32_t referencecount;    //0x00000001
    uint32_t fence;             //0x00000000
    uint32_t readfence;         //0x00000000
    uint32_t identifier;        //0x00000000
    uint32_t baseflush;         //0xffff0000
    uint32_t mipflush;          //0xffff0000
    uint32_t unknown7;
    uint16_t unknown8;
    uint8_t unknown9;
    uint8_t format;
    uint8_t unknown10;
    uint8_t height;
    uint16_t width;
    uint32_t unknown11;
    uint32_t mips;
    uint32_t unknown12; //mips something?
};

struct TextureInformationWII
{
    uint32_t unknown;
    uint32_t unknown2;
    uint16_t width;
    uint16_t height;
    uint32_t format;
    uint32_t mips; //?
    uint32_t unknown6; //?
    uint32_t unknown7;
    uint32_t unknown8;
};

//Table of contents structure

struct Header
{
    uint32_t uiItemsCount;
    uint32_t pArray; //20 start offset
    uint32_t pNames; //names start offset
    uint32_t uiTypeCount; //0x00000000
    uint32_t pTypeMap; //end offset/header size
};

struct TOC_Entry
{
    uint32_t Name; //offset from names offset
    uint32_t unknown; //ps3 = 7C 0F 16 78, xbox&wii = 9B 0F 16 78 (???x)
    uint64_t uiGuid;
    uint32_t Type; //AC 46 2E 4A (¬F.J)
    uint32_t uiIndex; //index
};

//Name here, null terminated string.

struct dictionaryStart
{
    uint32_t unknown;
    uint32_t unknown2;
};

struct arenaDictionary
{
    uint32_t ptr;           //absolute offset in the file. if it is a base resource however, it is the offset to the resource from the start of the resources.
    uint32_t reloc;         //ive never seen this not be zero
    uint32_t size;          //data size for that object/size of the entry
    uint32_t alignment;     //alignment of the entry
    uint32_t typeIndex;     //This refers to the index of the type in the types section.
    uint32_t typeId; 	    //RW Object type ID
};

std::string getFileNameWithoutExtension(const std::string& filePath) {
    // Extracting just the filename without the path
    std::string fileName = fs::path(filePath).filename().string();

    // Removing the extension
    size_t lastDotPos = fileName.find_last_of('.');
    if (lastDotPos != std::string::npos) {
        fileName = fileName.substr(0, lastDotPos);
    }

    return fileName;
}

template <typename T>
T swapEndianness(T value) {
    constexpr size_t size = sizeof(T);
    T result = 0;

    for (size_t i = 0; i < size; ++i) {
        result |= (static_cast<T>((value >> (8 * i)) & 0xFF) << (8 * (size - 1 - i)));
    }

    return result;
}

void read_RW4_textures_and_write_to_DDS(std::string inputfile) {
    std::ifstream inputFile(inputfile, std::ios::binary);
    Arena_Magic_Number magicnumber{};

    std::string filename = getFileNameWithoutExtension(inputfile);
    inputFile.read(reinterpret_cast<char*>(&magicnumber), sizeof(magicnumber));

    uint32_t RW4 = swapEndianness(0x89525734);
    uint32_t xbox = swapEndianness(0x78623200);
    uint32_t ps3 = swapEndianness(0x70733300);
    uint32_t wii = swapEndianness(0x72657600);

    std::string platform;

    if (magicnumber.prefix == RW4) {
        if (magicnumber.body == xbox) {
            platform = "XBOX";
        }
        else if (magicnumber.body == ps3) {
            platform = "PS3";
        }
        else if (magicnumber.body == wii) {
            platform = "WII";
        }
        else {
            //file type not supported
        }
    }
    else {
        //file type not supported
    }

    Arena_Header arenaHeader{};
    inputFile.read(reinterpret_cast<char*>(&arenaHeader), sizeof(arenaHeader));

    if (arenaHeader.majorVersion == swapEndianness(0x34353400)) {
        if (arenaHeader.minorVersion == swapEndianness(0x30303000)) {
        }
    }

    Arena arena{};
    inputFile.read(reinterpret_cast<char*>(&arena), sizeof(arena));

    int amountOfResourceDescriptors = 0;
    int amountOfResources = 0;
    if (platform == "XBOX") {
        amountOfResourceDescriptors = 5;
        amountOfResources = 6;
    }
    else if (platform == "PS3") {
        amountOfResourceDescriptors = 6;
        amountOfResources = 7;
    }
    if (platform == "WII") {
        amountOfResourceDescriptors = 4;
        amountOfResources = 5;
    }

    std::vector<BaseResourceDescriptor> resourceDescriptors;
    for (int i = 0; i < amountOfResourceDescriptors; ++i) {
        BaseResourceDescriptor resource{};
        inputFile.read(reinterpret_cast<char*>(&resource), sizeof(resource));
        resourceDescriptors.push_back(resource);
    }

    std::vector<BaseResourceDescriptor> resourcesUsed;
    for (int i = 0; i < amountOfResourceDescriptors; ++i) {
        BaseResourceDescriptor resource2{};
        inputFile.read(reinterpret_cast<char*>(&resource2), sizeof(resource2));
        resourcesUsed.push_back(resource2);
    }

    std::vector<TargetResource> resources;
    for (int i = 0; i < amountOfResources; ++i) {
        TargetResource resource{};
        inputFile.read(reinterpret_cast<char*>(&resource), sizeof(resource));
        resources.push_back(resource);
    }

    Manifest manifest{};
    inputFile.read(reinterpret_cast<char*>(&manifest), sizeof(manifest));
    std::vector<Dictionary_Entry> dictEntries;

    for (int i = 0; i < swapEndianness(manifest.numEntries); ++i) {
        Dictionary_Entry entry{};
        inputFile.read(reinterpret_cast<char*>(&entry), sizeof(entry));
        dictEntries.push_back(entry);
    }

    Types type{};
    inputFile.read(reinterpret_cast<char*>(&type), sizeof(type));
    std::vector<Type_Entry> typeEntries;
    
    for (int i = 0; i < swapEndianness(type.numEntries); ++i) {
        Type_Entry entry{};
        inputFile.read(reinterpret_cast<char*>(&entry), sizeof(entry));
        typeEntries.push_back(entry);
    }

    External_Arenas externalarenas{};
    inputFile.read(reinterpret_cast<char*>(&externalarenas), sizeof(externalarenas));

    External_Arena externalarena{};
    inputFile.read(reinterpret_cast<char*>(&externalarena), sizeof(externalarena));

    std::vector<External_Arena_Entry> externalArenaEntries;
    for (int i = 0; i < swapEndianness(externalarenas.numEntries); ++i) {
        External_Arena_Entry entry{};
        inputFile.read(reinterpret_cast<char*>(&entry), sizeof(entry));
        externalArenaEntries.push_back(entry);
    }

    Subreferences subreferences{};
    inputFile.read(reinterpret_cast<char*>(&subreferences), sizeof(subreferences));

    Atoms atoms{};
    inputFile.read(reinterpret_cast<char*>(&atoms), sizeof(atoms));

    std::vector<Arena_Subreference_Record> arenaSubReferenceRecords;
    for (int i = 0; i < swapEndianness(atoms.numEntries); ++i) {
        Arena_Subreference_Record entry{};
        inputFile.read(reinterpret_cast<char*>(&entry), sizeof(entry));
        arenaSubReferenceRecords.push_back(entry);
    }

    //read Dictionary
    int alignment = swapEndianness(arena.alignment);
    int dictionaryitemcount = swapEndianness(arena.numEntries);
    inputFile.seekg(swapEndianness(arena.DictionaryStart));
    std::vector<arenaDictionary> dictionaries;
    for (int i = 0; i < swapEndianness(arena.numEntries); ++i) {
        arenaDictionary arenadictionary{};
        inputFile.read(reinterpret_cast<char*>(&arenadictionary), sizeof(arenadictionary));
        dictionaries.push_back(arenadictionary);
    }
    
    int startoffset = 0;
    int startoffset2 = 0;
    int iteminfooffset = 0;

    bool texturefound = false;
    bool modelfound = false;

    std::vector<uint32_t> offsetstoinfos;
    for (const auto& dictionary : dictionaries) {
        if (dictionary.typeId == swapEndianness(0x00EB000B)) {
            startoffset = swapEndianness(dictionary.ptr);
        }
        else if (dictionary.typeId == swapEndianness(0x00EB0005)) {
            modelfound = true;
            startoffset2 = swapEndianness(dictionary.ptr);
        }
        else if (dictionary.typeId == swapEndianness(0x000200E8)) {
            offsetstoinfos.push_back(swapEndianness(dictionary.ptr));
            texturefound = true;
        }
    }

    inputFile.seekg(startoffset);
    Header header{};
    inputFile.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (modelfound) {
        inputFile.seekg(startoffset + swapEndianness(header.pNames));
        std::vector<Arena_Subreference_Record> arenasrrecords;
        for (int i = 0; i < swapEndianness(header.uiTypeCount); ++i) {
            Arena_Subreference_Record arenasrrecord{};
            inputFile.read(reinterpret_cast<char*>(&arenasrrecord), sizeof(arenasrrecord));
            arenasrrecords.push_back(arenasrrecord);
            //std::cout << std::hex << swapEndianness(arenasrrecord.objectID) << "\n";
        }
        inputFile.seekg(startoffset2);
        ModelHeader modelheader{};
        std::vector<ModelEntry> modelentries;
        inputFile.read(reinterpret_cast<char*>(&modelheader), sizeof(modelheader));
        for (int i = 0; i < swapEndianness(modelheader.uiTypeCount); ++i) {
            ModelEntry entry{};
            inputFile.read(reinterpret_cast<char*>(&entry), sizeof(entry));
            modelentries.push_back(entry);
        }
        inputFile.seekg(startoffset2 + swapEndianness(modelheader.headerSize));
        std::vector<IDEntry> identries;
        for (int i = 0; i < swapEndianness(modelheader.uiItemsCount); ++i) {
            IDEntry identry{};
            inputFile.read(reinterpret_cast<char*>(&identry), sizeof(identry));
            identries.push_back(identry);
        }
    }
    if (texturefound) {
        std::vector<TOC_Entry> tocentries;
        for (int i = 0; i < swapEndianness(header.uiItemsCount); ++i) {
            TOC_Entry tocentry{};
            inputFile.read(reinterpret_cast<char*>(&tocentry), sizeof(tocentry));
            tocentries.push_back(tocentry);
        }
        inputFile.seekg(startoffset + swapEndianness(header.pNames));
        std::vector< std::pair<std::string, int>> names;

        for (const auto& tocentry : tocentries) {
            bool haveName = true;
            uint32_t currentName = swapEndianness(tocentry.Name);
            if (currentName == 0) {
                haveName = false;
            }
            auto nextEntry = std::next(std::begin(tocentries), &tocentry - &tocentries[0] + 1);

            uint32_t nameDifference = 0;
            if (nextEntry != std::end(tocentries)) {
                uint32_t nextName = swapEndianness(nextEntry->Name);
                nameDifference = nextName - currentName;
            }
            else {
                nameDifference = swapEndianness(header.pTypeMap) - currentName;
            }
            std::vector<char> buffer(nameDifference);
            inputFile.read(buffer.data(), nameDifference);
            std::string name(buffer.begin(), buffer.end());
            if (!haveName) {
                name = "";
            }
            names.push_back(std::make_pair(name, swapEndianness(tocentry.uiIndex)));
        }

        if (platform == "XBOX") {
            std::vector<TextureInformationX360> texinfosx360;

            for (int i = 0; i < swapEndianness(header.uiItemsCount); ++i) {
                inputFile.seekg(offsetstoinfos[i]);
                TextureInformationX360 texinfox360{};
                inputFile.read(reinterpret_cast<char*>(&texinfox360), sizeof(texinfox360));
                texinfosx360.push_back(texinfox360);
                int index = names[i].second - 1;

                int offset1 = swapEndianness(dictionaries[index].ptr) + swapEndianness(resourceDescriptors[0].size);
                int offset2 = swapEndianness(dictionaries[index].size);
                std::vector<uint8_t> buffer(offset2);
                inputFile.seekg(offset1);
                inputFile.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(offset2));
                std::string name = names[i].first;

                getTextureInformationAndUntile(name, buffer, texinfox360.GPUTEXTURE_FETCH_CONSTANT, sizeof(texinfox360.GPUTEXTURE_FETCH_CONSTANT));
            }
        }
        else if (platform == "PS3") {
            std::vector<TextureInformationPS3> texinfosps3;
            for (int i = 0; i < swapEndianness(header.uiItemsCount); ++i) {
                inputFile.seekg(offsetstoinfos[i]);
                TextureInformationPS3 texinfops3{};
                inputFile.read(reinterpret_cast<char*>(&texinfops3), sizeof(texinfops3));
                texinfosps3.push_back(texinfops3);
                int index = names[i].second - 1;

                uint16_t curheight = 0;
                uint16_t curwidth = 0;
                uint16_t mips = texinfops3.mipmap;

                for (int i = 0; i < 2; ++i) {
                    curheight = (curheight << 8) | ((texinfops3.height >> (8 * i)) & 0xFF);
                    curwidth = (curwidth << 8) | ((texinfops3.width >> (8 * i)) & 0xFF);
                }

                int offset1 = swapEndianness(dictionaries[index].ptr) + swapEndianness(resourceDescriptors[0].size);
                int offset2 = swapEndianness(dictionaries[index].size);
                std::vector<uint8_t> buffer(offset2);
                inputFile.seekg(offset1);
                inputFile.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(offset2));
                std::string name = names[i].first;

                write_ps3_textures_to_DDS(name, buffer, curwidth, curheight, mips, texinfops3.format, swapEndianness(texinfops3.storeType));
            }
        }
        else if (platform == "WII") {
            std::vector<TextureInformationWII> texinfoswii;
            for (int i = 0; i < swapEndianness(header.uiItemsCount); ++i) {
                inputFile.seekg(offsetstoinfos[i]);
                TextureInformationWII texinfowii{};
                inputFile.read(reinterpret_cast<char*>(&texinfowii), sizeof(texinfowii));
                texinfoswii.push_back(texinfowii);
                int index = names[i].second;

                uint16_t curheight = 0;
                uint16_t curwidth = 0;
                uint16_t mips = texinfowii.mips;

                for (int i = 0; i < 2; ++i) {
                    curheight = (curheight << 8) | ((texinfowii.height >> (8 * i)) & 0xFF);
                    curwidth = (curwidth << 8) | ((texinfowii.width >> (8 * i)) & 0xFF);
                }
                int offset1 = swapEndianness(dictionaries[index].ptr) + swapEndianness(dictionaries[index].alignment);
                int offset2 = swapEndianness(dictionaries[index].size);
                std::vector<uint8_t> buffer(offset2);
                inputFile.seekg(offset1);
                inputFile.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(offset2));
                std::string name = names[i].first;

                if (name == "") {
                    if (i == 0) {
                        name = filename;
                    }
                    else {
                        name = filename + "_" + std::to_string(i);
                    }
                }

                unswizzle_wii_textures_and_write_to_DDS(name, buffer, curwidth, curheight, mips, DirectX::DDSPF_DXT1);
            }
        }
    }
}

void processFile(const std::string& filename) {
    // function to process each file
    read_RW4_textures_and_write_to_DDS(filename);
}

void processFilesInFolder(const std::string& folderPath) {
    try {
        // Check if the folder exists
        if (!fs::exists(folderPath)) {
            std::cerr << "Change folder path!, Folder does not exist: " << folderPath << std::endl;
            return;
        }

        // Iterate through the directory
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            std::filesystem::path ext = entry.path().extension();
            if (entry.is_regular_file() && (ext == ".rg2" or ext == ".rx2" or ext ==".psg")) {
                processFile(entry.path().string());
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception occurred." << std::endl;
    }
}

//for testing purposes
int main() {
    std::string folderPath = "";
    processFilesInFolder(folderPath);
    return 0;
}
