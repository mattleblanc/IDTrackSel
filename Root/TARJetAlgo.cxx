#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <IDTrackSel/TARJetAlgo.h>

#include <EventLoop/OutputStream.h>

// this is needed to distribute the algorithm to the workers
ClassImp(TARJetAlgo)

TARJetAlgo :: TARJetAlgo (){}

EL::StatusCode TARJetAlgo :: setupJob (EL::Job& job)
{
  ANA_CHECK_SET_TYPE (EL::StatusCode);
  job.useXAOD();
  ANA_CHECK(xAOD::Init()); // call before opening first file

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TARJetAlgo :: histInitialize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TARJetAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TARJetAlgo :: changeInput (bool firstFile) { return EL::StatusCode::SUCCESS; }

EL::StatusCode TARJetAlgo :: initialize ()
{
  std::cout << "initialize()" << std::endl;
  Info("initialize()", "TARJetAlgo_%s", m_inputTrackContainer.c_str() );
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  ANA_MSG_INFO("Initialise TAR Jet Tool ");

  m_TARDecoTool_handle.setTypeAndName("TARJetTool","TARJetTool");
  ANA_CHECK( m_TARDecoTool_handle.setProperty("InputTrackContainer", m_inputTrackContainer ) );
  ANA_CHECK( m_TARDecoTool_handle.setProperty("OutputTrackContainer",m_outputTrackContainer ) );
  ANA_CHECK( m_TARDecoTool_handle.setProperty("OutputAssociatedTracks",m_assocTracksOutName) );
  ANA_CHECK( m_TARDecoTool_handle.setProperty("MatchDeltaR",m_dRmatch) );
  ANA_CHECK( m_TARDecoTool_handle.setProperty("InputAssociatedTracks",m_assocTracksInName) );
  ANA_CHECK( m_TARDecoTool_handle.setProperty("VertexContainer",m_vertexColl) );
  ANA_CHECK( m_TARDecoTool_handle.setProperty("TrackVertexAssociation",m_trackVtxAssoc) );
  ANA_CHECK( m_TARDecoTool_handle.setProperty("DoTrackVertexAssociation",m_doTrackVtxAssoc) );
  //ANA_CHECK( m_TARDecoTool_handle.setProperty("TrackSelTool", ) );
  ANA_CHECK( m_TARDecoTool_handle.retrieve() );
 
  if (m_debug) ANA_MSG_INFO("NSubjettinessRatiosTool gets initialized");
  m_NSubjettinessRatiosTool_handle.setTypeAndName("NSubjettinessRatiosTool","NSubjettinessRatiosTool");    
  ANA_CHECK(m_NSubjettinessRatiosTool_handle.setProperty("inputContainer", "TARTracks"));
  ANA_CHECK(m_NSubjettinessRatiosTool_handle.setProperty("Prefix", "TAR_"));       
  ANA_CHECK(m_NSubjettinessRatiosTool_handle.initialize());

  if (m_debug) ANA_MSG_INFO("Jet reclustering tool gets initialized");
  m_jetReclTool_handle.setTypeAndName("JetReclusteringTool","JetReclusteringTool");
  //ANA_CHECK(ASG_MAKE_ANA_TOOL(jetReclTool, JetReclusteringTool));
  //ANA_CHECK(m_jetReclTool_handle.setProperty("InputJetContainer",  m_inputJetContainer));
  ANA_CHECK(m_jetReclTool_handle.setProperty("InputJetContainer","ShallowInput"+m_inputJetContainer+"selected"));
  ANA_CHECK(m_jetReclTool_handle.setProperty("OutputJetContainer", "RC"+m_inputJetContainer));
  ANA_CHECK(m_jetReclTool_handle.retrieve());

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TARJetAlgo :: execute ()
{
  std::cout << "execute()" << std::endl;

  m_debug = false;

  std::cout << "getting tracks" << std::endl;
  // Retrieve the input TrackParticleContainer
  const xAOD::TrackParticleContainer_v1* inputTracks  = 0;
  if(!m_event->retrieve(inputTracks, m_inputTrackContainer).isSuccess()){
    Error("execute()","Failed to retrieve input track container.\tEject!");
    return EL::StatusCode::FAILURE;
  }

  std::cout << "getting jets" << std::endl;
  // Retrieve the input JetContainer
  const xAOD::JetContainer* Jets=0;
  if(!m_event->retrieve(Jets, m_inputJetContainer).isSuccess()){
    Error("execute()","Failed to retrieve input jet container.\tEject!");
    return EL::StatusCode::FAILURE;
  }

  std::cout << "making a view container for some reason" << std::endl;
  TString selectedJetsName = "ShallowInput" + m_inputJetContainer + "selected";
  store->record( Jets , selectedJetsName.Data());

  //  //m_jetReclTool_handle->get()->execute();
  std::cout << "reclustering jets" << std::endl;
  m_jetReclTool_handle->execute();

  std::cout << "retrieving reclustered jets" << std::endl;
  const xAOD::JetContainer * constParticles;
  m_event->retrieve(constParticles, "RC"+m_inputJetContainer);

  // Create a shallow copy of the current jet container
  std::cout << "doing the shallow copy thing" << std::endl;
  TString shallowCopyName = "ShallowInputTest";//+m_outputJets;
  std::pair<xAOD::JetContainer*, xAOD::ShallowAuxContainer *> shallowCopyCont = xAOD::shallowCopyContainer(*constParticles);
  
  //xAOD::JetContainer* shallowCopyCont = nullptr;

  /*
  ANA_CHECK( CP::detail::ShallowCopy< xAOD::JetContainer >::getCopy( HelperFunctions::msg(),
								     m_store,
								     shallowCopyCont,
								     Jets, 
								      m_inputJetContainer+"Copy",
								      m_inputJetContainer+"CopyAux." ) );
  */
  
  //build TAR jets 
  std::cout << "decorate reclustered jet with TAR information" << std::endl;
  m_TARDecoTool_handle->modify(*shallowCopyCont.first);

  // Create the output TrackParticleContainer
  std::cout << "creating output track container" << std::endl;
  ConstDataVector<xAOD::TrackParticleContainer>* selectedTracks(nullptr);
  selectedTracks = new ConstDataVector<xAOD::TrackParticleContainer>(SG::VIEW_ELEMENTS);

  for (xAOD::Jet *jet : *shallowCopyCont.first) {
    //Props::sysName.set(jet, "nominal");
    ANA_CHECK(decorate(jet));
  }

  ANA_CHECK(m_store->record( shallowCopyCont.first , (shallowCopyName).Data()));
  ANA_CHECK(m_store->record( shallowCopyCont.second, (shallowCopyName + "Aux.").Data()));  

  m_store->record( selectedTracks, m_outputTrackContainer );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TARJetAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TARJetAlgo :: finalize () { return EL::StatusCode::SUCCESS; }

EL::StatusCode TARJetAlgo :: histFinalize () { return EL::StatusCode::SUCCESS; }

/*
MsgStream& TARJetAlgo :: msg () const { return m_msg; }
MsgStream& TARJetAlgo :: msg (int level) const {
  MsgStream& result = msg();
  result << MSG::Level (level);
  return result;
}
*/

EL::StatusCode TARJetAlgo::decorate(xAOD::Jet * jet)
{
  if(m_debug) ANA_MSG_INFO("TARJetHandler::decorate()");
  
  //Retrive NSubjettiness tools
  if(m_debug) ANA_MSG_INFO("Retrieve NSubjettinessRatiosTool");
  //asg::AnaToolHandle<NSubjettinessRatiosTool> tauRTool_handle = m_NSubjettinessRatiosTool_handle;
  
  //calculate jet substructure
  if(m_debug) ANA_MSG_INFO("Calculate NSubjettinessRatios");
  m_NSubjettinessRatiosTool_handle->modifyJet(*jet);
  
  //save substructure, mass and rescaled tracks in properties
  float TAR_Tau21, TAR_Tau32, TAR_Tau42;
  float TAR_Tau21_wta, TAR_Tau32_wta, TAR_Tau42_wta;
  float mTAR;
  
  std::vector<float> RSTracksPt, RSTracksEta, RSTracksPhi, RSTracksM; 
  RSTracksPt.clear(); RSTracksEta.clear(); RSTracksPhi.clear(); RSTracksM.clear();
  
  if(m_debug) ANA_MSG_INFO("Retrieve NSubjettiness ratios");
  if(!jet -> getAttribute<float>("TAR_Tau21", TAR_Tau21)) TAR_Tau21=-999.;
  if(!jet -> getAttribute<float>("TAR_Tau32", TAR_Tau32)) TAR_Tau32=-999.;
  if(!jet -> getAttribute<float>("TAR_Tau42", TAR_Tau42)) TAR_Tau42=-999.;
  if(!jet -> getAttribute<float>("TAR_Tau21_wta", TAR_Tau21_wta)) TAR_Tau21_wta=-999.;
  if(!jet -> getAttribute<float>("TAR_Tau32_wta", TAR_Tau32_wta)) TAR_Tau32_wta=-999.;
  if(!jet -> getAttribute<float>("TAR_Tau42_wta", TAR_Tau42_wta)) TAR_Tau42_wta=-999.;

  if(m_debug) ANA_MSG_INFO("Retrieve TAR mass");
  if(!jet ->getAttribute<float>("mTAR", mTAR)) mTAR=-999.;
  
  if(m_debug) ANA_MSG_INFO("Retrieve rescaled tracks");
  std::string RStracksName="TARTracks";
  std::vector<const xAOD::TrackParticle*> RStracks;
  if (!jet ->getAssociatedObjects< xAOD::TrackParticle >(RStracksName,RStracks)) 
    {
      ANA_MSG_INFO("Could not retrieve associated rescaled tracks");
    }
  else {
    for (auto track : RStracks) 
      {
	RSTracksPt.push_back(track->pt());
	RSTracksEta.push_back(track->eta());
	RSTracksPhi.push_back(track->phi());
	RSTracksM.push_back(track->m());
      }
  }  
  
  if(m_debug) ANA_MSG_INFO("Fill properties");
  /*
  Props::TAR_Tau21.set(jet, TAR_Tau21);
  Props::TAR_Tau32.set(jet, TAR_Tau32);
  Props::TAR_Tau42.set(jet, TAR_Tau42);
  Props::TAR_Tau21_wta.set(jet, TAR_Tau21_wta);
  Props::TAR_Tau32_wta.set(jet, TAR_Tau32_wta);
  Props::TAR_Tau42_wta.set(jet, TAR_Tau42_wta);
  
  Props::mTAR.set(jet, mTAR);
  Props::RSTracksPt.set(jet,RSTracksPt);
  Props::RSTracksEta.set(jet,RSTracksEta);
  Props::RSTracksPhi.set(jet,RSTracksPhi);
  Props::RSTracksM.set(jet,RSTracksM);
  */
  static SG::AuxElement::Decorator<float>  decor_TAR_Tau21("TAR_Tau21");
  static SG::AuxElement::Decorator<float>  decor_TAR_Tau32("TAR_Tau32");
  static SG::AuxElement::Decorator<float>  decor_TAR_Tau42("TAR_Tau42");
  static SG::AuxElement::Decorator<float>  decor_TAR_Tau21_wta("TAR_Tau21_wta");
  static SG::AuxElement::Decorator<float>  decor_TAR_Tau32_wta("TAR_Tau32_wta");
  static SG::AuxElement::Decorator<float>  decor_TAR_Tau42_wta("TAR_Tau42_wta");
  static SG::AuxElement::Decorator<float>  decor_mTAR("mTAR");

  decor_TAR_Tau21(*jet) = TAR_Tau21;
  decor_TAR_Tau32(*jet) = TAR_Tau32;
  decor_TAR_Tau42(*jet) = TAR_Tau42;
  decor_TAR_Tau21_wta(*jet) = TAR_Tau21_wta;
  decor_TAR_Tau32_wta(*jet) = TAR_Tau32_wta;
  decor_TAR_Tau42_wta(*jet) = TAR_Tau42_wta;
  decor_mTAR(*jet) = mTAR;
  
  return EL::StatusCode::SUCCESS;
}//end of decorate
