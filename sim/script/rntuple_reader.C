#include <iostream>

#include "ROOT/RNTuple.hxx"
#include "ROOT/RNTupleModel.hxx"

#include "types.hh"

using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;

void alternate(){
    ROOT::RDataFrame df = ROOT::RDF::Experimental::FromRNTuple("Test", "struct_1e6.root");
    df.Foreach([&](hit h){
        std::cout<<"hit.x "<<h.x<<std::endl;
    },{"hit"});
}

void hist(){
    ROOT::RDataFrame df = ROOT::RDF::Experimental::FromRNTuple("Test", "struct_1e6.root");
    //df.Histo1D("my_hist","px",0,1e6,200);
    auto hist = df.Histo1D("px");
    auto h2 = df.Filter("px < 100").Histo1D("px");
    hist->Draw();
    h2->Draw();
}

void draw(){
    using RNTuple = ROOT::Experimental::RNTuple;
    TFile* F = new TFile("struct_1e6.root","READ");
    auto ntp = F->Get<RNTuple>("Test");
    ntp->Draw("px");
    auto reader = RNTupleReader::Open(ntp);


}


void rntuple_reader(){
    auto model = RNTupleModel::Create();
    std::shared_ptr<float> px = model->MakeField<float>("px");

    auto ntuple = RNTupleReader::Open(std::move(model),"Test","struct_1e6.root");
    std::cout<<" The length is"<<std::endl;
    ntuple->PrintInfo();
    alternate();
    hist();
}

