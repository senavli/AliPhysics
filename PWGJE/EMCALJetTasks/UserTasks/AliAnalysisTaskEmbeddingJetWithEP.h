#ifndef ALIANALYSISTASKEMBEDDINGJETWITHEP_H
#define ALIANALYSISTASKEMBEDDINGJETWITHEP_H
// ******************************************************************************************
// \class AliAnalysisTaskEmbeddingJetWithEP
// \brief task used to load the Qn calibrations and get the calibrated Qn vectors for JE analyses
// \authors:
// T. Kumaoka, takuya.kumaoka@cern.ch
// ******************************************************************************************

#include <TH1F.h>
#include <TList.h>
#include <TString.h>
#include <TMath.h>

#include "AliAnalysisTaskSE.h"
#include "AliVEvent.h"
#include "AliAODEvent.h"

#include "AliEventCuts.h"

#include "AliAnalysisTaskEmcalJet.h"

#include "THistManager.h"
#include "AliYAMLConfiguration.h"
#include "AliEmcalJet.h"

#include "AliEmcalEmbeddingQA.h"


#include "AliOADBContainer.h"


class TClonesArray;
class TF1;

class AliRhoParameter;
class AliLocalRhoParameter;

class AliAnalysisTaskEmbeddingJetWithEP : public AliAnalysisTaskEmcalJet {
// class AliAnalysisTaskEmbeddingJetWithEP : public AliAnalysisTaskSE {

public:

  // enum Det{kFullTPC,kPosTPC,kNegTPC,kFullV0,kV0A,kV0C};

  AliAnalysisTaskEmbeddingJetWithEP();
  AliAnalysisTaskEmbeddingJetWithEP(const char *name);
  virtual ~AliAnalysisTaskEmbeddingJetWithEP();

  virtual void UserCreateOutputObjects();
  // virtual void UserExec(Option_t *option);

  void Terminate(Option_t *option);

  static AliAnalysisTaskEmbeddingJetWithEP* AddTaskEmbeddingJetWithEP(
      const char *ntracks            = "usedefault",
      const char *nclusters          = "usedefault",
      const char* ncells             = "usedefault",
      const char *suffix             = "");

  enum fitModulationType  { 
      kNoFit,           ///< background fit type plane
      kV2,              ///< background fit type v2
      kV3,              ///< background fit type v3
      kCombined,        ///< background fit type v2 and v3
      kFourierSeries,   ///< background fit type FourierSeries
      kIntegratedFlow,  ///< background fit type integrated flow
      kQC2,             ///< background fit type qumrant2
      kQC4              ///< background fit type qumrant4
    };

  void SetTriggerInfo(TString trigClass, unsigned long long mask=0) {fTriggerClass = trigClass; fTriggerMask = mask;}
  void SetRejectTPCPileup(bool val)                                  {fRejectTPCPileup = val;}


  //== s == Setter Prepare  ################################################
  void SetMessageLevel(Int_t mesLev) {fMesLev = mesLev;}
  void SetRunListFileName(std::string fileName) {fRunListFileName = fileName;}
  
  void SetUseAliEventCuts(Bool_t b)      { fUseAliEventCuts = b; }
  void SetUseManualEvtCuts(Bool_t input) { fUseManualEventCuts = input;}

  void SetQnOadbFile(TString oadbFileName){fOADBFileName = oadbFileName;}
  void SetCalibRefFile(TString calibRefFileName){fCalibRefFileName = calibRefFileName;}
  
  void SetPileupCut(Bool_t bPileupCut){fPileupCut =  bPileupCut;} 

  // void SetPileupCutQA(Bool_t bPileupCutQA){fPileupCutQA =  bPileupCutQA;}
  void SetEventQA(Int_t qaEventNum){fQaEventNum = qaEventNum;}
  void SetV0KindForBKG(Int_t iV0KindForBkg){fV0KindForBkg = iV0KindForBkg;}
  void SetV0Combine(Bool_t bV0Combin){fV0Combin = bV0Combin;}
  void SetQnCalibType(TString iQnVCalibType){fQnVCalibType = iQnVCalibType;}
  void SetTPCQnMeasure(Bool_t bTPCQnMeasure){fTPCQnMeasure = bTPCQnMeasure;}

