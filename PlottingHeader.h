/***********************************************************************************************
*** provided by,                                                                          ******
***     Friederike Bock, fbock@cern.ch                                                    ******
************************************************************************************************/

#ifndef PLOTTINGGENERAL
#define PLOTTINGGENERAL

    #include <TStyle.h>
    #include <TGaxis.h>
    #include <TCanvas.h>
    #include <TLegend.h>
    #include <TFrame.h>
    #include "TArrow.h"
    #include "TMarker.h"
    #include "TFile.h"
    #include "TMinuit.h"
    #include "TRandom.h"
    #include "TRandom3.h"
    #include "TSystem.h"
    #include "TObjString.h"
    #ifndef __CLING__
        #include <Riostream.h>
        #include <TLatex.h>
    #endif
    #include <TProfile.h>

    struct runInfo{
        runInfo(): runNr(0), species(""), energy(0), vop(0), lgSet(0), hgSet(0), assemblyNr(0){}
        int runNr;
        TString species;
        float energy;
        float vop;
        int lgSet;
        int hgSet;
        int assemblyNr;
    } ;
    using namespace std; // necessary for non-ROOT compilation

    /************************************************************************************************
    ************************************************************************************************
    * This header was created to make things easier with making plots for quick plotting.
    It offers you several functions for drawing and styling your histogramms.
    ************************************************************************************************
    ************************************************************************************************/

    //__________________________________________________________________________________________________________
    //__________________ Read run infos from text file _________________________________________________________
    //__________________________________________________________________________________________________________    
    // specialData: 0 - std. TB, 1 - SPE data ORNL
    std::vector<runInfo> readRunInfosFromFile(TString runListFileName, Int_t debug, Int_t specialData = 0 ){
        std::vector<runInfo> runs;
        std::cout << "INFO: You have given the following run list file: " << runListFileName.Data() << std::endl;
        ifstream runListFile;
        runListFile.open(runListFileName,ios_base::in);
        if (!runListFile) {
            std::cout << "ERROR: file " << runListFileName.Data() << " not found!" << std::endl;
            return runs;
        }

        for( TString tempLine; tempLine.ReadLine(runListFile, kTRUE); ) {
            // check if line should be considered
            if (tempLine.BeginsWith("%") || tempLine.BeginsWith("#")){
                continue;
            }
            if (debug > 0) std::cout << tempLine.Data() << std::endl;

            // Separate the string according to tabulators
            TObjArray *tempArr  = tempLine.Tokenize("\t");
            if(tempArr->GetEntries()<1){
                if (debug > 1) std::cout << "nothing to be done" << std::endl;
                delete tempArr;
                continue;
            } else if (tempArr->GetEntries() == 1 ){
                // Separate the string according to space
                tempArr       = tempLine.Tokenize(" ");
                if(tempArr->GetEntries()<1){
                    if (debug > 1) std::cout << "nothing to be done" << std::endl;
                    delete tempArr;
                    continue;
                } else if (tempArr->GetEntries() == 1  ) {
                    if (debug > 1) std::cout << ((TString)((TObjString*)tempArr->At(0))->GetString()).Data() << " has not been reset, no value given!" << std::endl;
                    delete tempArr;
                    continue;
                }
            }

            // Put them to the correct variables    
            runInfo tempRun;
            tempRun.runNr    = ((TString)((TObjString*)tempArr->At(0))->GetString()).Atoi();
            tempRun.species  = (TString)((TObjString*)tempArr->At(1))->GetString();
            tempRun.energy   = ((TString)((TObjString*)tempArr->At(2))->GetString()).Atof();
            tempRun.vop      = ((TString)((TObjString*)tempArr->At(3))->GetString()).Atof();
            tempRun.hgSet    = ((TString)((TObjString*)tempArr->At(4))->GetString()).Atoi();
            tempRun.lgSet    = ((TString)((TObjString*)tempArr->At(5))->GetString()).Atoi();
            if (specialData == 1) tempRun.assemblyNr = ((TString)((TObjString*)tempArr->At(6))->GetString()).Atoi();
                
            if (debug > 0) std::cout << "Run " << tempRun.runNr << "\t species: " << tempRun.species << "\t energy: "  << tempRun.energy << "\t Vop: " << tempRun.vop << std::endl;
            runs.push_back(tempRun);
        }
        std::cout << "registered " << runs.size() << std::endl;
        return runs;
    }
    
    //__________________________________________________________________________________________________________
    //__________________________________________________________________________________________________________
    //__________________________________________________________________________________________________________
    Int_t findCurrentRun(std::vector<runInfo> runs, Int_t run){
        Int_t currRun = 0;
        while (runs.at(currRun).runNr != run && currRun < (Int_t)runs.size()) currRun++;
        if (currRun == (Int_t)runs.size()) return -1;
        else return currRun;
    }
    
    //__________________________________________________________________________________________________________
    //__________________________________________________________________________________________________________
    //__________________________________________________________________________________________________________
    runInfo GetRunInfoObject(std::vector<runInfo> runs, Int_t runIndex){
        runInfo run;
        run.runNr = runs.at(runIndex).runNr;
        run.species = runs.at(runIndex).species;
        run.energy = runs.at(runIndex).energy;
        run.vop = runs.at(runIndex).vop;
        run.lgSet = runs.at(runIndex).lgSet;
        run.hgSet = runs.at(runIndex).hgSet;
        run.assemblyNr = runs.at(runIndex).assemblyNr;
        return run;
    }
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


    
    // ---------------------------- Function definiton --------------------------------------------------------------------------------------------
    // StyleSettingsThesis will make some standard settings for gStyle
    
    void StyleSettingsThesis( TString format = ""){
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
    /* StyleSettings will make some standard settings for gStyle
    */
    void StyleSettings(){
        //gStyle->SetOptTitle(kFALSE);
        gStyle->SetOptDate(0);   //show day and time
        gStyle->SetOptStat(0);  //show statistic
        gStyle->SetPalette(1,0);
        gStyle->SetFrameBorderMode(0);
        gStyle->SetFrameFillColor(0);
        gStyle->SetTitleFillColor(0);
        gStyle->SetTextSize(0.5);
        gStyle->SetLabelSize(0.03,"xyz");
        gStyle->SetLabelOffset(0.002,"xyz");
        gStyle->SetTitleFontSize(0.04);
        gStyle->SetTitleOffset(1,"y");
        gStyle->SetTitleOffset(0.7,"x");
        gStyle->SetCanvasColor(0);
        gStyle->SetPadTickX(1);
        gStyle->SetPadTickY(1);
        gStyle->SetLineWidth(1);

        gStyle->SetPadTopMargin(0.1);
        gStyle->SetPadBottomMargin(0.1);
        gStyle->SetPadRightMargin(0.08);
        gStyle->SetPadLeftMargin(0.12);

        gErrorIgnoreLevel=kError;

        TGaxis::SetMaxDigits(3);
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

    // 	Double_t stops[nRGBs] = {  0.34, 0.61, 0.84, 1.00 };
    // 	Double_t red[nRGBs]   = {  0.00, 0.87, 1.00, 0.51 };
    // 	Double_t green[nRGBs] = {  0.81, 1.00, 0.20, 0.00 };
    // // 	Double_t blue[nRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    // 	Double_t blue[nRGBs]  = {  1., 0.00, 0.00, 0.00};

    // 	Double_t blue[nRGBs]  = { 1.00, 0.97, 0.97, 0.00, 0.00, 0.00, 0.00 };
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
    // GammaScalingHistogram will scale the histogram by "Factor"
    void ScalingHistogram(TH1 *histo, Double_t Factor){
        histo->Sumw2();
        histo->Scale(Factor);
    }

    //__________________________________________________________________________________________________________
    // GammaScalingHistogram will scale the histogram by "Factor"
    void ScalingHistogram(TH2 *histo, Double_t Factor){
        histo->Sumw2();
        histo->Scale(Factor);
    }

    //__________________________________________________________________________________________________________
    void StylingSliceHistos(TH1 *histo, Float_t markersize){
        histo->SetMarkerStyle(20);
        histo->SetMarkerSize(markersize);
    }

    //__________________________________________________________________________________________________________
    void DivideTH1ByBinWidth(TH1 *histo){
        histo->Sumw2();
        for (Int_t i = 1; i < histo->GetNbinsX()+1; i++){
            histo->SetBinContent(i,histo->GetBinContent(i)/histo->GetXaxis()->GetBinWidth(i));
            histo->SetBinError(i,histo->GetBinError(i)/histo->GetXaxis()->GetBinWidth(i));
        }
    }
    //__________________________________________________________________________________________________________
    void DivideTH2ByBinWidth(TH2 *histo){
        histo->Sumw2();
        for (Int_t i = 1; i < histo->GetNbinsX()+1; i++){
          for (Int_t j = 1; j < histo->GetNbinsY()+1; j++){
              histo->SetBinContent(i,j,histo->GetBinContent(i,j)/histo->GetXaxis()->GetBinWidth(i)/histo->GetXaxis()->GetBinWidth(i));
              histo->SetBinError(i,j,histo->GetBinError(i,j)/histo->GetXaxis()->GetBinWidth(i)/histo->GetXaxis()->GetBinWidth(i));
          }
        }
    }
    //__________________________________________________________________________________________________________
    void RebinWithBinCorrection(TH1 *histo, Int_t rebinFactor, Int_t bin = 3){
        histo->Sumw2();
        histo->Rebin(rebinFactor);
        Double_t binWidth= histo->GetXaxis()->GetBinWidth(bin);
        for (Int_t i = 1; i < histo->GetNbinsX()+1; i++){
            histo->SetBinContent(i,histo->GetBinContent(i)/binWidth);
            histo->SetBinError(i,histo->GetBinError(i)/binWidth);
        }
    }

    //__________________________________________________________________________________________________________
    void RebinWithBinCorrection2D(TH2 *histo, Int_t rebinFactor1, Int_t rebinFactor2, Int_t bin = 3){
    // 	histo->Sumw2();
        histo->Rebin2D(rebinFactor1,rebinFactor2);
        Double_t binWidthY= histo->GetYaxis()->GetBinWidth(bin);
        Double_t binWidthX= histo->GetXaxis()->GetBinWidth(bin);
        histo->Scale(1/binWidthY*1/binWidthX);
    }

    //__________________________________________________________________________________________________________
    void RebinWithBinCorrection2DSumw2(TH2 *histo, Int_t rebinFactor1, Int_t rebinFactor2, Int_t bin = 3){
        histo->Sumw2();
        histo->Rebin2D(rebinFactor1,rebinFactor2);
        Double_t binWidthY= histo->GetYaxis()->GetBinWidth(bin);
        Double_t binWidthX= histo->GetXaxis()->GetBinWidth(bin);
        histo->Scale(1/binWidthY*1/binWidthX);
    }

    //__________________________________________________________________________________________________________
    /* DrawAutoHisto is function used for styling a histograma of the gamma conversion group with standart settings
    * histo1 - first histogram (Data)
    * Title - histogram title
    * XTitle - X-axis title
    * YTitle - Y-axis title
    * YRangeMax 	= kTRUE will scale by Maximum and Minimum Range in Y
    *YMaxFactor - will MaximumY by this factor if YRangeMay = kTRUE
    *YMinimum - this will be used if YRangeMax is set
    *YRange  	= kTRUE will Cut y-axis by YMin and YMax
    - will be set to kFAlSE if YRangeMax is set
    *YMin - minimum Y
    *YMax - maximum Y
    *XRange 	= kTRUE will Cut x-axis by XMin and XMax
    *XMin - minimum Y
    *XMax - maximum Y
    */
    void DrawAutoHisto(    TH1* histo1,
                                TString Title,
                                TString XTitle,
                                TString YTitle,
                                Bool_t YRangeMax,
                                Double_t YMaxFactor,
                                Double_t YMinimum,
                                Bool_t YRange,
                                Double_t YMin,
                                Double_t YMax,
                                Bool_t XRange,
                                Double_t XMin,
                                Double_t XMax,
                                Double_t yOffset=1.,
                                Double_t dummyWUP=1.) {
        if (dummyWUP){}
        if (YRangeMax && !XRange){
            YRange = kFALSE;
            Double_t maxRangeR = histo1->GetMaximum();
            Double_t minRangeR = histo1->GetMinimum();
            if(YMinimum > minRangeR){minRangeR = YMinimum;}
            histo1->GetYaxis()->SetRangeUser(minRangeR, maxRangeR*YMaxFactor);
        }
        if (YRangeMax && XRange){
            YRange = kFALSE;
            Double_t maxRangeR = histo1->GetMaximum();
            Double_t minRangeR = histo1->GetMinimum();
            if(YMinimum > minRangeR){minRangeR = YMinimum;}
            histo1->GetYaxis()->SetRangeUser(minRangeR, maxRangeR*YMaxFactor);
            histo1->GetXaxis()->SetRangeUser(XMin, XMax);
        }
        if (YRange && XRange){
            histo1->GetYaxis()->SetRangeUser(YMin, YMax);
            histo1->GetXaxis()->SetRangeUser(XMin, XMax);
        }
        if (!YRangeMax && !YRange && XRange){
            histo1->GetXaxis()->SetRangeUser(XMin, XMax);
        }

        if (YRange && !XRange){
            histo1->GetYaxis()->SetRangeUser(YMin, YMax);
        }


        histo1->SetTitle(Title.Data());

        if(XTitle.CompareTo("") != 0){
            histo1->SetXTitle(XTitle.Data());
        }
        if(YTitle.CompareTo("") != 0){
            histo1->SetYTitle(YTitle.Data());
        }

        histo1->GetYaxis()->SetLabelFont(42);
        histo1->GetXaxis()->SetLabelFont(42);
        histo1->GetYaxis()->SetTitleFont(62);
        histo1->GetXaxis()->SetTitleFont(62);
        histo1->GetYaxis()->SetLabelSize(0.035);
        histo1->GetYaxis()->SetTitleSize(0.043);
        histo1->GetYaxis()->SetDecimals();
        histo1->GetYaxis()->SetTitleOffset(yOffset);
        histo1->GetXaxis()->SetTitleSize(0.043);
        histo1->GetXaxis()->SetLabelSize(0.035);
        histo1->DrawCopy("e,hist");
    }

    //__________________________________________________________________________________________________________
    /*DrawAutoHisto2D is a function for drawing a 2D-histogram of the gamma conversion group
    * histo - histogramm which need to be drawn
    * Title - histogram title
    * XTitle - X- axis-title
    * YTitle - Y-axis-title
    * Input - Legend
    * YRange - if kTRUE will scale by YMin and YMay
    * YMin  - Y minimum
    * YMax - Y maximum
    * XRange - if kTRUE will scale by XMin and XMax
    * XMin - X minimum
    * XMax - X maximum
    */
    void DrawAutoHisto2D(  TH2 *histo,
                                TString Title,
                                TString XTitle,
                                TString YTitle,
                                TString Input,
                                Bool_t YRange,
                                Float_t YMin,
                                Float_t YMax,
                                Bool_t XRange,
                                Float_t XMin,
                                Float_t XMax,
                                Double_t titleOffsetX = 1.2,
                                Double_t titleOffsetY = 1.4,
                                Size_t labelSizeX = 0.035,
                                Size_t titleSizeX = 0.043,
                                Size_t labelSizeY = 0.035,
                                Size_t titleSizeY = 0.043){


        if (YRange && XRange){
            histo->GetYaxis()->SetRangeUser(YMin, YMax);
            histo->GetXaxis()->SetRangeUser(XMin, XMax);
        }
        if ( !YRange && XRange){
            histo->GetXaxis()->SetRangeUser(XMin, XMax);
        }

        if (YRange && !XRange){
            histo->GetYaxis()->SetRangeUser(YMin, YMax);
        }

        if(Title.CompareTo("") != 0){
            histo->SetTitle(Title.Data());
        }
        if(XTitle.CompareTo("") != 0){
            histo->SetXTitle(XTitle.Data());
        }
        if(YTitle.CompareTo("") != 0){
            histo->SetYTitle(YTitle.Data());
        }
        histo->GetYaxis()->SetLabelFont(42);
        histo->GetXaxis()->SetLabelFont(42);
        histo->GetYaxis()->SetTitleFont(62);
        histo->GetXaxis()->SetTitleFont(62);

        histo->GetYaxis()->SetLabelSize(labelSizeY);
        histo->GetYaxis()->SetTitleSize(titleSizeY);
        histo->GetYaxis()->SetTitleOffset(titleOffsetY);
        histo->GetYaxis()->SetDecimals();

        histo->GetXaxis()->SetLabelSize(labelSizeX);
        histo->GetXaxis()->SetTitleSize(titleSizeX);
        histo->GetXaxis()->SetTitleOffset(titleOffsetX);
        histo->DrawCopy("colz");
        if(Input.CompareTo("") != 0){
            TLegend* leg2 = new TLegend(0.6,0.82,0.83,0.9);
            leg2->SetTextSize(0.04);
            leg2->SetFillColor(0);
            leg2->AddEntry(histo,(Input.Data()));
            leg2->Draw("same");
        }
    }

    //__________________________________________________________________________________________________________
    /* DrawRatioHisto is function used for styling the ratio-histograms of the gamma conversion group
    * histo1 - histogram
    * Title - histogram title
    * XTitle - X-axis title
    * YTitle - Y-axis title
    * YRangeMax 	= kTRUE will scale by Maximum and Minimum Range in Y
    *YMaxFactor - will MaximumY by this factor if YRangeMay = kTRUE
    *YMinimum - this will be used if YRangeMax is set
    *YRange  	= kTRUE will Cut y-axis by YMin and YMax
    - will be set to kFAlSE if YRangeMax is set
    *YMin - minimum Y
    *YMax - maximum Y
    *XRange 	= kTRUE will Cut x-axis by XMin and XMax
    *XMin - minimum Y
    *XMax - maximum Y
    */
    void DrawRatioHisto(   TH1* histo1,
                                TString Title,
                                TString XTitle,
                                TString YTitle,
                                Bool_t YRangeMax,
                                Float_t YMaxFactor,
                                Float_t YMinimum,
                                Bool_t YRange,
                                Float_t YMin,
                                Float_t YMax,
                                Bool_t XRange,
                                Float_t XMin,
                                Float_t XMax) {
        if (YRangeMax && !XRange){
            YRange = kFALSE;
            Double_t maxRangeR = histo1->GetMaximum();
            Double_t minRangeR = histo1->GetMinimum();
            if(YMinimum > minRangeR){minRangeR = YMinimum;}
            histo1->GetYaxis()->SetRangeUser(minRangeR, maxRangeR*YMaxFactor);
        }
        if (YRangeMax && XRange){
            YRange = kFALSE;
            Double_t maxRangeR = histo1->GetMaximum();
            Double_t minRangeR = histo1->GetMinimum();
            if(YMinimum > minRangeR){minRangeR = YMinimum;}
            histo1->GetYaxis()->SetRangeUser(minRangeR, maxRangeR*YMaxFactor);
            histo1->GetXaxis()->SetRangeUser(XMin, XMax);
        }
        if (YRange && XRange){
            histo1->GetYaxis()->SetRangeUser(YMin, YMax);
            histo1->GetXaxis()->SetRangeUser(XMin, XMax);
        }
        if (!YRangeMax && !YRange && XRange){
            histo1->GetXaxis()->SetRangeUser(XMin, XMax);
        }

        if (YRange && !XRange){
            histo1->GetYaxis()->SetRangeUser(YMin, YMax);
        }

        if(Title.CompareTo("") != 0){	histo1->SetTitle(Title.Data());
        }else{	histo1->SetTitle("");}

        if(XTitle.CompareTo("") != 0){
            histo1->SetXTitle(XTitle.Data());
        }
        if(YTitle.CompareTo("") != 0){
            histo1->SetYTitle(YTitle.Data());
        }
        histo1->GetYaxis()->SetLabelFont(42);
        histo1->GetXaxis()->SetLabelFont(42);
        histo1->GetYaxis()->SetTitleFont(62);
        histo1->GetXaxis()->SetTitleFont(62);

        histo1->GetYaxis()->SetTitleSize(0.04);
        histo1->GetYaxis()->SetLabelSize(0.03);
        histo1->GetYaxis()->SetDecimals();
        histo1->GetYaxis()->SetTitleOffset(0.9);
        histo1->GetXaxis()->SetTitleOffset(0.9);
        histo1->GetXaxis()->SetTitleSize(0.04);
        histo1->GetXaxis()->SetLabelSize(0.03);
        histo1->SetLineColor(kBlue-5);
        histo1->SetMarkerStyle(20);
        histo1->SetMarkerSize(0.5);
        histo1->DrawCopy("hist,e");
        histo1->DrawCopy("same,p");
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
        
        double ddistance = dDist;
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
            cout << realsizeX << "\t" << nPixelsSinglePlotX << endl;
            cout << realsizeY << "\t" << nPixelsSinglePlotY << endl;
            cout << nPixelsLeftColumn << "\t" << nPixelsRightColumn  << "\t" << nPixelsLowerColumn << "\t" << nPixelsUpperColumn << endl;
        }
        Int_t pixel = 0;
        if(verbose)cout << "boundaries X" << endl;
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
            if(verbose)cout << "arrayBoundariesX: " << i << "\t" << arrayBoundariesX[i] << "\t" << pixel<<endl;
        }

        if(verbose)cout << "boundaries Y" << endl;
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
            if(verbose)cout << i << "\t" << arrayBoundariesY[i] <<"\t" << pixel<<endl;
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
        cout << textsizeLabels << endl;
        cout << textsizeFac << endl;

        return;

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
      //==============================================
      //||    8    ||    7    ||    6    ||    5    ||
      //==============================================
      //||    1    ||    2    ||    3    ||    4    ||
      //==============================================
      // rebuild pad geom in similar way (numbering -1)
      //*****************************************************************
      pads[7] = new TPad("pad8Panel_7", "", arrayBoundsXIndMeasRatio[0], arrayBoundsYIndMeasRatio[1],arrayBoundsXIndMeasRatio[1], arrayBoundsYIndMeasRatio[0],-1, -1, -2);
      pads[6] = new TPad("pad8Panel_6", "", arrayBoundsXIndMeasRatio[1], arrayBoundsYIndMeasRatio[1],arrayBoundsXIndMeasRatio[2], arrayBoundsYIndMeasRatio[0],-1, -1, -2);
      pads[5] = new TPad("pad8Panel_5", "", arrayBoundsXIndMeasRatio[2], arrayBoundsYIndMeasRatio[1],arrayBoundsXIndMeasRatio[3], arrayBoundsYIndMeasRatio[0],-1, -1, -2);
      pads[4] = new TPad("pad8Panel_4", "", arrayBoundsXIndMeasRatio[3], arrayBoundsYIndMeasRatio[1],arrayBoundsXIndMeasRatio[4], arrayBoundsYIndMeasRatio[0],-1, -1, -2);
      pads[0] = new TPad("pad8Panel_0", "", arrayBoundsXIndMeasRatio[0], arrayBoundsYIndMeasRatio[2], arrayBoundsXIndMeasRatio[1], arrayBoundsYIndMeasRatio[1],-1, -1, -2);
      pads[1] = new TPad("pad8Panel_1", "", arrayBoundsXIndMeasRatio[1], arrayBoundsYIndMeasRatio[2], arrayBoundsXIndMeasRatio[2], arrayBoundsYIndMeasRatio[1],-1, -1, -2);
      pads[2] = new TPad("pad8Panel_2", "", arrayBoundsXIndMeasRatio[2], arrayBoundsYIndMeasRatio[2], arrayBoundsXIndMeasRatio[3], arrayBoundsYIndMeasRatio[1],-1, -1, -2);
      pads[3] = new TPad("pad8Panel_3", "", arrayBoundsXIndMeasRatio[3], arrayBoundsYIndMeasRatio[2], arrayBoundsXIndMeasRatio[4], arrayBoundsYIndMeasRatio[1],-1, -1, -2);
      
      DefaultPadSettings( pads[7], relativeMarginsIndMeasRatioX[0], relativeMarginsIndMeasRatioX[1], relativeMarginsIndMeasRatioY[0], relativeMarginsIndMeasRatioY[1]);
      DefaultPadSettings( pads[6], relativeMarginsIndMeasRatioX[1], relativeMarginsIndMeasRatioX[1], relativeMarginsIndMeasRatioY[0], relativeMarginsIndMeasRatioY[1]);
      DefaultPadSettings( pads[5], relativeMarginsIndMeasRatioX[1], relativeMarginsIndMeasRatioX[1], relativeMarginsIndMeasRatioY[0], relativeMarginsIndMeasRatioY[1]);
      DefaultPadSettings( pads[4], relativeMarginsIndMeasRatioX[1], relativeMarginsIndMeasRatioX[2], relativeMarginsIndMeasRatioY[0], relativeMarginsIndMeasRatioY[1]);
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

    //********************************************************************************************************************************
    //********************** Returns default labeling strings  ***********************************************************************
    //********************************************************************************************************************************    
    TString GetStringFromRunInfo(runInfo currRunInfo, Int_t option = 1){
        if (option == 1){
            if (currRunInfo.species.CompareTo("cosmics") == 0){
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
        }
        
        return "";
    }
    
#endif
