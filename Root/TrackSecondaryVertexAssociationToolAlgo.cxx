#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <IDTrackSel/TrackSecondaryVertexAssociationToolAlgo.h>

#include <EventLoop/OutputStream.h>

#include <AsgTools/MessageCheck.h>

// this is needed to distribute the algorithm to the workers
ClassImp(TrackSecondaryVertexAssociationToolAlgo)

TrackSecondaryVertexAssociationToolAlgo :: TrackSecondaryVertexAssociationToolAlgo () {}

EL::StatusCode TrackSecondaryVertexAssociationToolAlgo :: setupJob (EL::Job& job)
{
  ANA_CHECK_SET_TYPE (EL::StatusCode);
  job.useXAOD();
  ANA_CHECK(xAOD::Init()); // call before opening first file

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrackSecondaryVertexAssociationToolAlgo :: histInitialize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TrackSecondaryVertexAssociationToolAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TrackSecondaryVertexAssociationToolAlgo :: changeInput (bool firstFile) { return EL::StatusCode::SUCCESS; }

EL::StatusCode TrackSecondaryVertexAssociationToolAlgo :: initialize ()
{
  Info("initialize()", "TrackSecondaryVertexAssociationToolAlgo_%s", m_inputTrackContainer.c_str() );
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

EL::StatusCode TrackSecondaryVertexAssociationToolAlgo :: execute ()
{
  // Find the PV
       
  const xAOD::VertexContainer* vertices = 0;
  //TString m_vertexContainerName = "PrimaryVertices";
  if ( !m_event->retrieve( vertices, m_inputVertexContainer ).isSuccess() ){
    Error("execute()","Failed to retrieve vertex container. Exiting.");
    return EL::StatusCode::FAILURE;
  }

  //const auto it_pv = std::find_if(vertices->cbegin(), vertices->cend(),[](const xAOD::Vertex* vtx){return vtx->vertexType() == xAOD::VxType::PriVtx;});
  //const xAOD::Vertex* primaryVertex = (it_pv == vertices->cend()) ? nullptr : *it_pv;
  
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

    // Only want to take each track ones
    bool alreadyHaveThisTrack = false;

    // Loop over the vertices ...
    // Make some requirements about their quality
    for (auto vertex : *vertices)
      {
	//std::cout << "vertex has " << vertex->trackParticleLinks().size() << " associated tracks" << std::endl;
	//std::cout << "vertex has a chi2 of " << vertex->chiSquared() << " / " << vertex->numberDoF() << std::endl;;

	if(vertex->trackParticleLinks().size() != 2) continue;
	if(vertex->chiSquared() / vertex->numberDoF() < 15) continue;

	// Loop over the tracks associated with that vertex
	int nGood = 0;
	for (unsigned int trackFromVertex=0; trackFromVertex<vertex->trackParticleLinks().size(); trackFromVertex++)
	  {
	    TLorentzVector outside_track;
	    TLorentzVector inside_track;

	    outside_track.SetPxPyPzE(track->p4().Px(), track->p4().Py(), track->p4().Pz(), track->p4().E());
	    inside_track.SetPxPyPzE((*vertex->trackParticleLinks().at(trackFromVertex))->p4().Px(),
				    (*vertex->trackParticleLinks().at(trackFromVertex))->p4().Py(),
				    (*vertex->trackParticleLinks().at(trackFromVertex))->p4().Pz(),
				    (*vertex->trackParticleLinks().at(trackFromVertex))->p4().E());
	    
	    if( ((outside_track.Eta() - inside_track.Eta()) < 0.1)
		&& ((outside_track.Phi() - inside_track.Phi()) < 0.1) )
	      {
		Info("execute()","nice.");
		nGood++;
	      }

	    // If these tracks match the outside ones, they already pass our previous criteria (applied in prev. algos),
	    // and so we push 'em back in the output. Each track should only enter once.
	
	    if(alreadyHaveThisTrack) std::cout << "Tracks associated with multiple vertices!!!! MLB ALERT ALERT!!!!" << std::endl;
	    
	    if(nGood!=2)
	      {
		Info("execute()","Don't have two good ones. Bummer.");
		continue;
	      }
	    else
	      {
		if(!alreadyHaveThisTrack)
		  {
		    selectedTracks->push_back(track);	
		    alreadyHaveThisTrack = true;
		  }
	      }
	  }
      }
  }

  m_store->record( selectedTracks, m_outputTrackContainer );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TrackSecondaryVertexAssociationToolAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TrackSecondaryVertexAssociationToolAlgo :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TrackSecondaryVertexAssociationToolAlgo :: histFinalize () { return EL::StatusCode::SUCCESS; }
