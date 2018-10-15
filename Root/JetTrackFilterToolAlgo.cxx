#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <IDTrackSel/JetTrackFilterToolAlgo.h>

#include <EventLoop/OutputStream.h>

#include <AsgTools/MessageCheck.h>

// this is needed to distribute the algorithm to the workers
ClassImp(JetTrackFilterToolAlgo)

JetTrackFilterToolAlgo :: JetTrackFilterToolAlgo () {}

EL::StatusCode JetTrackFilterToolAlgo :: setupJob (EL::Job& job)
{
  ANA_CHECK_SET_TYPE (EL::StatusCode);
  job.useXAOD();
  ANA_CHECK(xAOD::Init()); // call before opening first file

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetTrackFilterToolAlgo :: histInitialize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode JetTrackFilterToolAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode JetTrackFilterToolAlgo :: changeInput (bool firstFile) { return EL::StatusCode::SUCCESS; }

EL::StatusCode JetTrackFilterToolAlgo :: initialize ()
{
  Info("initialize()", "JetTrackFilterToolAlgo_%s", m_inputTrackContainer.c_str() );
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  m_JetTrackFilterTool = new InDet::JetTrackFilterTool( "JetTrackFilterTool/"+m_name );

  //ANA_CHECK( m_JetTrackFilterToolAlgo->setProperty("CutLevel", m_CutLevel ) ); // set tool to apply the pre-defined "Loose-Primary" cuts

  ANA_CHECK( m_JetTrackFilterTool->initialize() );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetTrackFilterToolAlgo :: execute ()
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
    /*
    if(!m_TrackSelTool->accept(*track))
      {
	if(m_debug) Info("execute()","track rejected\t");
	continue;
      }
    */
    if(m_debug) Info("execute()","track accepted\t");
    selectedTracks->push_back(track);
  }

  m_store->record( selectedTracks, m_outputTrackContainer );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode JetTrackFilterToolAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode JetTrackFilterToolAlgo :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode JetTrackFilterToolAlgo :: histFinalize () { return EL::StatusCode::SUCCESS; }
