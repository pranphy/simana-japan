void FFB_study(int runNo = 15997, TString FFB = "ON", TString user_cut = "ErrorFlag==0"){
  gStyle->SetTitleFontSize(.06);
  gStyle->SetTitleSize(.06,"XY");
  gStyle->SetTitleOffset(0.55,"Y");
  gStyle->SetTitleOffset(0.8,"X");
  gStyle->SetLabelSize(.06, "YX");
  TString rootfile_dir = "/chafs2/work1/parity/japanOutput";
  TFile* fin = TFile::Open(Form("%s/sbs_CntHouse_%d.root",rootfile_dir.Data(),runNo),"READ");
  TTree* mul = (TTree*)fin->Get("mul");
  TTree* pr = (TTree*)fin->Get("pr");
  TString bpm[] = {"bpm4a","bpm4e","bpm12"/*,"bpm8","bpm14"*/};
  const int nbpm = 3;
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.035);
  latex.SetTextColor(2);
  TCanvas* cmulx = new TCanvas("cmulx","BPMX vs TIME (mul tree)",700,900);
  cmulx->Divide(1,nbpm);
  for(int ibpm=0;ibpm<nbpm;ibpm++){
    cmulx->cd(ibpm+1);
    mul->Draw(Form("diff_%sX/um:CodaEventNumber",bpm[ibpm].Data()),user_cut);
    latex.DrawLatex(0.15,0.85,Form("FFB: %s",FFB.Data()));
  }
  TCanvas* cmuly = new TCanvas("cmuly","BPMY vs TIME (mul tree)",700,900);
  cmuly->Divide(1,nbpm);
  for(int ibpm=0;ibpm<nbpm;ibpm++){
    cmuly->cd(ibpm+1);
    mul->Draw(Form("diff_%sY/um:CodaEventNumber",bpm[ibpm].Data()),user_cut);
    latex.DrawLatex(0.15,0.85,Form("FFB: %s",FFB.Data()));
  }
 TCanvas* cprx = new TCanvas("cprx","BPMX vs TIME (pr tree)",700,900);
 cprx->Divide(1,nbpm);
 for(int ibpm=0;ibpm<nbpm;ibpm++){
    cprx->cd(ibpm+1);
    pr->Draw(Form("diff_%sX/um:CodaEventNumber",bpm[ibpm].Data()),user_cut);
    latex.DrawLatex(0.15,0.85,Form("FFB: %s",FFB.Data()));
 }
 TCanvas* cpry = new TCanvas("cpry","BPMY vs TIME (pr tree)",700,900);
 cpry->Divide(1,nbpm);
 for(int ibpm=0;ibpm<nbpm;ibpm++){
   cpry->cd(ibpm+1);
   pr->Draw(Form("diff_%sY/um:CodaEventNumber",bpm[ibpm].Data()),user_cut);
   latex.DrawLatex(0.15,0.85,Form("FFB: %s",FFB.Data()));
 }
 cmulx->SaveAs(Form("./plots/temp/FFB_study_mulx_run%d.pdf",runNo));
 cmulx->SaveAs(Form("./plots/temp/FFB_study_muly_run%d.pdf",runNo));
 cmulx->SaveAs(Form("./plots/temp/FFB_study_pairx_run%d.pdf",runNo));
 cmulx->SaveAs(Form("./plots/temp/FFB_study_pairy_run%d.pdf",runNo));
 gSystem->Exec(Form("pdfunite ./plots/temp/FFB_study* ./plots/FFB_study_run%d.pdf",runNo));
 gSystem->Exec("rm -rf ./plots/temp/FFB_study*.pdf");
}

