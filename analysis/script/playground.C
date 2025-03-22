#include "analysis/script/util.h"

const static char* QW_ROOTFILES="/chafs2/work1/parity/japanOutput";
/*

const static char* QW_ROOTFILES="/mnt/stg/data/japan/beam/root";

std::string p  ="hi";;
utl::format("%s/%s_%d.root",p, "some/path/filename",121)

*/

void bpm_names(){
    const std::vector<std::string>device_name{
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
        "0i01b"  //ADC11
    };
    std::vector<std::string>ch_name{"XP","XM","YP","YM"};

}

struct bpm {
 double Device_Error_Code;
 double block0;
 double block0_raw;
 double block1;
 double block1_raw;
 double block2;
 double block2_raw;
 double block3;
 double block3_raw;
 double hw_sum;
 double hw_sum_raw;
 double num_samples;
};
    

void test_tree(){

    int run_no = 16988;
    const char* QW_ROOTFILES = "/mnt/stg/data/japan/beam/root";
    std::string filename=std::string(Form("%s/prexinj_%d.%03d.root",QW_ROOTFILES,run_no,0));
    std::cout<<"The filename is "<<filename<<std::endl;

    auto [F,evt] = utl::get_tree(filename,"evt");
    bpm bpm1;
    evt->SetBranchAddress("bpm0i01XP",&bpm1);

    auto evt = ROOT::RDataFrame("evt",filename).Filter("cleandata");

    auto schist = std::make_unique<TH1F>("scdata0i01XPhwsum","0i01XP; hw_sum [mm]; Count",100,37e3,39e3);
    evt.Foreach([](bpm bpm1)->void {
            schist->Fill(bpm1.hw_sum_raw);
    },{"bpm0i01XP"});
    schist->Draw();

    evt.Foreach([](double bpm1)->void {
            schist->Fill(bpm1);
    },{"bpm0i01XP.hw_sum"});
    schist->Draw();

}

void fit_pedestal(){
    auto [F,evt] = utl::get_tree(filename,"evt");
    evt->Draw("bpm0l02XP.hw_sum:scandata1","CodaEventNumber>10","prof");

}

void playground(){
    test_tree();
}
