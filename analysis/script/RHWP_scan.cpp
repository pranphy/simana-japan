/* -*- coding: utf-8 -*-
 vim: ai ts=4 sts=4 et sw=4 ft=cpp
 author : Prakash [pgautam@jlab.org]
 date   : 2025-03-21


 So basically this script makes a profile plot of beam position asymmetry and 
 the position differences x and y vs the angle of the RHWP. When we take the data
 for different angles, the scandata1 corresponds basically to the different angles.
 `scandaa1 == 4000` is 180 degrees and 0 is 0 degrees.

 This script basically reads of three variables from the mul tree. 
 1. asym_bpm1i02WS.hw_sum 2. diff_bpm1i02X.hw_sum 3. diff_bpm1i02Y.hw_sum for a given
 bpm device. Then fits a function of the  from f(x) = a + b*sin(2x) + c*sin(4x) where
 x is the angles.
*/

#include "util.h"

//const static char* QW_ROOTFILES="/mnt/stg/data/japan/beam/root";
const static char* QW_ROOTFILES="/chafs2/work1/parity/japanOutput";
const static char* fpattern = "%s/prexinj_%d.%03d.root";

using RDF = ROOT::RDataFrame;

auto sine_func(double* x, double a[]){
    // we just basicaly fit a + sin(2x) + sin(4x); but 2x and 4x can have relative shift. which is why a[2] and a[4] subtracted.
    return a[0] + a[1]*std::sin(2*utl::to_radian(x[0]-a[2])) + a[3]*std::sin(4*utl::to_radian(x[0]-a[4]));
}

template <typename T>
void fit_func(T& profile){
    TF1 sine("sine",sine_func,0,360,5);
    profile->Fit(&sine,"R");
    for(int i = 0; i < 5; i++) std::printf("param %d: %f\n",i,sine.GetParameter(i));
    sine.Draw("same");
}

template<typename GRD> // Generic RDataframe (GRD)
void book(GRD& mul, int runnumber,std::string device="1i02"){

    auto scale = [](double x){return [x](double y){return x*y;};};
    auto dmul = mul.Define("Aq",scale(1e6),{"asym_bpm"+device+"WS.hw_sum"}) // scale to ppm  by 1e6
                    .Define("Dx",scale(1e6),{"diff_bpm"+device+"X.hw_sum"}) //scale to nm from mm by 1e6
                    .Define("Dy",scale(1e6),{"diff_bpm"+device+"Y.hw_sum"}) // ditto
                    .Define("angle",scale(180.0/4000.0),{"scandata1"}); // So the scandata1 is such that 4000 corresponds to 180 rotation.
    
    int nbins = 25;

    auto hAq = dmul.Profile1D({"aq"," Aq; angle[degree]; Mean (ppm)",nbins,0,180},"angle","Aq");
    auto hDx = dmul.Profile1D({"dx"," Dx; angle[degree]; Dx[nm]",nbins,0,180},"angle","Dx");
    auto hDy = dmul.Profile1D({"dy"," Dy; angle[degree]; Dy[nm]",nbins,0,180},"angle","Dy");

    fit_func(hAq); // event loops is already done here. has to be now.
    fit_func(hDx);
    fit_func(hDy);

    // Do the fitting.

    auto c = utl::plt::subplots<TProfile>(3,1,{hAq,hDx,hDy});
    c->SaveAs(Form("asset/image/test/RHWP_scan_%d_%s.pdf",runnumber,device.c_str()));
}

void RHWP_scan(int runnumber, std::string device){
    ROOT::EnableImplicitMT();
    std::string filename{Form(fpattern,QW_ROOTFILES,runnumber,0)};
    std::printf("Processing %s\n",filename.c_str());
    auto mul = RDF("mul",filename).Filter("cleandata"); // we only care about cleandata so immediately filter it.

    book(mul,runnumber,device);

}

