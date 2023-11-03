//This file plots the TGraphs of MPV values found from the LanGau fit
void LanGauPlots()
{

    std::string file0 = "/Users/ar2545/Downloads/LanGauFitData/47.root";
    std::string file1 = "/Users/ar2545/Downloads/LanGauFitData/48.root";
    std::string file2 = "/Users/ar2545/Downloads/LanGauFitData/50.root";
    std::string file3 = "/Users/ar2545/Downloads/LanGauFitData/51.root";
    std::string file4 = "/Users/ar2545/Downloads/LanGauFitData/52.root";
    std::string file5 = "/Users/ar2545/Downloads/LanGauFitData/53.root";
    std::string file6 = "/Users/ar2545/Downloads/LanGauFitData/54.root";
//    std::string file2 = "/Users/ar2545/Downloads/LanGauData/122.root";
    
    TFile* fsyspp0;
    TFile* fsyspp1;
    TFile* fsyspp2;
    TFile* fsyspp3;
    TFile* fsyspp4;
    TFile* fsyspp5;
    TFile* fsyspp6;
   

    fsyspp0 = new TFile(file0.c_str());
    fsyspp1 = new TFile(file1.c_str());
    fsyspp2 = new TFile(file2.c_str());
    fsyspp3 = new TFile(file3.c_str());
    fsyspp4 = new TFile(file4.c_str());
    fsyspp5 = new TFile(file5.c_str());
    fsyspp6 = new TFile(file6.c_str());
    
    int Voltage = 57;
    string VoV;
    map<int,string> VtoVov = {{58,"5.5 VoV"},{57.5,"5.5 VoV"},{57,"4.5 VoV"},{56.5,"4 VoV"},{56,"3.5 VoV"}};
    auto noiseIt = VtoVov.find(Voltage);
    VoV = noiseIt->second;
    
    TH1D* mpv0 = (TH1D*)fsyspp0->Get("mpv_hist");
    TH1D* mpv1 = (TH1D*)fsyspp1->Get("mpv_hist");
//    TH1D* mpv2 = (TH1D*)fsyspp2->Get("mpv_hist");
    
    TGraphAsymmErrors* g_mpv0 = (TGraphAsymmErrors*)fsyspp0->Get("MPV");
    g_mpv0->SetFillStyle(1001);
    g_mpv0->SetFillColorAlpha(kGray+3, 0.3);
    g_mpv0->SetLineColor(kBlack);
    g_mpv0->SetLineWidth(2);
    
    TGraphAsymmErrors*  g_mpv1 = (TGraphAsymmErrors*)fsyspp1->Get("MPV");
    g_mpv1->SetFillStyle(1001);
    g_mpv1->SetFillColorAlpha(kGray+3, 0.3);
    g_mpv1->SetLineColor(kBlack);
    g_mpv1->SetLineWidth(2);
    
    TGraphAsymmErrors* g_mpv2 = (TGraphAsymmErrors*)fsyspp2->Get("MPV");
    g_mpv2->SetFillStyle(1001);
    g_mpv2->SetFillColorAlpha(kGray+3, 0.3);
    g_mpv2->SetLineColor(kBlack);
    g_mpv2->SetLineWidth(2);
    
    TGraphAsymmErrors*  g_mpv3 = (TGraphAsymmErrors*)fsyspp3->Get("MPV");
    g_mpv3->SetFillStyle(1001);
    g_mpv3->SetFillColorAlpha(kGray+3, 0.3);
    g_mpv3->SetLineColor(kBlack);
    g_mpv3->SetLineWidth(2);
    
    TGraphAsymmErrors*  g_mpv4 = (TGraphAsymmErrors*)fsyspp4->Get("MPV");
    g_mpv4->SetFillStyle(1001);
    g_mpv4->SetFillColorAlpha(kGray+3, 0.3);
    g_mpv4->SetLineColor(kBlack);
    g_mpv4->SetLineWidth(2);
    
    TGraphAsymmErrors*  g_mpv5 = (TGraphAsymmErrors*)fsyspp5->Get("MPV");
    g_mpv5->SetFillStyle(1001);
    g_mpv5->SetFillColorAlpha(kGray+3, 0.3);
    g_mpv5->SetLineColor(kBlack);
    g_mpv5->SetLineWidth(2);
    
    TGraphAsymmErrors*  g_mpv6 = (TGraphAsymmErrors*)fsyspp6->Get("MPV");
    g_mpv6->SetFillStyle(1001);
    g_mpv6->SetFillColorAlpha(kGray+3, 0.3);
    g_mpv6->SetLineColor(kBlack);
    g_mpv6->SetLineWidth(2);

//g->SetMarkerStyle()
g_mpv0->SetLineWidth(4);g_mpv0->SetFillStyle(1001);g_mpv0->SetFillColorAlpha(kGreen+4, 0.3);g_mpv0->SetLineColor(kGreen+2);
g_mpv1->SetLineWidth(4);g_mpv1->SetFillStyle(1001);g_mpv1->SetFillColorAlpha(kRed+2, 0.3);g_mpv1->SetLineColor(kRed);
g_mpv2->SetLineWidth(4);g_mpv2->SetFillStyle(1001);g_mpv2->SetFillColorAlpha(kMagenta+2, 0.3);g_mpv2->SetLineColor(kMagenta);
g_mpv3->SetLineWidth(4);g_mpv3->SetFillStyle(1001);g_mpv3->SetFillColorAlpha(kGray+2, 0.3);g_mpv3->SetLineColor(kGray);
g_mpv4->SetLineWidth(4);g_mpv4->SetFillStyle(1001);g_mpv4->SetFillColorAlpha(kAzure+2, 0.3);g_mpv4->SetLineColor(kAzure);
g_mpv5->SetLineWidth(4);g_mpv5->SetFillStyle(1001);g_mpv5->SetFillColorAlpha(kOrange+8, 0.3);g_mpv5->SetLineColor(kOrange+6);
g_mpv6->SetLineWidth(4);g_mpv6->SetFillStyle(1001);g_mpv6->SetFillColorAlpha(kCyan-7, 0.3);g_mpv6->SetLineColor(kCyan);

    TCanvas* c = new TCanvas("c", "c", 850, 600);
//    TCanvas* c1 = new TCanvas("c", "c", 850, 600);
    gStyle->SetOptStat(0);
    g_mpv0->GetYaxis()->SetRangeUser(-2000,2000);
    g_mpv0->Draw();
    g_mpv1->Draw("SAME");
    g_mpv2->Draw("SAME");
    g_mpv3->Draw("SAME");
    g_mpv4->Draw("SAME");
    g_mpv5->Draw("SAME");
//    g_mpv6->Draw("SAME");

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    //  TLegend* leg = new TLegend(0.58, 0.65, 0.68, 0.85);
    TLegend* leg = new TLegend(0.20, 0.20, 0.42, 0.40);
    //  TLegend* leg = new TLegend(0.15, 0.50, 0.30, 0.62);
    leg->SetFillColor(0);
    leg->SetBorderSize(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.035);
    // leg->SetNDC(true);
    
    TLegend* leg1 = new TLegend(0.50, 0.82, 0.72, 0.90);
    //  TLegend* leg = new TLegend(0.15, 0.50, 0.30, 0.62);
    leg1->SetFillColor(0);
    leg1->SetBorderSize(0);
    leg1->SetTextFont(42);
    leg1->SetTextSize(0.12);
    
    TLegend* leg2 = new TLegend(0.20, 0.82, 0.42, 0.90);
    //  TLegend* leg = new TLegend(0.15, 0.50, 0.30, 0.62);
    leg2->SetFillColor(0);
    leg2->SetBorderSize(0);
    leg2->SetTextFont(42);
    leg2->SetTextSize(0.12);
    
    TLatex* lat = new TLatex();
    lat->SetTextFont(42);
    lat->SetNDC(true);
    lat->SetTextSize(0.035);
    //  lat->SetTextAlign(33);
    
    TLatex* lat1 = new TLatex();
    lat1->SetTextFont(42);
    lat1->SetNDC(true);
    lat1->SetTextSize(0.035);
    lat1->SetTextAlign(1);
    
    //Writing particle track cut range on plots
    TLatex latex1;
    latex1.SetNDC ();
    latex1.SetTextFont(42);
    const char *str1 = "#it{#pi}";
    latex1.SetTextSize(0.035);
    
    TLatex latex2;
    latex2.SetNDC ();
    latex2.SetTextFont(42);
    latex2.SetTextSize(0.035);
    //Writing collision energy
    TLatex latex3;
    latex3.SetNDC();
    latex3.SetTextFont(42);
    const char *str3 = "#sqrt{#it{s}} = 13 TeV";
    latex3.SetTextSize(0.035);

//$$$$$$$$$$$$$$$$$$$$$$$$$$ plotting finishes$$$$$$$$$$$$$$$$$$$$$$$$$$$$

    latex1.DrawLatex(0.15,0.80 ,Form("Landau-Gauss Fit"));
    latex2.DrawLatex(0.15,0.85 ,Form("%s, Species: %s",VoV.c_str(),str1));
    //
    std::stringstream ss;
    ss.str("");
    

//      leg->AddEntry(g_mpv0,"54 V");
//      leg->AddEntry(g_mpv1,"55 V");
      
      leg->AddEntry(g_mpv0,"300 GeV");
      leg->AddEntry(g_mpv1,"250 GeVV");
      leg->AddEntry(g_mpv2,"200 GeV");
      leg->AddEntry(g_mpv3,"150 GeV");
      leg->AddEntry(g_mpv4,"100 GeV");
      leg->AddEntry(g_mpv5,"80 GeV");
//      leg->AddEntry(g_mpv6,"60 GeV");
      leg->Draw("same");
    
//    ss << "Pion_VoltageScan300GeV.pdf";
//    c->SaveAs(ss.str().c_str());
}