  void SetOwnEventCut(Bool_t bOwnEventCut){fOwnEventCut = bOwnEventCut;}
  void SetDoEP(Bool_t bDoEP){fDoEP = bDoEP;}
  void SetDoTrack(Bool_t bDoTrack){fDoTrack = bDoTrack;}
  void SetDoBkg(Bool_t bDoBkg){fDoBkg = bDoBkg;}
  void SetDoJet(Bool_t bDoJet){fDoJet = bDoJet;}
  void SetTrackHistQA(Bool_t bTrackQA){fTrackQA = bTrackQA;}
  void SetBkgHistQA(Bool_t bBkgQA){fBkgQA = bBkgQA;}
  void SetJetHistQA(Bool_t bJetQA){fJetQA = bJetQA;}
  void Set2DRMwithEP2(Bool_t b2DRMwithEP2){f2DRMwithEP2 = b2DRMwithEP2;}
  void Set2DRMwithEP3(Bool_t b2DRMwithEP3){f2DRMwithEP3 = b2DRMwithEP3;}
  

  void SetExLJetFromFit(Bool_t bExLJetFromFit){fExLJetFromFit = bExLJetFromFit;}
  void SetExSubLJetFromFit(Bool_t bExSubLJetFromFit){fExSubLJetFromFit = bExSubLJetFromFit;}

  void SetJetHistWEP(Bool_t bSepEP){fSepEP = bSepEP;}
  void SetModulationFitType(fitModulationType type) {fFitModulationType = type; }
  void SetRhoLocalSubType(Bool_t bRhoLocalSubType){fRhoLocalSubType = bRhoLocalSubType;}

  // == s == Setter Embedding Parameters ==================
  void SetDoJetMatchingGeometrical(Bool_t b)                { fDoJetMatchingGeometrical = b; }
  void SetDoDifferentialRM(Bool_t b)                        { fDoDifferentialRM = b; }
  void SetDoJetMatchingMCFraction(Bool_t b)                 { fDoJetMatchingMCFraction = b; }
  void SetRequireMatchedJetAccepted(Bool_t b)               { fRequireMatchedJetAccepted = b; }
  void SetJetMatchingR(Double_t r)                          { fJetMatchingR = r; }
  void SetMinimumSharedMomentumFraction(Double_t d)         { fMinSharedMomentumFraction = d; }
  void SetMCJetMinMatchingPt(Double_t min)                  { fMCJetMinMatchingPt = min; }
  void SetDetJetMinMatchingPt(Double_t min)                 { fDetJetMinMatchingPt = min; }
  void SetPlotJetMatchCandThresh(Double_t r)                { fPlotJetMatchCandThresh = r; }
  // void SetFillChargedFluctuations(Bool_t b)                 { fFillChargedFluctuations = b; }
  // == e == Setter Embedding Parameters ==================

  // == s == Setter Qn vector Calibration histograms ==================
  void SetCalibOrigRefObjList(TList *hWgtsV0ZDC){this->fCalibRefObjList = (TList *) hWgtsV0ZDC->Clone();}

