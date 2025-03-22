//const static char* QW_ROOTFILES="/chafs2/work1/parity/japanOutput";
#include "util.h"
const static char* QW_ROOTFILES="/mnt/stg/data/japan/beam/root";
const static char* fpattern = "%s/prexinj_%d.%03d.root";

const std::vector<std::string> devices{ // device name
    "2i00","2i00a", //ADC0
    "2i01a","1i02", //ADC1
    "1i04","1i05", // ADC2
    "1i07","0i01", //ADC3
    "0i05","0l01", // ADC4
    "0l02","0l03", //ADC5
    "0l04","0l05", //ADC6
    "0l06","0l07", //ADC7
    "0l08","0l09", //ADC8
    "0l10","0i07", //ADC9
    "0r05","0r03", //ADC10
    "0i01b"
}; //ADC11

const std::vector<std::string> channels{"XP","XM","YP","YM"}; // channels
using RDF = ROOT::RDataFrame;

using RP1D = ROOT::RDF::RResultPtr<TH1D>; // Result pointer to 1D histograms
// per device channel scandata statistics; looks a bit ridiculous but basically
// its just pdcss["100i"]["XP"][2000] = RP1D object.
using pdcss = std::map<std::string,std::map<std::string,std::map<double,RP1D>>>;

double quotient(double a, double b){return a/b;}

template<typename GRD>
pdcss book(GRD mps, std::set<double>& scandata){
    pdcss adc_mean;
    for(auto& dv : devices){
        for(auto& ch : channels){
            std::string hwsum{"bpm"+dv+ch+".hw_sum"};
            std::string nsamp{"bpm"+dv+ch+".num_samples"};
            auto ae = mps.Define("adc_mean",quotient,{hwsum,nsamp});
            for(auto& sd: scandata){
                std::string cut = "scandata1=="+std::to_string(sd);
                auto h = ae.Filter(cut).Histo1D("adc_mean");
                adc_mean[dv][ch][sd] = h;
            }
        }
    }
    return adc_mean;
}

TGraphErrors fit_linear(std::map<double,RP1D>& dchmean){
    std::vector<double> scandata, mean, error;
    for(auto& [sc,mh]: dchmean){
        double mv = mh->GetMean();
        double er = mh->GetRMS()/TMath::Sqrt(mh->GetEntries());
        scandata.push_back(sc);mean.push_back(mv);error.push_back(er);
    } 
    TGraphErrors gr(scandata.size(),scandata.data(),mean.data(),error.data(),error.data());
    gr.Fit("pol1","Q");

    return gr;
}

void calculate_bpm_pedestal(int run_no,const char* user_cut="1"){
    ROOT::EnableImplicitMT();
    std::string filename = std::string(Form(fpattern,QW_ROOTFILES,run_no,0));
    auto mps = RDF("evt",filename).Filter("cleandata"); // we only care about cleandata so immediately filter it.

    auto scandatas = mps.Filter(user_cut).Take<double>("scandata1"); // I am not sure we can avoid this loop once, because we don't scandatas beforehand
    std::set<double> unique_scandata(scandatas.begin(),scandatas.end());

    for(auto &v : unique_scandata){
        std::printf("Scandata1 : %.0f\n",v);
    }

    auto adc_mean = book(mps,unique_scandata);
    for(auto& device : devices){
    //for(std::string device : {"2i00"}){
        for(auto& ch : channels){
            auto gr = fit_linear(adc_mean[device][ch]);
            auto fx = gr.GetFunction("pol1");
            std::printf("%s%s  %.2f 1.0 \n", device.c_str(),ch.c_str(), fx->GetParameter(0), fx->GetParameter(1) );
            //std::string filename = "asset/image/hey_"+device+"_"+ch+".png";
            //std::cout<<"Saving "<<filename<<std::endl;
            //gr.Draw();
            //c.SaveAs(filename.c_str());
        }
    }
}
