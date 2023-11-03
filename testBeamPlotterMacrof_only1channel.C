#include <TROOT.h>
#include <TString.h>
#include <TObject.h>
#include <TObjString.h>
#include <TSystem.h>
#include <TChain.h>
#include <TMath.h>
#include <TVector3.h>
#include <iostream>
#include <fstream>
#include <TParticlePDG.h>
#include <TDatabasePDG.h>
#include <algorithm>

#include <TCanvas.h>
#include <TPad.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TH2.h>
#include <TH3.h>
#include <TFile.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TString.h>
#include <TDatime.h>
#include <TF1.h>
#include <TF2.h>
#include <THStack.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TGraphAsymmErrors.h>
#include <TLine.h>
#include <TLatex.h>
#include <TArrow.h>
#include <TGraphErrors.h>
#include <TGaxis.h>
#include <TLegend.h>
#include <TFrame.h>
#include <TLorentzVector.h>


const int gMaxChannels = 64;
const int gMaxBoard    = 2;
using namespace std;
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
    if(verbose)std::cout << i << "\t" << arrayBoundariesX[i] << "\t" << pixel<<std::endl;
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

TH1D* GetHist(TFile* inputTfile, TH1D *hist, char* histName){
    
    //TFile *inputTfile = new TFile(inputfile);
    if (inputTfile){
        //std::cout <<" Opened things properly"<<std::endl;
        hist= (TH1D*)inputTfile->Get(histName);
        //hist->Draw("hist");
        
    }
    else{
        std::cout<<"Shit didn't open"<< std::endl;
        
    }
    return hist;
    
}


std::vector<string> testParserTStrings(string fileName)
{
    
    //string str = "1,2,3,4,5,6";
    vector<string> v;
 
    stringstream ss(fileName);
 
    while (ss.good()) {
        string substr;
        getline(ss, substr, ',');
        v.push_back(substr);
    }
    return v;
    //for (size_t i = 0; i < v.size(); i++)
      //  cout << v[i] << endl;
}

int getIndex(vector<int> v, int K)
{
    auto it = find(v.begin(), v.end(), K);
  
    // If element was found
    if (it != v.end())
    {
      
        // calculating the index
        // of K
        int index = it - v.begin();
        return index;
    }
    else {
        // If the element is not
        // present in the vector
        cout<<"Couldn't find index" <<endl;
        return 99999999;
    }
}
void setLegend(TLegend* leg, int color, int BorderSize, int FontSize, float TextSize, TH1D* hist, string label)
{
    leg->SetFillColor(color);
    leg->SetBorderSize(BorderSize);
    leg->SetTextFont(FontSize);
    leg->SetTextSize(TextSize);
    leg->AddEntry(hist,label.c_str());
    leg->Draw();
    
}
void graphingHistOnPad(TCanvas* c, Int_t i, Int_t j, std::vector <int> board,  TFile* inputfile, string assembly,TH1D* hist,int color,float y_max,bool iflog, int rebinfac, float xrange, TLegend* leg,string label,string settings, string histPrefix){
    
    int Index = getIndex(board,i)+1;
    if (Index == 100000000) throw std::exception();
    int padInd;
    if (Index >4 )padInd = 9;
    if (Index <5)padInd=-4;
    c->cd(abs(padInd-(Index)));
    //printf(" found channel %d in vector %s in channel %d and Pad index %d\n",i,assembly.c_str(),Index,abs(padInd-(Index-1)));
    hist=GetHist(inputfile, hist,Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i));
    hist->RebinX(rebinfac);
    
    hist->SetStats(0);
    hist->SetLineColor(color);
//    hist->SetLineWidth(2);
    hist->Scale(1./hist->Integral());
    hist->SetMaximum(y_max*1.1);
    
    hist->GetXaxis()->SetRangeUser(0.001,xrange);
    hist->GetXaxis()->SetLabelSize(0.04);
    hist->GetXaxis()->SetNdivisions(5);
    //hist->GetXaxis()->SetLabelOffset(0.500001);
    if(iflog==true){gPad->SetLogy();}
    //gStyle->SetTitleFontSize(.08);
     //gStyle->SetLabelSize(.05, "XY");
    hist->GetXaxis()->SetTitleSize(0.05);
    hist->GetXaxis()->SetLabelSize(0.052);
    hist->GetYaxis()->SetLabelSize(0.052);
    hist->GetXaxis()->SetTitleOffset(1.01);
    hist->GetYaxis()->SetTitleOffset(0.5000009);
    hist->GetYaxis()->SetTitleSize(0.05);

//    hist->GetXaxis()->SetLabelOffset(1.01);
//    hist->GetYaxis()->SetTitle("Entries");
    hist->Draw("SAME Hist");
    if (abs(padInd-(Index)) ==4) setLegend(leg,0,0,45,10,hist,label.c_str());
    if (abs(padInd-(Index)) ==8)  hist->GetXaxis()->SetTitle("ADC");
    if (abs(padInd-(Index)) ==1) {
        hist->GetYaxis()->SetTitle("Entries");
        TLatex latex11;
        latex11.SetNDC ();
        latex11.SetTextSize(0.06);
        c->cd(1);
        latex11.DrawLatex(0.65,0.85 ,Form("%s %s",assembly.c_str(), settings.c_str()));
        }
        
       // gPad->SetTickx(2);
    }

void graphingChannelSorted(TCanvas* c, std::vector <pair<int,int>> channel,  TFile* inputfile, string assembly,TH1D* hist,int color,bool iflog,float y_max, int rebinfac, float xrange, TLegend* leg,string label, string settings,string histPrefix){
    //int Index = getIndex(board,i)+1;
   //if (Index == 100000000) throw std::exception();
    //int padInd;
//    if (Index >4 )padInd = 9;
//    if (Index <5)padInd=-4;
    
    
    //wanna loop thru all the layers here!!
    for (Int_t i=0; i<channel.size();i++){
        c->cd();
        //c->cd(i+1);
        hist=GetHist(inputfile, hist,Form("%s_HG_B%d_C%02d",histPrefix.c_str(),channel[i].first,channel[i].second));
        hist->RebinX(rebinfac);
        
        hist->SetStats(0);
        hist->SetLineColor(color);
        hist->SetLineWidth(2);
        hist->Scale(1./hist->Integral());
        //hist->SetMaximum(y_max*1.1);
        hist->SetMinimum(10);
        hist->GetXaxis()->SetRangeUser(0.001,xrange);
        hist->GetXaxis()->SetLabelSize(0.04);
        hist->GetXaxis()->SetNdivisions(5);
        //hist->GetXaxis()->SetLabelOffset(0.500001);
        if(iflog==true){gPad->SetLogy();}
        //gStyle->SetTitleFontSize(.08);
         //gStyle->SetLabelSize(.05, "XY");
        hist->GetXaxis()->SetTitleSize(0.05);
        hist->GetXaxis()->SetLabelSize(0.052);
        hist->GetYaxis()->SetLabelSize(0.052);
        hist->GetXaxis()->SetTitleOffset(1.01);
        hist->GetYaxis()->SetTitleOffset(0.5000009);
        hist->GetYaxis()->SetTitleSize(0.05);

        //setLegend(leg,0,0,45,23,hist,assembly.c_str());
    //    hist->GetXaxis()->SetLabelOffset(1.01);
    //    hist->GetYaxis()->SetTitle("Entries");
        hist->Draw("SAME Hist");
        setLegend(leg,0,0,45,25,hist,label.c_str());
        hist->GetXaxis()->SetTitle("ADC");
       // if (i ==4) setLegend(leg,0,0,45,15,hist,label.c_str());
        //if (i == 12  )hist->GetXaxis()->SetTitle("ADC");
        //if (i ==1) {
            hist->GetYaxis()->SetTitle("Entries");
            TLatex latex11;
            latex11.SetNDC ();
            latex11.SetTextSize(0.06);
            //c->cd(1);
            latex11.DrawLatex(0.2,0.91 ,assembly.c_str());
           // }
       // if (i==2){
//            TLatex latex11;
//            latex11.SetNDC ();
//            latex11.SetTextSize(0.06);
       //     c->cd(2);
            latex11.DrawLatex(0.35,0.91 , Form("%s",settings.c_str()));
       // }
            
    }
    
    
       // gPad->SetTickx(2);
}

//double getHistMean(TFile* inputfile,TH1D* hist,Int_t i, Int_t j ){
//    hist=GetHist(inputfile, hist,Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i));
//    double mean = hist->GetMaximum();
//    return mean;
//}