  void SetLRefMultV0BefCorPfpx(AliOADBContainer *hList){this->fMultV0BefCorPfpx = (AliOADBContainer *) hList->Clone();}
  void SetLRefQx2am(TObjArray *hList){this->fOADBzArray_contQx2am = (TObjArray *) hList->Clone();}
  void SetLRefQy2am(TObjArray *hList){this->fOADBzArray_contQy2am = (TObjArray *) hList->Clone();}
  void SetLRefQx2as(TObjArray *hList){this->fOADBzArray_contQx2as = (TObjArray *) hList->Clone();}
  void SetLRefQy2as(TObjArray *hList){this->fOADBzArray_contQy2as = (TObjArray *) hList->Clone();}
  void SetLRefQx3am(TObjArray *hList){this->fOADBzArray_contQx3am = (TObjArray *) hList->Clone();}
  void SetLRefQy3am(TObjArray *hList){this->fOADBzArray_contQy3am = (TObjArray *) hList->Clone();}
  void SetLRefQx3as(TObjArray *hList){this->fOADBzArray_contQx3as = (TObjArray *) hList->Clone();}
  void SetLRefQy3as(TObjArray *hList){this->fOADBzArray_contQy3as = (TObjArray *) hList->Clone();}
  void SetLRefQx2cm(TObjArray *hList){this->fOADBzArray_contQx2cm = (TObjArray *) hList->Clone();}
  void SetLRefQy2cm(TObjArray *hList){this->fOADBzArray_contQy2cm = (TObjArray *) hList->Clone();}
  void SetLRefQx2cs(TObjArray *hList){this->fOADBzArray_contQx2cs = (TObjArray *) hList->Clone();}
  void SetLRefQy2cs(TObjArray *hList){this->fOADBzArray_contQy2cs = (TObjArray *) hList->Clone();}
  void SetLRefQx3cm(TObjArray *hList){this->fOADBzArray_contQx3cm = (TObjArray *) hList->Clone();}
  void SetLRefQy3cm(TObjArray *hList){this->fOADBzArray_contQy3cm = (TObjArray *) hList->Clone();} 
  void SetLRefQx3cs(TObjArray *hList){this->fOADBzArray_contQx3cs = (TObjArray *) hList->Clone();} 
  void SetLRefQy3cs(TObjArray *hList){this->fOADBzArray_contQy3cs = (TObjArray *) hList->Clone();}
  void SetLRefTPCposEta(TObjArray *hList){this->fOADBcentArray_contTPCposEta = (TObjArray *) hList->Clone();}
  void SetLRefTPCnegEta(TObjArray *hList){this->fOADBcentArray_contTPCnegEta = (TObjArray *) hList->Clone();}
  // == e == Setter Qn vector Calibration histograms ==================
  void CreateQnVectorHandlers(); // Create the QnVector handlers, including loading the calibration files
  //== e == Setter Prepare  ################################################


  // ========================================================================================
    bool ExtractRecentPara(TFile *RefFile, TObjArray *lRefQx2am, TObjArray *lRefQy2am, TObjArray *lRefQx2as, TObjArray *lRefQy2as, TObjArray *lRefQx3am, TObjArray *lRefQy3am, TObjArray *lRefQx3as, TObjArray *lRefQy3as, TObjArray *lRefQx2cm, TObjArray *lRefQy2cm, TObjArray *lRefQx2cs, TObjArray *lRefQy2cs, TObjArray *lRefQx3cm,TObjArray *lRefQy3cm, TObjArray *lRefQx3cs, TObjArray *lRefQy3cs, TObjArray *lRefTPCposEta, TObjArray *lRefTPCnegEta);

    bool SetAODEvent(AliAODEvent* event); 
    void ResetAODEvent(); 
    
    void SetTPCEtaLimits(double absetamin=0., double absetamx=0.8) {fEtaMinTPC=absetamin; fEtaMaxTPC=absetamx;}
    void SetTPCPtLimits(double ptmin=0.2, double ptmax=5) {fPtMinTPC=ptmin; fPtMaxTPC=ptmax;}
    void SetFractionOfTPCtracksToUse(double fracToKeep) {fFractionOfTracksForQnTPC = fracToKeep;}
    void SetCalibrationsOADBFileName(TString OADBfileName) {fOADBFileName = OADBfileName;}
    
    TString GetCalibrationsOADBFileName() const {return fOADBFileName;}
    
    void Getqn(Double_t Qn[3], Double_t QnNorm[3], Double_t Multi[3]);


    void EnablePhiDistrHistos();
    TH2F* GetPhiDistrHistosTPCPosEta() const {return fPhiVsCentrTPC[0];}
    TH2F* GetPhiDistrHistosTPCNegEta() const {return fPhiVsCentrTPC[1];}
    // ========================================================================================

protected:
  AliEventCuts fEventCuts;       //!<! Event selection  
  // TList        *fEventCutList; ///< Output list for event cut histograms
  Bool_t       fUseAliEventCuts; ///< Flag to use AliEventCuts (otherwise AliAnalysisTaskEmcal will be used)
  Bool_t       fUseManualEventCuts; ///< Flag to use manual event cuts

