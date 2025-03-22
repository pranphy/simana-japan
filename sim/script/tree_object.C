struct hit {
    float x, y, z;
    int pid;
};

void tree_object(int N){
    std::string filename = "tree_object_"+std::to_string(int(N/1e6))+"e6.root";
    auto f = new TFile(filename.c_str(),"RECREATE");
    auto T = new TTree("Test","description of tree");
    float px; hit ht;
    T->Branch("px",&px,"px/F");
    T->Branch("hit",&ht);

    for(int i=0; i < N; ++i){
        px = 1.0*i;
        hit h; h.x = i; h.y = 0.5*i; h.z = 0.3*i; h.pid = i;
        ht = h;
        T->Fill();
    }
    T->Write();
}
