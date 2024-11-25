/***********************************************************************************************
*** provided by,                                                                          ******
***     Friederike Bock, fbock@cern.ch                                                    ******
************************************************************************************************/

#ifndef COMMONVARIABLES
#define COMMONVARIABLES
  
  // Definition of color/marker arrays for plotting
  Color_t colorLayer[16]    = { kRed-7, kBlue+1, kCyan-3, kMagenta+1, kPink-1, kViolet-9, kOrange, kGreen+1,  
                                kRed+1, kBlue-9, kCyan+1, kMagenta-7, kOrange+7, kSpring+5, kPink+5, kViolet+8 };
  Color_t colorReadBoard[8] = { kRed+1, kBlue+1, kCyan+1, kMagenta+1, kOrange, kGreen+1, kPink+5, kViolet-9};
  Style_t markerLayer[16]     = { 20, 25, 33, 28, 29, 37, 41, 46,
                                  24, 21, 27, 34, 30, 39, 40, 47};
  Style_t markerReadBoard[8]  = { 20, 21, 33, 34, 29, 39, 40, 46};
  
  Color_t colorBeamE[16]    = { kPink-1, kRed-7, kCyan-3, kMagenta+1, kBlue+1,  kViolet-9, kOrange, kGreen+1,  
                                kRed+1, kGreen+1, kCyan+1, kMagenta-7, kOrange+7, kSpring+5, kPink+5, kViolet+8 };
  Color_t colorVoltage[16]  = { kPink-1, kRed-7, kCyan-3, kMagenta+1, kBlue+1,  kViolet-9, kOrange, kGreen+1,  
                                kRed+1, kGreen+1, kCyan+1, kMagenta-7, kOrange+7, kSpring+5, kPink+5, kViolet+8 };
  
  
  // SPS - H4 2023 variables
  std::vector<Float_t> beamE_SPS2023    = {20, 40, 60, 80, 100, 150, 200, 220, 250, 300, 350};
  std::vector<Int_t> gainScan_SPS2023   = {10, 20, 30, 40, 50, 60, 63};
                                  // e,   h
  Int_t species_SPS2023[2]      = {0,     1};
  std::vector<Float_t> vov_SPS2023      = {0.5, 1.5, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5};
  
  //*******************************************************************
  // Find index in array for voltage
  //*******************************************************************
  Int_t GetOverVoltageIndex(Double_t vov, Int_t tbdata){
    // TB data 2023 SPS H4
    if (tbdata == 1){ 
      Int_t n = 0; 
      while (n < (Int_t)vov_SPS2023.size() && vov != vov_SPS2023.at(n) ){
        n++;
      }
      if (n ==  (Int_t)vov_SPS2023.size()) return -1;
      else return n;
    } 
    return -1;
  }

  //*******************************************************************
  // Get over voltage from index
  //*******************************************************************
  Float_t GetOverVoltageFromIndex(Int_t index, Int_t tbdata){
    // TB data 2023 SPS H4
    if (tbdata == 1){ 
      if (index < 0 || index > (Int_t)vov_SPS2023.size()-1) return -1.;
      else return vov_SPS2023.at(index);
    } 
    return -1.;
  }
  
  
  //*******************************************************************
  // Find index in array for beam energy
  //*******************************************************************
  Int_t GetBeamEnergyIndex(Double_t energy, Int_t tbdata){
    // TB data 2023 SPS H4
    if (tbdata == 1){ 
      Int_t n = 0; 
      while ( n < (Int_t)beamE_SPS2023.size() && energy != beamE_SPS2023.at(n) ) n++;
      if (n ==  (Int_t)beamE_SPS2023.size()) return -1;
      else return n;
    } 
    return -1;
  }
  //*******************************************************************
  // Get beam energy from index
  //*******************************************************************
  Float_t GetBeamEnergyFromIndex(Int_t index, Int_t tbdata){
    // TB data 2023 SPS H4
    if (tbdata == 1){ 
      if (index < 0 || index > (Int_t)beamE_SPS2023.size()-1) return -1.;
      else return beamE_SPS2023.at(index);
    } 
    return -1.;
  }
  
  //*******************************************************************
  // Find index in array for gain setting
  //*******************************************************************
  Int_t GetGainSetIndex(Int_t hgSet, Int_t lgSet, Int_t tbdata){
    // TB data 2023 SPS H4
    if (tbdata == 1){ 
      Int_t n = 0; 
      if (hgSet != lgSet) return -1;
      while (n < (Int_t)gainScan_SPS2023.size() && hgSet != gainScan_SPS2023.at(n) ) n++;
      if (n ==  (Int_t)gainScan_SPS2023.size()) return -1;
      else return n;
    } 
    return -1;
  }
  
  //*******************************************************************
  // Get low Gain setting from index
  //*******************************************************************
  Int_t GetLGGainSetIndex(Int_t index, Int_t tbdata){
    // TB data 2023 SPS H4
    if (tbdata == 1){ 
      if (index < 0 || index > (Int_t)gainScan_SPS2023.size()-1) return -1;
      else return gainScan_SPS2023.at(index);
    } 
    return -1;
  }
  //*******************************************************************
  // Get high Gain setting from index
  //*******************************************************************
  Int_t GetHGGainSetIndex(Int_t index, Int_t tbdata){
    // TB data 2023 SPS H4
    if (tbdata == 1){ 
      if (index < 0 || index > (Int_t)gainScan_SPS2023.size()-1) return -1;
      else return gainScan_SPS2023.at(index);
    } 
    return -1;
  }
  
  
#endif
