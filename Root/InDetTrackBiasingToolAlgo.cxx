#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <IDTrackSel/InDetTrackBiasingToolAlgo.h>

#include <EventLoop/OutputStream.h>

#include <AsgTools/MessageCheck.h>

// this is needed to distribute the algorithm to the workers
ClassImp(InDetTrackBiasingToolAlgo)

InDetTrackBiasingToolAlgo :: InDetTrackBiasingToolAlgo () {}

EL::StatusCode InDetTrackBiasingToolAlgo :: setupJob (EL::Job& job)
{
  ANA_CHECK_SET_TYPE (EL::StatusCode);
  job.useXAOD();
  ANA_CHECK(xAOD::Init()); // call before opening first file

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode InDetTrackBiasingToolAlgo :: histInitialize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackBiasingToolAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackBiasingToolAlgo :: changeInput (bool firstFile) { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackBiasingToolAlgo :: initialize ()
{
  Info("initialize()", "InDetTrackBiasingToolAlgo_%s", m_inputTrackContainer.c_str() );
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  m_InDetTrackBiasingTool = new InDet::InDetTrackBiasingTool( "weakmodes" );

  //ANA_CHECK( m_JetTrackFilterToolAlgo->setProperty("CutLevel", m_CutLevel ) ); // set tool to apply the pre-defined "Loose-Primary" cuts

  //Most (>2/3) of the lumi came after 304178. https://atlas.web.cern.ch/Atlas/GROUPS/DATAPREPARATION/PublicPlots/DataSummary/figs/intlumivstimeRun2DQ.png
  // post d0 fix: 304198 - 311481 //should randomly do this, with the right proportions? that is the last pp run of 2016
  //rnumb = 304178; // pre d0 fix: 301912 - 304178 mid-july: https://atlas-tagservices.cern.ch/tagservices/RunBrowser/runBrowserReport/rBR_Period_Report.php?fnt=data16_13TeV&pn=F
  //rnumb = 300908; // Calibrating for 2016 runs before IBL temperature change (297730 to 300908)

  uint32_t rnumb = 311481; 
  ANA_CHECK( m_InDetTrackBiasingTool->initialize() );
  ANA_CHECK( m_InDetTrackBiasingTool->applySystematicVariation( m_systSetTrkWeak) );
  ANA_CHECK( m_InDetTrackBiasingTool->setProperty("runNumber",rnumb) );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode InDetTrackBiasingToolAlgo :: execute ()
{
  m_debug = false;

  // Retrieve the input TrackParticleContainer
  const xAOD::TrackParticleContainer_v1* inputTracks  = 0;
  if(!m_event->retrieve(inputTracks, m_inputTrackContainer).isSuccess()){
    Error("execute()","Failed to retrieve input track container.\tEject!");
    return EL::StatusCode::FAILURE;
  }

  // Create the output TrackParticleContainer
  ConstDataVector<xAOD::TrackParticleContainer>* biasedTracks(nullptr);
  biasedTracks = new ConstDataVector<xAOD::TrackParticleContainer>(SG::VIEW_ELEMENTS);

  // Loop over the tracks
  //for(const xAOD::TrackParticle* track : *inputTracks){
  for(auto track : *inputTracks)
    {
      // If there ain't no track, don't take a track ...
      if(!track)
	continue;

      // Correct the tracks and push back into the new container
      xAOD::TrackParticle* newTrack = nullptr;
      m_InDetTrackBiasingTool->correctedCopy( *track, newTrack );      
      biasedTracks->push_back(track);
      delete newTrack;
    }

  //m_InDetTrackBiasingTool->applyContainerCorrection( *biasedTracks );
  m_store->record( biasedTracks, m_outputTrackContainer );
  
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode InDetTrackBiasingToolAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackBiasingToolAlgo :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackBiasingToolAlgo :: histFinalize () { return EL::StatusCode::SUCCESS; }
