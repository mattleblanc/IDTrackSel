#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <IDTrackSel/InDetTrackTruthOriginToolAlgo.h>

#include <EventLoop/OutputStream.h>

#include <AsgTools/MessageCheck.h>

// this is needed to distribute the algorithm to the workers
ClassImp(InDetTrackTruthOriginToolAlgo)

InDetTrackTruthOriginToolAlgo :: InDetTrackTruthOriginToolAlgo() {}

EL::StatusCode InDetTrackTruthOriginToolAlgo :: setupJob (EL::Job& job)
{
  ANA_CHECK_SET_TYPE (EL::StatusCode);
  job.useXAOD();
  ANA_CHECK(xAOD::Init()); // call before opening first file

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode InDetTrackTruthOriginToolAlgo :: histInitialize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackTruthOriginToolAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackTruthOriginToolAlgo :: changeInput (bool firstFile) { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackTruthOriginToolAlgo :: initialize ()
{
  Info("initialize()", "InDetTrackTruthOriginToolAlgo_%s", m_inputTrackContainer.c_str() );
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  m_InDetTrackTruthOriginTool = new InDet::InDetTrackTruthOriginTool( "InDet::InDetTrackTruthOriginTool" );
 
  ANA_CHECK( m_InDetTrackTruthOriginTool->initialize() );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode InDetTrackTruthOriginToolAlgo :: execute ()
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
  /*
  for(auto track : *inputTracks)
    {
   
      // If there ain't no track, don't take a track ...
      if(!track)
	continue;
      
      // If the track don't fit, don't take a track ...
      
      //if(!m_TrackSelTool->accept(*track))
      //{
      //if(m_debug) Info("execute()","track rejected\t");
      //continue;
      //}
      
      if(m_debug) Info("execute()","track accepted\t");
      selectedTracks->push_back(track);
    }
  */

//m_store->record( selectedTracks, m_outputTrackContainer );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode InDetTrackTruthOriginToolAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackTruthOriginToolAlgo :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode InDetTrackTruthOriginToolAlgo :: histFinalize () { return EL::StatusCode::SUCCESS; }
