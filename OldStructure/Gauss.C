#include <TCanvas.h>
#include <TH1F.h>
#include <TF1.h>
#include <TRandom3.h>

const int gMaxChannels = 64;
const int gMaxBoard    = 1;
// track hits
Long64_t gTrID;
Double_t gTRtimeStamp;
Long64_t* gBoard          = new Long64_t[gMaxChannels];
Long64_t* gChannel        = new Long64_t[gMaxChannels];
Long64_t* gLG             = new Long64_t[gMaxChannels];
Long64_t* gHG             = new Long64_t[gMaxChannels];
void SetBranchAddressesTree(TTree* inputTree){
    //**********************************
    //format
    //**********************************

    //     *Tree    :tree      :                                                        *
    //     ******************************************************************************
    //     *Br    0 :t_stamp   : t_stamp/D                                              *
    //     *Br    1 :trgid     : trgid/L                                                *
    //     *Br    2 :board     : board[64]/L                                            *
    //     *Br    3 :channel   : channel[64]/L                                          *
    //     *Br    4 :LG        : LG[64]/L                                               *
    //     *Br    5 :HG        : HG[64]/L                                               *
    //******************************************************************************
  
  
    if (inputTree->GetBranchStatus("t_stamp") ){
      inputTree->SetBranchAddress("trgid",          &gTrID);
      inputTree->SetBranchAddress("t_stamp",          &gTRtimeStamp);
      inputTree->SetBranchAddress("board",            gBoard);
      inputTree->SetBranchAddress("channel",          gChannel);
      inputTree->SetBranchAddress("LG",               gLG);
      inputTree->SetBranchAddress("HG",               gHG);
    }
}

