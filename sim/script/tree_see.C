void tree_see(int N){
    std::string filename = "tree_"+std::to_string(int(N/1e6))+"e6.root";
    auto f = new TFile(filename.c_str(),"RECREATE");
    auto T = new TTree("Test","description of tree");
    float px;
    T->Branch("px",&px,"px/F");

    for(int i=0; i < N; ++i){
        px = 1.0*i;
        T->Fill();
    }
    T->Write();
}
