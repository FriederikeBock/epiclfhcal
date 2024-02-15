
#ifndef TBPARSINGSIMPLE
#define TBPARSINGSIMPLE


  //__________________________________________________________________________________________________________
  //__________________________________________________________________________________________________________
  //__________________________________________________________________________________________________________
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
        inputTree->SetBranchAddress("channel",                 gChannel);
        inputTree->SetBranchAddress("LG",               gLG);
        inputTree->SetBranchAddress("HG",               gHG);
      }
  }

  //__________________________________________________________________________________________________________
  //_________________________ Determine neighbors in layer
  //__________________________________________________________________________________________________________
  void GetNeighborWithinBoard(Int_t chBoard, Int_t &nNeighbors, Int_t* neighbors){
    nNeighbors = 0;
    // reset neighbors array
    for (Int_t i = 0; i < 5; i++) neighbors[i]= -1;
    if (chBoard == 1){
        neighbors[0] = 2;
        neighbors[1] = 8;
        neighbors[2] = 7;
    } else if (chBoard == 2){
        neighbors[0] = 1;
        neighbors[1] = 3;
        neighbors[2] = 7;
        neighbors[3] = 6;
        neighbors[4] = 8;
    } else if (chBoard == 3){
        neighbors[0] = 2;
        neighbors[1] = 4;
        neighbors[2] = 6;
        neighbors[3] = 5;
        neighbors[4] = 7;
    } else if (chBoard == 4){
        neighbors[0] = 3;
        neighbors[1] = 5;
        neighbors[2] = 6;
    } else if (chBoard == 5){
        neighbors[0] = 6;
        neighbors[1] = 4;
        neighbors[2] = 3;
    } else if (chBoard == 6){
        neighbors[0] = 7;
        neighbors[1] = 5;
        neighbors[2] = 3;
        neighbors[3] = 4;
        neighbors[4] = 2;
    } else if (chBoard == 7){
        neighbors[0] = 8;
        neighbors[1] = 6;
        neighbors[2] = 2;
        neighbors[3] = 3;
        neighbors[4] = 1;
    } else if (chBoard == 8){  
        neighbors[0] = 7;
        neighbors[1] = 1;
        neighbors[2] = 2;
    }
    for (Int_t i = 0; i < 5; i++){
      if (neighbors[i] != -1) nNeighbors++;
    }
    return;
  }


  //__________________________________________________________________________________________________________
  // find bin with largest content
  //__________________________________________________________________________________________________________
  Double_t FindLargestBin1DHist(TH1* hist, Double_t minX = -10000, Double_t maxX = -10000 ){
    Double_t largestContent     = 0;
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

  //__________________________________________________________________________________________________________
  // Cleanup results graphs
  //  - removes entries from TGraphErrors if their entry & error is 0 (default) or another setable y value and error 0
  //__________________________________________________________________________________________________________
  void CleanUpResultsGraph(TGraphErrors* graph, TString nameUpdate,  Double_t yClean = 0 ){
    for (Int_t i= 0; i < graph->GetN(); i++){
      if (graph->GetY()[i] == yClean && graph->GetEY()[i] == 0){
        graph->RemovePoint(i);
        i--;
      }
    }
    graph->SetName(nameUpdate.Data());
  }
  

  //__________________________________________________________________________________________________________
  // Creates TGraphErrors from TH1 and cleans up unwanted values
  //  - creates graph
  //  - sets name
  //  - sets axis lables according to histo labels
  //  - removes entries from TGraphErrors if their entry & error is 0 (default) or another setable y value and error 0
  //__________________________________________________________________________________________________________
  TGraphErrors* CreateGraphFromHistAndCleanup(TH1* hist, TString nameUpdate,  Double_t yClean = 0 ){
    TGraphErrors* graph = new TGraphErrors(hist);
    graph->SetName(nameUpdate.Data());
    graph->GetXaxis()->SetTitle(hist->GetXaxis()->GetTitle());
    graph->GetYaxis()->SetTitle(hist->GetYaxis()->GetTitle());
    for (Int_t i= 0; i < graph->GetN(); i++){
      if (graph->GetY()[i] == yClean && graph->GetEY()[i] == 0){
        graph->RemovePoint(i);
        i--;
      }
    }
    return graph;
  }
  


  //__________________________________________________________________________________________________________
  // Plot noise nicely
  //__________________________________________________________________________________________________________
  void PlotNoiseSingle (TCanvas* canvas, TH1D* histo, TF1* fit, 
                        Double_t mean, Double_t meanErr, Double_t sigma, Double_t sigmaErr, 
                        Int_t cb, Int_t cc, Int_t sl, Int_t sc, 
                        TString baseNameOut, runInfo currRunInfo,  Double_t textSizeRel = 0.04){
    canvas->cd();
      SetStyleHistoTH1ForGraphs(  histo,  histo->GetXaxis()->GetTitle(),  histo->GetYaxis()->GetTitle(), 0.85*textSizeRel, textSizeRel, 0.85*textSizeRel, textSizeRel,0.9, 0.9);  
      SetMarkerDefaults( histo, 20, 0.8, kBlue+1,kBlue+1);
      SetStyleFit(fit , 0, 120, 7, 7, kBlack);
      histo->GetXaxis()->SetRangeUser(0,200);
      histo->Draw("pe");
      fit->Draw("same");

      DrawLatex(0.95, 0.92, Form("%s-beam, #it{E}_{#it{b}}= %.0f GeV, Run %d, #it{V}_{#it{op}} = %1.1f V", currRunInfo.species.Data(), currRunInfo.energy, currRunInfo.runNr, currRunInfo.vop  ), true, textSizeRel, 42);
      
      TLegend* legend = GetAndSetLegend2( 0.58, 0.76-textSizeRel, 0.95, 0.93-textSizeRel,textSizeRel, 1, Form("CAEN B %d, C %d, Stack L %d, C%d",cb, cc, sl, sc), 42,0.2);
      legend->AddEntry(fit, "Gauss noise fit", "l");
      legend->AddEntry((TObject*)0, Form("#mu = %2.2f #pm %2.2f",mean, meanErr ) , " ");
      legend->AddEntry((TObject*)0, Form("#sigma = %2.2f #pm %2.2f",sigma, sigmaErr ) , " ");
      legend->Draw();
    canvas->SaveAs(Form("%s_B%d_C%02d.pdf", baseNameOut.Data(), cb,cc));
    return;
  }

  //__________________________________________________________________________________________________________
  // Plot noise nicely
  //__________________________________________________________________________________________________________
  void PlotMIPSingle (TCanvas* canvas, TH1D* histo, TF1* fit, 
                        double chi2, int ndf, double maxLG, double fwhm,
                        Int_t sl, Int_t sc, 
                        TString baseNameOut, runInfo currRunInfo, Double_t textSizeRel = 0.04){
    canvas->cd();
    canvas->SetLogy(0);
      histo->GetYaxis()->SetRangeUser(0,FindLargestBin1DHist(histo,150,2200)*1.1);
      SetStyleHistoTH1ForGraphs(  histo,  histo->GetXaxis()->GetTitle(),  histo->GetYaxis()->GetTitle(), 0.85*textSizeRel, textSizeRel, 0.85*textSizeRel, textSizeRel,0.9, 0.9);  
      SetMarkerDefaults( histo, 20, 0.2, kBlue+1,kBlue+1);
      SetStyleFit(fit , 0, 2200, 7, 7, kBlack);
      histo->GetXaxis()->SetRangeUser(0,2200);
      histo->Draw("hist,pe");
      fit->Draw("same");
      
      DrawLatex(0.95, 0.92, Form("%s-beam, #it{E}_{#it{b}}= %.0f GeV, Run %d, #it{V}_{#it{op}} = %1.1f V", currRunInfo.species.Data(), currRunInfo.energy, currRunInfo.runNr, currRunInfo.vop  ), true, textSizeRel, 42);
      
      TLegend* legend = GetAndSetLegend2( 0.63, 0.69-textSizeRel, 0.95, 0.93-textSizeRel,textSizeRel, 1, Form("Stack L %d, C%d",sl, sc), 42,0.12);
      legend->AddEntry(fit, "Landau-Gauss fit", "l");
      legend->AddEntry((TObject*)0, Form("#chi^{2}/ndf = %2.2f",(Double_t)(chi2/ndf) ) , " ");
      legend->AddEntry((TObject*)0, Form("MPV = %2.2f #pm %2.2f",(Double_t)(fit->GetParameter(1)), (Double_t)(fit->GetParError(1))  ) , " ");
      legend->AddEntry((TObject*)0, Form("Max = %2.2f",(Double_t)(maxLG) ) , " ");
      legend->AddEntry((TObject*)0, Form("FWHM = %2.2f",(Double_t)(fwhm) ) , " ");
      legend->Draw();
    canvas->SaveAs(Form("%s_L%d_C%02d.pdf", baseNameOut.Data(), sl,sc));
    
    canvas->SetLogy(1);
      histo->GetYaxis()->SetRangeUser(1,FindLargestBin1DHist(histo,150,2200)*3);
      histo->Draw("pe");
      fit->Draw("same");
      DrawLatex(0.95, 0.92, Form("%s-beam, #it{E}_{#it{b}}= %.0f GeV, Run %d, #it{V}_{#it{op}} = %1.1f V", currRunInfo.species.Data(), currRunInfo.energy, currRunInfo.runNr, currRunInfo.vop  ), true, textSizeRel, 42);
      legend->Draw();

    canvas->SaveAs(Form("%s_L%d_C%02d_logY.pdf", baseNameOut.Data(), sl,sc));
    return;
  }

  //__________________________________________________________________________________________________________
  // Plot overlay of different triggers
  //__________________________________________________________________________________________________________
  void PlotOverlayDiffTriggers( TCanvas* canvas, TH1D* hAll, TH1D* hTriggC, TH1D* hTriggN, TF1* fitN,
                                Double_t minPX, Double_t maxPX, TString baseNameOut,
                                Int_t cb, Int_t cc, Int_t sl, Int_t sc, runInfo currRunInfo,
                                Float_t textSizeRel = 0.04 ){
    canvas->cd();
      SetStyleHistoTH1ForGraphs( hAll, hAll->GetXaxis()->GetTitle(), hAll->GetYaxis()->GetTitle(), 0.85*textSizeRel, textSizeRel, 0.85*textSizeRel, textSizeRel,0.9, 0.9);  
      SetMarkerDefaults( hAll, 20, 0.8, kBlue+1,kBlue+1);
      
      hAll->GetXaxis()->SetRangeUser(minPX,maxPX);
      hAll->Draw("pe");
      if (hTriggN){
  //                   SetMarkerDefaults( hTriggN, 24, 0.8, kBlack,kBlack);
          SetLineDefaults( hTriggN,kBlack, 2, 1);
          hTriggN->Draw("hist,same");
      }
      if (hTriggC){
  //                   SetMarkerDefaults( hTriggC, 20, 0.8, kRed+1,kRed+1);
          SetLineDefaults( hTriggC,kRed+1, 3, 1);
          hTriggC->Draw("hist,same");
      }
      if (fitN){
        SetStyleFit(fitN, 0, 120, 7, 7, kGray+1);
        fitN->Draw("same");
        DrawLines(fitN->GetParameter(1),fitN->GetParameter(1), hAll->GetMinimum(), hAll->GetMaximum()*0.3, 3, kGray+2, 3, 1);
      }
      
      DrawLatex(0.95, 0.92, Form("%s-beam, #it{E}_{#it{b}}= %.0f GeV, Run %d, #it{V}_{#it{op}} = %1.1f V", currRunInfo.species.Data(), currRunInfo.energy, currRunInfo.runNr, currRunInfo.vop  ), true, textSizeRel, 42);
      
      TLegend* legend = GetAndSetLegend2( 0.61, 0.75-textSizeRel, 0.95, 0.93-textSizeRel,textSizeRel, 1, Form("CAEN B %d, C %d, Stack L %d, C%d",cb, cc, sl, sc), 42,0.2);
      legend->AddEntry(hAll, "All triggers", "p");
      if (hTriggC)legend->AddEntry(hTriggC, "Straight line trigg", "l");
      if (hTriggN)legend->AddEntry(hTriggN, "noise trigg", "l");
      if (fitN)legend->AddEntry(fitN, "noise fit", "l");
      legend->Draw();
    canvas->SaveAs(Form("%s_B%d_C%02d.pdf", baseNameOut.Data(), cb,cc));
    canvas->SaveAs(Form("%s_L%d_C%02d.pdf", baseNameOut.Data(), sl, sc));
    return;
  }


  //__________________________________________________________________________________________________________
  // Plot overlay of different channels within one readout board
  //__________________________________________________________________________________________________________
  void PlotChannelOverlaySameLayer( TCanvas* canvas, TH1D** histos, Int_t minC, Int_t maxC,
                                  Double_t minPX, Double_t maxPX, Double_t minPY,  TString baseNameOut,
                                  Int_t sl, runInfo currRunInfo,
                                  Float_t textSizeRel = 0.04, TString plotStyle = "p,e" ){
    canvas->cd();
      Int_t maxCount = 0;
      TLegend* legend = GetAndSetLegend2( 0.58, 0.93-4*textSizeRel, 0.95, 0.93-textSizeRel,textSizeRel, 4, Form("Layer %d, channel:",sl), 42,0.2);
      for (Int_t c = minC; c < maxC; c++){
        if (maxCount < FindLargestBin1DHist(histos[c],minPX, maxPX)) maxCount = FindLargestBin1DHist(histos[c],minPX, maxPX);
      }
      for (Int_t c = minC; c < maxC; c++){
        SetStyleHistoTH1ForGraphs( histos[c], histos[c]->GetXaxis()->GetTitle(), histos[c]->GetYaxis()->GetTitle(), 0.85*textSizeRel, textSizeRel, 0.85*textSizeRel, textSizeRel,0.9, 0.9);  
        SetMarkerDefaults( histos[c], markerReadBoard[c-1], 0.8, colorReadBoard[c-1],colorReadBoard[c-1]);
        if (plotStyle.Contains("hist")) histos[c]->SetLineWidth(3);
        histos[c]->GetXaxis()->SetRangeUser(minPX,maxPX);
        histos[c]->GetYaxis()->SetRangeUser(0.7*minPY,maxCount*1.1);      
        if (c == minC) histos[c]->Draw(plotStyle.Data());
        else histos[c]->Draw(Form("%s,same",plotStyle.Data()));
        if (c == maxC-1) histos[minC]->Draw("same,axis");
        if (plotStyle.Contains("p")) legend->AddEntry(histos[c], Form("%d",c), "p");
        else legend->AddEntry(histos[c], Form("%d",c), "l");
      }
      
      DrawLatex(0.95, 0.92, Form("%s-beam, #it{E}_{#it{b}}= %.0f GeV, Run %d, #it{V}_{#it{op}} = %1.1f V", currRunInfo.species.Data(), currRunInfo.energy, currRunInfo.runNr, currRunInfo.vop  ), true, textSizeRel, 42);

      legend->Draw();
    canvas->SaveAs(Form("%s_L%d.pdf", baseNameOut.Data(), sl));
    return;
  }

  //__________________________________________________________________________________________________________
  // Plot overlay of different channels within one readout board
  //__________________________________________________________________________________________________________
  void PlotChannelOverlaySameReadoutChannel( TCanvas* canvas, TH1D* histos[gMaxLayers][9], Bool_t* bLayer, Int_t minL, Int_t maxL,
                                            Double_t minPX, Double_t maxPX, Double_t minPY,  TString baseNameOut,
                                            Int_t sc, runInfo currRunInfo,
                                            Float_t textSizeRel = 0.04, TString plotStyle = "p,e" ){
    canvas->cd();
      Int_t maxCount = 0;
      TLegend* legend = GetAndSetLegend2( 0.58, 0.93-4*textSizeRel, 0.95, 0.93-textSizeRel,textSizeRel, 4, Form("Read-out channel %d, layer:",sc), 42,0.2);
      for (Int_t l = minL; l < maxL; l++){
        if (!bLayer[l]) continue;
        if (maxCount < FindLargestBin1DHist(histos[l][sc],minPX, maxPX)) maxCount = FindLargestBin1DHist(histos[l][sc],minPX, maxPX);
      }
      for (Int_t l = minL; l < maxL; l++){
        if (!bLayer[l]) continue;
        SetStyleHistoTH1ForGraphs( histos[l][sc], histos[l][sc]->GetXaxis()->GetTitle(), histos[l][sc]->GetYaxis()->GetTitle(), 0.85*textSizeRel, textSizeRel, 0.85*textSizeRel, textSizeRel,0.9, 0.9);  
        SetMarkerDefaults( histos[l][sc], markerLayer[l], 0.8, colorLayer[l],colorLayer[l]);
        if (plotStyle.Contains("hist")) histos[l][sc]->SetLineWidth(3);
        histos[l][sc]->GetXaxis()->SetRangeUser(minPX,maxPX);
        histos[l][sc]->GetYaxis()->SetRangeUser(0.7*minPY,maxCount*1.1);      
        if (l == minL) histos[l][sc]->Draw(plotStyle.Data());
        else histos[l][sc]->Draw(Form("%s,same",plotStyle.Data()));
        histos[minL][sc]->Draw("same,axis");
        if (plotStyle.Contains("p")) legend->AddEntry(histos[l][sc], Form("%d",l), "p");
        else legend->AddEntry(histos[l][sc], Form("%d",l), "l");
      }
      
      DrawLatex(0.95, 0.92, Form("%s-beam, #it{E}_{#it{b}}= %.0f GeV, Run %d, #it{V}_{#it{op}} = %1.1f V", currRunInfo.species.Data(), currRunInfo.energy, currRunInfo.runNr, currRunInfo.vop  ), true, textSizeRel, 42);

      legend->Draw();
    canvas->SaveAs(Form("%s_RB%d.pdf", baseNameOut.Data(), sc));
    return;
  }

  //__________________________________________________________________________________________________________
  // Plot Noise with Fits for Full layer
  //__________________________________________________________________________________________________________
  void PlotNoiseWithFitsFullLayer (TCanvas* canvas8Panel, TPad* pads[8], Double_t* topRCornerX,  Double_t* topRCornerY, Double_t* relSize8P, Int_t textSizePixel, 
                                  TH1D** hists, TF1** fits,
                                  Double_t xPMin, Double_t xPMax, Double_t scaleYMax, Int_t layer, TString nameOutput, runInfo currRunInfo){
                                  
    Double_t maxY = 0;
    for (Int_t p = 0; p < 8; p++){
      if (maxY < FindLargestBin1DHist(hists[p+1], xPMin , xPMax)) maxY = FindLargestBin1DHist(hists[p+1], xPMin , xPMax);
    }
    
    for (Int_t p = 0; p < 8; p++){
      canvas8Panel->cd();
      pads[p]->Draw();
      pads[p]->cd();
      pads[p]->SetLogy();
      SetStyleHistoTH1ForGraphs( hists[p+1], hists[p+1]->GetXaxis()->GetTitle(), hists[p+1]->GetYaxis()->GetTitle(), 0.85*textSizePixel, textSizePixel, 0.85*textSizePixel, textSizePixel,0.9, 1.1, 510, 510, 43, 63);  
      SetMarkerDefaults(hists[p+1], 20, 1, kBlue+1, kBlue+1, kFALSE);   
      hists[p+1]->GetXaxis()->SetRangeUser(xPMin,xPMax);
      hists[p+1]->GetYaxis()->SetRangeUser(0.7,scaleYMax*maxY);
      
      hists[p+1]->Draw("pe");
      
      TString label = Form("RB ch. %d", p+1);
      if (p == 4) label = Form("layer %d, RB ch. %d", layer, p+1);
      TLatex *labelChannel    = new TLatex(topRCornerX[p]-0.04,topRCornerY[p]-1.2*relSize8P[p],label);
      SetStyleTLatex( labelChannel, 0.85*textSizePixel,4,1,43,kTRUE,31);

      if (fits){
        if (fits[p+1]){
          SetStyleFit(fits[p+1] , 0, 120, 7, 7, kBlack);
          fits[p+1]->Draw("same");
          TLegend* legend = GetAndSetLegend2( topRCornerX[p]-8*relSize8P[p], topRCornerY[p]-4*0.85*relSize8P[p]-0.4*relSize8P[p], topRCornerX[p]-0.04, topRCornerY[p]-0.6*relSize8P[p],0.85*textSizePixel, 1, label, 43,0.2);
          legend->AddEntry(fits[p+1], "Gauss noise fit", "l");
          legend->AddEntry((TObject*)0, Form("#mu = %2.2f #pm %2.2f",fits[p+1]->GetParameter(1), fits[p+1]->GetParError(1) ) , " ");
          legend->AddEntry((TObject*)0, Form("#sigma = %2.2f #pm %2.2f",fits[p+1]->GetParameter(2), fits[p+1]->GetParError(2) ) , " ");
          legend->Draw();

        } else {
          labelChannel->Draw();
        }
      } else {
        labelChannel->Draw();
      }
      if (p ==4 ){
        DrawLatex(topRCornerX[p]-0.04, topRCornerY[p]-4*0.85*relSize8P[p]-1.4*relSize8P[p], Form("%s-beam, #it{E}_{#it{b}}= %.0f GeV", currRunInfo.species.Data(), currRunInfo.energy), true, 0.85*relSize8P[p], 42);
        DrawLatex(topRCornerX[p]-0.04, topRCornerY[p]-4*0.85*relSize8P[p]-2.2*relSize8P[p], Form("Run %d, #it{V}_{#it{op}} = %1.1f V", currRunInfo.runNr, currRunInfo.vop  ), true, 0.85*relSize8P[p], 42);
      }
    }
    canvas8Panel->SaveAs(nameOutput.Data());
  }

  //__________________________________________________________________________________________________________
  // Plot Noise with Fits for Full layer
  //__________________________________________________________________________________________________________
  void PlotDiffTriggersFullLayer (TCanvas* canvas8Panel, TPad* pads[8], Double_t* topRCornerX,  Double_t* topRCornerY, Double_t* relSize8P, Int_t textSizePixel, 
                                  TH1D** histsAll,  TH1D** hTriggC, TH1D** hTriggN, TF1** fits,
                                  Double_t xPMin, Double_t xPMax, Double_t scaleYMax, Int_t layer, TString nameOutput, runInfo currRunInfo){
                                  
    Double_t maxY = 0;
    for (Int_t p = 0; p < 8; p++){
      if (maxY < FindLargestBin1DHist(histsAll[p+1], xPMin , xPMax)) maxY = FindLargestBin1DHist(histsAll[p+1], xPMin , xPMax);
    }
    
    for (Int_t p = 0; p < 8; p++){
      canvas8Panel->cd();
      pads[p]->Draw();
      pads[p]->cd();
      pads[p]->SetLogy();
      SetStyleHistoTH1ForGraphs( histsAll[p+1], histsAll[p+1]->GetXaxis()->GetTitle(), histsAll[p+1]->GetYaxis()->GetTitle(), 0.85*textSizePixel, textSizePixel, 0.85*textSizePixel, textSizePixel,0.9, 1.1, 510, 510, 43, 63);  
      SetMarkerDefaults(histsAll[p+1], 20, 1, kBlue+1, kBlue+1, kFALSE);
      histsAll[p+1]->GetXaxis()->SetRangeUser(xPMin,xPMax);
      histsAll[p+1]->GetYaxis()->SetRangeUser(0.7,scaleYMax*maxY);
      histsAll[p+1]->Draw("pe");
      
      if (hTriggN){
        if (hTriggN[p+1]){
            SetLineDefaults( hTriggN[p+1],kBlack, 2, 1);
            hTriggN[p+1]->Draw("hist,same");
        }
      }
      if (hTriggC){
        if (hTriggC[p+1]){
          SetLineDefaults( hTriggC[p+1],kRed+1, 3, 1);
          hTriggC[p+1]->Draw("hist,same");
        }
      }

      if (fits){
        if (fits[p+1]){
          SetStyleFit(fits[p+1] , 0, 120, 7, 7, kGray+1);
          fits[p+1]->Draw("same");
        }
      } 
      
      if (p == 4){
        TLegend* legend = GetAndSetLegend2( topRCornerX[p]-8*relSize8P[p], topRCornerY[p]-5*0.85*relSize8P[p]-0.4*relSize8P[p], topRCornerX[p]-0.04, topRCornerY[p]-1.6*relSize8P[p],0.85*textSizePixel, 1, "", 43,0.2);
        legend->AddEntry(histsAll[p+1], "All triggers", "p");
        if (hTriggC[p+1])legend->AddEntry(hTriggC[p+1], "Straight line trigg", "l");
        if (hTriggN[p+1])legend->AddEntry(hTriggN[p+1], "noise trigg", "l");
        if (fits){
          if (fits[p+1])legend->AddEntry(fits[p+1], "noise fit", "l");
        }
        legend->Draw();
      } else if (p == 3){
        DrawLatex(topRCornerX[p]-0.04, topRCornerY[p]-2.2*relSize8P[p], Form("%s-beam, #it{E}_{#it{b}}= %.0f GeV", currRunInfo.species.Data(), currRunInfo.energy), true, 0.85*relSize8P[p], 42);
        DrawLatex(topRCornerX[p]-0.04, topRCornerY[p]-3.*relSize8P[p], Form("Run %d, #it{V}_{#it{op}} = %1.1f V", currRunInfo.runNr, currRunInfo.vop  ), true, 0.85*relSize8P[p], 42); 
      }
      histsAll[p+1]->Draw("axis,same");
      
      TString label = Form("RB ch. %d", p+1);
      if (p == 4) label = Form("layer %d, RB ch. %d", layer, p+1);
      TLatex *labelChannel    = new TLatex(topRCornerX[p]-0.04,topRCornerY[p]-1.2*relSize8P[p],label);
      SetStyleTLatex( labelChannel, 0.85*textSizePixel,4,1,43,kTRUE,31);
      labelChannel->Draw();

    }
    canvas8Panel->SaveAs(nameOutput.Data());
  }

  
 //__________________________________________________________________________________________________________
  void PlotStraigtLineTriggAndFitFullLayer (TCanvas* canvas8Panel, TPad* pads[8], Double_t* topRCornerX,  Double_t* topRCornerY, Double_t* relSize8P, Int_t textSizePixel, 
                                            TH1D** hTriggC, TF1** fits, Double_t maxVals[9],
                                            Double_t xAMin, Double_t xAMax, Double_t xPMin, Double_t xPMax, Double_t scaleYMax, Int_t layer, TString nameOutput, runInfo currRunInfo){
                                  
    Double_t maxY = 0;
    for (Int_t p = 0; p < 8; p++){
      if (maxY < FindLargestBin1DHist(hTriggC[p+1], xAMin , xAMax)) maxY = FindLargestBin1DHist(hTriggC[p+1], xAMin , xAMax);
    }
    
    for (Int_t p = 0; p < 8; p++){
      canvas8Panel->cd();
      pads[p]->Draw();
      pads[p]->cd();
      pads[p]->SetLogy();
      SetStyleHistoTH1ForGraphs( hTriggC[p+1], hTriggC[p+1]->GetXaxis()->GetTitle(), hTriggC[p+1]->GetYaxis()->GetTitle(), 0.85*textSizePixel, textSizePixel, 0.85*textSizePixel, textSizePixel,0.9, 1.1, 505, 510, 43, 63);  
      SetMarkerDefaults(hTriggC[p+1], 20, 1, kBlue+1, kBlue+1, kFALSE);
      hTriggC[p+1]->GetXaxis()->SetRangeUser(xPMin,xPMax);
      hTriggC[p+1]->GetYaxis()->SetRangeUser(0.2,scaleYMax*maxY);
      hTriggC[p+1]->Draw("hist");
      
      if (fits){
        if (fits[p+1]){
          TBox* fitErrBox = CreateBox(kRed-8, fits[p+1]->GetParameter(1)-fits[p+1]->GetParError(1), 0.2, fits[p+1]->GetParameter(1)+fits[p+1]->GetParError(1), 0.7*scaleYMax*2, 1001 ) ;
          fitErrBox->Draw();
          hTriggC[p+1]->Draw("hist,same");
          DrawLines(fits[p+1]->GetParameter(1), fits[p+1]->GetParameter(1), 0.2, 0.7*scaleYMax*2, 3, kRed+2, 4,1);
          DrawLines(maxVals[p+1], maxVals[p+1], 0.2, 0.7*scaleYMax*4, 3, kGray+1, 4,1);
          SetStyleFit(fits[p+1] , xPMin, xPMax, 7, 7, kBlack);
          fits[p+1]->Draw("same");
        }
        
      } 
      
      if (p == 4){
        TLegend* legend = GetAndSetLegend2( topRCornerX[p]-9*relSize8P[p], topRCornerY[p]-5*0.85*relSize8P[p]-0.4*relSize8P[p], topRCornerX[p]-0.04, topRCornerY[p]-1.6*relSize8P[p],0.85*textSizePixel, 1, "", 43,0.2);
        legend->AddEntry(hTriggC[p+1], "Straight line trigg", "p");
        if (fits){
          if (fits[p+1]){
            legend->AddEntry(fits[p+1], "Landau-Gauss fit", "l");
            TLine* dummyL = new TLine();
            dummyL->SetLineWidth(3);
            dummyL->SetLineColor(kRed+2);
            dummyL->SetLineStyle(4);
            TLine* dummyL2 = new TLine();
            dummyL2->SetLineWidth(3);
            dummyL2->SetLineColor(kGray+1);
            dummyL2->SetLineStyle(4);
            legend->AddEntry(dummyL, "MP Landau", "l");
            legend->AddEntry(dummyL2, "Max Landau-Gauss", "l");

          }
        }
        legend->Draw();
      } else if (p == 3){
        DrawLatex(topRCornerX[p]-0.04, topRCornerY[p]-2.2*relSize8P[p], Form("%s-beam, #it{E}_{#it{b}}= %.0f GeV", currRunInfo.species.Data(), currRunInfo.energy), true, 0.85*relSize8P[p], 42);
        DrawLatex(topRCornerX[p]-0.04, topRCornerY[p]-3.*relSize8P[p], Form("Run %d, #it{V}_{#it{op}} = %1.1f V", currRunInfo.runNr, currRunInfo.vop  ), true, 0.85*relSize8P[p], 42); 
      }
      hTriggC[p+1]->Draw("axis,same");
      
      TString label = Form("RB ch. %d", p+1);
      if (p == 4) label = Form("layer %d, RB ch. %d", layer, p+1);
      TLatex *labelChannel    = new TLatex(topRCornerX[p]-0.04,topRCornerY[p]-1.2*relSize8P[p],label);
      SetStyleTLatex( labelChannel, 0.85*textSizePixel,4,1,43,kTRUE,31);
      labelChannel->Draw();

    }
    canvas8Panel->SaveAs(nameOutput.Data());
  }  

 //__________________________________________________________________________________________________________
  void PlotStraigtLineTriggAndFitFullLayerLin (TCanvas* canvas8Panel, TPad* pads[8], Double_t* topRCornerX,  Double_t* topRCornerY, Double_t* relSize8P, Int_t textSizePixel, 
                                            TH1D** hTriggC, TF1** fits, Double_t maxVals[9],
                                            Double_t xAMin, Double_t xAMax, Double_t xPMin, Double_t xPMax, Double_t scaleYMax, Int_t layer, TString nameOutput, runInfo currRunInfo){
                                  
    Double_t maxY = 0;
    for (Int_t p = 0; p < 8; p++){
      if (maxY < FindLargestBin1DHist(hTriggC[p+1], xAMin , xAMax)) maxY = FindLargestBin1DHist(hTriggC[p+1], xAMin , xAMax);
    }
    
    for (Int_t p = 0; p < 8; p++){
      canvas8Panel->cd();
      pads[p]->Draw();
      pads[p]->cd();
      pads[p]->SetLogy(0);
      SetStyleHistoTH1ForGraphs( hTriggC[p+1], hTriggC[p+1]->GetXaxis()->GetTitle(), hTriggC[p+1]->GetYaxis()->GetTitle(), 0.85*textSizePixel, textSizePixel, 0.85*textSizePixel, textSizePixel,0.9, 1.1, 505, 510, 43, 63);  
      SetMarkerDefaults(hTriggC[p+1], 20, 1, kBlue+1, kBlue+1, kFALSE);
      hTriggC[p+1]->GetXaxis()->SetRangeUser(xPMin,xPMax);
      hTriggC[p+1]->GetYaxis()->SetRangeUser(0.,scaleYMax*maxY);
      hTriggC[p+1]->Draw("hist");
      
      if (fits){
        if (fits[p+1]){
          TBox* fitErrBox = CreateBox(kRed-8, fits[p+1]->GetParameter(1)-fits[p+1]->GetParError(1), 0., fits[p+1]->GetParameter(1)+fits[p+1]->GetParError(1), maxY/10., 1001 ) ;
          fitErrBox->Draw();
          hTriggC[p+1]->Draw("hist,same");
          DrawLines(fits[p+1]->GetParameter(1), fits[p+1]->GetParameter(1), 0., maxY/10., 3, kRed+2, 4,1);
          DrawLines(maxVals[p+1], maxVals[p+1], 0., maxY/10., 3, kGray+1, 4,1);
          SetStyleFit(fits[p+1] , xPMin, xPMax, 7, 7, kBlack);
          fits[p+1]->Draw("same");
        }
        
      } 
      
      if (p == 4){
        TLegend* legend = GetAndSetLegend2( topRCornerX[p]-9*relSize8P[p], topRCornerY[p]-5*0.85*relSize8P[p]-0.4*relSize8P[p], topRCornerX[p]-0.04, topRCornerY[p]-1.6*relSize8P[p],0.85*textSizePixel, 1, "", 43,0.2);
        legend->AddEntry(hTriggC[p+1], "Straight line trigg", "p");
        if (fits){
          if (fits[p+1]){
            legend->AddEntry(fits[p+1], "Landau-Gauss fit", "l");
            TLine* dummyL = new TLine();
            dummyL->SetLineWidth(3);
            dummyL->SetLineColor(kRed+2);
            dummyL->SetLineStyle(4);
            TLine* dummyL2 = new TLine();
            dummyL2->SetLineWidth(3);
            dummyL2->SetLineColor(kGray+1);
            dummyL2->SetLineStyle(4);
          legend->AddEntry(dummyL, "MP Landau", "l");
          legend->AddEntry(dummyL2, "Max Landau-Gauss", "l");
          }
        }
        legend->Draw();
      } else if (p == 3){
        DrawLatex(topRCornerX[p]-0.04, topRCornerY[p]-2.2*relSize8P[p], Form("%s-beam, #it{E}_{#it{b}}= %.0f GeV", currRunInfo.species.Data(), currRunInfo.energy), true, 0.85*relSize8P[p], 42);
        DrawLatex(topRCornerX[p]-0.04, topRCornerY[p]-3.*relSize8P[p], Form("Run %d, #it{V}_{#it{op}} = %1.1f V", currRunInfo.runNr, currRunInfo.vop  ), true, 0.85*relSize8P[p], 42); 
      }
      hTriggC[p+1]->Draw("axis,same");
      
      TString label = Form("RB ch. %d", p+1);
      if (p == 4) label = Form("layer %d, RB ch. %d", layer, p+1);
      TLatex *labelChannel    = new TLatex(topRCornerX[p]-0.04,topRCornerY[p]-1.2*relSize8P[p],label);
      SetStyleTLatex( labelChannel, 0.85*textSizePixel,4,1,43,kTRUE,31);
      labelChannel->Draw();

    }
    canvas8Panel->SaveAs(nameOutput.Data());
  }  
  
  //__________________________________________________________________________________________________________
  // Plot Noise with Fits for Full layer
  //__________________________________________________________________________________________________________
  void PlotOverlayFullLayer ( TCanvas* canvas8Panel, TPad* pads[8], Double_t* topRCornerX,  Double_t* topRCornerY, Double_t* relSize8P, Int_t textSizePixel, 
                              TH1D* hists[gMaxLayers][9], Bool_t* bLayer, Int_t minL, Int_t maxL,
                              Double_t xRMin, Double_t xRMax, Double_t xPMin, Double_t xPMax, Double_t scaleYMax, TString nameOutput, runInfo currRunInfo, TString plotStyle = "pe"){
                                  
    Double_t maxY = 0;
    Double_t minY = 1;
    for (Int_t l = 0; l < maxL; l++){
      if (!bLayer[l]) continue;
      for (Int_t p = 0; p < 8; p++){
        if (maxY < FindLargestBin1DHist(hists[l][p+1], xRMin , xRMax)) maxY = FindLargestBin1DHist(hists[l][p+1],xRMin, xRMax);
        if (minY > FindSmallestBin1DHist(hists[l][p+1])) minY = FindSmallestBin1DHist(hists[l][p+1]);
        
      }
    }
    
    TLegend* legend;
    for (Int_t p = 0; p < 8; p++){
      canvas8Panel->cd();
      pads[p]->Draw();
      pads[p]->cd();
      pads[p]->SetLogy();
      
      if (p == 4) legend = GetAndSetLegend2( 0.04, topRCornerY[p]-3*0.85*relSize8P[p]-0.6*relSize8P[p], 0.5, topRCornerY[p]-0.6*relSize8P[p],0.85*textSizePixel, 4, "layer", 43,0.3);

      for (Int_t l = minL; l < maxL; l++){
        if (!bLayer[l]) continue;
        SetStyleHistoTH1ForGraphs( hists[l][p+1], hists[l][p+1]->GetXaxis()->GetTitle(), hists[l][p+1]->GetYaxis()->GetTitle(), 0.85*textSizePixel, textSizePixel, 0.85*textSizePixel, textSizePixel,0.9, 1.1, 505, 510, 43, 63);  
        SetMarkerDefaults(hists[l][p+1], markerLayer[l], 0.8, colorLayer[l],colorLayer[l], kFALSE);
        if (plotStyle.Contains("hist")) hists[l][p+1]->SetLineWidth(2);
        hists[l][p+1]->GetXaxis()->SetRangeUser(xPMin,xPMax);
        hists[l][p+1]->GetYaxis()->SetRangeUser(0.7*minY,scaleYMax*maxY);
        if(l==minL)hists[l][p+1]->Draw(plotStyle.Data());
        else hists[l][p+1]->Draw(Form("%s,same",plotStyle.Data()));
        hists[minL][p+1]->Draw("same,axis");
        if (p == 4){
          if (plotStyle.Contains("p")) legend->AddEntry(hists[l][p+1], Form("%d",l), "p");
          else legend->AddEntry(hists[l][p+1], Form("%d",l), "l");
        }
      }
      if (p == 4) legend->Draw();
      else if (p == 3){
        DrawLatex(topRCornerX[p]-0.04, topRCornerY[p]-2.2*relSize8P[p], Form("%s-beam, #it{E}_{#it{b}}= %.0f GeV", currRunInfo.species.Data(), currRunInfo.energy), true, 0.85*relSize8P[p], 42);
        DrawLatex(topRCornerX[p]-0.04, topRCornerY[p]-3.*relSize8P[p], Form("Run %d, #it{V}_{#it{op}} = %1.1f V", currRunInfo.runNr, currRunInfo.vop  ), true, 0.85*relSize8P[p], 42); 
      }
      TString label = Form("RB ch. %d", p+1);
      TLatex *labelChannel    = new TLatex(topRCornerX[p]-0.04,topRCornerY[p]-1.2*relSize8P[p],label);
      SetStyleTLatex( labelChannel, 0.85*textSizePixel,4,1,43,kTRUE,31);
      labelChannel->Draw();
    }
    canvas8Panel->SaveAs(nameOutput.Data());
  }

  //__________________________________________________________________________________________________________
  // Plot 2D fit variables overview
  //__________________________________________________________________________________________________________  
  void PlotSimpleMultiLayer2D( TCanvas* canvas2D, TH2* hist, Int_t maxl, Float_t textSizeRel, TString nameOutput, runInfo currRunInfo, Bool_t hasNeg = kFALSE ){
      canvas2D->cd();
      SetStyleHistoTH2ForGraphs( hist, hist->GetXaxis()->GetTitle(), hist->GetYaxis()->GetTitle(), 0.85*textSizeRel, textSizeRel, 0.85*textSizeRel, textSizeRel,0.9, 0.9);  
      hist->GetYaxis()->SetRangeUser(-0.5,maxl+1.5);
      if (!hasNeg)
        hist->GetZaxis()->SetRangeUser(hist->GetMinimum(0),hist->GetMaximum());
      else 
        hist->GetZaxis()->SetRangeUser(hist->GetMinimum(),hist->GetMaximum());
      hist->Draw("colz,text");
      
      DrawLatex(0.85, 0.92, Form("%s-beam, #it{E}_{#it{b}}= %.0f GeV, Run %d, #it{V}_{#it{op}} = %1.1f V", currRunInfo.species.Data(), currRunInfo.energy, currRunInfo.runNr, currRunInfo.vop  ), true, textSizeRel, 42);

    canvas2D->SaveAs(nameOutput.Data());
  }
  
#endif
