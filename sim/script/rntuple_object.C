//R__LOAD_LIBRARY(ROOTNTuple)
#include "ROOT/RNTuple.hxx"

struct hit {
    float x, y, z;
    int pid;
};

using RNTupleModel  = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;
void rntuple_object(int N){
    auto model = RNTupleModel::Create();
    auto px = model->MakeField<float>("px");
    auto ht = model->MakeField<hit>("hit");
    auto ntuple = RNTupleWriter::Recreate(std::move(model),"Test","struct_"+std::to_string(int(N/1e6))+"e6.root");

    for(int i=0; i < N; ++i){
        hit h; h.x = i; h.y = 0.5*i; h.z = 0.3*i; h.pid = i;
        *px = 1.0*i;
        *ht = h;
        ntuple->Fill();
    }
    // ntuple goes out of scope here and is written.
}