  void              ExecOnce();
  Bool_t            Run();
  
  // virtual Bool_t    IsEventSelected();
  THistManager fHistManager;        ///< Histogram manager

private:

    double GetEventPlaneAngle(double Qx, double Qy) const {return (TMath::Pi()+TMath::ATan2(-Qy,-Qx))/2;}

    TList *fOutputList;               //!<! output list for histograms
    TH1F *fHistNEvents;               //!<! histo with number of events
    
    AliAODEvent* fAOD;                /// AOD event
    TString fOADBFileName;            /// OADB input file name
    
    Int_t   fMesLev = 0;              ///<

    Bool_t  fPileupCut = kFALSE;      ///<
    Bool_t  fTPCQnMeasure = kFALSE;   ///<
    
    Bool_t  fPileupCutQA = kFALSE;    ///<

    Bool_t  fOwnEventCut = kFALSE;    ///<
    Bool_t  fDoEP = kFALSE;           ///<
    Bool_t  fDoTrack = kFALSE;        ///<
    Bool_t  fDoBkg = kFALSE;          ///<
    Bool_t  fDoJet = kFALSE;          ///<
    
    Bool_t  fTrackQA = kFALSE;        ///<
    Bool_t  fBkgQA = kFALSE;          ///<
    Bool_t  fJetQA = kFALSE;          ///<
    Bool_t  f2DRMwithEP2 = kFALSE;    ///<
    Bool_t  f2DRMwithEP3 = kFALSE;    ///<

    Bool_t  fSepEP = kFALSE;          ///<
    
    fitModulationType  fFitModulationType;  ///< fit modulation type
    TString       fQnVCalibType = "kOrig";  ///< fCalibration Type
    Int_t         fV0KindForBkg = 0;        ///< 0:V0M, 1:V0C, 2:V0A
    Bool_t        fV0Combin = kFALSE;       ///<
    int           fQaEventNum = -1;         ///<
    Bool_t        fRhoLocalSubType;         ///<
    
    std::string   fRunListFileName;         ///< Run list file Name
    TString       fCalibRefFileName;        ///< Calibration input file name

    // == s ==  Embedding Parameters  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Bool_t             fDoDetLevelMatching = kFALSE;    ///< Whether or not we do det level 
    Bool_t             fDoPartLevelMatching = kFALSE;   ///< Whether or not we do particle level matching
    Bool_t             fDoJetMatchingGeometrical;       ///< Do geometrical matching between det-level and truth-level jet container
    Bool_t             fDoJetMatchingMCFraction;        ///< Do MC-fraction based matching using PbPb det-level, pp det-level, and pp truth-level jet containers
    Bool_t             fDoDifferentialRM;               ///< This allows a differential RM depenent on the angularity of the particle lvl jet
    AliEmcalEmbeddingQA  fEmbeddingQA;          //!<! QA hists for embedding (will only be added if embedding)
    AliJetContainer*     fMCJetContainer;       //!<!Pointer to jet container of truth-level jets
    AliJetContainer*     fDetJetContainer;      //!<!Pointer to jet container of det-level jets
    AliJetContainer*     fDetJetContainerPPIntermediate; //!<!Pointer to jet container of intermediate pp det-level jets, if MC-fraction matching
    Bool_t               fRequireMatchedJetAccepted;     ///< Flag to require matched truth jet to be accepted (aside from geometrical acceptance)
    Double_t             fJetMatchingR;                  ///< Jet matching R threshold
    Double_t             fMinSharedMomentumFraction;     ///< Minimum shared momentum (pp det-level track pT in combined jet) / (pp det-level track pT)
    Double_t             fMCJetMinMatchingPt;            ///< Min jet pT for MC jets being matched, for when container criteria is not applied
    Double_t             fDetJetMinMatchingPt;           ///< Min jet pT for Det jets being matched, for when container criteria is not applied
    Double_t             fPlotJetMatchCandThresh;        ///< Threshold for jet R to count candidates, affects plotting only
    // == e ==  Embedding Parameters  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Bool_t        fExLJetFromFit = kTRUE;   ///< exclude tracks from bkg fit.
    Bool_t        fExSubLJetFromFit = kTRUE;   ///< exclude tracks from bkg fit.
    AliEmcalJet*  fLeadingJet;              //! leading jet
    AliEmcalJet*  fSubLeadingJet;           //! subleading jet
    AliEmcalJet*  fLeadingJetAfterSub;      //! leading jet after background subtraction
    TF1*          fFitModulation;           //-> modulation fit for rho

