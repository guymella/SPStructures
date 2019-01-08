# SPStructures
Simple &amp; Powerful Data Structures

A set of lightweight, fast and usable c++ data structures. Easily extensible. Avoid Using STL structures to keep code size down.

Structures are polymorphic with strong interface seggragation. 

## priorities: 
1. Emphasis is on allowing coders to easily create thier own datastructures for cache and architecture optimization. 
2. Keeping compiled code small, for crossplatform/emscripton optimized code.
3. Speed. 
4. efficiency, allow coders to tweek the memory usage vs performace to fit thier needs. 

## TODO: stage 2
 - [ ] sArray - StaticArray
	- [ ] sBlock	
	- [ ] Indexable 
	- [ ] Iterable (begin, end)
 - [ ] bArray - BlockArray
	- [ ] Partition - subsection of block
	- [ ] front stack ( pushfront, popfront, front)
	- [ ] back stack (pushback , popback, back)
	- [ ] editable (fill, insert, remove, replace)
 - [ ] dArray - Dynamic Array
	- [ ] dBlock - dynamic memory Block
	- [ ] dPartiton - partition on dBlock
 - [ ] wArray -  wrapping dynamic array
	- [ ] wPartition
		- [ ] wPartion iterator
 - [ ] multiArray - dArray of dArray
 - [ ] InlineMultiArray - dynamic multi array in continuous memory block, slow write, super fast read. 
	- [ ] MultiPartition
 - [ ] zipper Partition - link multiple memory blocks together so they apear to be continuos. fast write, medium edit, medium search, slow read. can be flattened to continuos memory.
	- [ ] zipper Blocks Array
	- [ ] zipper Partition iterator
	- [ ] zipper partition indexer
	- [ ] bulk Editable
 - [ ] tree Partition - link subPattition that splice into superpartion and appear to be continuouse. Fast write, fast edit, slow search, slow read. can be blattend to continuos memory.
	- [ ] tree partition iterator
	- [ ] tree partition indexer