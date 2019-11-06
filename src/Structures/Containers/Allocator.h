#pragma once
//------------------------------------------------------------------------------
/*
    @class Structures::Allocator
    @ingroup Block
    @brief master processor storage and memory allocator. allocates storage and handles fragmentation
*/

#include <stdint.h>
#include <string.h>
#include <memory>


class Allocator {
public:
	Allocator() {};
	
private:
	size_t HardThreads;
	size_t Processors;
	size_t GPUs;
	size_t LocalCacheDomains; // processor independant caches
	size_t* localCacheLevels;
	size_t* localCacheSizes;
	size_t sharedCacheDomains; // processor shared caches
	size_t* sharedCacheSizes;
	size_t LocalRamDomains = 1; // random access memory (numa count)
	size_t* RamAllotments;
	size_t* RamAllocations;
	size_t LocalRasDomains; // random access storage (optane count)
	size_t* RasAllotments;
	size_t* RasAllocations;
	size_t localSasDomains; // sequential access storage (ssd count)
	size_t* SasAllotments;
	size_t* SasAllocations;
	size_t LocalLasDomains; // latent access storage (hdd count)
	size_t* LasAllotments;
	size_t* LasAllocations;
	size_t NasDomains; //network access storage (local netowrk drives)


	

};