#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <IDTrackSel/InDetTrackTruthFilterToolAlgo.h>

#include <EventLoop/OutputStream.h>

#include <AsgTools/MessageCheck.h>

// this is needed to distribute the algorithm to the workers
ClassImp(InDetTrackTruthFilterToolAlgo)

InDetTrackTruthFilterToolAlgo :: InDetTrackTruthFilterToolAlgo() {}

EL::StatusCode InDetTrackTruthFilterToolAlgo :: setupJob (EL::Job& job)
{
  ANA_CHECK_SET_TYPE (EL::StatusCode);
  job.useXAOD();
  ANA_CHECK(xAOD::Init()); // call before opening first file

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode InDetTrackTruthFilterToolAlgo :: histInitialize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackTruthFilterToolAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackTruthFilterToolAlgo :: changeInput (bool firstFile) { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackTruthFilterToolAlgo :: initialize ()
{
  Info("initialize()", "InDetTrackTruthFilterToolAlgo_%s", m_inputTrackContainer.c_str() );
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  
  CP::SystematicSet systSetTrk = {};
  if(m_systematic=="incl")
    {
       systSetTrk = {
	InDet::TrackSystematicMap[InDet::TRK_EFF_TIGHT_GLOBAL],
	InDet::TrackSystematicMap[InDet::TRK_EFF_TIGHT_IBL],
	InDet::TrackSystematicMap[InDet::TRK_EFF_TIGHT_PP0],
	InDet::TrackSystematicMap[InDet::TRK_EFF_TIGHT_PHYSMODEL]
      };
    }
  if(m_systematic=="fake")
    {
      CP::SystematicSet systSetTrk = {
	InDet::TrackSystematicMap[InDet::TRK_FAKE_RATE_LOOSE]
      };
    }
  

  m_InDetTrackTruthFilterTool = new InDet::InDetTrackTruthFilterTool( m_systematic );
  ANA_CHECK( m_InDetTrackTruthFilterTool->initialize() );
  ANA_CHECK( m_InDetTrackTruthFilterTool->applySystematicVariation(systSetTrk) );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode InDetTrackTruthFilterToolAlgo :: execute ()
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
  for(auto track : *inputTracks){
   
    // If there ain't no track, don't take a track ...
    if(!track)
      continue;

    // If the track don't fit, don't take a track ...
    if(!m_InDetTrackTruthFilterTool->accept(track))
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

EL::StatusCode InDetTrackTruthFilterToolAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackTruthFilterToolAlgo :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackTruthFilterToolAlgo :: histFinalize () { return EL::StatusCode::SUCCESS; }
