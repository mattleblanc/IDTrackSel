#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <IDTrackSel/TightTrackVertexAssociationToolAlgo.h>

#include <EventLoop/OutputStream.h>

#include <AsgTools/MessageCheck.h>

// this is needed to distribute the algorithm to the workers
ClassImp(TightTrackVertexAssociationToolAlgo)

TightTrackVertexAssociationToolAlgo :: TightTrackVertexAssociationToolAlgo () {}

EL::StatusCode TightTrackVertexAssociationToolAlgo :: setupJob (EL::Job& job)
{
  ANA_CHECK_SET_TYPE (EL::StatusCode);
  job.useXAOD();
  ANA_CHECK(xAOD::Init()); // call before opening first file

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TightTrackVertexAssociationToolAlgo :: histInitialize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TightTrackVertexAssociationToolAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TightTrackVertexAssociationToolAlgo :: changeInput (bool firstFile) { return EL::StatusCode::SUCCESS; }

EL::StatusCode TightTrackVertexAssociationToolAlgo :: initialize ()
{
  Info("initialize()", "TightTrackVertexAssociationToolAlgo_%s", m_inputTrackContainer.c_str() );
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  
  m_TightTrktoVxTool = new CP::TightTrackVertexAssociationTool("TightTrackVertexAssociationTool/"+m_name);

  if(m_dzSinTheta_cut!=-999)
    ANA_CHECK( m_TightTrktoVxTool->setProperty("dzSinTheta_cut", m_dzSinTheta_cut) ); // mm
  
  if(m_d0_cut!=-999)
    ANA_CHECK( m_TightTrktoVxTool->setProperty("d0_cut", m_d0_cut) ); // mm

  ANA_CHECK( m_TightTrktoVxTool->setProperty("Applyd0Selection",m_Applyd0Selection) );
  ANA_CHECK( m_TightTrktoVxTool->setProperty("doPV",m_doPV) );

  ANA_CHECK( m_TightTrktoVxTool->initialize() );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TightTrackVertexAssociationToolAlgo :: execute ()
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
    if(!m_TightTrktoVxTool->isCompatible(*track,*primaryVertex)) 
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

EL::StatusCode TightTrackVertexAssociationToolAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TightTrackVertexAssociationToolAlgo :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TightTrackVertexAssociationToolAlgo :: histFinalize () { return EL::StatusCode::SUCCESS; }
