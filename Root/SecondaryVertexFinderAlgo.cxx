#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <IDTrackSel/SecondaryVertexFinderAlgo.h>

#include <EventLoop/OutputStream.h>

#include <AsgTools/MessageCheck.h>

// this is needed to distribute the algorithm to the workers
ClassImp(SecondaryVertexFinderAlgo)

SecondaryVertexFinderAlgo :: SecondaryVertexFinderAlgo () {}

EL::StatusCode SecondaryVertexFinderAlgo :: setupJob (EL::Job& job)
{
  ANA_CHECK_SET_TYPE (EL::StatusCode);
  job.useXAOD();
  ANA_CHECK(xAOD::Init()); // call before opening first file

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode SecondaryVertexFinderAlgo :: histInitialize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode SecondaryVertexFinderAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode SecondaryVertexFinderAlgo :: changeInput (bool firstFile) { return EL::StatusCode::SUCCESS; }

EL::StatusCode SecondaryVertexFinderAlgo :: initialize ()
{
  Info("initialize()", "SecondaryVertexFinderAlgo_%s", m_inputTrackContainer.c_str() );
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode SecondaryVertexFinderAlgo :: execute ()
{
  m_debug = true;

  // Create the output TrackParticleContainer
  ConstDataVector<xAOD::TrackParticleContainer>* selectedTracks(nullptr);
  selectedTracks = new ConstDataVector<xAOD::TrackParticleContainer>(SG::VIEW_ELEMENTS);

  // Retrieve the input TrackParticleContainer 
  const xAOD::TrackParticleContainer_v1* inputTracks  = 0;
  if(!m_event->retrieve(inputTracks, m_inputTrackContainer).isSuccess())
    {
      Error("execute()","Failed to retrieve input track container.\tEject!");
      return EL::StatusCode::FAILURE;
    }

  //ANA_MSG_DEBUG("execute()\tnTracks in = \t" << inputTracks->size());
  std::cout << "execute()\tnTracks in = \t" << inputTracks->size() << std::endl;

  Float_t m_trackPtMin = 0.5;
  
  // lambda range
  Float_t m_invMassMin = 1105.;
  Float_t m_invMassMax = 1125.;
  
  // Secondary vertex max chi2
  Float_t m_Chi2Cut = 20.;

  // Loop over the tracks
  for(auto track : *inputTracks)
    {
      // If there ain't no track, don't take a track ...
      if(!track)
	continue;

      std::cout << track->pt() << std::endl;
      if(track->pt()/(1.e-3) < m_trackPtMin) continue;

      selectedTracks->push_back(track);
    }

  // Get all possible opposite-sign pairs of tracks
  std::vector< std::pair<const xAOD::TrackParticle*, const xAOD::TrackParticle*> > pairs;
  if(selectedTracks->size()>=2)
    {
      xAOD::TrackParticleContainer::const_iterator outerItr;
      xAOD::TrackParticleContainer::const_iterator innerItr;
      
      for(outerItr=selectedTracks->begin();outerItr<selectedTracks->end();outerItr++)
	{
	  for(innerItr=(outerItr+1);innerItr!=selectedTracks->end();innerItr++)
	    {           
	      std::pair<const xAOD::TrackParticle*, const xAOD::TrackParticle*> pair(*innerItr, *outerItr);
	      
	      // Ensure opposite sign pairs
	      if(! (pair.first->charge()==(-1)*pair.second->charge()) ) continue;

	      // Invariant mass pre-selection
	      TLorentzVector pairVec, v1, v2;
	      pairVec.Clear();
	      v1.Clear();
	      v2.Clear();
	      v1.SetPtEtaPhiE(pair.first->pt(), pair.first->eta(), pair.first->phi(), pair.first->e());
	      v2.SetPtEtaPhiE(pair.second->pt(), pair.second->eta(), pair.second->phi(), pair.second->e());
	      pairVec+=v1;
	      pairVec+=v2;

	      if((pairVec.M() < m_invMassMin) || (pairVec.M() > m_invMassMax)) continue;
	      	      
	      std::cout << pairVec.M() << std::endl;

	      // fit to vertex, check chi2

	      std::vector<const xAOD::TrackParticle*> theTracks; theTracks.clear();
	      theTracks.push_back(pair.first);
	      theTracks.push_back(pair.second);
	      xAOD::Vertex* myVxCandidate = FitVertex(theTracks, selectedTracks);

	      if (myVxCandidate != 0) {
                // Chi2 cut if requested
                double chi2 = myVxCandidate->chiSquared();
		std::cout << "chi2 is: " << chi2 << std::endl;
                if (m_Chi2Cut == 0.0 || chi2 <= m_Chi2Cut) { 
		  std::cout << "\t refit this one " << std::endl;
		}

	      // OK, we Gucci
	      pairs.push_back(pair);

	    }
	}
    }

  std::cout << pairs.size() << " lambda candidates"<< std::endl;
  
  //ANA_MSG_DEBUG("execute()\tnTracks out = \t" << selectedTracks->size());  
  std::cout << "execute()\tnTracks out = \t" << selectedTracks->size() << std::endl;

  m_store->record( selectedTracks, m_outputTrackContainer );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode SecondaryVertexFinderAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode SecondaryVertexFinderAlgo :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode SecondaryVertexFinderAlgo :: histFinalize () { return EL::StatusCode::SUCCESS; }

 xAOD::Vertex*  SecondaryVertexFinderAlgo :: FitVertex (std::vector<const xAOD::TrackParticle*> inputTracks,
							const xAOD::TrackParticleContainer* tracks) 
 { 
  Trk::TrkV0VertexFitter* concreteVertexFitter=0;

  // making a concrete fitter for the V0Fitter
  concreteVertexFitter = dynamic_cast<Trk::TrkV0VertexFitter * >(&(*m_iV0VertexFitter));
  if(concreteVertexFitter == 0) {
    ATH_MSG_FATAL("The vertex fitter passed is not a V0 Vertex Fitter");
    return NULL;
  }
  
  const Trk::Perigee& aPerigee1 = inputTracks[0]->perigeeParameters();
  const Trk::Perigee& aPerigee2 = inputTracks[1]->perigeeParameters();
  int sflag = 0;
  int errorcode = 0;   
  Amg::Vector3D startingPoint = m_vertexEstimator->getCirclesIntersectionPoint(&aPerigee1,&aPerigee2,sflag,errorcode);
  if (errorcode != 0) {startingPoint(0) = 0.0; startingPoint(1) = 0.0; startingPoint(2) = 0.0;}

  xAOD::Vertex* myVxCandidate = concreteVertexFitter->fit(inputTracks, startingPoint);
  if(myVxCandidate != 0)
    {
      for(unsigned int i=0; i< myVxCandidate->trackParticleLinks().size(); i++)
	{ 
	  ElementLink<DataVector<xAOD::TrackParticle> > mylink=myVxCandidate->trackParticleLinks()[i]; //makes a copy (non-const) 
	  mylink.setStorableObject(*importedTrackCollection, true); 
	  newLinkVector.push_back( mylink ); 
	}            
      myVxCandidate->clearTracks();
      myVxCandidate->setTrackParticleLinks( newLinkVector );
    }


  return myVxCandidate;
}
