#pragma once
//------------------------------------------------------------------------------
/**
    @file Core/Types.h
    @brief defines basic data types for Oryol
*/
#include <stdint.h>

namespace Oryol {

// those typedefs are obsolete, don't use
typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
typedef float float32;
typedef double float64;
typedef intptr_t intptr;
typedef uintptr_t uintptr;
typedef unsigned char uchar;

static const int32_t InvalidIndex = -1;
static const int32_t EndOfString = -1;
static const int32_t EndOfFile = -1;
static const int32_t EndOfRange = -1;

} // namespace Oryol

enum baseTypes : uint8_t {
	Byte = 0, // 1 byte
	Short = 1, // 2 bytes
	Word = 2, // 4 bytes 
	Long = 3, // 8 bytes (cpu registers)
	Quad = 4, // 16 bytes (sse register)
	Oct = 5, // 32 bytes (avx register)
	hex = 6, // 64 bytes (avx512 register)
	DoHex = 7, // 128 Bytes (small wave)
	QuadHex = 8, //256 bytes (Wave/cache line)
	OctHex = 9, //512 bytes (large wave/half fetch)
	KiB = 10, //1024 Bytes (small Tensor/ cache fetch) 
	DKiB = 11, //2048 bytes (tensor/wavefront)
	QKiB = 12, // 4096 bytes (Page)
	OKiB = 13, // 8192 bytes (cache Pane)
	HKiB = 14, // 16384 bytes (cache Window)
	WKiB = 15, // 32768 bytes (Cache Bank)
	Bank = 16, // 65536 bytes (ram Bank)
	MiB = 17, // 1048576 Bytes
	GiB = 18, // 1,073,741,824 Bytes
	Fat = 19, // 4,294,967,296? Bytes
	uint8 = 20,
	int8 = 21,
	uint16 = 22,
	int16 = 23,
	uint32 = 24,
	int32 = 25,
	uint64 = 26,
	int64 = 27,
	uint80 = 28,
	int80 = 29,
	uint128 = 30,
	int128 = 31,
	float16 = 32,
	flaot32 = 33,
	float64 = 34,
	Scalar16_ = 35,
	Scalar32_ = 36,
	Scalar64_ = 37,
	Vector16_ = 38,
	Vector32_ = 39,
	Vector64_ = 40,
	Matrix16_ = 41,
	Matrix32_ = 42,
	Matrix64_ = 43,
	Tensor16_ = 44,
	Tensor32_ = 45,
	Tensor64_ = 46,
	TransLinear = 47,
	decimal = 48,
	chr = 49,
	nchr = 50,
	string = 51,
	rational = 52,
	complex = 53,
	quaternion = 54,
	octernion = 55,
	lateral = 56,
	varlateral = 57,
	formal = 58, // formal symbolic language
	informal = 59, // informal symbolic language
	Abstract = 60,
	Symbolic = 61,
	Continueouse = 62,
	Disjoint = 63,
	Struct = 64,
	VarStruct = 65,
	VarBinary = 66,
	VarUint = 67,
	VarInt = 68,
	VarChar = 69,
	VarFloat = 70,
	VarDecimal = 71,
	VarRational = 72,
	VarLateral = 73,

	SRef = 125, //static reference (Domain|key|Hash ::  0|?|? is reserved for standard library,{1-8b}|?|? = local cache Registaration; Hashes smaller than 256 bits are considered version numbers )
	PRef = 126, //phisical reference (ipv6:port|key|Hash)
	LRef = 127, //local reference (1-4B|?|? reserved for process local scopes Catalog, 5-8B|?|?  used for App Catalog 9-12B|?|? for cluster Catalog)

	BlockS = 128, //Static Block
	Block = 129,  //Dynamic Block 
	BlockV = 130, // back loaded table
	BlockF = 131, //front loaded table
	BlockD = 132, //Double ended block
	BlockPwr = 133, //Power allocated Block
	Allocator = 134, //block Allocator
	AllocatorPwr = 135, // Power Allocator (Preallocator)
	TableSF = 136, //Static Table
	TableS = 137, //Static Fixed table
	TableF = 138, //dynamic Table fixed Size Partitions
	TableD = 139, //full dynamic table
	TableRef = 140, //table of referenced Blocks

	Val = 253, // undefined value stub. polymorphic base value
	Poly = 254, //undefined object stub, pure polymorphic struct
	Void = 255 //empty uninitialized
};