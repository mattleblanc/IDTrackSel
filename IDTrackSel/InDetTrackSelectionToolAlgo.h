#ifndef IDTrackSel_InDetTrackSelectionToolAlgo_H
#define IDTrackSel_InDetTrackSelectionToolAlgo_H

#include <EventLoop/Algorithm.h>

#include <xAODRootAccess/Init.h>
#include <xAODRootAccess/TEvent.h>
#include <xAODRootAccess/TStore.h>

#include "InDetTrackSelectionTool/InDetTrackSelectionTool.h"
#include "xAODTracking/TrackParticleContainer.h"

class InDetTrackSelectionToolAlgo : public EL::Algorithm
{
public:
  std::string m_name = "MattLeBlanc";
  std::string m_inputTrackContainer;
  std::string m_outputTrackContainer;
  
  std::string m_CutLevel="Loose";
  float m_minPt = -999;
  float m_minP = -999;
  float m_maxAbsEta = -999;
  float m_maxD0 = -999;
  float m_maxZ0 = -999;
  float m_maxZ0SinTheta = -999;
  int m_minNInnerMostLayerHits = -999;
  int m_minNNextToInnermostLayerHits = -999;
  int m_minNBothInnermostLayersHits = -999;
  int m_maxNInnerMostLayerSharedHits = -999;
  //bool m_useMinBiasInnermostLayersCut = -999;
  int m_minNSiHits = -999;
  int m_maxNSiSharedHits = -999;
  int m_minNSiHitsPhysical  = -999;
  int m_minNSiHitsIfSiSharedHits  = -999;
  int m_maxNSiHoles = -999;
  int m_minNPixelHits  = -999;
  int m_maxNPixelSharedHits = -999;
  int m_maxNPixelHoles = -999;
  int m_minNPixelHitsPhysical  = -999;
  int m_minNSctHits = -999;
  int m_maxNSctSharedHits = -999;
  int m_maxNSctHoles = -999;
  int m_maxNSctDoubleHoles = -999;
  float m_minEtaForStrictNSiHitsCut  = -999;
  int m_minNSiHitsAboveEtaCutoff  = -999;
  int m_maxOneSharedModule  = -999;
  float m_maxTrtEtaAcceptance  = -999;
  float m_maxEtaForTrtHitCuts  = -999;
  int m_minNTrtHits = -999;
  int m_minNTrtHitsPlusOutliers = -999;
  int m_minNTrtHighThresholdHits = -999;
  int m_minNTrtHighThresholdHitsPlusOutliers = -999;
  int m_maxTrtHighEFraction  = -999;
  int m_maxTrtHighEFractionWithOutliers = -999;
  bool m_useEtaDependentMaxChiSq  = -999;
  float m_maxChiSq = -999;
  float m_minProb  = -999;
  float m_maxChiSqperNdf = -999;
  float m_minPtForProbCut  = -999;
  float m_minProbAbovePtCutoff = -999;
  int m_minNUsedHitsdEdx = -999;
  int m_minNOverflowHitsdEdx = -999;
  float m_minEProbabilityHT = -999;
  float m_eProbHTonlyForXe = -999;

  //Float_t m_maxZ0SinTheta;
  //Float_t m_maxD0overSigmaD0;
  Bool_t m_debug;
    
 private:
  xAOD::TEvent *m_event; //!
  xAOD::TStore *m_store; //!

  InDet::InDetTrackSelectionTool* m_TrackSelTool; //!

 public:
  // this is a standard constructor
  InDetTrackSelectionToolAlgo ();

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  // this is needed to distribute the algorithm to the workers
  ClassDef(InDetTrackSelectionToolAlgo, 1);
};

#endif