void graphingHistOnSepPad(TCanvas* canvas, Int_t i, Int_t j, std::vector <int> board, int assembly_number, TFile* inputfile, string assembly, TH1D* hist, int color[13],float y_max, bool iflog, float xrange, TLegend* lege, int rebinfac,string histPrefix){
    int Index = getIndex(board,i)+1;
    if (Index == 100000000) throw std::exception();
    int padInd;
    if (Index >4 )padInd = 9;
    if (Index <5)padInd=-4;
    canvas->SetFillColor(0);
    canvas->SetFrameFillStyle(0);
    canvas->cd(abs(padInd-(Index)));
//    int padnum = abs(padInd-(Index));
//    string num = std::to_string(padnum);
//    std::stringstream ss;
//    ss<<"pad"<<num;
//    std::string padname = ss.str();
//    canvas->cd(padnum);
    //printf(" found channel %d in vector %s in channel %d and Pad index %d\n",i,assembly.c_str(),Index,abs(padInd-(Index-1)));
    hist=GetHist(inputfile, hist,Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i));
    hist->SetStats(0);
    hist->GetXaxis()->SetNdivisions(4);
    hist->GetXaxis()->SetTitleSize(0.05);
    hist->GetXaxis()->SetLabelSize(0.052);
    hist->GetYaxis()->SetLabelSize(0.052);
    hist->SetLineColor(color[assembly_number]);
    hist->RebinX(rebinfac);
    hist->Scale(1./hist->Integral());
    hist->GetYaxis()->SetRangeUser(0,y_max*1.1);
    hist->SetLineWidth(2);
    hist->GetXaxis()->SetRangeUser(5,xrange);
    hist->GetXaxis()->SetTitleOffset(1.012);
    hist->GetYaxis()->SetTitleOffset(0.9000009);
    hist->GetYaxis()->SetTitleSize(0.05);
    if(iflog==true){gPad->SetLogy();}
    if (abs(padInd-(Index)) ==8)  hist->GetXaxis()->SetTitle("ADC");
    if (abs(padInd-(Index)) ==1) {
        hist->GetYaxis()->SetTitle("Entries");
    
    }
    //hist->SetLineWidth(6);
    hist->Draw("SAME");
    if (abs(padInd-(Index)) ==4) setLegend(lege,0,0,10,15,hist,assembly.c_str());
    
}