    void          VzeroGainCalibQA();

    // AliMultSelection      *fMultSelection;      //! For Centrality 
    void       SetupPileUpRemovalFunctions();
    Bool_t     CheckEventIsPileUp2018();

    
    void       AllocateBkgHistograms();
    void       AllocateJetHistograms();
    void       AllocateMatchedJetHistograms();
    void       AllocateTrackHistograms();
    
    void       MeasureTpcEPQA();
    
    Bool_t     MeasureBkg(Double_t baseJetRho);
    
    Bool_t     DoEventPlane();
    void       DoJetLoop();
    void       DoJetMatching();
    Bool_t     PerformGeometricalJetMatching(AliJetContainer& contBase, AliJetContainer& contTag, Double_t maxDist);
    const AliEmcalJet* GetMatchedPartLevelJet(const AliEmcalJet* detJet, Double_t detJetPt, TString groupName);
    const AliEmcalJet* GetMatchedJet(const AliEmcalJet* detJet, Double_t detJetPt, TString groupName);
    Double_t GetAngularity(const AliEmcalJet* jet);
    Double_t GetRelativePhi(Double_t mphi, Double_t vphi);
    void     CaclJetEnergyScaleShift();

    void       DoTrackLoop();
    
    Bool_t     QnJEHandlarEPGet();
    Bool_t     QnV0GainCalibration();
    Bool_t     QnRecenteringCalibration();
    Bool_t     QnCalcWOCalib();

    Double_t CalcEPAngle(double Qx,double Qy) const {return (TMath::Pi()+TMath::ATan2(-Qy,-Qx))/2;}
    Double_t CalcEPReso(Int_t n, Double_t &psiA, Double_t &psiB, Double_t &psiC);

    void  CalcRandomCone(Double_t &pt, Double_t &eta, Double_t &phi, \
        Double_t &leadingJetEta, Double_t &leadingJetPhi, Double_t &jetR
    ) const;

    // TH1F*   GetResoFromOutputFile(detectorType det, Int_t h, TArrayD* cen);
    Double_t CalculateEventPlaneChi(Double_t res);

    /* inline */  static Double_t ChiSquarePDF(Int_t ndf, Double_t x) {
        Double_t n(ndf/2.), denom(TMath::Power(2, n)*TMath::Gamma(n));
        if (denom!=0)  return ((1./denom)*TMath::Power(x, n-1)*TMath::Exp(-x/2.)); 
        return -999; 
    }

    // note that the cdf of the chisquare distribution is the normalized lower incomplete gamma function
    /* inline */  static Double_t ChiSquareCDF(Int_t ndf, Double_t x) { return TMath::Gamma(ndf/2., x/2.); }

    /* inline */  static Double_t ChiSquare(TH1& histo, TF1* func) {
        // evaluate the chi2 using a poissonian error estimate on bins
        Double_t chi2(0.);
        for(Int_t i(0); i < histo.GetXaxis()->GetNbins(); i++) {
            if(histo.GetBinContent(i+1) <= 0.) continue;
            chi2 += TMath::Power((histo.GetBinContent(i+1) \
                - func->Eval(histo.GetXaxis()->GetBinCenter(1+i))), 2)\
                /histo.GetBinContent(i+1);
        }
        return chi2;
    }

    AliEmcalJet* GetLeadingJet(AliLocalRhoParameter* localRho = 0x0);
    void GetLeadingAndSubJet();

