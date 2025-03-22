
time root -b -q -l script/rntuple_see.C(1'000'000'000)

Processing script/rntuple_see.C...
Warning in <[ROOT.NTuple] Warning /usr/src/debug/root/root-6.30.04/tree/ntuple/v7/src/RPageStorageFile.cxx:52 in ROOT::Experimental::Detail::RPageSinkFile::RPageSinkFile(std::string_view, const ROOT::Experimental::RNTupleWriteOptions&)>: The RNTuple file format will change. Do not store real data with this version of RNTuple!

real	1m27.600s
user	1m25.444s
sys	0m1.734s


time root -b -q -l 'script/rntuple_see.C(100000000)'

Processing script/rntuple_see.C(100000000)...
Warning in <[ROOT.NTuple] Warning /usr/src/debug/root/root-6.30.04/tree/ntuple/v7/src/RPageStorageFile.cxx:52 in ROOT::Experimental::Detail::RPageSinkFile::RPageSinkFile(std::string_view, const ROOT::Experimental::RNTupleWriteOptions&)>: The RNTuple file format will change. Do not store real data with this version of RNTuple!

real	0m9.280s
user	0m8.923s
sys	0m0.288s