void Gauss(const char* noisefile, const char* datafile, int runnumber, TString outputDir    = "NoiseSubData/MIP/")
{
    
    
    outputDir = Form("%s/Run%05d",outputDir.Data(), runnumber );
    gSystem->Exec("mkdir -p "+outputDir);
    // Create a ROOT canvas to display the histogram and fit results
    TCanvas *c1 = new TCanvas("c1", "Gaussian Fit Example", 800, 600);
    
    //When you dont have a noise sweep, you can fit the noise up to 40ADC. So the noise and data file are the same in this case except you still have to subtract at the tree level
    //This is the processed noise data.
    TFile* fnoise = new TFile(noisefile,"READ");
    //This is th unprocessed data file
    TFile* fdata = new TFile(datafile,"READ");
    
    //These are noise histograms that exist in processed data file
    TH1F* nhistLG[gMaxBoard][gMaxChannels];//Histogram array
    TH1F* nhistHG[gMaxBoard][gMaxChannels];
    
    TH1F* histLG[gMaxBoard][gMaxChannels];
    TH1F* histHG[gMaxBoard][gMaxChannels];
    
    //noise fit histograms
    TH1F* nfithistLG[gMaxBoard][gMaxChannels];
    TH1F* nfithistHG[gMaxBoard][gMaxChannels];
    
    //signal histogram
    TH1F* shistLG[gMaxBoard][gMaxChannels];
    TH1F* shistHG[gMaxBoard][gMaxChannels];
    
    const int rows = gMaxBoard; // Number of rows
    const int cols = gMaxChannels; // Number of columns
    std::vector<std::vector<double>> gauFitSigma(rows, std::vector<double>(cols));
    std::vector<std::vector<double>> gauFitMean(rows, std::vector<double>(cols));
    
    std::vector<std::vector<double>> gauFitSigmaLG(rows, std::vector<double>(cols));
    std::vector<std::vector<double>> gauFitMeanLG(rows, std::vector<double>(cols));
    
    TF1* gauFit[gMaxBoard][gMaxChannels];
     

    //Create histograms noise, data and signal
    for (Int_t j = 0; j < gMaxBoard; j++)
    {
        for (Int_t i = 0; i < gMaxChannels; i++)
        {
            // Define the Gaussian function. Will define custom parameters after the fit.
            nfithistHG[j][i] = new TH1F(Form("hfit_HG_B%d_C%02d",j,i),"",4001, 0, 4001);
            nfithistLG[j][i] = new TH1F(Form("hfit_LG_B%d_C%02d",j,i),"",4001, 0, 4001);
            //Data histogram
            histHG[j][i] = new TH1F(Form("HG_TriggB%d_C%02d",j,i),"",4001,0,4001);//changing the name for each hist and adding to histogram list
            histHG[j][i]->RebinX(2);
            histLG[j][i] = new TH1F(Form("LG_B%d_C%02d",j,i),"",4001,0,4001);
            //Subtracted histogram
            shistHG[j][i] = new TH1F(Form("sh_HG_B%d_C%02d",j,i),"",4001,0,4001);//changing the name for each hist and adding to histogram list
            shistHG[j][i]->RebinX(2);
            shistLG[j][i] = new TH1F(Form("sh_LG_B%d_C%02d",j,i),"",4001,0,4001);
            
        }
    }

     TRandom3 randomGenerator;
    //Now fit the noise data and create fit histograms
    for (Int_t j = 0; j < gMaxBoard; j++){
        for (Int_t i = 0; i < gMaxChannels; i++){
            //noise histogram
            nhistHG[j][i] = (TH1F*)fnoise->Get(Form("h_HGNoise_B%d_C%02d",j,i));
            nhistHG[j][i]->RebinX(2);
//            nhistHG[j][i]->Scale(1./nhistHG[j][i]->Integral());
            
            cout<<nhistHG[j][i]->GetName()<<endl;
            cout<<nhistHG[j][i]->GetEntries()<<endl;
            cout<<nhistHG[j][i]->GetMaximum()<<endl;
            
            TF1 *gaussianFit = new TF1("gaussianFit", "gaus");

//
            gaussianFit->SetParameter(0, nhistHG[j][i]->GetMaximum()); // Amplitude
            gaussianFit->SetParameter(1, nhistHG[j][i]->GetMean());   // Mean
            gaussianFit->SetParameter(2, nhistHG[j][i]->GetStdDev());   // Sigma
            nhistHG[j][i]->Fit("gaussianFit", "R","",0,100); //fit a gaussian to the noise histogram
        

            double_t amplitude = gaussianFit->GetParameter(0);
            double_t mean = gaussianFit->GetParameter(1);
            double_t sigma = gaussianFit->GetParameter(2);
            

           gauFitMean[j][i] = mean;
           gauFitSigma[j][i] = sigma;
           gauFit[j][i]= gaussianFit;

            
        }
    }
    
    //Now open the data file, read it and subtract noise from signal on the fly
    //For the actual signal we have raw data
    
    TTree *tt_event = (TTree*)fdata->Get("tree");
    SetBranchAddressesTree(tt_event);
    double nEntriesTree = tt_event->GetEntries();
    Long64_t startEvent = 0;
    Long64_t nEventsProcessed=0;
    for (Long64_t i=startEvent; i<nEntriesTree;i++)
    {
        // load current event
        tt_event->GetEntry(i);
        nEventsProcessed++;
        for(Int_t ch=0; ch<gMaxChannels; ch++)
        {
             
            histHG[gBoard[ch]][gChannel[ch]]->Fill(gHG[ch]);

            shistHG[gBoard[ch]][gChannel[ch]]->Fill((gHG[ch]-randomGenerator.Gaus(gauFitMean[gBoard[ch]][gChannel[ch]],gauFitSigma[gBoard[ch]][gChannel[ch]])));

//            cout<<gauFit[gBoard[ch]][gChannel[ch]]->GetRandom()<<endl;
            
        }
//
    }
    

    //    histHG[0][24]->SetLineColor(kBlue);
    //    histHG[0][24]->GetXaxis()->SetRangeUser(0,500);
    //    histHG[0][24]->Draw();
    TLegend *legend_dat = new TLegend(0.60,0.60,0.80,0.90);
    legend_dat->AddEntry(nhistHG[0][24],"Noise","l");
    legend_dat->AddEntry(shistHG[0][24],"Noise subtracted Triggered","l");
    legend_dat->SetBorderSize(0);
    nhistHG[0][24]->SetLineColor(kBlue);
    nhistHG[0][24]->GetXaxis()->SetTitle("ADC");
    nhistHG[0][24]->GetYaxis()->SetTitle("Entries");
    nhistHG[0][24]->Draw();
    shistHG[0][24]->SetLineColor(kGreen+4);
    shistHG[0][24]->Draw("SAME");
    legend_dat->Draw("SAME");
    
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    //Write this to a new file
    //Get_run_name from data file
    
    TFile* fileOutput = new TFile(Form("%s/%s",outputDir.Data(),datafile),"RECREATE");
    for (Int_t j = 0; j < gMaxBoard; j++)
    {
        for (Int_t i = 0; i < gMaxChannels; i++)
        {
            shistHG[j][i]->Write();
            shistLG[j][i]->Write();

        }
    }
    
}
   
 
   
