#pragma once
// vim: ai ts=4 sts=4 et sw=4 ft=cpp foldmethod=marker
 
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <functional>
#include <utility>
 
 
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TPolyLine.h"
#include "TArc.h"
#include "TGraphErrors.h"
 
#include "ROOT/RDataFrame.hxx"
 
using SPTree = std::shared_ptr<TTree>;
using UPTree = std::unique_ptr<TTree>;
using RDF = ROOT::RDataFrame;
using dbl_cut = std::function<bool(double)>;
 
 
 
// ===================== CPP STUFFS ====================
namespace utl {
 
// standard cpp{{{
 
const double pi = 3.1415926535897932384626433832795;
std::vector<std::string> split(std::string full, char delimiter=',') {
    std::string segment; std::stringstream fullstream(full);
    std::vector<std::string> seglist;
 
    while(std::getline(fullstream, segment, delimiter)) {
        seglist.push_back(segment);
    }
    return seglist;
}
 
std::string get_extension(std::string str){
    auto npos = str.find_last_of('.');
    return npos == str.npos ? "" : str.substr(npos+1,str.size());
}
 
 
 
template <typename T>
bool contains(std::vector<T>& container,const T& elem) {
    return container.end() != std::find(container.begin(),container.end(),elem);
}
 
template <typename T>
std::vector<T> logspace(int min, int max, int bins, T offset){
    T bw = (1.0*max - min) / bins;
    std::vector<T> vals(bins+1);
    for(int i = 0; i <= bins; ++i) vals.at(i) = offset + std::pow(10, i*bw + min);
    return vals;
}
 
template <typename T>
std::vector<T> logspace(T min, T max, int bins){
    T xe = std::log10(max/min); T bw = xe / bins;
    std::vector<T> vals(bins+1);
    for(int i = 0; i <= bins; ++i) vals.at(i) =  min  * std::pow(10, i*bw);
    return vals;
}
 
 
template <typename T>
bool is_inside_pgm(const T& x, const T& y,std::vector<T> box){
    T x0 = box[0], x1 = box[4], y0 = box[2],  y1 = box[6],
      x2 = box[1], x3 = box[5], y2 = box[3],  y3 = box[7];
    bool L0up   =  (y-y0) > ( (y1-y0)/(x1-x0)*(x-x0) );
    bool L1down =  (y-y0) < ( (y2-y1)/(x2-x1)*(x-x1) );
    bool L2down =  (y-y2) < ( (y2-y3)/(x2-x3)*(x-x3) );
    bool L3up   =  (y-y3) > ( (y3-y0)/(x3-x0)*(x-x0) );
    return L0up and L1down and L2down and L3up;
}
 
 
template <typename T>
bool is_inside(const T& x, const T& y,std::vector<T> box){
    if(box.size() <= 4)
        return (x > box[0] and x <= box[1] and y > box[2] and y <= box[3]); //box = [x0,x1, y0, y1]
    else
        return is_inside_pgm(x,y, box);
}
 
template <typename T, typename... Vectors>
std::vector<T> merge(Vectors&&... vecs) {
    std::vector<T> result;
    result.reserve((vecs.size() + ...));
    (result.insert(result.end(), std::make_move_iterator(vecs.begin()), std::make_move_iterator(vecs.end())), ...);
    return result;
}
 
 
template <typename T>
void print(std::vector<T>& container, const std::string formatter="%8.1f, "){
    printf("[ ");
    std::for_each(container.begin(),container.end(),[&](T c){ printf(formatter.c_str(),c);});
    printf("] ");
}
 
template <typename T>
void println(std::vector<T>& container,const std::string formatter="%8.1f, "){
    print(container,formatter);
    printf("\n");
}
 
// General utilities
 
double hypot(double x, double y) { return std::hypot(x, y); }
double to_degree(double a) { return a*180/pi;}
double to_radian(double a) { return a*pi/180;}
 
// inverse tangent in degrees
double atan_deg(double y, double x){
    double rad = std::atan2(y,x);
    return to_degree(rad);
}
 
double neg_deg(double a){
    int sign =  a < 0 ? -1 : 1;
    return -sign*(180.0-std::abs(a));
}
 
double diff_deg(double a, double b){
    double d1 = std::abs(a - b);
    double d2 = std::abs(utl::neg_deg(a) - utl::neg_deg(b));
    return std::min(d1,d2);
}
 
// inverse tangent in degrees
double atan_rad(double y, double x){
    return std::atan2(y,x);
}
 
std::vector<std::string> readlines(const std::string filename,int nol=-1){
    std::vector<std::string> all_lines;
    std::string line; std::ifstream fileobj(filename);
    while(std::getline(fileobj,line)){
        all_lines.push_back(std::move(line));
        if(all_lines.size() >= nol) break;
    }
    return all_lines;
}
 
std::unique_ptr<TChain> make_chain(std::string filelist,int nof = 0, bool verbose=false){
    std::ifstream flist(filelist);
    std::string filename{};
    auto TC = std::make_unique<TChain>("T");
    int cnt = 0;
    while(std::getline(flist,filename)){
        if(++cnt > nof && nof > 0)  break;
        if(filename.length()<2) continue;
        if(verbose) std::cout<<"Adding "<<filename<<std::endl;
        TC->Add(filename.c_str());
    }
    return TC;
}
 
 
 
 
// }}} CPP helpers
 
// {{{ Root Stuffs

template <typename... Args> std::string format(std::string fmtstr, Args&&... args){
    return std::string(Form(fmtstr.c_str(), args...));
}

void save(std::shared_ptr<TObject> Obj, std::string name, const std::string directory="", std::string rootfile="asset/files/saved-root-objects.root"){
    auto F = std::make_unique<TFile>(rootfile.c_str(),"UPDATE");
    if(directory != ""){
        TDirectory* objdir = F->GetDirectory(directory.c_str());
        if(!objdir) objdir = F->mkdir(directory.c_str());
        F->cd(directory.c_str());
    }
    Obj->Write(name.c_str());
    F->Close();
}
 
template<typename T> void save(std::string treename, std::vector<T>& data, std::string name, const std::string directory="", std::string rootfile="asset/files/saved-root-data.root"){
    auto F = std::make_unique<TFile>(rootfile.c_str(),"UPDATE");
    if(directory != ""){
        TDirectory* objdir = F->GetDirectory(directory.c_str());
        if(!objdir) objdir = F->mkdir(directory.c_str());
        F->cd(directory.c_str());
    }
 
    std::unique_ptr<TTree> tree;
    if (F->Get(treename.c_str()) == nullptr) tree = std::make_unique<TTree>(treename.c_str(), "Data Tree");
    else tree = std::make_unique<TTree>(dynamic_cast<TTree*>(F->Get(treename.c_str())));
 
    T elem;
    tree->Branch(name.c_str(), &elem);
    F->cd();
    for(auto dt: data) {elem = dt; tree->Fill(); }
    F->Write();
    F->Close();
}
 
template<typename T>
void save(std::vector<T>& data, std::string name, const std::string directory="", std::string rootfile="asset/files/saved-root-objects.root"){
    save("T",data,name,directory,rootfile);
}
 
ROOT::RDataFrame  get_df(const std::string filename,const std::string treename="T"){
    return ROOT::RDataFrame(treename,filename);
}
 
std::tuple<std::shared_ptr<TFile>,std::shared_ptr<TTree>> get_tree(std::string filename,const std::string treename="T"){
    auto  F = std::make_shared<TFile>(filename.c_str());
    auto T = std::shared_ptr<TTree>(F->Get<TTree>(treename.c_str()));
    return std::make_tuple(F,T);
}

std::shared_ptr<TTree> get_tree(std::shared_ptr<TFile> F,std::string treename="T"){
    auto T = std::shared_ptr<TTree>(F->Get<TTree>(treename.c_str()));
    return T;
}
 
 
bool get_tree(std::string filename, TTree** T, const std::string treename="T"){
    TFile* F = new TFile(filename.c_str());
    if(F->IsZombie())
        return false;
    else
        *T = F->Get<TTree>(treename.c_str());
    return true;
}
/// {{{ Japan output specific

//std::tuple<double,double,double> get_maximum(std::shared_ptr<TTree> T, std::string variable, std::string cut){
//    T->Draw(Form("%s>>tmpp_%s",variable.c_str(),variable.c_str()), cut.c_str(), "goff");
//    TH1D* hmax = (TH1D*) gDirectory->FindObject("htemp");
//    int xmax = (int) hmax->GetXAxis()->GetXmax();
//    int xmin = (int) hmax->GetXAxis()->GetXmin();
//    int nbin = (int) hmax->GetXAxis()->GetNBins();
//    return std::make_tuple(nbin,xmin,xmax);
//}
//
//std::set<int> get_scandatas(SPTree T){
//    
//}

/// }}} Japan Output Specific
 
 
 
namespace plt {


template <typename T>
std::unique_ptr<TCanvas> subplots(int rows, int cols, std::vector<ROOT::RDF::RResultPtr<T>> plots,std::string fill="E1"){
    auto c1 = std::make_unique<TCanvas>("c1");
    c1->Divide(cols,rows);
    for(int i=0; i<plots.size(); i++){
       c1->cd(i+1);
        plots[i]->Draw(fill.c_str());
    }
    return c1;
}

std::unique_ptr<TLine> line(std::vector<double>& p0, std::vector<double>& p1){
    auto line = std::make_unique<TLine>(p0[0],p0[1], p1[0],p1[1]);
    line->SetLineColor(kBlue); line->SetLineColor(2); line->SetLineWidth(1);
    line->Draw();
    return line;
}
 
TLine vline(double xval){
    TLine line(xval, gPad->GetUymin(), xval, gPad->GetUymax());
    line.SetLineColor(kRed);
    line.SetLineStyle(2);
    line.SetLineWidth(1);
    line.Draw();
    return line;
}
 
std::unique_ptr<TLine> hline(double yval){
    auto line = std::make_unique<TLine>( gPad->GetUxmin(), yval, gPad->GetUxmax(),yval);
    line->SetLineColor(kRed);
    line->SetLineStyle(2);
    line->SetLineWidth(1);
    line->Draw();
    return line;
}
 
std::unique_ptr<TArc> circle(float radius, float x = 0, float y = 0){
    auto circle = std::make_unique<TArc>(x, y, radius);
    circle->SetLineColor(kRed); circle->SetLineStyle(2); circle->SetLineWidth(1);
    circle->SetFillStyle(4001);
    circle->Draw();
    return circle;
}
 
template<typename T>
void polygon(std::vector<T>& x, std::vector<T>& y,int colour=kRed, int thickness=2,int style=2){
    TPolyLine* poly = new TPolyLine(x.size(),x.data(),y.data());
    poly->SetLineWidth(thickness); poly->SetLineColor(colour); poly->SetLineStyle(style); poly->Draw();
}
 
template<typename T>
void quad(std::vector<T>& box,int colour=kRed, int thickness=2,int style=2){
    std::vector<T> x={box[0],box[0],box[1],box[1],box[0]};
    std::vector<T> y={box[2],box[3],box[3],box[2],box[2],};
    draw_polygon(x, y, colour, thickness, style);
}
 
template<typename T>
void rectangle(std::vector<T>& box,int colour=kRed, int thickness=2,int style=2){
    std::vector<float> x={box[0],box[0],box[1],box[1],box[0]};
    std::vector<float> y={box[2],box[3],box[3],box[2],box[2],};
    polygon(x, y, colour, thickness, style);
}
 
void text(std::string text, Double_t normx, Double_t normy, int colour=kRed, Double_t size = 0.04, int NDC=0) {
  TLatex* label = new TLatex();
  label->SetNDC(NDC);
  label->SetTextAlign(22);
  label->SetTextSize(size);
  label->SetTextColor(colour);
  label->DrawLatex(normx, normy, text.c_str());
}

std::shared_ptr<TCanvas> subplot(int row, int col, std::string name=""){
    std::shared_ptr<TCanvas> spc = std::make_shared<TCanvas>(name.c_str(),name.c_str());
    spc->Divide(row,col);
    return spc;
}
 
} // plt::
 
// }}} Graphics Stuffs

// }}} Root Stuffs
 
 
} // utl::
 
namespace plt = utl::plt;
