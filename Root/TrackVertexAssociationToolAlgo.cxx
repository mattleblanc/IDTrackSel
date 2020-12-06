#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <IDTrackSel/TrackVertexAssociationToolAlgo.h>

#include <EventLoop/OutputStream.h>

#include <AsgTools/MessageCheck.h>

// this is needed to distribute the algorithm to the workers
ClassImp(TrackVertexAssociationToolAlgo)

TrackVertexAssociationToolAlgo :: TrackVertexAssociationToolAlgo () {}

EL::StatusCode TrackVertexAssociationToolAlgo :: setupJob (EL::Job& job)
{
  ANA_CHECK_SET_TYPE (EL::StatusCode);
  job.useXAOD();
  ANA_CHECK(xAOD::Init()); // call before opening first file

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrackVertexAssociationToolAlgo :: histInitialize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TrackVertexAssociationToolAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TrackVertexAssociationToolAlgo :: changeInput (bool firstFile) { return EL::StatusCode::SUCCESS; }

EL::StatusCode TrackVertexAssociationToolAlgo :: initialize ()
{
  Info("initialize()", "TrackVertexAssociationToolAlgo_%s", m_inputTrackContainer.c_str() );
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  
  m_TrktoVxTool = new CP::TrackVertexAssociationTool( "TrackVertexAssociationTool/"+m_name );
  ANA_CHECK(m_TrktoVxTool->setProperty( "WorkingPoint", m_workingPoint));
  ANA_CHECK(m_TrktoVxTool->initialize());

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrackVertexAssociationToolAlgo :: execute ()
{
  // Find the PV
       
  const xAOD::VertexContainer* vertices = 0;
  TString m_vertexContainerName = "PrimaryVertices";
  if ( !m_event->retrieve( vertices, m_vertexContainerName.Data() ).isSuccess() ){
    Error("execute()","Failed to retrieve vertex container. Exiting.");
    return EL::StatusCode::FAILURE;
  }
  
  xAOD::Vertex* primaryVertex = nullptr;
    if(vertices->size()==1) // special case, or else there are problems
    {
      primaryVertex = *vertices->cbegin();
    }
  else
    {
      const auto it_pv = std::find_if(vertices->cbegin(), vertices->cend(),[](const xAOD::Vertex* vtx){return vtx->vertexType() == xAOD::VxType::PriVtx;});
      primaryVertex = (it_pv == vertices->cend()) ? nullptr : *it_pv;
    }

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

    // If the track ain't associated with the primary vertex, don't take a track ...
    if(!m_TrktoVxTool->isCompatible(*track,*primaryVertex)) 
      {
	//if(m_debug) Info("execute()","track rejected");
	//Info("execute()","track rejected");
	continue;
      }

    //if(m_debug) Info("execute()","track accepted\t");
    //Info("execute()","track accepted\t");
    selectedTracks->push_back(track);
  }

  m_store->record( selectedTracks, m_outputTrackContainer );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrackVertexAssociationToolAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TrackVertexAssociationToolAlgo :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TrackVertexAssociationToolAlgo :: histFinalize () { return EL::StatusCode::SUCCESS; }