    // static Double_t CalcEPChi(Double_t res)
    // {
    //   // return chi for given resolution to combine event plane estimates from two subevents
    //   // see Phys. Rev. C no. CS6346 (http://arxiv.org/abs/nucl-ex/9805001)
    //   Double_t chi(2.), delta(1.), con((TMath::Sqrt(TMath::Pi()))/(2.*TMath::Sqrt(2)));
    //   for (Int_t i(0); i < 15; i++) {
    //       chi = ((con*chi*TMath::Exp(-chi*chi/4.)*(TMath::BesselI0(chi*chi/4.)+TMath::BesselI1(chi*chi/4.))) < res) ? chi + delta : chi - delta;
    //       delta = delta / 2.;
    //   }
    //   return chi;
    // }
    
    //  Functions for Pile Up Event Removal:
    // TF1   *fV0CutPU;        //!<!
    // TF1   *fSPDCutPU;       //!<!
    // TF1   *fMultCutPU;      //!<!
    // TF1   *fCenCutLowPU;    //!<!
    // TF1   *fCenCutHighPU;   //!<!
    
    Double_t V0Mult2[3];    /// For q2 V0 0:combin, 1:eta negative (C side), 2:eta positive (A side)
    Double_t V0Mult3[3];    /// For q3 V0 0:combin, 1:eta negative (C side), 2:eta positive (A side)
    Double_t V0MultForAngle[8];    /// Multiplicity for 8 region
    
    //qnVector 0:x, 1:y
    Double_t q2VecV0M[2];   ///< Q2 V0 C+A vector(x,y)
    Double_t q2VecV0C[2];   ///< Q2 V0 C+A vector(x,y)
    Double_t q2VecV0A[2];   ///< Q2 V0 C+A vector(x,y)
    Double_t q2V0[3];       ///< psi2 V0 0:combin, 1:eta negative (C side), 2:eta positive (A side)
    Double_t q2NormV0[3];   ///< Q2 V0 0:combin, 1:eta negative (C side), 2:eta positive (A side)
    Double_t q3VecV0M[2];   ///< Q3 V0 C+A vector(x,y)
    Double_t q3VecV0C[2];   ///< Q3 V0 C vector(x,y)
    Double_t q3VecV0A[2];   ///< Q3 V0 A vector(x,y)
    Double_t q3V0[3];       ///< Q3 V0 0:combin, 1:eta negative (C side), 2:eta positive (A side)
    Double_t q3NormV0[3];   ///< Q3 V0 0:combin, 1:eta negative (C side), 2:eta positive (A side)
    
    Double_t psi2V0[3];     ///< psi2 V0 0:combin, 1:eta negative (C side), 2:eta positive (A side)
    Double_t psi3V0[3];     ///< psi3 V0 0:combin, 1:eta negative (C side), 2:eta positive (A side)


    Double_t TpcMult2[3];  ///< For q2 V0 0:combin, 1:eta negative (C side), 2:eta positive (A side)
    Double_t TpcMult3[3];  ///< For q3 V0 0:combin, 1:eta negative (C side), 2:eta positive (A side)
    //qnVector 0:eta posi, 1:eta nega
    Double_t q2VecTpcM[2];  ///<  Q2 TPC posi + nega
    Double_t q2VecTpcP[2];  ///<  Q2 TPC nega
    Double_t q2VecTpcN[2];  ///<  Q2 TPC nega
    Double_t q2Tpc[3];      ///<  psi2 TPC 0:eta negative (C side), 1:eta positive (A side)side)
    Double_t q2NormTpc[3];  ///<  psi2 TPC 0:eta negative (C side), 1:eta positive (A side)side)
    Double_t q3VecTpcM[2];  ///<  Q3 TPC posi + nega
    Double_t q3VecTpcP[2];  ///<  Q3 TPC posi
    Double_t q3VecTpcN[2];  ///<  Q3 TPC nega
    Double_t q3Tpc[3];      ///<  psi2 TPC 0:eta negative (C side), 1:eta positive (A side)
    Double_t q3NormTpc[3];  ///<  psi2 TPC 0:eta negative (C side), 1:eta positive (A side)side)

