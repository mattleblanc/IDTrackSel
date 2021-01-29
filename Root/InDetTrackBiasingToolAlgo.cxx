#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <IDTrackSel/InDetTrackBiasingToolAlgo.h>

#include "xAODCore/ShallowCopy.h"

#include <EventLoop/OutputStream.h>

#include <AsgTools/MessageCheck.h>

#include "xAODEventInfo/EventInfo.h"

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
  
  // from Ben:
  //Most (>2/3) of the lumi came after 304178. https://atlas.web.cern.ch/Atlas/GROUPS/DATAPREPARATION/PublicPlots/DataSummary/figs/intlumivstimeRun2DQ.png
  // post d0 fix: 304198 - 311481 //should randomly do this, with the right proportions? that is the last pp run of 2016
  //rnumb = 304178; // pre d0 fix: 301912 - 304178 mid-july: https://atlas-tagservices.cern.ch/tagservices/RunBrowser/runBrowserReport/rBR_Period_Report.php?fnt=data16_13TeV&pn=F
  //rnumb = 300908; // Calibrating for 2016 runs before IBL temperature change (297730 to 300908)

  const xAOD::EventInfo* eventInfo = 0;
  if( !m_event->retrieve( eventInfo, "EventInfo" ).isSuccess()) return EL::StatusCode::FAILURE;
  uint32_t rn = eventInfo->runNumber();

  uint32_t rnumb = -1;
  if(!m_isData)
    {
      if(rn==284500) rnumb = 311481; // MC16a
      if(rn==300000) rnumb = 337833; // MC16d
      if(rn==310000) rnumb = 363400; // MC16e      
    }
  else rnumb = rn;

  m_systSetTrkWeak = {
    InDet::TrackSystematicMap[InDet::TRK_BIAS_D0_WM],
    InDet::TrackSystematicMap[InDet::TRK_BIAS_Z0_WM],
    InDet::TrackSystematicMap[InDet::TRK_BIAS_QOVERP_SAGITTA_WM]
  };

  ANA_CHECK( m_InDetTrackBiasingTool->initialize() );
  if(!m_isData) ANA_CHECK( m_InDetTrackBiasingTool->applySystematicVariation( m_systSetTrkWeak) );
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

  //Create a shallow copy of the tracking container and apply the correction to it
  std::pair< xAOD::TrackParticleContainer*, xAOD::ShallowAuxContainer* > inputTracks_shallowCopy = xAOD::shallowCopyContainer( *inputTracks );
  //  ANA_CHECK( m_store->record( inputTracks_shallowCopy.first,  m_outputTrackContainer+"ShallowCopy"));
  //  ANA_CHECK( m_store->record( inputTracks_shallowCopy.second, m_outputTrackContainer+"ShallowCopyAux."));

  m_InDetTrackBiasingTool->applyContainerCorrection( *(inputTracks_shallowCopy.first)); 

  // Create the output TrackParticleContainer
  ConstDataVector<xAOD::TrackParticleContainer>* biasedTracks(nullptr);
  biasedTracks = new ConstDataVector<xAOD::TrackParticleContainer>(SG::VIEW_ELEMENTS);

  for(auto track : *(inputTracks_shallowCopy.first))
    {
      // If there ain't no track, don't take a track ...
      if(!track)
	continue;

      biasedTracks->push_back(track);

    }

  m_store->record( biasedTracks, m_outputTrackContainer );
  
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode InDetTrackBiasingToolAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackBiasingToolAlgo :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackBiasingToolAlgo :: histFinalize () { return EL::StatusCode::SUCCESS; }
