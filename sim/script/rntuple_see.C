//R__LOAD_LIBRARY(ROOTNTuple)
#include "ROOT/RNTuple.hxx"
using RNTupleModel  = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;
void rntuple_see(int N){
    auto model = RNTupleModel::Create();
    auto px = model->MakeField<float>("px");
    auto ntuple = RNTupleWriter::Recreate(std::move(model),"Test","rntuple_"+std::to_string(int(N/1e6))+"e6.root");

    for(int i=0; i < N; ++i){
        *px = 1.0*i;
        ntuple->Fill();
    }
    // ntuple goes out of scope here and is written.
}