    Double_t psi2Tpc[3];    ///<  psi2 TPC 0:eta negative (C side), 1:eta positive (A side)
    Double_t psi3Tpc[3];    ///<  psi3 TPC 0:eta negative (C side), 1:eta positive (A side)

    Double_t fV2ResoV0;     ///<  V2 resolution value
    Double_t fV3ResoV0;     ///<  V3 resolution value

    Double_t fMinShareFraction;
    Double_t fMaxDeltaR;


    TList     *fCalibRefObjList;   ///<
    TH2F      *fHCorrV0ChWeghts;   //!<!
    TH1D      *fHCorrQ2xV0C;       //!<!
    TH1D      *fHCorrQ2yV0C;       //!<!
    TH1D      *fHCorrQ2xV0A;       //!<!
    TH1D      *fHCorrQ2yV0A;       //!<!
    TH1D      *fHCorrQ3xV0C;       //!<!
    TH1D      *fHCorrQ3yV0C;       //!<!
    TH1D      *fHCorrQ3xV0A;       //!<!
    TH1D      *fHCorrQ3yV0A;       //!<! 
    
    int fPrevEventRun;         ///< run number of event previously analysed

    TString fTriggerClass;                 ///< trigger class
    unsigned long long fTriggerMask;       ///< trigger mask
    bool fRejectTPCPileup;                 ///< TPC pileup rejection
    
  /// ========================================================================================
    //utils methods
    void ComputeQvecV0(Double_t QnVecV0M[2],Double_t QnVecV0C[2],Double_t QnVecV0A[2], Double_t QnNorm[3], Double_t Multi[3], unsigned int harmonic);
    void ComputeQvecTpc(Double_t QnVecTpcM[2],Double_t QnVecTpcN[2],Double_t QnVecTpcP[2], Double_t QnNorm[3], Double_t Multi[3], unsigned int harmonic);
    Double_t ComputeEventPlaneAngle(Double_t QnVec[2], Double_t harmonic) const {return (TMath::Pi()+TMath::ATan2(-QnVec[1],-QnVec[0]))/harmonic;};
    

    short GetVertexZbin() const;
    Int_t GetCentBin();
    bool OpenInfoCalbration();

    bool IsTrackSelected(AliAODTrack* track);

    //data members
    double fEtaMinTPC;         ///< Absolute minimum value of eta for TPC Q vector
    double fEtaMaxTPC;         ///< Absolute maximum value of eta for TPC Q vector
    double fPtMinTPC;          ///< Minimum value of pt for TPC Q vector
    double fPtMaxTPC;          ///< Maximum value of pt for TPC Q vector

    TBits fUsedTrackPosIDs;    ///< IDs of tracks (with positive ID) used for the computation of the Q vector (TPC)
    TBits fUsedTrackNegIDs;    ///< IDs of tracks (with negative ID) used for the computation of the Q vector (TPC)

    //random rejection 
    double fFractionOfTracksForQnTPC;   ///< Fraction of tracks to keep when rejection enabled

    //data members needed for calibrations
  
    AliAODVZERO* fV0;                        ///< V0 info for the considered event
    int fRun;                                ///< Run number
    double fZvtx;                            ///< Primary vertx Z 
    double fCentrality;                      ///< Event centrality

    bool fIsOADBFileOpen;                    ///< Flag to test whether the OADB file is open

    // OADB Objects for streaming
    AliOADBContainer * fMultV0BefCorPfpx;    ///< OADB object containing the hMultV0BefCorPfpx histograms

    TObjArray *fOADBzArray_contQx2am; ///< Array of OADB contQx2am object, index is z-vertex bin
    TObjArray *fOADBzArray_contQy2am; ///< Array of OADB contQy2am object, index is z-vertex bin
    TObjArray *fOADBzArray_contQx2as; ///< Array of OADB contQx2as object, index is z-vertex bin
    TObjArray *fOADBzArray_contQy2as; ///< Array of OADB contQy2as object, index is z-vertex bin

