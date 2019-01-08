fips_begin_module(Structures)
    fips_vs_warning_level(3)
    fips_files(
        Block.h Block.cc
    )
    fips_dir(interfaces)
    fips_files(
        iBlock.h iBlock.cc      
    )
fips_end_module()

fips_begin_unittest(Structures)
    fips_vs_warning_level(3)
    fips_dir(UnitTests)
    fips_files(
        BlockTest.cc        
    )
    fips_deps(Structures)
fips_end_unittest()

