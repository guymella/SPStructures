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
	Word = 2, // 4bytes 
	Duel = 3, // 8 bytes (cpu registers)
	Quad = 4, // 16 bytes (sse register)
	Oct = 5, // 32 bytes (avx1 register)
	hex = 6, // 64 bytes (avx2 register)
	DoHex = 7, // 128 Bytes (small wave)
	QuadHex = 8, //256 bytes (Wave/cache line)
	OctHex = 9, //512 bytes (large wave/half fetch)
	KiB = 10, //1024 Bytes (small Tensor/ cache fetch) 
	DKiB = 11, //2048 bytes (tensor/wavefront)
	QKiB = 12, // 4096 bytes (Page)
	OKiB = 13, // 8192 bytes 
	HKiB = 14, // 16384 bytes
	WKiB = 15, // 32768 bytes (L1)
	Bank = 16, // 65536 bytes (ram Bank)
	MiB = 17, // 1048576 Bytes
	Fat = 18, // 4,294,967,296? Bytes
	uint8 = 19,
	int8 = 20,
	uint16 = 21,
	int16 = 22,
	uint32 = 23,
	int32 = 24,
	uint64 = 25,
	int64 = 26,
	uint80 = 27,
	int80 = 28,
	float16 = 29,
	flaot32 = 30,
	float64 = 31,
	decimal = 32,
	chr = 33,
	nchr = 34,
	string = 35,
	rational = 36,
	complex = 37,
	quaternion = 38,
	octernion = 39,
	lateral = 40,
	varlateral = 41,
	formal = 42, // formal symbolic language
	informal = 43, // informal symbolic language
	Abstract = 44,
	Symbolic = 45,
	Continueouse = 46,
	Disjoint = 47,
	Struct = 48,
	VarStruct = 49,
	VarBinary = 50,
	VarUint = 51,
	VarInt = 52,
	VarChar = 53,
	VarFloat = 54,
	VarDecimal = 55,
	VarRational = 56,
	SRef = 57, //static reference (hash 0|0|? is reserved for this enum and standard library; Hash|key|registration)
	PRef = 58, //phisical reference (ipv6|port|registration)
	LRef = 59, //local reference (0|0|? reserved for local scope registration, all others used for IPC)

	ProcF = 125,
	ProcR = 126,
	Proc = 127

};