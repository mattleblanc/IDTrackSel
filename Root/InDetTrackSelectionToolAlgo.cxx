#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <IDTrackSel/InDetTrackSelectionToolAlgo.h>

#include <EventLoop/OutputStream.h>

#include <AsgTools/MessageCheck.h>

// this is needed to distribute the algorithm to the workers
ClassImp(InDetTrackSelectionToolAlgo)

InDetTrackSelectionToolAlgo :: InDetTrackSelectionToolAlgo () {}

EL::StatusCode InDetTrackSelectionToolAlgo :: setupJob (EL::Job& job)
{
  ANA_CHECK_SET_TYPE (EL::StatusCode);
  job.useXAOD();
  ANA_CHECK(xAOD::Init()); // call before opening first file

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode InDetTrackSelectionToolAlgo :: histInitialize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackSelectionToolAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackSelectionToolAlgo :: changeInput (bool firstFile) { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackSelectionToolAlgo :: initialize ()
{
  Info("initialize()", "InDetTrackSelectionToolAlgo_%s", m_inputTrackContainer.c_str() );
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  m_TrackSelTool = new InDet::InDetTrackSelectionTool( "InDetTrackSelectionTool/"+m_name );

  ANA_CHECK( m_TrackSelTool->setProperty("CutLevel", m_CutLevel ) ); // set tool to apply the pre-defined "Loose-Primary" cuts

  // Only apply other cuts if the user has specified them.

  if(m_minPt!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minPt",m_minPt));

  if(m_minP!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minP",m_minP));

  if(m_maxAbsEta!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("maxAbsEta",m_maxAbsEta));

  if(m_maxD0!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("maxD0",m_maxD0));

  if(m_maxZ0!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("maxZ0",m_maxZ0));

  if(m_maxZ0SinTheta!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("maxZ0SinTheta",m_maxZ0SinTheta));

  if(m_minNInnerMostLayerHits!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minNInnerMostLayerHits",m_minNInnerMostLayerHits));

  if(m_minNNextToInnermostLayerHits!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minNNextToInnermostLayerHits",m_minNNextToInnermostLayerHits));

  if(m_minNBothInnermostLayersHits!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minNBothInnermostLayersHits",m_minNBothInnermostLayersHits));

  if(m_maxNInnerMostLayerSharedHits!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("maxNInnerMostLayerSharedHits",m_maxNInnerMostLayerSharedHits));

  //if(m_useMinBiasInnermostLayersCut!=-999) 
  //  ANA_CHECK(m_TrackSelTool.setProperty("useMinBiasInnermostLayersCut",m_useMinBiasInnermostLayersCut));

  if(m_minNSiHits!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minNSiHits",m_minNSiHits));

  if(m_maxNSiSharedHits!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("maxNSiSharedHits",m_maxNSiSharedHits));

  if(m_minNSiHitsPhysical!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minNSiHitsPhysical",m_minNSiHitsPhysical));

  if(m_minNSiHitsIfSiSharedHits!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minNSiHitsIfSiSharedHits",m_minNSiHitsIfSiSharedHits));

  if(m_maxNSiHoles!=-999)
    ANA_CHECK(m_TrackSelTool->setProperty("maxNSiHoles",m_maxNSiHoles));

  if(m_minNPixelHits!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minNPixelHits",m_minNPixelHits));

  if(m_maxNPixelSharedHits!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("maxNPixelSharedHits",m_maxNPixelSharedHits));

  if(m_maxNPixelHoles!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("maxNPixelHoles",m_maxNPixelHoles));

  if(m_minNPixelHitsPhysical!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minNPixelHitsPhysical",m_minNPixelHitsPhysical));

  if(m_minNSctHits!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minNSctHits",m_minNSctHits));

  if(m_maxNSctSharedHits!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("maxNSctSharedHits",m_maxNSctSharedHits));

  if(m_maxNSctHoles!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("maxNSctHoles",m_maxNSctHoles));

  if(m_maxNSctDoubleHoles!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("maxNSctDoubleHoles",m_maxNSctDoubleHoles));

  if(m_minEtaForStrictNSiHitsCut!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minEtaForStrictNSiHitsCut",m_minEtaForStrictNSiHitsCut));

  if(m_minNSiHitsAboveEtaCutoff!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minNSiHitsAboveEtaCutoff",m_minNSiHitsAboveEtaCutoff));

  if(m_maxOneSharedModule!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("maxOneSharedModule",m_maxOneSharedModule));

  if(m_maxTrtEtaAcceptance!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("maxTrtEtaAcceptance",m_maxTrtEtaAcceptance));

  if(m_maxEtaForTrtHitCuts!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("maxEtaForTrtHitCuts",m_maxEtaForTrtHitCuts));

  if(m_minNTrtHits!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minNTrtHits",m_minNTrtHits));

  if(m_minNTrtHitsPlusOutliers!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minNTrtHitsPlusOutliers",m_minNTrtHitsPlusOutliers));

  if(m_minNTrtHighThresholdHits!=-999)
    ANA_CHECK(m_TrackSelTool->setProperty("minNTrtHighThresholdHits",m_minNTrtHighThresholdHits));

  if(m_minNTrtHighThresholdHitsPlusOutliers!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minNTrtHighThresholdHitsPlusOutliers",m_minNTrtHighThresholdHitsPlusOutliers));

  if(m_maxTrtHighEFraction!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("maxTrtHighEFraction",m_maxTrtHighEFraction));

  if(m_maxTrtHighEFractionWithOutliers!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("maxTrtHighEFractionWithOutliers",m_maxTrtHighEFractionWithOutliers));

  if(m_useEtaDependentMaxChiSq!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("useEtaDependentMaxChiSq",m_useEtaDependentMaxChiSq));

  if(m_maxChiSq!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("maxChiSq",m_maxChiSq));

  if(m_minProb!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minProb",m_minProb));

  if(m_maxChiSqperNdf!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("maxChiSqperNdf",m_maxChiSqperNdf));

  if(m_minPtForProbCut!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minPtForProbCut",m_minPtForProbCut));

  if(m_minProbAbovePtCutoff!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minProbAbovePtCutoff",m_minProbAbovePtCutoff));

  if(m_minNUsedHitsdEdx!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minNUsedHitsdEdx",m_minNUsedHitsdEdx));

  if(m_minNOverflowHitsdEdx!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minNOverflowHitsdEdx",m_minNOverflowHitsdEdx));

  if(m_minEProbabilityHT!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("minEProbabilityHT",m_minEProbabilityHT));

  if(m_eProbHTonlyForXe!=-999) 
    ANA_CHECK(m_TrackSelTool->setProperty("eProbHTonlyForXe",m_eProbHTonlyForXe));

  ANA_CHECK( m_TrackSelTool->initialize() );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode InDetTrackSelectionToolAlgo :: execute ()
{
  m_debug = false;

  // Retrieve the input TrackParticleContainer
  const xAOD::TrackParticleContainer_v1* inputTracks  = 0;
  if(!m_event->retrieve(inputTracks, m_inputTrackContainer).isSuccess()){
    Error("execute()","Failed to retrieve input track container.\tEject!");
    return EL::StatusCode::FAILURE;
  }

  // Create the output TrackParticleContainer
  ConstDataVector<xAOD::TrackParticleContainer>* selectedTracks(nullptr);
  selectedTracks = new ConstDataVector<xAOD::TrackParticleContainer>(SG::VIEW_ELEMENTS);

  // Loop over the tracks
  //for(const xAOD::TrackParticle* track : *inputTracks){
  for(auto track : *inputTracks){
   
    // If there ain't no track, don't take a track ...
    if(!track)
      continue;

    // If the track don't fit, don't take a track ...
    if(!m_TrackSelTool->accept(*track))
      {
	if(m_debug) Info("execute()","track rejected\t");
	continue;
      }

    if(m_debug) Info("execute()","track accepted\t");
    selectedTracks->push_back(track);
  }

  m_store->record( selectedTracks, m_outputTrackContainer );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode InDetTrackSelectionToolAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackSelectionToolAlgo :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackSelectionToolAlgo :: histFinalize () { return EL::StatusCode::SUCCESS; }