    TObjArray *fOADBzArray_contQx2cm; ///< Array of OADB contQx2cm object, index is z-vertex bin
    TObjArray *fOADBzArray_contQy2cm; ///< Array of OADB contQy2cm object, index is z-vertex bin
    TObjArray *fOADBzArray_contQx2cs; ///< Array of OADB contQx2cs object, index is z-vertex bin
    TObjArray *fOADBzArray_contQy2cs; ///< Array of OADB contQy2cs object, index is z-vertex bin

    TObjArray *fOADBzArray_contQx3am; ///< Array of OADB contQx3am object, index is z-vertex bin
    TObjArray *fOADBzArray_contQy3am; ///< Array of OADB contQy3am object, index is z-vertex bin
    TObjArray *fOADBzArray_contQx3as; ///< Array of OADB contQx3as object, index is z-vertex bin
    TObjArray *fOADBzArray_contQy3as; ///< Array of OADB contQy3as object, index is z-vertex bin

    TObjArray *fOADBzArray_contQx3cm; ///< Array of OADB contQx3cm object, index is z-vertex bin
    TObjArray *fOADBzArray_contQy3cm; ///< Array of OADB contQy3cm object, index is z-vertex bin
    TObjArray *fOADBzArray_contQx3cs; ///< Array of OADB contQx3cs object, index is z-vertex bin
    TObjArray *fOADBzArray_contQy3cs; ///< Array of OADB contQy3cs object, index is z-vertex bin

    TObjArray *fOADBcentArray_contTPCposEta; ///< Array of OADB contTPCposEta, index is cent bin
    TObjArray *fOADBcentArray_contTPCnegEta; ///< Array of OADB contTPCnegEta, index is cent bin

    int fCalibObjRun;            ///< Run of loaded calibration objects

    TH1D* fHistMultV0;           ///< Profile from V0 multiplicity

    TH1D* fQx2mV0A[14];          ///< <Qxn> V0A
    TH1D* fQy2mV0A[14];          ///< <Qyn> V0A
    TH1D* fQx2sV0A[14];          ///< sigma Qxn V0A
    TH1D* fQy2sV0A[14];          ///< sigma Qyn V0A
    
    TH1D* fQx2mV0C[14];          ///< <Qxn> V0C
    TH1D* fQy2mV0C[14];          ///< <Qyn> V0C
    TH1D* fQx2sV0C[14];          ///< sigma Qxn V0C
    TH1D* fQy2sV0C[14];          ///< sigma Qyn V0C

    TH1D* fQx3mV0A[14];          ///< <Qxn> V0A
    TH1D* fQy3mV0A[14];          ///< <Qyn> V0A
    TH1D* fQx3sV0A[14];          ///< sigma Qxn V0A
    TH1D* fQy3sV0A[14];          ///< sigma Qyn V0A
    
    TH1D* fQx3mV0C[14];          ///< <Qxn> V0C
    TH1D* fQy3mV0C[14];          ///< <Qyn> V0C
    TH1D* fQx3sV0C[14];          ///< sigma Qxn V0C
    TH1D* fQy3sV0C[14];          ///< sigma Qyn V0C

    bool fV0CalibZvtxDiff;       ///< flag to properly manage Zvtx differential V0 calibrations

    TH1D* fWeightsTPCPosEta[11];  ///< Weights for TPC tracks with eta > 0
    TH1D* fWeightsTPCNegEta[11];  ///< Weights for TPC tracks with eta < 0
    bool fEnablePhiDistrHistos;  ///< Enable phi distribution histos
    TH2F* fPhiVsCentrTPC[2];     ///< Phi vs. centr TH2 of selected TPC tracks in eta>0 and eta<0
    
  /// ========================================================================================

    // not implemented
    
    AliAnalysisTaskEmbeddingJetWithEP(const AliAnalysisTaskEmbeddingJetWithEP&); // not implemented
    AliAnalysisTaskEmbeddingJetWithEP &operator=(const AliAnalysisTaskEmbeddingJetWithEP&);

    ClassDef(AliAnalysisTaskEmbeddingJetWithEP, 135);
};

#endif