void DrawGammaCanvasSettings( TCanvas* c1,
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
void DrawGammaPadSettings( TPad* pad1,
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


void testBeamPlotterMacrof_only1channel(int assemblynum, string fileName, string energy, string Wlayers, bool energyScan, bool Plot3D, bool Overlay, string outputDir, bool iflog, int rebinfac, float xrange, string scan, bool channelSorted, int Voltage, string particle, bool noiseSubtracted) {
    
    //create output rootfile
    string runNumber =fileName.substr(fileName.find_last_of("R"),fileName.find_last_of("/")-fileName.find_last_of("R"));
    cout <<runNumber<<endl;
    string rootOutName = outputDir+runNumber +string("_")+scan+string(".root");
    cout <<"HEREEEEEEE " <<rootOutName <<endl;
    TFile outRoot(rootOutName.c_str(),"RECREATE");
    
    string histPrefix;
    if (noiseSubtracted) histPrefix = "sh";
    else histPrefix = "h";
        
    TH1D* noNoiseHistsLG[gMaxBoard][gMaxChannels];
    TH1D* noNoiseHistsHG[gMaxBoard][gMaxChannels];
    TH1D* noiseHistsLG[gMaxBoard][gMaxChannels];
    TH1D* AllSignalsHistsLG[gMaxBoard][gMaxChannels];
    
    //given a specific input Voltage find the VoV.
    string VoV;
    map<int,string> VtoVov = {{58,"5.5 VoV"},{57.5,"5.5 VoV"},{57,"4.5 VoV"},{56.5,"4 VoV"},{56,"3.5 VoV"}};
    auto noiseIt = VtoVov.find(Voltage);
    VoV = noiseIt->second;
    
    string settings = particle+ " , "+Wlayers+ " , "+VoV;
    //Colors for histograms of energy and tungsten scan
    int  col[6] = {kTeal-1, kPink+7, kOrange+4, kBlue-7, kGreen+4, kPink+1};
    //Colors for overlaid histograms of all boards
    int  col_overlay[14] = {kTeal-1, kPink+7, kOrange+4, kBlue-7, kGreen+4, kPink+1, kRed+3, kAzure-6, kRed, kMagenta+2, kGray+2, kCyan-3, kMagenta-8,kGreen};
//std::vector <TString> Digiti
    
    //board 1 channels
    //we eventually want to read these from the run setup .txt for the mean time we will hard code them
    std::vector <int> A13, A12, A11, A9, A8, A7, A6, A5, A4, A3, A2, A1, A0;
    std::vector <pair<int,int>> channel1, channel2,channel3,channel4,channel5,channel6,channel7,channel8;
if(assemblynum==1 && !channelSorted)//we only want these whenever we are plotting the channels for every layer
{
   // A11 = {30,28,26,24,22,20,18,16};
   // A8 = {14,12,10,8,6,4,2,0};
    A2 = {32,34,36,38,40,42,44,46};
//    A3 = {48,50,52,54,56,58,60,62};
//    A13 = {33,35,37,39,41,43,45,47};
//    A5 = {49,51,53,55,57,59,61,63};
//    A6 = {31,29,27,25,23,21,19,17};
//    A7 = {15,13,11,9,7,5,3,1};
//    
//    //board 0 channels
//    A1 = {30,28,26,24,22,20,18,16};
//    A9 = {14,12,10,8,6,4,2,0};
//    A0 = {62,60,58,56,54,52,50,48};
//    A12 = {46,44,42,40,38,36,34,32};A4 = {31,29,27,25,23,21,19,17};
   }

else{
    if(energy == 3)
    {
        A0 = {30,28,26,24,22,20,18,16};
        A1 = {14,12,10,8,6,4,2,0};
        A2 = {32,34,36,38,40,42,44,46};
        A3 = {48,50,52,54,56,58,60,62};
        A4 = {33,35,37,39,41,43,45,47};
        A5 = {49,51,53,55,57,59,61,63};
        A6 = {31,29,27,25,23,21,19,17};
        A7 = {15,13,11,9,7,5,3,1};
        
        //board 0 channels
        A8 = {30,28,26,24,22,20,18,16};
        A9 = {14,12,10,8,6,4,2,0};
        A11 = {62,60,58,56,54,52,50,48};
        A12 = {46,44,42,40,38,36,34,32};
        A13 = {31,29,27,25,23,21,19,17};
    }
}
    std::vector <float> maxes;
    //we need to find the max y values and any rebinning for alllllll the different data we want!
    std::vector<string> getMaxEntriesInFiles = testParserTStrings(fileName);
    for (Int_t k = 0; k< getMaxEntriesInFiles.size(); k++){
        TFile *inputfileMaxEntries = new TFile(getMaxEntriesInFiles[k].c_str());
        //for (Int_t j = 0; j ==1; j++){
        for (Int_t j = 0; j < gMaxBoard; j++){
            printf("Going thru board  %d to get max y range \n",j);
            //for (Int_t i = 0; i ==0; i++){
            for (Int_t i = 0; i < gMaxChannels; i++){
                noNoiseHistsHG[j][i] = GetHist(inputfileMaxEntries,noNoiseHistsLG[j][i],Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i));
                noNoiseHistsHG[j][i]->RebinX(rebinfac);
                noNoiseHistsHG[j][i]->Scale(1./noNoiseHistsHG[j][i]->Integral());
                maxes.push_back(noNoiseHistsHG[j][i]->GetMaximum());
            }
        }
        printf("Going thru file %d to get max y range \n",k);
    }//finished looped thru file names :)
    int max_y = distance(maxes.begin(), max_element(maxes.begin(), maxes.end()));
    float y_max = maxes[max_y]*1.10;
    
    cout<<y_max<<endl;
    
    //channel sorted means we want the plot of the same assembly channel for all layers.
    if (channelSorted){
        
        //channel mapping from first entry is CAEN board # and second entry is the channel
//        channel1 =  {
//            {1,30},{1,14},{1,32},{1,48},{1,33},{1,49},{1,31},{1,15},{0,30},{0,14},{0,62},{0,46},{0,31} };
//        
//        channel2 =  {
//            {1,28},{1,12},{1,34},{1,50},{1,35},{1,51},{1,29},{1,13},{0,28},{0,12},{0,60},{0,44},{0,29} };
 //       channel3 =  { {1,10}};
//            {1,26},{1,10},{1,36},{1,52},{1,37},{1,53},{1,27},{1,11},{0,26},{0,10},{0,58},{0,42},{0,27} };
//        channel4 =  {
//            {1,24},{1,8},{1,38},{1,54},{1,39},{1,55},{1,25},{1,9},{0,24},{0,8},{0,56},{0,40},{0,25} };
//        channel5 =  {
//            {1,22},{1,6},{1,40},{1,56},{1,41},{1,57},{1,23},{1,7},{0,22},{0,6},{0,54},{0,38},{0,23} };
//        channel6={{1,42}};
        //        channel6 =  {
//            {1,20},{1,4},{1,42},{1,58},{1,43},{1,59},{1,21},{1,5},{0,20},{0,4},{0,52},{0,36},{0,21} };
       // channel7 =  {
         //   {1,18},{1,2},{1,44},{1,60},{1,45},{1,61},{1,19},{1,3},{0,18},{0,2},{0,50},{0,34},{0,19} };
        
        channel5={ {1,40}};
        //channel8 =  {
           // {1,16},{1,0},{1,46},{1,62},{1,47},{1,63},{1,17},{1,1},{0,16},{0,0},{0,48},{0,32},{0,17} };
        //we need 8 Canvases
        TCanvas *c8 = new TCanvas("Chan1","Channe1 info",1800,1600);
//        c1->Divide(5,3,0,0);
//        TCanvas *c2 = new TCanvas("Chan2","Channel2 info",1800,1600);
//        c2->Divide(5,3,0,0);
//        TCanvas *c3 = new TCanvas("Chan3","Channel3 info",1800,1600);
//        c3->Divide(5,3,0,0);
//        TCanvas *c4 = new TCanvas("Chan4","Channel4 info",1800,1600);
//        c4->Divide(5,3,0,0);
//        TCanvas *c5 = new TCanvas("Chan5","Channel5 info",1800,1600);
//        c5->Divide(5,3,0,0);
//        TCanvas *c6 = new TCanvas("Chan6","Channel6 info",1800,1600);
//        c6->Divide(5,3,0,0);
//        TCanvas *c7 = new TCanvas("Chan7","Channel7 info",1800,1600);
//        c7->Divide(5,3,0,0);
//        TCanvas *c8 = new TCanvas("Chan8","Channel8 info",1800,1600);
//        c8->Divide(5,3,0,0);
//    
        //first we want to loop thru all the channels we give them :0
        //we want to loop thru the all the entries in the channel vectors
        std::vector<string> fileNames = testParserTStrings(fileName);
        std::vector<string> energiesLegend = testParserTStrings(energy);
        
        for (Int_t k = 0; k< fileNames.size(); k++){
            //now i want to set the different labels
            string label = energiesLegend[k];
            TFile *inputfile = new TFile(fileNames[k].c_str());
            int color = col[k];
            float x1, x2;
            if (k ==0){
                x1 = 0.65;
                x2 = 0.89;
            }
            else{
                x1 = 0.65 + 0.06*(k/20);
                x2 = 0.89 + 0.06*(k/20);
            }
            float y1 = 0.89 - 0.05*k;
            float y2 = 0.85 - 0.05*k;
            
            cout << x1 << " " <<y1 << " " << x2 << " "<<y2 <<endl;
            //printf("%d %d %d %d \n",x1,x2,y1,y2);
            //now loop thru the channels and plot them :D
            
            TLegend* leg1 = new TLegend(x1, y1, x2, y2);
//            TLegend* leg2 = new TLegend(x1, y1, x2, y2);
//            TLegend* leg3 = new TLegend(x1, y1, x2, y2);
//            TLegend* leg4 = new TLegend(x1, y1, x2, y2);
//            TLegend* leg5 = new TLegend(x1, y1, x2, y2);
//            TLegend* leg6 = new TLegend(x1, y1, x2, y2);
//            TLegend* leg7 = new TLegend(x1, y1, x2, y2);
//            TLegend* leg8 = new TLegend(x1, y1, x2, y2);
            
            //(TCanvas* c, std::vector <pair<int,int>> channel,  TFile* inputfile, string assembly,TH1D* hist,int color,float y_max,bool iflog, int rebinfac, float xrange, TLegend* leg,string label)
//            graphingChannelSorted(c1,channel1,inputfile,"Channel1",noNoiseHistsLG[0][0],color,true,y_max,rebinfac,xrange,leg1,label,settings,histPrefix);
//            graphingChannelSorted(c2,channel2,inputfile,"Channel2",noNoiseHistsLG[0][0],color,true,y_max,rebinfac,xrange,leg2,label,settings,histPrefix);
//            graphingChannelSorted(c3,channel3,inputfile,"Channel3",noNoiseHistsLG[0][0],color,true,y_max,rebinfac,xrange,leg3,label,settings,histPrefix);
//            graphingChannelSorted(c4,channel4,inputfile,"Channel4",noNoiseHistsLG[0][0],color,true,y_max,rebinfac,xrange,leg4,label,settings,histPrefix);
//            graphingChannelSorted(c5,channel5,inputfile,"Channel5",noNoiseHistsLG[0][0],color,true,y_max,rebinfac,xrange,leg5,label,settings,histPrefix);
//            graphingChannelSorted(c6,channel6,inputfile,"Channel6",noNoiseHistsLG[0][0],color,true,y_max,rebinfac,xrange,leg6,label,settings,histPrefix);
//            graphingChannelSorted(c7,channel7,inputfile,"Channel7",noNoiseHistsLG[0][0],color,true,y_max,rebinfac,xrange,leg7,label,settings,histPrefix);
            graphingChannelSorted(c8,channel5,inputfile,"Channel5",noNoiseHistsLG[0][0],color,true,y_max,rebinfac,xrange,leg1,label,settings,histPrefix);
            
            string CanvasName=outputDir+scan+string("Chan5.pdf");
            TPaveLabel* pl = new TPaveLabel(-2.722343,6.44797,1.572668,6.937022,"Channel6","br");
            pl->Draw();
//            c1->SaveAs(CanvasName.c_str());
//            CanvasName=outputDir+scan+string("Chan2.pdf");
//            c2->SaveAs(CanvasName.c_str());
//            CanvasName=outputDir+scan+string("Chan3.pdf");
//            c3->SaveAs(CanvasName.c_str());
//            CanvasName=outputDir+scan+string("Chan4.pdf");
//            c4->SaveAs(CanvasName.c_str());
//            CanvasName=outputDir+scan+string("Chan5.pdf");
//            c5->SaveAs(CanvasName.c_str());
//            CanvasName=outputDir+scan+string("Chan6.pdf");
//            c6->SaveAs(CanvasName.c_str());
//            CanvasName=outputDir+scan+string("Chan7.pdf");
//            c7->SaveAs(CanvasName.c_str());
            CanvasName=outputDir+scan+string("Chan5.pdf");
            c8->SaveAs(CanvasName.c_str());
            outRoot.cd();
//            c1->Write();
//            c2->Write();
//            c3->Write();
//            c4->Write();
//            c5->Write();
//            c6->Write();
//            c7->Write();
            c8->Write();
            //            graphingHistOnPad(c11,i,j,A11,inputfile,"A11",noNoiseHistsLG[j][i],color,y_max,true,rebinfac,xrange,leg1,label);
            
        }//finishing all channels plots
        
//        delete c1;
//        delete c2;
//        delete c3;
//        delete c4;
//        delete c5;
//        delete c6;
//        delete c7;
        delete c8;
    }//finishing channel sort!
    
    
    //std::vector <int> chan8 ={16,0,}
//    if (energyScan){
//        
//        
//        
//        //now i want to make a canvas per board for energy scan!
//        TCanvas *c0 = new TCanvas("A0","A0 info",1800,1600);
//        c0->Divide(4,2,0,0);
//        //c0->Divide(4,2,0.00000000001,0.00000000001);
//        
//        TCanvas *c1 = new TCanvas("A1","A1 info",1800,1600);
//        c1->Divide(4,2,0,0);
//        
//        TCanvas *c2 = new TCanvas("A2","A2 info",1800,1600);
//        c2->Divide(4,2,0,0);
//        TCanvas *c3 = new TCanvas("A3","A3 info",1800,1600);
//        c3->Divide(4,2,0,0);
//        TCanvas *c4 = new TCanvas("A4","A4 info",1800,1600);
//        c4->Divide(4,2,0,0);
//        TCanvas *c5 = new TCanvas("A5","A5 info",1800,1600);
//        c5->Divide(4,2,0,0);
//        TCanvas *c6 = new TCanvas("A6","A6 info",1800,1600);
//        c6->Divide(4,2,0,0);
//        TCanvas *c7 = new TCanvas("A7","A7 info",1800,1600);
//        c7->Divide(4,2,0,0);
//        TCanvas *c8 = new TCanvas("A8","A8 info",1800,1600);
//        c8->Divide(4,2,0,0);
//        TCanvas *c9 = new TCanvas("A9","A9 info",1800,1600);
//        c9->Divide(4,2,0,0);
//        TCanvas *c11 = new TCanvas("A11","A11 info",1800,1600);
//        c11->Divide(4,2,0,0);
//        TCanvas *c12 = new TCanvas("A12","A12 info",1800,1600);
//        c12->Divide(4,2,0,0);
//        TCanvas *c13 = new TCanvas("A13","A13 info",1800,1600);
//        c13->Divide(4,2,0,0);
//        
//       
//        //
//        
//        //update the first parameter so we can take different root files as inputs
//        std::vector<string> fileNames = testParserTStrings(fileName);
//        std::vector<string> energiesLegend = testParserTStrings(energy);
////        std::vector<string> ColorsHists = testParserTStrings(graphColors);
//        //std::vector<string> wlayersLegend = testPartTString(Wlayers);
//        
//        //with_separator(fileNames,", ");
//        //cout<< energiesLegend[0] <<" , "<< energiesLegend[1]<<std::endl;
//        //loop thru the different files
//        for (Int_t k = 0; k< fileNames.size(); k++){
//            //now i want to set the different labels
//            string label = energiesLegend[k];
//            cout<< label<<endl;
//            if(label == "3 GeV") {cout<<"3 GeV beam energy had a different assembly configuration. Skipped!"<<endl;continue;}//3GeV files had different assembly
////            {
////                std::vector <int> A0 = {30,28,26,24,22,20,18,16};
////                std::vector <int> A1 = {14,12,10,8,6,4,2,0};
////                std::vector <int> A2 = {32,34,36,38,40,42,44,46};
////                std::vector <int> A3 = {48,50,52,54,56,58,60,62};
////                std::vector <int> A4 = {33,35,37,39,41,43,45,47};
////                std::vector <int> A5 = {49,51,53,55,57,59,61,63};
////                std::vector <int> A6 = {31,29,27,25,23,21,19,17};
////                std::vector <int> A7 = {15,13,11,9,7,5,3,1};
////
////                //board 0 channels
////                std::vector <int> A8 = {30,28,26,24,22,20,18,16};
////                std::vector <int> A9 = {14,12,10,8,6,4,2,0};
////                std::vector <int> A11 = {62,60,58,56,54,52,50,48};
////                std::vector <int> A12 = {46,44,42,40,38,36,34,32};
////                std::vector <int> A13 = {31,29,27,25,23,21,19,17};
////            }
//            ////===============================
//            int color = col[k];
//            //==============
//            
//            float x1, x2;
//            if (k ==0){
//                x1 = 0.50;
//                x2 = 0.80;
//            }
//            else{
//                x1 = 0.50 + 0.05*(k/20);
//                x2 = 0.80 + 0.05*(k/20);
//            }
//            float y1 = 0.95 - 0.07*k;
//            float y2 = 0.90 - 0.07*k;
//            
//            cout << x1 << " " <<y1 << " " << x2 << " "<<y2 <<endl;
//            //printf("%d %d %d %d \n",x1,x2,y1,y2);
//            TFile *inputfile = new TFile(fileNames[k].c_str());
//            
//            //Now we want to loop through the TH1Fs from the file and get them all!
//            for (Int_t j = 0; j < gMaxBoard; j++){
//                for (Int_t i = 0; i < gMaxChannels; i++){
//                    if (j == 0){
//                        if (std::find(A11.begin(), A11.end(),i)!=A11.end() ){
//                            TLegend* leg1 = new TLegend(x1, y1, x2, y2);
//                            graphingHistOnPad(c11,i,j,A11,inputfile,"A11",noNoiseHistsLG[j][i],color,y_max,true,rebinfac,xrange,leg1,label,settings,histPrefix);
//                                
//                        }
//                        if (std::find(A8.begin(), A8.end(),i)!=A8.end() ){
//                            TLegend* leg2 = new TLegend(x1, y1, x2, y2);
//                            graphingHistOnPad(c8,i,j,A8,inputfile,"A8",noNoiseHistsLG[j][i],color,y_max,true,rebinfac,xrange,leg2,label,settings,histPrefix);
//                     
//                        }
//                        if (std::find(A2.begin(), A2.end(),i)!=A2.end() ){
//                            TLegend* leg3 = new TLegend(x1, y1, x2, y2);
//                            graphingHistOnPad(c2,i,j,A2,inputfile,"A2",noNoiseHistsLG[j][i],color,y_max,true,rebinfac,xrange,leg3,label,settings,histPrefix);
//                         
//                        }
//                        if (std::find(A3.begin(), A3.end(),i)!=A3.end() ){
//                            TLegend* leg4 = new TLegend(x1, y1, x2, y2);
//                            graphingHistOnPad(c3,i,j,A3,inputfile,"A3",noNoiseHistsLG[j][i],color,y_max,true,rebinfac,xrange,leg4,label,settings,histPrefix);
//                            
//                        }
//                        if (std::find(A13.begin(), A13.end(),i)!=A13.end() ){
//                            TLegend* leg5 = new TLegend(x1, y1, x2, y2);
//                            graphingHistOnPad(c13,i,j,A13,inputfile,"A13",noNoiseHistsLG[j][i],color,y_max,true,rebinfac,xrange,leg5,label,settings,histPrefix);
//                            
//                            
//                        }
//                        if (std::find(A5.begin(), A5.end(),i)!=A5.end() ){
//                            TLegend* leg6 = new TLegend(x1, y1, x2, y2);
//                            graphingHistOnPad(c5,i,j,A5,inputfile,"A5",noNoiseHistsLG[j][i],color,y_max,true,rebinfac,xrange,leg6,label,settings,histPrefix);
//
//                        }
//                        if (std::find(A6.begin(), A6.end(),i)!=A6.end() ){
//                            TLegend* leg7 = new TLegend(x1, y1, x2, y2);
//                            graphingHistOnPad(c6,i,j,A6,inputfile,"A6",noNoiseHistsLG[j][i],color,y_max,true,rebinfac,xrange,leg7,label,settings,histPrefix);
//
//                        }
//                        if (std::find(A7.begin(), A7.end(),i)!=A7.end() ){
//                            TLegend* leg8 = new TLegend(x1, y1, x2, y2);
//                            graphingHistOnPad(c7,i,j,A7,inputfile,"A7",noNoiseHistsLG[j][i],color,y_max,true,rebinfac,xrange,leg8,label,settings,histPrefix);
//                            
//                        }
//                        
//                        
//                    }//checking if digitizer board 0
//                    if (j==1){
//                        if (std::find(A0.begin(), A0.end(),i)!=A0.end() ){
//                            TLegend* leg9 = new TLegend(x1, y1, x2, y2);
//                            graphingHistOnPad(c0,i,j,A0,inputfile,"A0",noNoiseHistsLG[j][i],color,y_max,true,rebinfac,xrange,leg9,label,settings,histPrefix);
//                            
//                        }
//                        if (std::find(A1.begin(), A1.end(),i)!=A1.end() ){
//                            TLegend* leg10 = new TLegend(x1, y1, x2, y2);
//                            graphingHistOnPad(c1,i,j,A1,inputfile,"A1",noNoiseHistsLG[j][i],color,y_max,true,rebinfac,xrange,leg10,label,settings,histPrefix);
//                            
//                        }
//                        if (std::find(A9.begin(), A9.end(),i)!=A9.end() ){
//                            TLegend* leg11 = new TLegend(x1, y1, x2, y2);
//                            graphingHistOnPad(c9,i,j,A9,inputfile,"A9",noNoiseHistsLG[j][i],color,y_max,true,rebinfac,xrange,leg11,label,settings,histPrefix);
//                            
//                        }
//                        if (std::find(A12.begin(), A12.end(),i)!=A12.end() ){
//                            TLegend* leg12 = new TLegend(x1, y1, x2, y2);
//                            graphingHistOnPad(c12,i,j,A12,inputfile,"A12",noNoiseHistsLG[j][i],color,y_max,true,rebinfac,xrange,leg12,label,settings,histPrefix);
//                            
//                        }
//                        if (std::find(A4.begin(), A4.end(),i)!=A4.end() ){
//                            TLegend* leg13 = new TLegend(x1, y1, x2, y2);
//                            graphingHistOnPad(c4,i,j,A4,inputfile,"A4",noNoiseHistsLG[j][i],color,y_max,true,rebinfac,xrange,leg13,label,settings,histPrefix);
//        
//                        }
//                        
//                        
//                    }//checking if digitizer board 1
//                    
//                }//
//            }//end of board loop
//            
//        }//end of TFile loop!
//        string CanvasName=outputDir+scan+string("BoardA11.pdf");
//        TPaveLabel* pl = new TPaveLabel(-2.722343,6.44797,1.572668,6.937022,"A11","br");
//        pl->Draw();
//        c11->SaveAs(CanvasName.c_str());
//        
//        CanvasName=outputDir+scan+string("BoardA4.pdf");
//        c4->SaveAs(CanvasName.c_str());
//        CanvasName=outputDir+scan+string("BoardA12.pdf");
//        c12->SaveAs(CanvasName.c_str());
//        CanvasName=outputDir+scan+string("BoardA9.pdf");
//        c9->SaveAs(CanvasName.c_str());
//        CanvasName=outputDir+scan+string("BoardA1.pdf");
//        c1->SaveAs(CanvasName.c_str());
//        CanvasName=outputDir+scan+string("BoardA0.pdf");
//        c0->SaveAs(CanvasName.c_str());
//        CanvasName=outputDir+scan+string("BoardA7.pdf");
//        c7->SaveAs(CanvasName.c_str());
//        CanvasName=outputDir+scan+string("BoardA6.pdf");
//        c6->SaveAs(CanvasName.c_str());
//         CanvasName=outputDir+scan+string("BoardA5.pdf");
//        c5->SaveAs(CanvasName.c_str());
//         CanvasName=outputDir+scan+string("BoardA13.pdf");
//        c13->SaveAs(CanvasName.c_str());
//         CanvasName=outputDir+scan+string("BoardA2.pdf");
//        c2->SaveAs(CanvasName.c_str());
//         CanvasName=outputDir+scan+string("BoardA3.pdf");
//        c3->SaveAs(CanvasName.c_str());
//         CanvasName=outputDir+scan+string("BoardA8.pdf");
//        c8->SaveAs(CanvasName.c_str());
//        
//        outRoot.cd();
//        c1->Write();
//        c2->Write();
//        c3->Write();
//        c4->Write();
//        c5->Write();
//        c6->Write();
//        c7->Write();
//        c8->Write();
//        c9->Write();
//        c11->Write();
//        c12->Write();
//        c13->Write();
//        c0->Write();
//        
//
//        delete c1;
//        delete c2;
//        delete c3;
//        delete c4;
//        delete c5;
//        delete c6;
//        delete c7;
//        delete c8;
//        delete c9;
//        delete c11;
//        delete c12;
//        delete c13;
//        delete c0;
//    }//end of checkiing for energy loop
//    
//
//    
//            
//            if (Plot3D){
//                TH3D* hist3DMap = new TH3D("h_map_z_x_y", "3D Average Energy Distribution;Depth ;x - Row ;y - Column", 13, -0.5, 12.5, 4, -0.5, 3.5, 2, -0.5, 1.5);
//                //want to
//                A11 = {30,28,26,24,22,20,18,16};
//                A8 = {14,12,10,8,6,4,2,0};
//                A2 = {32,34,36,38,40,42,44,46};
//                A3 = {48,50,52,54,56,58,60,62};
//                A13 = {33,35,37,39,41,43,45,47};
//                A5 = {49,51,53,55,57,59,61,63};
//                A6 = {31,29,27,25,23,21,19,17};
//                A7 = {15,13,11,9,7,5,3,1};
//                
//                //board 0 channels
//                A1 = {30,28,26,24,22,20,18,16};
//                A9 = {14,12,10,8,6,4,2,0};
//                A0 = {62,60,58,56,54,52,50,48};
//                A12 = {46,44,42,40,38,36,34,32};
//                A4 = {31,29,27,25,23,21,19,17};
//                
//                //now we want the board location
//                
//                map<int,pair<int,int>> boardLocationMapping = {
//                    {1,{0,0}},{2,{0,1}},{3,{0,2}},{4,{0,3}},{5,{1,3}},{6,{1,2}},{7,{1,1}},{8,{1,0}}
//                };
//                
//                
//                
//                //now i want to loop thru all the files, boards and channels
//                //std::vector<string> energiesLegend = testParserTStrings(energy);
//                //string Label = energiesLegend[k];
//
//                for (Int_t k = 0; k< getMaxEntriesInFiles.size(); k++){
//                    TFile *inputfileMaxEntries = new TFile(getMaxEntriesInFiles[k].c_str());
//                   
//                    for (Int_t j = 0; j < gMaxBoard; j++){
//                        printf("Going thru board  %d to get max y range \n",j);
//                        for (Int_t i = 0; i < gMaxChannels; i++){
//                            //cout << " HEREEEEEEE: "<<i<<endl;
//                            int padInd,channel,Index;
//                            double mean;
//                            TH1D* meanHist;
//                            //auto it = boardLocationMapping.find(i);
//                            //iterator it;
//                            if (j == 1){
//                                if (std::find(A11.begin(), A11.end(),i)!=A11.end() )
//                                {
//                                    Index = getIndex(A11,i)+1;
//                                    if (Index >4 )padInd = 9;
//                                    if (Index <5)padInd=-4;
//                                    channel = abs(padInd-(Index));
//                                    meanHist= (TH1D*)(inputfileMaxEntries->Get(Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i)));
//                                    //mean = meanHist->GetMean();
//                                    mean = meanHist->GetMean();
//                                    
//                                    //GetHist(inputfileMaxEntries,noNoiseHistsLG[j][i],Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i));
//                                    //mean = getHistMean(inputfileMaxEntries,noNoiseHistsLG[j][i],j,i);
//                                    auto it = boardLocationMapping.find(channel);
//                                    if (mean>1000)cout << mean << " " << i << " "<<(double)it->second.second<<" "<<(double)it->second.first<<endl;
//                                    //now we want to get the 3d plot.
//                                    //cout << mean << " " << i << " "<<(double)it->second.second<<" "<<(double)it->second.first<<endl;
//                                    hist3DMap->Fill((double)0,(double)it->second.second,(double)it->second.first,mean);
//                                }
//                                if (std::find(A8.begin(), A8.end(),i)!=A8.end() ){
//                                    Index = getIndex(A8,i)+1;
//                                    if (Index >4 )padInd = 9;
//                                    if (Index <5)padInd=-4;
//                                    channel = abs(padInd-(Index));
//                                    meanHist= (TH1D*)(inputfileMaxEntries->Get(Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i)));
//                                    mean = meanHist->GetMean();
//                                    auto it1 = boardLocationMapping.find(channel);
//                                    cout << mean << " " << i << " "<<(double)it1->second.second<<" "<<(double)it1->second.first<<endl;
//                                    //now we want to get the 3d plot.
//                                    hist3DMap->Fill((double)1,(double)it1->second.second,(double)it1->second.first,mean);
//
//                                }
//                                if (std::find(A2.begin(), A2.end(),i)!=A2.end() ){
//                                    Index = getIndex(A2,i)+1;
//                                    if (Index >4 )padInd = 9;
//                                    if (Index <5)padInd=-4;
//                                    channel = abs(padInd-(Index));
//                                    meanHist= (TH1D*)(inputfileMaxEntries->Get(Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i)));
//                                    mean = meanHist->GetMean();
//                                    auto it2 = boardLocationMapping.find(channel);
//                                    if (mean>1000)cout << mean << " " << i << " "<<(double)it2->second.second<<" "<<(double)it2->second.first<<endl;
//                                    //now we want to get the 3d plot.
//                                    hist3DMap->Fill((double)2,(double)it2->second.second,(double)it2->second.first,mean);
//
//                                }
//                                if (std::find(A3.begin(), A3.end(),i)!=A3.end() ){
//                                    Index = getIndex(A3,i)+1;
//                                    if (Index >4 )padInd = 9;
//                                    if (Index <5)padInd=-4;
//                                    channel = abs(padInd-(Index));
//                                    meanHist= (TH1D*)(inputfileMaxEntries->Get(Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i)));
//                                    mean = meanHist->GetMean();
//                                    auto it3 = boardLocationMapping.find(channel);
//                                    if (mean>1000)cout << mean << " " << i << " "<<(double)it3->second.second<<" "<<(double)it3->second.first<<endl;
//                                    //now we want to get the 3d plot.
//                                    hist3DMap->Fill((double)3,(double)it3->second.second,(double)it3->second.first,mean);
//                                }
//                                if (std::find(A13.begin(), A13.end(),i)!=A13.end() ){
//                                    Index = getIndex(A13,i)+1;
//                                    if (Index >4 )padInd = 9;
//                                    if (Index <5)padInd=-4;
//                                    channel = abs(padInd-(Index));
//                                    meanHist= (TH1D*)(inputfileMaxEntries->Get(Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i)));
//                                    mean = meanHist->GetMean();
//                                    
//                                    //GetHist(inputfileMaxEntries,noNoiseHistsLG[j][i],Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i));
//                                    //mean = getHistMean(inputfileMaxEntries,noNoiseHistsLG[j][i],j,i);
//                                    auto it4 = boardLocationMapping.find(channel);
//                                    if (mean>1000)cout << mean << " " << i << " "<<(double)it4->second.second<<" "<<(double)it4->second.first<<endl;
//                                    //now we want to get the 3d plot.
//                                    //cout << mean << " " << i << " "<<(double)it4->second.second<<" "<<(double)it4->second.first<<endl;
//                                    //auto it = boardLocationMapping.find(channel);
//                                    //now we want to get the 3d plot.
//                                    hist3DMap->Fill((double)4,(double)it4->second.second,(double)it4->second.first,mean);
//
//                                }
//                                if (std::find(A5.begin(), A5.end(),i)!=A5.end() ){
//                                    int Index = getIndex(A5,i)+1;
//                                    if (Index >4 )padInd = 9;
//                                    if (Index <5)padInd=-4;
//                                    channel = abs(padInd-(Index));
//                                    meanHist= (TH1D*)(inputfileMaxEntries->Get(Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i)));
//                                    mean = meanHist->GetMean();
//                                    
//                                    //GetHist(inputfileMaxEntries,noNoiseHistsLG[j][i],Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i));
//                                    //mean = getHistMean(inputfileMaxEntries,noNoiseHistsLG[j][i],j,i);
//                                    auto it = boardLocationMapping.find(channel);
//                                    if (mean>1000)cout << mean << " " << i << " "<<(double)it->second.second<<" "<<(double)it->second.first<<endl;
//                                    //now we want to get the 3d plot.
//                                    //cout << mean << " " << i << " "<<(double)it->second.second<<" "<<(double)it->second.first<<endl;
//                                   // auto it = boardLocationMapping.find(channel);
//                                    //now we want to get the 3d plot.
//                                    hist3DMap->Fill((double)5,(double)it->second.second,(double)it->second.first,mean);
//
//                                }
//                                if (std::find(A6.begin(), A6.end(),i)!=A6.end() ){
//                                    int Index = getIndex(A6,i)+1;
//                                    if (Index >4 )padInd = 9;
//                                    if (Index <5)padInd=-4;
//                                    channel = abs(padInd-(Index));
//                                    meanHist= (TH1D*)(inputfileMaxEntries->Get(Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i)));
//                                    mean = meanHist->GetMean();
//                                    
//                                    //GetHist(inputfileMaxEntries,noNoiseHistsLG[j][i],Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i));
//                                    //mean = getHistMean(inputfileMaxEntries,noNoiseHistsLG[j][i],j,i);
//                                    auto it = boardLocationMapping.find(channel);
//                                    if (mean>1000)cout << mean << " " << i << " "<<(double)it->second.second<<" "<<(double)it->second.first<<endl;
//                                    //now we want to get the 3d plot.
//                                    //cout << mean << " " << i << " "<<(double)it->second.second<<" "<<(double)it->second.first<<endl;
//                                    //now we want to get the 3d plot.
//                                    hist3DMap->Fill((double)6,(double)it->second.second,(double)it->second.first,mean);
//
//                                }
//                                if (std::find(A7.begin(), A7.end(),i)!=A7.end() ){
//                                    int Index = getIndex(A7,i)+1;
//                                    if (Index >4 )padInd = 9;
//                                    if (Index <5)padInd=-4;
//                                    channel = abs(padInd-(Index));
//                                    meanHist= (TH1D*)(inputfileMaxEntries->Get(Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i)));
//                                    mean = meanHist->GetMean();
//                                    
//                                    //GetHist(inputfileMaxEntries,noNoiseHistsLG[j][i],Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i));
//                                    //mean = getHistMean(inputfileMaxEntries,noNoiseHistsLG[j][i],j,i);
//                                    auto it = boardLocationMapping.find(channel);
//                                    if (mean>1000)cout << mean << " " << i << " "<<(double)it->second.second<<" "<<(double)it->second.first<<endl;
//                                    //now we want to get the 3d plot.
//                                    //cout << mean << " " << i << " "<<(double)it->second.second<<" "<<(double)it->second.first<<endl;
//                                    //now we want to get the 3d plot.
//                                    hist3DMap->Fill((double)7,(double)it->second.second,(double)it->second.first,mean);
//                                    
//                                }
//                                
//                                
//                            }//checking if digitizer board 1
//                            if (j==0){
//                                if (std::find(A0.begin(), A0.end(),i)!=A0.end() ){
//                                    int Index = getIndex(A0,i)+1;
//                                    if (Index >4 )padInd = 9;
//                                    if (Index <5)padInd=-4;
//                                    channel = abs(padInd-(Index));
//                                    meanHist= (TH1D*)(inputfileMaxEntries->Get(Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i)));
//                                    mean = meanHist->GetMean();
//                                    
//                                    //GetHist(inputfileMaxEntries,noNoiseHistsLG[j][i],Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i));
//                                    //mean = getHistMean(inputfileMaxEntries,noNoiseHistsLG[j][i],j,i);
//                                    auto it = boardLocationMapping.find(channel);
//                                    if (mean>1000)cout << mean << " " << i << " "<<(double)it->second.second<<" "<<(double)it->second.first<<endl;
//                                    //now we want to get the 3d plot.
//                                    ////cout << mean << " " << i << " "<<(double)it->second.second<<" "<<(double)it->second.first<<endl;                                    //auto it = boardLocationMapping.find(channel);
//                                    //now we want to get the 3d plot.
//                                    hist3DMap->Fill((double)10,(double)it->second.second,(double)it->second.first,mean);
//                                }
//                                if (std::find(A1.begin(), A1.end(),i)!=A1.end() ){
//                                    int Index = getIndex(A1,i)+1;
//                                    if (Index >4 )padInd = 9;
//                                    if (Index <5)padInd=-4;
//                                    channel = abs(padInd-(Index));
//                                    meanHist= (TH1D*)(inputfileMaxEntries->Get(Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i)));
//                                    mean = meanHist->GetMean();
//                                    
//                                    //GetHist(inputfileMaxEntries,noNoiseHistsLG[j][i],Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i));
//                                    //mean = getHistMean(inputfileMaxEntries,noNoiseHistsLG[j][i],j,i);
//                                    auto it = boardLocationMapping.find(channel);
//                                    //now we want to get the 3d plot.
//                                    ////cout << mean << " " << i << " "<<(double)it->second.second<<" "<<(double)it->second.first<<endl;                                    //auto it = boardLocationMapping.find(channel);
//                                    //now we want to get the 3d plot.
//                                    if (mean>1000)cout << mean << " " << i << " "<<(double)it->second.second<<" "<<(double)it->second.first<<endl;
//                                    hist3DMap->Fill((double)8,(double)it->second.second,(double)it->second.first,mean);
//                                }
//                                if (std::find(A9.begin(), A9.end(),i)!=A9.end() ){
//                                    int Index = getIndex(A9,i)+1;
//                                    if (Index >4 )padInd = 9;
//                                    if (Index <5)padInd=-4;
//                                    channel = abs(padInd-(Index));
//                                    meanHist= (TH1D*)(inputfileMaxEntries->Get(Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i)));
//                                    mean = meanHist->GetMean();
//                                    
//                                    //GetHist(inputfileMaxEntries,noNoiseHistsLG[j][i],Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i));
//                                    //mean = getHistMean(inputfileMaxEntries,noNoiseHistsLG[j][i],j,i);
//                                    auto it = boardLocationMapping.find(channel);
//                                    if (mean>1000)cout << mean << " " << i << " "<<(double)it->second.second<<" "<<(double)it->second.first<<endl;
//                                    //now we want to get the 3d plot.
//                                    //cout << mean << " " << i << " "<<(double)it->second.second<<" "<<(double)it->second.first<<endl;                                    //auto it = boardLocationMapping.find(channel);
//                                    //now we want to get the 3d plot.
//                                    hist3DMap->Fill((double)9,(double)it->second.second,(double)it->second.first,mean);
//
//                                }
//                                if (std::find(A12.begin(), A12.end(),i)!=A12.end() ){
//                                    int Index = getIndex(A12,i)+1;
//                                    if (Index >4 )padInd = 9;
//                                    if (Index <5)padInd=-4;
//                                    channel = abs(padInd-(Index));
//                                    meanHist= (TH1D*)(inputfileMaxEntries->Get(Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i)));
//                                    mean = meanHist->GetMean();
//                                    
//                                    //GetHist(inputfileMaxEntries,noNoiseHistsLG[j][i],Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i));
//                                    //mean = getHistMean(inputfileMaxEntries,noNoiseHistsLG[j][i],j,i);
//                                    auto it = boardLocationMapping.find(channel);
//                                    if (mean>1000)cout << mean << " " << i << " "<<(double)it->second.second<<" "<<(double)it->second.first<<endl;
//                                    //now we want to get the 3d plot.
//                                    //cout << mean << " " << i << " "<<(double)it->second.second<<" "<<(double)it->second.first<<endl;                                    //auto it = boardLocationMapping.find(channel);
//                                    //now we want to get the 3d plot.
//                                    hist3DMap->Fill((double)11,(double)it->second.second,(double)it->second.first,mean);
//
//                                }
//                                if (std::find(A4.begin(), A4.end(),i)!=A4.end() ){
//                                    int Index = getIndex(A4,i)+1;
//                                    if (Index >4 )padInd = 9;
//                                    if (Index <5)padInd=-4;
//                                    channel = abs(padInd-(Index));
//                                    meanHist= (TH1D*)(inputfileMaxEntries->Get(Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i)));
//                                    mean = meanHist->GetMean();
//                                    
//                                    //GetHist(inputfileMaxEntries,noNoiseHistsLG[j][i],Form("%s_HG_B%d_C%02d",histPrefix.c_str(),j,i));
//                                    //mean = getHistMean(inputfileMaxEntries,noNoiseHistsLG[j][i],j,i);
//                                    auto it = boardLocationMapping.find(channel);
//                                    if (mean>1000)cout << mean << " " << i << " "<<(double)it->second.second<<" "<<(double)it->second.first<<endl;
//                                    //now we want to get the 3d plot.
//                                    //cout << mean << " " << i << " "<<(double)it->second.second<<" "<<(double)it->second.first<<endl;                                    //auto it = boardLocationMapping.find(channel);
//                                    //now we want to get the 3d plot.
//                                    hist3DMap->Fill((double)12,(double)it->second.second,(double)it->second.first,mean);
//
//                                }
//                                
//                                
//                            }//checking if digitizer board 0
//                            
//                        }
//                    }
//                }//finished looped thru file names :)
//                //hist3DMap->Fill((double)-2,-2.0,-2.0,2000);
//                TCanvas *c0 = new TCanvas("Plot3D","3D plot ADC distribution",1800,1600);
//                gStyle->SetPalette(55);
////                TLatex latex11;
////                latex11.SetNDC ();
////                latex11.SetTextSize(0.06);
////                //c->cd(1);
////                latex11.DrawLatex(-0.9,0.5 ,settings.c_str());
//                
//                //}
//    //gStyle->SetPalette(55);
//                // Draw the 3D histogram
//                //hist3DMap->GetZaxis()->SetRangeUser(0,1200);
//                hist3DMap->Draw("BOX2 Z");
//                hist3DMap->GetXaxis()->SetTitleOffset(1.35);
//                TPaveText *l4 = new TPaveText(-0.9,0.8,-0.5,0.95);
//                //l4->SetFillColor(42);
//                l4->SetTextAlign(12);
//                l4->AddText(Form("%s %s",energy.c_str(),settings.c_str()));
//                l4->Draw("same");
//                c0->Update();
//                c0->Draw();
//                outRoot.cd();
//            
//                hist3DMap->Write();
//                c0->Write();
//                
//                
//            }
//            
//    if (Overlay){
//        TCanvas *canvasPReso8PanelPlot = new TCanvas("A1","A1 info",1800,1600);
//        canvasPReso8PanelPlot->Divide(4,2,0,0);
//        
////        Double_t arrayBound8PanPResoX1_4[5];
////        Double_t arrayBound8PanPResoY1_4[4];
////        Double_t relativeMarginsEResoX[3];
////        Double_t relativeMarginsEResoY[3];
////
////        Double_t textSizeSinglePad        = 0.05;
////        Double_t textSizeLabelsPixel      = 35;
////        Double_t textSizeLabelsRel        = 58./1300;
////        textSizeLabelsPixel       = 750*0.05;
////        ReturnCorrectValuesForCanvasScaling(2250,750, 4, 2,0.035, 0.003, 0.01,0.095,arrayBound8PanPResoX1_4,arrayBound8PanPResoY1_4,relativeMarginsEResoX,relativeMarginsEResoY);
////        TCanvas* canvasPReso8PanelPlot = new TCanvas("canvasPReso8PanelPlot","",0,0,2250,750); // gives the page size
////        DrawGammaCanvasSettings( canvasPReso8PanelPlot, 0.05, 0.01, 0.01,0.095);
////
////        TPad* pad1      = new TPad("pad1", "", arrayBound8PanPResoX1_4[0], arrayBound8PanPResoY1_4[2], arrayBound8PanPResoX1_4[1], arrayBound8PanPResoY1_4[1],-1, -1, -2);
////        DrawGammaPadSettings( pad1, relativeMarginsEResoX[0], relativeMarginsEResoX[1], relativeMarginsEResoY[0], relativeMarginsEResoY[2]);
////        pad1->SetNumber(1);
////        pad1->Draw();
////
////        TPad* pad2       = new TPad("pad2", "", arrayBound8PanPResoX1_4[1], arrayBound8PanPResoY1_4[2], arrayBound8PanPResoX1_4[2], arrayBound8PanPResoY1_4[1],-1, -1, -2);
////        DrawGammaPadSettings( pad2, relativeMarginsEResoX[1], relativeMarginsEResoX[1], relativeMarginsEResoY[0], relativeMarginsEResoY[2]);
////        pad2->SetNumber(2);
////        pad2->Draw();
////
////        TPad* pad3       = new TPad("pad3", "", arrayBound8PanPResoX1_4[2], arrayBound8PanPResoY1_4[2], arrayBound8PanPResoX1_4[3], arrayBound8PanPResoY1_4[1],-1, -1, -2);
////        DrawGammaPadSettings( pad3, relativeMarginsEResoX[1], relativeMarginsEResoX[1], relativeMarginsEResoY[0], relativeMarginsEResoY[2]);
////        pad3->SetNumber(3);
////        pad3->Draw();
////
////        TPad* pad4      = new TPad("pad4", "", arrayBound8PanPResoX1_4[3], arrayBound8PanPResoY1_4[2], arrayBound8PanPResoX1_4[4], arrayBound8PanPResoY1_4[1],-1, -1, -2);
////        DrawGammaPadSettings( pad4, relativeMarginsEResoX[1], relativeMarginsEResoX[2], relativeMarginsEResoY[0], relativeMarginsEResoY[2]);
////        pad4->SetNumber(4);
////        pad4->Draw();
////
////        TPad* pad8       = new TPad("pad8", "", arrayBound8PanPResoX1_4[0], arrayBound8PanPResoY1_4[1], arrayBound8PanPResoX1_4[1], arrayBound8PanPResoY1_4[0],-1, -1, -2);
////        DrawGammaPadSettings( pad8, relativeMarginsEResoX[0], relativeMarginsEResoX[1], relativeMarginsEResoY[0], relativeMarginsEResoY[2]);
////        pad8->SetNumber(8);
////        pad8->Draw();
////
////        TPad* pad7       = new TPad("pad7", "", arrayBound8PanPResoX1_4[1], arrayBound8PanPResoY1_4[1], arrayBound8PanPResoX1_4[2], arrayBound8PanPResoY1_4[0],-1, -1, -2);
////        DrawGammaPadSettings( pad7, relativeMarginsEResoX[1], relativeMarginsEResoX[1], relativeMarginsEResoY[0], relativeMarginsEResoY[2]);
////        pad7->SetNumber(7);
////        pad7->Draw();
////
////        TPad* pad6       = new TPad("pad6", "", arrayBound8PanPResoX1_4[2], arrayBound8PanPResoY1_4[1], arrayBound8PanPResoX1_4[3], arrayBound8PanPResoY1_4[0],-1, -1, -2);
////        DrawGammaPadSettings( pad6, relativeMarginsEResoX[1], relativeMarginsEResoX[1], relativeMarginsEResoY[0], relativeMarginsEResoY[2]);
////        pad6->SetNumber(6);
////        pad6->Draw();
////
////        TPad* pad5       = new TPad("pad5", "", arrayBound8PanPResoX1_4[3], arrayBound8PanPResoY1_4[1], arrayBound8PanPResoX1_4[4], arrayBound8PanPResoY1_4[0],-1, -1, -2);
////        DrawGammaPadSettings( pad5, relativeMarginsEResoX[1], relativeMarginsEResoX[2], relativeMarginsEResoY[0], relativeMarginsEResoY[2]);
////        pad5->SetNumber(5);
////        pad5->Draw();
//    
//        
//        //update the first parameter so we can take different root files as inputs
//        std::vector<string> fileNames = testParserTStrings(fileName);
//        std::vector<string> energiesLegend = testParserTStrings(energy);
//        //std::vector<string> wlayersLegend = testPartTString(Wlayers);
//        
//        //loop thru the different files
//        for (Int_t k = 0; k< fileNames.size(); k++){
//  
//            if(fileNames.size()>1) {cout<<"Too many files to overlay assembly boards. Please input a single file"<<endl; break;}
//            TFile *inputfile = new TFile(fileNames[k].c_str());
//            
//            float x1[13]={};
//            float y1[13]={};
//            float x2[13]={};
//            float y2[13]={};
//            for(int i = 0; i<6; i++){
//            if(i==6)break;
//                if (i ==0){
//                    x1[i] = 0.50;
//                    x2[i] = 0.70;
//                }
//                else{
//                    x1[i] = 0.50 ;
//                    x2[i] = 0.70 ;
//                }
//                y1[i] = 0.95 - 0.07*i;
//                y2[i] = 0.90 - 0.07*i;
//            }
//            for(int i = 6; i<13; i++){
//                if (i ==6){
//                    x1[i] = 0.50+0.2;
//                    x2[i] = 0.90;
//                    cout<<"x1 "<<x1[i]<<" x2 "<<x2[i]<<endl;
//                }
//                else{
//                    x1[i] = 0.70 ;
//                    x2[i] = 0.90 ;
//                     cout<<"x1 "<<x1[i]<<" x2 "<<x2[i]<<endl;
//                }
//                y1[i] = 0.95 - 0.07*(i-6);
//                y2[i] = 0.90 - 0.07*(i-6);
//                 cout<<"y1 "<<y1[i]<<" y2 "<<y2[i]<<endl;
//            }
//            //Now we want to loop through the TH1Fs from the file and get them all!
//            for (Int_t j = 0; j < gMaxBoard; j++){
//                for (Int_t i = 0; i < gMaxChannels; i++){
//                    if (j == 1){
//                        if (std::find(A11.begin(), A11.end(),i)!=A11.end() ){
//                            TLegend* leg11 = new TLegend(x1[0], y1[0], x2[0], y2[0]);
//                            graphingHistOnSepPad(canvasPReso8PanelPlot,i,j,A11,11,inputfile,"A11",noNoiseHistsLG[j][i],col_overlay,y_max,true,xrange,leg11,rebinfac,histPrefix);
//            
//                        }
//                        if (std::find(A8.begin(), A8.end(),i)!=A8.end() ){
//                             TLegend* leg2 = new TLegend(x1[1], y1[1], x2[1], y2[1]);
//                            graphingHistOnSepPad(canvasPReso8PanelPlot,i,j,A8,8,inputfile,"A8",noNoiseHistsLG[j][i],col_overlay,y_max,true,xrange,leg2,rebinfac,histPrefix);
//                            
//                        }
//                        if (std::find(A2.begin(), A2.end(),i)!=A2.end() ){
//                            TLegend* leg3 = new TLegend(x1[2], y1[2], x2[2], y2[2]);
//                            graphingHistOnSepPad(canvasPReso8PanelPlot,i,j,A2,2,inputfile,"A2",noNoiseHistsLG[j][i],col_overlay,y_max,true,xrange,leg3,rebinfac,histPrefix);
//                            
//                        }
//                        if (std::find(A3.begin(), A3.end(),i)!=A3.end() ){
//                            TLegend* leg4 = new TLegend(x1[3], y1[3], x2[3], y2[3]);
//                            graphingHistOnSepPad(canvasPReso8PanelPlot,i,j,A3,3,inputfile,"A3",noNoiseHistsLG[j][i],col_overlay,y_max,true,xrange,leg4,rebinfac,histPrefix);
//                        }
//                        if (std::find(A13.begin(), A13.end(),i)!=A13.end() ){
//                            TLegend* leg5 = new TLegend(x1[4], y1[4], x2[4], y2[4]);
//                            graphingHistOnSepPad(canvasPReso8PanelPlot,i,j,A13,13,inputfile,"A13",noNoiseHistsLG[j][i],col_overlay,y_max,true,xrange,leg5,rebinfac,histPrefix);
//                            
//                        }
//                        if (std::find(A5.begin(), A5.end(),i)!=A5.end() ){
//                           TLegend* leg6 = new TLegend(x1[5], y1[5], x2[5], y2[5]);
//                            graphingHistOnSepPad(canvasPReso8PanelPlot,i,j,A5,5,inputfile,"A5",noNoiseHistsLG[j][i],col_overlay,y_max,true,xrange,leg6,rebinfac,histPrefix);
//                            
//                        }
//                        if (std::find(A6.begin(), A6.end(),i)!=A6.end() ){
//                            TLegend* leg7 = new TLegend(x1[6], y1[6], x2[6], y2[6]);
//                            graphingHistOnSepPad(canvasPReso8PanelPlot,i,j,A6,6,inputfile,"A6",noNoiseHistsLG[j][i],col_overlay,y_max,true,xrange,leg7,rebinfac,histPrefix);
//                            
//                        }
//                        if (std::find(A7.begin(), A7.end(),i)!=A7.end() ){
//                           TLegend* leg8 = new TLegend(x1[7], y1[7], x2[7], y2[7]);
//                            graphingHistOnSepPad(canvasPReso8PanelPlot,i,j,A7,7,inputfile,"A7",noNoiseHistsLG[j][i],col_overlay,y_max,true,xrange,leg8,rebinfac,histPrefix);
//                            
//                        }
//                        
//                        
//                    }//checking if digitizer board 0
//                    if (j==0){
//                        if (std::find(A0.begin(), A0.end(),i)!=A0.end() ){
//                             TLegend* leg0 = new TLegend(x1[8], y1[8], x2[8], y2[8]);
//                            graphingHistOnSepPad(canvasPReso8PanelPlot,i,j,A0,0,inputfile,"A0",noNoiseHistsLG[j][i],col_overlay,y_max,true,xrange,leg0,rebinfac,histPrefix);
//                        }
//                        if (std::find(A1.begin(), A1.end(),i)!=A1.end() ){
//                              TLegend* leg1 = new TLegend(x1[9], y1[9], x2[9], y2[9]);
//                            graphingHistOnSepPad(canvasPReso8PanelPlot,i,j,A1,1,inputfile,"A1",noNoiseHistsLG[j][i],col_overlay,y_max,true,xrange,leg1,rebinfac,histPrefix);
//                            
//                        }
//                        if (std::find(A9.begin(), A9.end(),i)!=A9.end() ){
//                            TLegend* leg9 = new TLegend(x1[10], y1[10], x2[10], y2[10]);
//                            graphingHistOnSepPad(canvasPReso8PanelPlot,i,j,A9,9,inputfile,"A9",noNoiseHistsLG[j][i],col_overlay,y_max,true,xrange,leg9,rebinfac,histPrefix);
//                            
//                        }
//                        if (std::find(A12.begin(), A12.end(),i)!=A12.end() ){
//                             TLegend* leg12 = new TLegend(x1[11], y1[11], x2[11], y2[11]);
//                            graphingHistOnSepPad(canvasPReso8PanelPlot,i,j,A12,12,inputfile,"A12",noNoiseHistsLG[j][i],col_overlay,y_max,true,xrange,leg12,rebinfac,histPrefix);
//                            
//                        }
//                        if (std::find(A4.begin(), A4.end(),i)!=A4.end() ){
//                            TLegend* leg4 = new TLegend(x1[12], y1[12], x2[12], y2[12]);
//                            graphingHistOnSepPad(canvasPReso8PanelPlot,i,j,A4,4,inputfile,"A4",noNoiseHistsLG[j][i],col_overlay,y_max,true,xrange,leg4,rebinfac,histPrefix);
//                            
//                        }
//                        
//                        
//                    }//checking if digitizer board 1
//                    
//                }//
            //}//end of board loop
            
      //  }//end of TFile loop!
  //  }//end of checkiing for overlay :)  loop
    outRoot.Close();
    
}



                        
                   
