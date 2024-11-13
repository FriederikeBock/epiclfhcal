#ifndef PLOTTHELPER_H
#define PLOTTHELPER_H

#include "TLegend.h"
#include "TAxis.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TBox.h"
#include "TPad.h"
#include "TFrame.h"
#include "TLatex.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TGaxis.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TileSpectra.h"  
  //__________________________________________________________________________________________________________
  //__________________________________________________________________________________________________________
  //__________________________________________________________________________________________________________
  TString ReturnDateStr(){
      TDatime today;
      int iDate           = today.GetDate();
      int iYear           = iDate/10000;
      int iMonth          = (iDate%10000)/100;
      int iDay            = iDate%100;
      return Form("%i_%02d_%02d",iYear, iMonth, iDay);
  }


  //__________________________________________________________________________________________________________
  // find bin with largest content
  //__________________________________________________________________________________________________________
  Double_t FindLargestBin1DHist(TH1* hist, Double_t minX = -10000, Double_t maxX = -10000 ){
    Double_t largestContent     = 0;
    if (!hist){
        std::cout << "histogram pointer was empty, skipping!" << std::endl;
        return 0.;
    }
    Int_t minBin = 1;
    Int_t maxBin = hist->GetNbinsX()+1;
    if (minX != -10000) minBin = hist->GetXaxis()->FindBin(minX);
    if (maxX != -10000) maxBin = hist->GetXaxis()->FindBin(maxX)+0.0001;
    for (Int_t i= minBin; i < maxBin; i++){
        if (largestContent < hist->GetBinContent(i)){
            largestContent = hist->GetBinContent(i);
        }
    }
    return largestContent;
  }
  //__________________________________________________________________________________________________________
  // find bin with largest content
  //__________________________________________________________________________________________________________
  Double_t FindBinWithLargestBin1DHist(TH1* hist, Double_t minX = -10000, Double_t maxX = -10000 ){
    Double_t largestContent     = 0;
    Int_t minBin = 1;
    Int_t maxBin = hist->GetNbinsX()+1;
    if (minX != -10000) minBin = hist->GetXaxis()->FindBin(minX);
    if (maxX != -10000) maxBin = hist->GetXaxis()->FindBin(maxX)+0.0001;
    Int_t largestBin = minBin;
    for (Int_t i= minBin; i < maxBin; i++){
      if (largestContent < hist->GetBinContent(i)){
        largestContent = hist->GetBinContent(i);
        largestBin = i;
      }
    }
    return largestContent;
  }

  //__________________________________________________________________________________________________________
  // find bin with smallest content
  //__________________________________________________________________________________________________________
  Double_t FindSmallestBin1DHist(TH1* hist, Double_t maxStart = 1e6 ){
    Double_t smallesContent     = maxStart;
    for (Int_t i= 0; i < hist->GetNbinsX(); i++){
      if (hist->GetBinContent(i) != 0 && smallesContent > hist->GetBinContent(i)){
        smallesContent = hist->GetBinContent(i);
      }
    }
    return smallesContent;
  }
  
  // ---------------------------- Function definiton --------------------------------------------------------------------------------------------
  // StyleSettingsBasics will make some standard settings for gStyle
  
  void StyleSettingsBasics( TString format = ""){
      //gStyle->SetOptTitle(kFALSE);
      gStyle->SetOptDate(0);   //show day and time
      gStyle->SetOptStat(0);  //show statistic
      gStyle->SetPalette(1,0);
      gStyle->SetFrameBorderMode(0);
      gStyle->SetFrameFillColor(0);
      gStyle->SetTitleFillColor(0);
      gStyle->SetTextSize(0.5);
      gStyle->SetLabelSize(0.03,"xyz");
      gStyle->SetLabelOffset(0.006,"xyz");
      gStyle->SetTitleFontSize(0.04);
      gStyle->SetTitleOffset(1,"y");
      gStyle->SetTitleOffset(0.7,"x");
      gStyle->SetCanvasColor(0);
      gStyle->SetPadTickX(1);
      gStyle->SetPadTickY(1);
      gStyle->SetLineWidth(1);

      gStyle->SetPadTopMargin(0.03);
      gStyle->SetPadBottomMargin(0.09);
      gStyle->SetPadRightMargin(0.03);
      gStyle->SetPadLeftMargin(0.13);


      TGaxis::SetMaxDigits(4);
      gErrorIgnoreLevel=kError;

      if (format.CompareTo("eps") == 0 ||format.CompareTo("pdf") == 0  ) gStyle->SetLineScalePS(1);
  }

  //__________________________________________________________________________________________________________
  void SetPlotStyle() {
  // 	const Int_t nRGBs = 7;
      const Int_t nRGBs = 5;
      const Int_t nCont = 255;

      Double_t stops[nRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
      Double_t red[nRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
      Double_t green[nRGBs] = { 0.31, 0.81, 1.00, 0.20, 0.00 };
      Double_t blue[nRGBs]  = { 0.51, 1., 0.12, 0.00, 0.00};

      TColor::CreateGradientColorTable(nRGBs, stops, red, green, blue, nCont);
      gStyle->SetNumberContours(nCont);
  }

  //__________________________________________________________________________________________________________
  void SetPlotStyleNConts(    Int_t nCont = 255) {
      const Int_t nRGBs = 5;
      Double_t stops[nRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
      Double_t red[nRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
      Double_t green[nRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
      Double_t blue[nRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
      TColor::CreateGradientColorTable(nRGBs, stops, red, green, blue, nCont);
      gStyle->SetNumberContours(nCont);
  }

  //__________________________________________________________________________________________________________
  void DrawCanvasSettings( TCanvas* c1,
                          Double_t leftMargin,
                          Double_t rightMargin,
                          Double_t topMargin,
                          Double_t bottomMargin){

      c1->SetTickx();
      c1->SetTicky();
      c1->SetGridx(0);
      c1->SetGridy(0);
      c1->SetLogy(0);
      c1->SetLeftMargin(leftMargin);
      c1->SetRightMargin(rightMargin);
      c1->SetTopMargin(topMargin);
      c1->SetBottomMargin(bottomMargin);
      c1->SetFillColor(0);
  }

  //__________________________________________________________________________________________________________
  TCanvas *GetAndSetCanvas( TString name,
                            Double_t leftmargin = 0.11,
                            Double_t bottommargin = 0.1,
                            Double_t x = 1400,
                            Double_t y = 1000){

      TCanvas *canvas =  new TCanvas(name,name,x,y);
      canvas->SetLeftMargin(leftmargin);
      canvas->SetRightMargin(0.015);
      canvas->SetTopMargin(0.03);
      canvas->SetBottomMargin(bottommargin);
      canvas->SetFillColor(0);

      return canvas;

  }

  //__________________________________________________________________________________________________________
  TLegend *GetAndSetLegend( Double_t positionX,
                          Double_t positionY,
                          Double_t entries,
                          Int_t Columns = 1,
                          TString header =""){

      if(header.CompareTo("") != 0) entries++;
      Double_t positionYPlus = 0.04*1.1*(Double_t)entries;
      TLegend *legend = new TLegend(positionX,positionY,positionX+(0.25*Columns),positionY+positionYPlus);
      legend->SetNColumns(Columns);
      legend->SetLineColor(0);
      legend->SetLineWidth(0);
      legend->SetFillColor(0);
      legend->SetFillStyle(0);
      legend->SetLineStyle(0);
      legend->SetTextSize(0.04);
      legend->SetTextFont(42);
      if(header.CompareTo("") != 0)legend->SetHeader(header);
      return legend;
  }
  
  //__________________________________________________________________________________________________________
  TLegend *GetAndSetLegend2(  Double_t positionX,
                              Double_t positionY,
                              Double_t positionXRight,
                              Double_t positionYUp,
                              Size_t textSize,
                              Int_t columns               = 1,
                              TString header              = "",
                              Font_t textFont             = 43,
                              Double_t margin             = 0
  ){

      TLegend *legend = new TLegend(positionX,positionY,positionXRight,positionYUp);
      legend->SetNColumns(columns);
      legend->SetLineColor(0);
      legend->SetLineWidth(0);
      legend->SetFillColor(0);
      legend->SetFillStyle(0);
      legend->SetLineStyle(0);
      legend->SetBorderSize(0);
      legend->SetTextFont(textFont);
      legend->SetTextSize(textSize);
      if (margin != 0) legend->SetMargin(margin);
      if (header.CompareTo("")!= 0) legend->SetHeader(header);
      return legend;
  }

  //__________________________________________________________________________________________________________
  void SetHistogramm( TH1 *hist,
                      TString xLabel,
                      TString yLabel,
                      Double_t rangeYlow  = -99.,
                      Double_t rangeYhigh = -99.,
                      Double_t xOffset    = 1.0,
                      Double_t yOffset    = 1.15,
                      Font_t font         = 42
  ){

      Double_t scale = 1./gPad->GetAbsHNDC();
      //hist->GetXaxis()->SetRangeUser(rangeX[0],rangeX[1]);
      if(rangeYlow != -99.) hist->GetYaxis()->SetRangeUser(rangeYlow,rangeYhigh);
      hist->SetTitle("");
      hist->SetXTitle(xLabel);
      hist->SetYTitle(yLabel);
      hist->GetYaxis()->SetDecimals();
      hist->GetYaxis()->SetTitleOffset(yOffset/scale);
      hist->GetXaxis()->SetTitleOffset(xOffset);
      hist->GetXaxis()->SetTitleSize(0.04*scale);
      hist->GetYaxis()->SetTitleSize(0.04*scale);
      hist->GetXaxis()->SetLabelSize(0.035*scale);
      hist->GetYaxis()->SetLabelSize(0.035*scale);
      hist->GetXaxis()->SetLabelFont(font);
      hist->GetYaxis()->SetLabelFont(font);
      hist->SetMarkerSize(1.);
      hist->SetMarkerStyle(20);
  }

  //__________________________________________________________________________________________________________
  void SetGraph( TGraph *graph,
                  TString xLabel,
                  TString yLabel,
                  Double_t rangeYlow = -99.,
                  Double_t rangeYhigh = -99.,
                  Double_t xOffset = 1.0,
                  Double_t yOffset = 1.15){

      Double_t scale = 1./gPad->GetAbsHNDC();
      //graph->GetXaxis()->SetRangeUser(rangeX[0],rangeX[1]);
      if(rangeYlow != -99.) graph->GetYaxis()->SetRangeUser(rangeYlow,rangeYhigh);
      graph->GetXaxis()->SetTitle(xLabel);
      graph->GetYaxis()->SetTitle(yLabel);
      graph->GetYaxis()->SetDecimals();
      graph->GetYaxis()->SetTitleOffset(yOffset/scale);
      graph->GetXaxis()->SetTitleOffset(xOffset);
      graph->GetXaxis()->SetTitleSize(0.04*scale);
      graph->GetYaxis()->SetTitleSize(0.04*scale);
      graph->GetXaxis()->SetLabelSize(0.035*scale);
      graph->GetYaxis()->SetLabelSize(0.035*scale);
      graph->GetXaxis()->SetLabelFont(42);
      graph->GetYaxis()->SetLabelFont(42);
      graph->SetMarkerSize(1.);
      graph->SetMarkerStyle(20);
  }

  //__________________________________________________________________________________________________________
  void SetMarkerDefaults(    TH1* histo1,
                              Style_t markerStyle,
                              Size_t markerSize,
                              Color_t markerColor,
                              Color_t lineColor,
                              Bool_t setFont = kTRUE) {
      histo1->SetMarkerStyle(markerStyle);
      histo1->SetMarkerSize(markerSize);
      histo1->SetMarkerColor(markerColor);
      histo1->SetLineColor(lineColor);
      if (setFont){
        histo1->GetYaxis()->SetLabelFont(42);
        histo1->GetXaxis()->SetLabelFont(42);
        histo1->GetYaxis()->SetTitleFont(62);
        histo1->GetXaxis()->SetTitleFont(62);
      }
  }
  //__________________________________________________________________________________________________________
  void SetMarkerDefaults(    TH1* histo1,
                              TString xtitle = "",
                              TString ytitle = "",
                              Style_t markerStyle = 20,
                              Size_t markerSize = 1,
                              Color_t markerColor = kBlack,
                              Color_t lineColor = kBlack,
                              double textsize = 0.045,
                              double labelsize = 0.045,
                              double xoffset = 1.,
                              double yoffset = 1. ) {
      histo1->SetTitle("");
      histo1->SetStats(0);
      histo1->SetMarkerStyle(markerStyle);
      histo1->SetMarkerSize(markerSize);
      histo1->SetMarkerColor(markerColor);
      histo1->SetLineColor(lineColor);
      histo1->GetYaxis()->SetLabelFont(42);
      histo1->GetXaxis()->SetLabelFont(42);
      histo1->GetYaxis()->SetLabelSize(labelsize);
      histo1->GetXaxis()->SetLabelSize(labelsize);
      histo1->GetYaxis()->SetTitleFont(62);
      histo1->GetXaxis()->SetTitleFont(62);
      histo1->GetYaxis()->SetTitleSize(textsize);
      histo1->GetXaxis()->SetTitleSize(textsize);
      if(!xtitle.EqualTo("")) histo1->GetXaxis()->SetTitle(xtitle);
      if(!ytitle.EqualTo("")) histo1->GetYaxis()->SetTitle(ytitle);
      histo1->GetXaxis()->SetTitleOffset(xoffset);
      histo1->GetYaxis()->SetTitleOffset(yoffset);
  }
  //__________________________________________________________________________________________________________
  void SetMarkerDefaultsProfile( TProfile* prof,
                                  Style_t markerStyle,
                                  Size_t markerSize,
                                  Color_t markerColor,
                                  Color_t lineColor ) {
      prof->SetMarkerStyle(markerStyle);
      prof->SetMarkerSize(markerSize);
      prof->SetMarkerColor(markerColor);
      prof->SetLineColor(lineColor);
      prof->GetYaxis()->SetLabelFont(42);
      prof->GetXaxis()->SetLabelFont(42);
      prof->GetYaxis()->SetTitleFont(62);
      prof->GetXaxis()->SetTitleFont(62);
  }

  //__________________________________________________________________________________________________________
  void SetLineDefaults(    TH1* histo1,
                              Int_t LineColor,
                              Int_t LineWidth,
                              Int_t LineStyle ) {
      histo1->SetLineColor(LineColor);
      histo1->SetLineWidth(LineWidth);
      histo1->SetLineStyle(LineStyle);
  }

  //__________________________________________________________________________________________________________
  void SetLineDefaultsTF1(  TF1*  Fit1,
                              Int_t LineColor,
                              Int_t LineWidth,
                              Int_t LineStyle ) {
      Fit1->SetLineColor(LineColor);
      Fit1->SetLineWidth(LineWidth);
      Fit1->SetLineStyle(LineStyle);
  }
  

    //__________________________________________________________________________________________________________
  void DefaultCancasSettings( TCanvas* c1,
                              Double_t leftMargin,
                              Double_t rightMargin,
                              Double_t topMargin,
                              Double_t bottomMargin){
      c1->SetTickx();
      c1->SetTicky();
      c1->SetGridx(0);
      c1->SetGridy(0);
      c1->SetLogy(0);
      c1->SetLeftMargin(leftMargin);
      c1->SetRightMargin(rightMargin);
      c1->SetTopMargin(topMargin);
      c1->SetBottomMargin(bottomMargin);
      c1->SetFillColor(0);
  }

  //__________________________________________________________________________________________________________
  void DefaultPadSettings( TPad* pad1,
                          Double_t leftMargin,
                          Double_t rightMargin,
                          Double_t topMargin,
                          Double_t bottomMargin){
      pad1->SetFillColor(0);
      pad1->GetFrame()->SetFillColor(0);
      pad1->SetBorderMode(0);
      pad1->SetLeftMargin(leftMargin);
      pad1->SetBottomMargin(bottomMargin);
      pad1->SetRightMargin(rightMargin);
      pad1->SetTopMargin(topMargin);
      pad1->SetTickx();
      pad1->SetTicky();
  }

  //__________________________________________________________________________________________________________
  void SetMarkerDefaultsTGraph(  TGraph* graph,
                                  Style_t markerStyle,
                                  Size_t markerSize,
                                  Color_t markerColor,
                                  Color_t lineColor,
                                  Width_t lineWidth       = 1,
                                  Style_t lineStyle       = 1,
                                  Bool_t boxes            = kFALSE,
                                  Color_t fillColor       = 0,
                                  Bool_t isHollow         = kFALSE
                                ) {
      graph->SetMarkerStyle(markerStyle);
      graph->SetMarkerSize(markerSize);
      graph->SetMarkerColor(markerColor);
      graph->SetLineColor(lineColor);
      graph->SetLineWidth(lineWidth);
      graph->SetLineWidth(lineStyle);
      if (boxes){
          graph->SetFillColor(fillColor);
          if (fillColor!=0){
              if (!isHollow){
                  graph->SetFillStyle(1001);
              } else {
                  graph->SetFillStyle(0);
              }
          } else {
              graph->SetFillStyle(0);
          }
      }
  }

  //__________________________________________________________________________________________________________
  void SetMarkerDefaultsTGraphErr(   TGraphErrors* graph,
                                      Style_t markerStyle,
                                      Size_t markerSize,
                                      Color_t markerColor,
                                      Color_t lineColor,
                                      Width_t lineWidth       = 1,
                                      Bool_t boxes            = kFALSE,
                                      Color_t fillColor       = 0,
                                      Bool_t isHollow         = kFALSE) {
      graph->SetMarkerStyle(markerStyle);
      graph->SetMarkerSize(markerSize);
      graph->SetMarkerColor(markerColor);
      graph->SetLineColor(lineColor);
      graph->SetLineWidth(lineWidth);
      if (boxes){
          graph->SetFillColor(fillColor);
          if (fillColor!=0){
              if (!isHollow){
                  graph->SetFillStyle(1001);
              } else {
                  graph->SetFillStyle(0);
              }
          } else {
              graph->SetFillStyle(0);
          }
      }
  }

  //__________________________________________________________________________________________________________
  void SetMarkerDefaultsTGraphAsym(  TGraphAsymmErrors* graph,
                                      Style_t markerStyle,
                                      Size_t markerSize,
                                      Color_t markerColor,
                                      Color_t lineColor,
                                      Width_t lineWidth   =1,
                                      Bool_t boxes        = kFALSE,
                                      Color_t fillColor   = 0,
                                      Bool_t isHollow     = kFALSE
                                    ) {
      if (!graph) return;
      graph->SetMarkerStyle(markerStyle);
      graph->SetMarkerSize(markerSize);
      graph->SetMarkerColor(markerColor);
      graph->SetLineColor(lineColor);
      graph->SetLineWidth(lineWidth);
      if (boxes){
          graph->SetFillColor(fillColor);
          if (fillColor!=0){
              if (!isHollow){
                  graph->SetFillStyle(1001);
              } else {
                  graph->SetFillStyle(0);
              }
          } else {
              graph->SetFillStyle(0);
          }
      }
  }

  //__________________________________________________________________________________________________________
  void SetMarkerDefaultsTF1( TF1* fit1,
                              Style_t lineStyle,
                              Size_t lineWidth,
                              Color_t lineColor ) {
      if (!fit1) return;
      fit1->SetLineColor(lineColor);
      fit1->SetLineStyle(lineStyle);
      fit1->SetLineWidth(lineWidth);
  }

  //__________________________________________________________________________________________________________
  void SetStyleTLatex( TLatex* text,
                      Size_t textSize,
                      Width_t lineWidth,
                      Color_t textColor = 1,
                      Font_t textFont = 42,
                      Bool_t kNDC = kTRUE,
                      Short_t align = 11
                  ){
      if (kNDC) {text->SetNDC();}
      text->SetTextFont(textFont);
      text->SetTextColor(textColor);
      text->SetTextSize(textSize);
      text->SetLineWidth(lineWidth);
      text->SetTextAlign(align);
  }

  //__________________________________________________________________________________________________________
  void DrawLatex(const double  PosX = 0.5, const double  PosY = 0.5, TString text = "", const bool alignRight = false, const double TextSize = 0.044, const int font = 42, const double dDist = 0.05, const int color = 1){

      std::vector<TString> Latex;
      
      TObjArray *textStr = text.Tokenize(";");
      for(Int_t i = 0; i<textStr->GetEntries() ; i++){
          TObjString* tempObj     = (TObjString*) textStr->At(i);
          Latex.push_back( tempObj->GetString());
      }
      for(unsigned int i = 0; i < Latex.size(); ++i){
          TLatex l(PosX, PosY - i*dDist, Latex[i]);
          l.SetNDC();
          l.SetTextFont(font);
          l.SetTextColor(color);
          l.SetTextSize(TextSize);
          if(alignRight) l.SetTextAlign(31);
          l.DrawClone("same");
      }
  }

  //__________________________________________________________________________________________________________
  void SetStyleHisto( TH1* histo,
                      Width_t lineWidth,
                      Style_t lineStyle,
                      Color_t lineColor) {
      if (!histo) return;
      histo->SetLineWidth(lineWidth);
      histo->SetLineStyle(lineStyle);
      histo->SetLineColor(lineColor);
  }

  //__________________________________________________________________________________________________________
  void SetStyleFit(   TF1* fit,
                      Double_t xRangeStart,
                      Double_t xRangeEnd,
                      Width_t lineWidth,
                      Style_t lineStyle,
                      Color_t lineColor) {
      if (!fit) return;
      fit->SetRange(xRangeStart,xRangeEnd);
      fit->SetLineWidth(lineWidth);
      fit->SetLineStyle(lineStyle);
      fit->SetLineColor(lineColor);
  }

  //__________________________________________________________________________________________________________
  void SetStyleHistoTH2ForGraphs( TH2* histo,
                                  TString XTitle,
                                  TString YTitle,
                                  Size_t xLableSize,
                                  Size_t xTitleSize,
                                  Size_t yLableSize,
                                  Size_t yTitleSize,
                                  Float_t xTitleOffset    = 1,
                                  Float_t yTitleOffset    = 1,
                                  Int_t xNDivisions       = 510,
                                  Int_t yNDivisions       = 510,
                                  Font_t textFontLabel    = 42,
                                  Font_t textFontTitle    = 62
                                ){
      histo->SetXTitle(XTitle);
      histo->SetYTitle(YTitle);
      histo->SetTitle("");
      histo->SetStats(0);

      histo->GetXaxis()->SetLabelFont(textFontLabel);
      histo->GetYaxis()->SetLabelFont(textFontLabel);
      histo->GetXaxis()->SetTitleFont(textFontTitle);
      histo->GetYaxis()->SetTitleFont(textFontTitle);

      histo->GetXaxis()->SetLabelSize(xLableSize);
      histo->GetXaxis()->SetTitleSize(xTitleSize);
      histo->GetXaxis()->SetTitleOffset(xTitleOffset);
      histo->GetXaxis()->SetNdivisions(xNDivisions,kTRUE);

      histo->GetYaxis()->SetDecimals();
      histo->GetYaxis()->SetLabelSize(yLableSize);
      histo->GetYaxis()->SetTitleSize(yTitleSize);
      histo->GetYaxis()->SetTitleOffset(yTitleOffset);
      histo->GetYaxis()->SetNdivisions(yNDivisions,kTRUE);
  }

  //__________________________________________________________________________________________________________
  void SetStyleHistoTH1ForGraphs( TH1* histo,
                                  TString XTitle,
                                  TString YTitle,
                                  Size_t xLableSize,
                                  Size_t xTitleSize,
                                  Size_t yLableSize,
                                  Size_t yTitleSize,
                                  Float_t xTitleOffset    = 1,
                                  Float_t yTitleOffset    = 1,
                                  Int_t xNDivisions       = 510,
                                  Int_t yNDivisions       = 510,
                                  Font_t textFontLabel    = 42,
                                  Font_t textFontTitle    = 62
                                ){
      histo->SetXTitle(XTitle);
      histo->SetYTitle(YTitle);
      histo->SetTitle("");

      histo->GetYaxis()->SetLabelFont(textFontLabel);
      histo->GetXaxis()->SetLabelFont(textFontLabel);
      histo->GetYaxis()->SetTitleFont(textFontTitle);
      histo->GetXaxis()->SetTitleFont(textFontTitle);

      histo->GetXaxis()->SetLabelSize(xLableSize);
      histo->GetXaxis()->SetTitleSize(xTitleSize);
      histo->GetXaxis()->SetTitleOffset(xTitleOffset);
      histo->GetXaxis()->SetNdivisions(xNDivisions,kTRUE);

      histo->GetYaxis()->SetDecimals();
      histo->GetYaxis()->SetLabelSize(yLableSize);
      histo->GetYaxis()->SetTitleSize(yTitleSize);
      histo->GetYaxis()->SetTitleOffset(yTitleOffset);
      histo->GetYaxis()->SetNdivisions(yNDivisions,kTRUE);
  }

  /* // DrawLines will draw the lines in the histogram for you
  * startX - starting point of drawing in x
  * endX - end point of drawing in x
  * startY -starting point of drawing in y
  * endY - end point of drawing in y
  * linew - line width
  */
  void DrawLines(Float_t startX, Float_t endX,
                  Float_t startY, Float_t endY,
                  Float_t linew, Float_t lineColor = 4, Style_t lineStyle = 1, Float_t opacity = 1.){
      TLine * l1 = new TLine (startX,startY,endX,endY);
      l1->SetLineColor(lineColor);
      l1->SetLineWidth(linew);
      l1->SetLineStyle(lineStyle);
      if (opacity != 1.)
          l1->SetLineColorAlpha(lineColor,opacity);

      l1->Draw("same");
  }

  //********************************************************************************************************************************
  //********************************************************************************************************************************
  //********************************************************************************************************************************
  TBox* CreateBox(Color_t colorBox, Double_t xStart, Double_t yStart, Double_t xEnd, Double_t yEnd, Style_t fillStyle = 1001 ) {
      TBox* box = new TBox(xStart ,yStart , xEnd, yEnd);
      box->SetLineColor(colorBox);
      box->SetFillColor(colorBox);
      box->SetFillStyle(fillStyle);
      return box;
  }
  
  
  
  //********************************************************************************************************************************
  //********************** Returns default labeling strings  ***********************************************************************
  //********************************************************************************************************************************    
  TString GetStringFromRunInfo(RunInfo currRunInfo, Int_t option = 1){
      if (option == 1){
          if (currRunInfo.species.Contains("cosmics")){
              return  Form("cosmics, Run %d, #it{V}_{#it{op}} = %1.1f V", currRunInfo.runNr, currRunInfo.vop  );
          } else if (currRunInfo.species.CompareTo("g") == 0){
              return  Form("LED, Run %d, #it{V}_{#it{op}} = %1.1f V", currRunInfo.runNr, currRunInfo.vop  );
          } else {
              return  Form("%s-beam, #it{E}_{#it{b}}= %.0f GeV, Run %d, #it{V}_{#it{op}} = %1.1f V", currRunInfo.species.Data(), currRunInfo.energy, currRunInfo.runNr, currRunInfo.vop  );
          }
      } else if (option == 2){
          if (currRunInfo.species.CompareTo("cosmics") == 0){
              return  "cosmics";
          } else if (currRunInfo.species.CompareTo("g") == 0){
              return  "LED";
          } else {
              return  Form("%s-beam, #it{E}_{#it{b}}= %.0f GeV", currRunInfo.species.Data(), currRunInfo.energy);
          }
      } else if (option == 3){
          return Form("Run %d, #it{V}_{#it{op}} = %1.1f V", currRunInfo.runNr, currRunInfo.vop  )   ;
      } else if (option == 4){
          if (currRunInfo.species.CompareTo("cosmics") == 0){
              return Form("cosmics, Run %d, #it{V}_{#it{op}} = %1.1f V, HG = %1d, LG = %1d", currRunInfo.runNr, currRunInfo.vop, currRunInfo.hgSet, currRunInfo.lgSet);
          } else if (currRunInfo.species.CompareTo("g") == 0){
              return Form("LED, Run %d, #it{V}_{#it{op}} = %1.1f V, HG = %1d, LG = %1d", currRunInfo.runNr, currRunInfo.vop, currRunInfo.hgSet, currRunInfo.lgSet);
          } else{
              return Form("%s-beam, #it{E}_{#it{b}}= %.0f GeV, Run %d, #it{V}_{#it{op}} = %1.1f V, HG = %1d, LG = %1d", currRunInfo.species.Data(), currRunInfo.energy, currRunInfo.runNr, currRunInfo.vop, currRunInfo.hgSet, currRunInfo.lgSet);
          }
      } else if (option == 5){
          return Form("pedestal, Run %d, #it{V}_{#it{op}} = %1.1f V", currRunInfo.runNr, currRunInfo.vop  )   ;
      }
      
      return "";
  }


  
  //__________________________________________________________________________________________________________
  // Plot 2D fit variables overview
  //__________________________________________________________________________________________________________  
  void PlotSimple2D( TCanvas* canvas2D, TH2* hist, Int_t maxy, Int_t maxx, Float_t textSizeRel, TString nameOutput, RunInfo currRunInfo, int labelOpt = 1, Bool_t hasNeg = kFALSE, TString drwOpt ="colz"  ){
      canvas2D->cd();
      SetStyleHistoTH2ForGraphs( hist, hist->GetXaxis()->GetTitle(), hist->GetYaxis()->GetTitle(), 0.85*textSizeRel, textSizeRel, 0.85*textSizeRel, textSizeRel,0.9, 1.05);  
      hist->GetZaxis()->SetLabelSize(0.85*textSizeRel);
      hist->GetZaxis()->SetTitleOffset(1.05);
      hist->GetZaxis()->SetTitleSize(textSizeRel);
      // if (hist->GetYaxis()->GetTitle().CompareTo("") != 0)
        
      if (maxy > -10000)hist->GetYaxis()->SetRangeUser(-0.5,maxy+0.1);
      if (maxx > -10000)hist->GetXaxis()->SetRangeUser(0.5,maxx+0.1);
      if (!hasNeg)
        hist->GetZaxis()->SetRangeUser(hist->GetMinimum(0),hist->GetMaximum());
      else 
        hist->GetZaxis()->SetRangeUser(hist->GetMinimum(),hist->GetMaximum());
      hist->Draw(drwOpt.Data());
      
      DrawLatex(0.85, 0.92, GetStringFromRunInfo(currRunInfo,labelOpt), true, textSizeRel, 42);

    canvas2D->SaveAs(nameOutput.Data());
  }

  //__________________________________________________________________________________________________________
  void ReturnCorrectValuesForCanvasScaling(   Int_t sizeX,
                                              Int_t sizeY,
                                              Int_t nCols,
                                              Int_t nRows,
                                              Double_t leftMargin,
                                              Double_t rightMargin,
                                              Double_t upperMargin,
                                              Double_t lowerMargin,
                                              Double_t* arrayBoundariesX,
                                              Double_t* arrayBoundariesY,
                                              Double_t* relativeMarginsX,
                                              Double_t* relativeMarginsY,
                                              Bool_t verbose = kTRUE){
      Int_t realsizeX             = sizeX- (Int_t)(sizeX*leftMargin)- (Int_t)(sizeX*rightMargin);
      Int_t realsizeY             = sizeY- (Int_t)(sizeY*upperMargin)- (Int_t)(sizeY*lowerMargin);

      Int_t nPixelsLeftColumn     = (Int_t)(sizeX*leftMargin);
      Int_t nPixelsRightColumn    = (Int_t)(sizeX*rightMargin);
      Int_t nPixelsUpperColumn    = (Int_t)(sizeY*upperMargin);
      Int_t nPixelsLowerColumn    = (Int_t)(sizeY*lowerMargin);

      Int_t nPixelsSinglePlotX    = (Int_t) (realsizeX/nCols);
      Int_t nPixelsSinglePlotY    = (Int_t) (realsizeY/nRows);
      if(verbose){
          std::cout << realsizeX << "\t" << nPixelsSinglePlotX << std::endl;
          std::cout << realsizeY << "\t" << nPixelsSinglePlotY << std::endl;
          std::cout << nPixelsLeftColumn << "\t" << nPixelsRightColumn  << "\t" << nPixelsLowerColumn << "\t" << nPixelsUpperColumn << std::endl;
      }
      Int_t pixel = 0;
      if(verbose)std::cout << "boundaries X" << std::endl;
      for (Int_t i = 0; i < nCols+1; i++){
          if (i == 0){
              arrayBoundariesX[i] = 0.;
              pixel = pixel+nPixelsLeftColumn+nPixelsSinglePlotX;
          } else if (i == nCols){
              arrayBoundariesX[i] = 1.;
              pixel = pixel+nPixelsRightColumn;
          } else {
              arrayBoundariesX[i] = (Double_t)pixel/sizeX;
              pixel = pixel+nPixelsSinglePlotX;
          }
          if(verbose)std::cout << "arrayBoundariesX: " << i << "\t" << arrayBoundariesX[i] << "\t" << pixel<<std::endl;
      }

      if(verbose)std::cout << "boundaries Y" << std::endl;
      pixel = sizeY;
      for (Int_t i = 0; i < nRows+1; i++){
          if (i == 0){
              arrayBoundariesY[i] = 1.;
              pixel = pixel-nPixelsUpperColumn-nPixelsSinglePlotY;
          } else if (i == nRows){
              arrayBoundariesY[i] = 0.;
              pixel = pixel-nPixelsLowerColumn;
          } else {
              arrayBoundariesY[i] = (Double_t)pixel/sizeY;
              pixel = pixel-nPixelsSinglePlotY;
          }
          if(verbose)std::cout << i << "\t" << arrayBoundariesY[i] <<"\t" << pixel<<std::endl;
      }

      relativeMarginsX[0]         = (Double_t)nPixelsLeftColumn/(nPixelsLeftColumn+nPixelsSinglePlotX);
      relativeMarginsX[1]         = 0;
      relativeMarginsX[2]         = (Double_t)nPixelsRightColumn/(nPixelsRightColumn+nPixelsSinglePlotX);;

      relativeMarginsY[0]         = (Double_t)nPixelsUpperColumn/(nPixelsUpperColumn+nPixelsSinglePlotY);
      relativeMarginsY[1]         = 0;
      relativeMarginsY[2]         = (Double_t)nPixelsLowerColumn/(nPixelsLowerColumn+nPixelsSinglePlotY);;

      return;
  }

  //__________________________________________________________________________________________________________
  void ReturnCorrectValuesTextSize(   TPad * pad,
                                      Double_t &textsizeLabels,
                                      Double_t &textsizeFac,
                                      Int_t textSizeLabelsPixel,
                                      Double_t dummyWUP){
      if(dummyWUP){}

      textsizeLabels = 0;
      textsizeFac = 0;
      if (pad->XtoPixel(pad->GetX2()) < pad->YtoPixel(pad->GetY1())){
          textsizeLabels = (Double_t)textSizeLabelsPixel/pad->XtoPixel(pad->GetX2()) ;
          textsizeFac = (Double_t)1./pad->XtoPixel(pad->GetX2()) ;
      } else {
          textsizeLabels = (Double_t)textSizeLabelsPixel/pad->YtoPixel(pad->GetY1());
          textsizeFac = (Double_t)1./pad->YtoPixel(pad->GetY1());
      }
      std::cout << textsizeLabels << std::endl;
      std::cout << textsizeFac << std::endl;

      return;

  }

  //********************************************************************************************************************************
  //******** CreateCanvasAndPadsFor8PannelTBPlot ***********************************************************************************
  //********************************************************************************************************************************
  void CreateCanvasAndPadsFor8PannelTBPlot(TCanvas* &canvas, TPad* pads[8],  Double_t* topRCornerX, Double_t* topRCornerY,  Double_t* relSize8P, Int_t textSizePixel = 30){
    Double_t arrayBoundsXIndMeasRatio[5];
    Double_t arrayBoundsYIndMeasRatio[3];
    Double_t relativeMarginsIndMeasRatioX[3];
    Double_t relativeMarginsIndMeasRatioY[3];
    ReturnCorrectValuesForCanvasScaling(2200,1200, 4, 2,0.03, 0.005, 0.005,0.05,arrayBoundsXIndMeasRatio,arrayBoundsYIndMeasRatio,relativeMarginsIndMeasRatioX,relativeMarginsIndMeasRatioY);

    canvas = new TCanvas("canvas8Panel","",0,0,2200,1200);  // gives the page size
    canvas->cd();

    //*****************************************************************
    // Test beam geometry (beam coming from viewer)
    //===========================================================
    //||    8 (4)    ||    7 (5)   ||    6 (6)   ||    5 (7)   ||  row 0
    //===========================================================
    //||    1 (0)    ||    2 (1)   ||    3 (2)   ||    4 (3)   ||  row 1
    //===========================================================
    //    col 0     col 1       col 2     col  3
    // rebuild pad geom in similar way (numbering -1)
    //*****************************************************************
    
    pads[0] = new TPad("pad8Panel_0", "", arrayBoundsXIndMeasRatio[0], arrayBoundsYIndMeasRatio[2], arrayBoundsXIndMeasRatio[1], arrayBoundsYIndMeasRatio[1],-1, -1, -2);
    pads[1] = new TPad("pad8Panel_1", "", arrayBoundsXIndMeasRatio[1], arrayBoundsYIndMeasRatio[2], arrayBoundsXIndMeasRatio[2], arrayBoundsYIndMeasRatio[1],-1, -1, -2);
    pads[2] = new TPad("pad8Panel_2", "", arrayBoundsXIndMeasRatio[2], arrayBoundsYIndMeasRatio[2], arrayBoundsXIndMeasRatio[3], arrayBoundsYIndMeasRatio[1],-1, -1, -2);
    pads[3] = new TPad("pad8Panel_3", "", arrayBoundsXIndMeasRatio[3], arrayBoundsYIndMeasRatio[2], arrayBoundsXIndMeasRatio[4], arrayBoundsYIndMeasRatio[1],-1, -1, -2);
    pads[4] = new TPad("pad8Panel_4", "", arrayBoundsXIndMeasRatio[0], arrayBoundsYIndMeasRatio[1],arrayBoundsXIndMeasRatio[1], arrayBoundsYIndMeasRatio[0],-1, -1, -2);
    pads[5] = new TPad("pad8Panel_5", "", arrayBoundsXIndMeasRatio[1], arrayBoundsYIndMeasRatio[1],arrayBoundsXIndMeasRatio[2], arrayBoundsYIndMeasRatio[0],-1, -1, -2);
    pads[6] = new TPad("pad8Panel_6", "", arrayBoundsXIndMeasRatio[2], arrayBoundsYIndMeasRatio[1],arrayBoundsXIndMeasRatio[3], arrayBoundsYIndMeasRatio[0],-1, -1, -2);
    pads[7] = new TPad("pad8Panel_7", "", arrayBoundsXIndMeasRatio[3], arrayBoundsYIndMeasRatio[1],arrayBoundsXIndMeasRatio[4], arrayBoundsYIndMeasRatio[0],-1, -1, -2);
    
    DefaultPadSettings( pads[4], relativeMarginsIndMeasRatioX[0], relativeMarginsIndMeasRatioX[1], relativeMarginsIndMeasRatioY[0], relativeMarginsIndMeasRatioY[1]);
    DefaultPadSettings( pads[5], relativeMarginsIndMeasRatioX[1], relativeMarginsIndMeasRatioX[1], relativeMarginsIndMeasRatioY[0], relativeMarginsIndMeasRatioY[1]);
    DefaultPadSettings( pads[6], relativeMarginsIndMeasRatioX[1], relativeMarginsIndMeasRatioX[1], relativeMarginsIndMeasRatioY[0], relativeMarginsIndMeasRatioY[1]);
    DefaultPadSettings( pads[7], relativeMarginsIndMeasRatioX[1], relativeMarginsIndMeasRatioX[2], relativeMarginsIndMeasRatioY[0], relativeMarginsIndMeasRatioY[1]);
    DefaultPadSettings( pads[0], relativeMarginsIndMeasRatioX[0], relativeMarginsIndMeasRatioX[1], relativeMarginsIndMeasRatioY[1], relativeMarginsIndMeasRatioY[2]);
    DefaultPadSettings( pads[1], relativeMarginsIndMeasRatioX[1], relativeMarginsIndMeasRatioX[1], relativeMarginsIndMeasRatioY[1], relativeMarginsIndMeasRatioY[2]);
    DefaultPadSettings( pads[2], relativeMarginsIndMeasRatioX[1], relativeMarginsIndMeasRatioX[1], relativeMarginsIndMeasRatioY[1], relativeMarginsIndMeasRatioY[2]);
    DefaultPadSettings( pads[3], relativeMarginsIndMeasRatioX[1], relativeMarginsIndMeasRatioX[2], relativeMarginsIndMeasRatioY[1], relativeMarginsIndMeasRatioY[2]);
    
    topRCornerX[0]  = 1-relativeMarginsIndMeasRatioX[1];
    topRCornerY[0]  = 1-relativeMarginsIndMeasRatioY[1];
    topRCornerX[1]  = 1-relativeMarginsIndMeasRatioX[1];
    topRCornerY[1]  = 1-relativeMarginsIndMeasRatioY[1];
    topRCornerX[2]  = 1-relativeMarginsIndMeasRatioX[1];
    topRCornerY[2]  = 1-relativeMarginsIndMeasRatioY[1];
    topRCornerX[3]  = 1-relativeMarginsIndMeasRatioX[2];
    topRCornerY[3]  = 1-relativeMarginsIndMeasRatioY[1];
    topRCornerX[4]  = 1-relativeMarginsIndMeasRatioX[2];
    topRCornerY[4]  = 1-relativeMarginsIndMeasRatioY[0];
    topRCornerX[5]  = 1-relativeMarginsIndMeasRatioX[1];
    topRCornerY[5]  = 1-relativeMarginsIndMeasRatioY[0];
    topRCornerX[6]  = 1-relativeMarginsIndMeasRatioX[1];
    topRCornerY[6]  = 1-relativeMarginsIndMeasRatioY[0];
    topRCornerX[7]  = 1-relativeMarginsIndMeasRatioX[1];
    topRCornerY[7]  = 1-relativeMarginsIndMeasRatioY[0];
    
    for (Int_t p = 0; p < 8; p++){
      if (pads[p]->XtoPixel(pads[p]->GetX2()) < pads[p]->YtoPixel(pads[p]->GetY1())){
        relSize8P[p]  = (Double_t)textSizePixel/pads[p]->XtoPixel(pads[p]->GetX2()) ;
      } else {
        relSize8P[p]  = (Double_t)textSizePixel/pads[p]->YtoPixel(pads[p]->GetY1());
      }
      std::cout << p << "\t" << topRCornerX[p]<< "\t" << topRCornerY[p] << "\t" << relSize8P[p] << std::endl;
    }
    return;
  }
  
  
  //__________________________________________________________________________________________________________
  // Plot Noise with Fits for Full layer
  //__________________________________________________________________________________________________________
  void PlotNoiseWithFitsFullLayer (TCanvas* canvas8Panel, TPad* pads[8], Double_t* topRCornerX,  Double_t* topRCornerY, Double_t* relSize8P, Int_t textSizePixel, 
                                  std::map<int,TileSpectra> spectra, Setup* setupT, bool isHG, 
                                  Double_t xPMin, Double_t xPMax, Double_t scaleYMax, int layer, int mod,  TString nameOutput, RunInfo currRunInfo){
                                  
    Double_t maxY = 0;
    std::map<int, TileSpectra>::iterator ithSpectra;
    
    int nRow = setupT->GetNMaxRow()+1;
    int nCol = setupT->GetNMaxColumn()+1;
    
    for (int r = 0; r < nRow; r++){
      for (int c = 0; c < nCol; c++){
        int tempCellID = setupT->GetCellID(r,c, layer, mod);
        ithSpectra=spectra.find(tempCellID);
        if(ithSpectra==spectra.end()){
          std::cout << "WARNING: skipping cell ID: " << tempCellID << "\t row " << r << "\t column " << c << "\t layer " << layer << "\t module " << mod << std::endl;
          continue;
        } 
        TH1D* tempHist = nullptr;
        if (isHG){
          tempHist = ithSpectra->second.GetHG();
        } else {
          tempHist = ithSpectra->second.GetLG();
        }
        if (maxY < FindLargestBin1DHist(tempHist, xPMin , xPMax)) maxY = FindLargestBin1DHist(tempHist, xPMin , xPMax);
      }  
    }
    
    for (int r = 0; r < nRow; r++){
      for (int c = 0; c < nCol; c++){
        canvas8Panel->cd();
        int tempCellID = setupT->GetCellID(r,c, layer, mod);
        int p = setupT->GetChannelInLayer(tempCellID);
        pads[p]->Draw();
        pads[p]->cd();
        pads[p]->SetLogy();
        ithSpectra=spectra.find(tempCellID);
        if(ithSpectra==spectra.end()){
          std::cout << "WARNING: skipping cell ID: " << tempCellID << "\t row " << r << "\t column " << c << "\t layer " << layer << "\t module " << mod << std::endl;
          continue;
        } 
        TH1D* tempHist = nullptr;
        if (isHG){
            tempHist = ithSpectra->second.GetHG();
        } else {
            tempHist = ithSpectra->second.GetLG();
        }
        SetStyleHistoTH1ForGraphs( tempHist, tempHist->GetXaxis()->GetTitle(), tempHist->GetYaxis()->GetTitle(), 0.85*textSizePixel, textSizePixel, 0.85*textSizePixel, textSizePixel,0.9, 1.1, 510, 510, 43, 63);  
        SetMarkerDefaults(tempHist, 20, 1, kBlue+1, kBlue+1, kFALSE);   
        tempHist->GetXaxis()->SetRangeUser(xPMin,xPMax);
        tempHist->GetYaxis()->SetRangeUser(0.7,scaleYMax*maxY);
        
        tempHist->Draw("pe");
        
        TString label           = Form("row %d col %d", r, c);
        if (p == 7){
          label = Form("row %d col %d layer %d", r, c, layer);
        }
        TLatex *labelChannel    = new TLatex(topRCornerX[p]-0.04,topRCornerY[p]-1.2*relSize8P[p],label);
        SetStyleTLatex( labelChannel, 0.85*textSizePixel,4,1,43,kTRUE,31);

        
        TF1* fit = nullptr;
        if (isHG){
          fit = ithSpectra->second.GetBackModel(1);
        } else {
          fit = ithSpectra->second.GetBackModel(0);  
        }
        if (fit){
          SetStyleFit(fit , 0, 400, 7, 7, kBlack);
          fit->Draw("same");
          TLegend* legend = GetAndSetLegend2( topRCornerX[p]-8*relSize8P[p], topRCornerY[p]-4*0.85*relSize8P[p]-0.4*relSize8P[p], topRCornerX[p]-0.04, topRCornerY[p]-0.6*relSize8P[p],0.85*textSizePixel, 1, label, 43,0.2);
          legend->AddEntry(fit, "Gauss noise fit", "l");
          legend->AddEntry((TObject*)0, Form("#mu = %2.2f #pm %2.2f",fit->GetParameter(1), fit->GetParError(1) ) , " ");
          legend->AddEntry((TObject*)0, Form("#sigma = %2.2f #pm %2.2f",fit->GetParameter(2), fit->GetParError(2) ) , " ");
          legend->Draw();
            
        } else {
          labelChannel->Draw();  
        }
      
        if (p ==7 ){
          DrawLatex(topRCornerX[p]-0.04, topRCornerY[p]-4*0.85*relSize8P[p]-1.4*relSize8P[p], GetStringFromRunInfo(currRunInfo, 2), true, 0.85*relSize8P[p], 42);
          DrawLatex(topRCornerX[p]-0.04, topRCornerY[p]-4*0.85*relSize8P[p]-2.2*relSize8P[p], GetStringFromRunInfo(currRunInfo, 3), true, 0.85*relSize8P[p], 42);
        }
      }
    }
    canvas8Panel->SaveAs(nameOutput.Data());
  }


  
#endif
