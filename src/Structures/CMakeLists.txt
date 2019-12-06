fips_begin_module(Structures)
    fips_vs_warning_level(3)
	
	fips_files(
		Types.h
    )
	
    fips_dir(Containers)
    fips_files(
        Allocator.h 
		Block.h 
		Table.h 
		TableStruct.h
		Partition.h
		Array.h
		SparseArray.h
		ArrayPartition.h
		Slice.h
		SmartArrayPartition.h
		SmartSlice.h
		Trie.h
    )

    fips_dir(interfaces)
    fips_files(
        iBlock.h 
		iTable.h
		iArray.h
		iStruct.h iStruct.cc
		iCountable.h
		iDynamic.h
		iQueue.h
		iIndexable.h
		iIterable.h
		iIterator.h
		iStandard.h
		iTrie.h
    )

    fips_dir(Types)
    fips_files(
        VarInt.h   
		TypeDescr.h
		KeyString.h
    )
	
	#fips_dir(Containers)
    #fips_files(
        #ArrayMap.h
        #Buffer.h
        #HashSet.h
		#KeyValuePair.h
        #Map.h
        #Queue.h
        #Set.h
        #StaticArray.h
        #elementBuffer.h
        #InlineArray.h
    #)
	
    #fips_dir(Memory)
    #fips_files(Memory.cc Memory.h)
    #fips_dir(String)
    #fips_files(
        #String.cc String.h
        #StringAtom.cc StringAtom.h
        #StringBuilder.cc StringBuilder.h
        #StringConverter.cc StringConverter.h
        #WideString.cc WideString.h
        #stringAtomBuffer.cc stringAtomBuffer.h
        #stringAtomTable.cc stringAtomTable.h
        #ConvertUTF.c ConvertUTF.h
    #)


fips_end_module()

fips_begin_app(TestApp cmdline)
    fips_vs_warning_level(3)	
    fips_files(TestApp.cc)
    fips_dir(Unittests)
    fips_files(
        ArrayTest.h
		BlockTest.h
		TableTest.h
		SliceTest.h
		SmartSliceTest.h
		SparseArrayTest.h
    )
fips_end_app()