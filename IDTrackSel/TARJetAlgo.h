#ifndef IDTrackSel_TARJetAlgo_H
#define IDTrackSel_TARJetAlgo_H

#include <EventLoop/Algorithm.h>

#include <xAODRootAccess/Init.h>
#include <xAODRootAccess/TEvent.h>
#include <xAODRootAccess/TStore.h>

#include "AsgTools/AnaToolHandle.h"
//#include "SystematicsHandles/CopyHelpers.h"
//#include <AsgTools/MessageCheck.h>
#include "JetInterface/IJetModifier.h"
#include "JetRecTools/TARJetTool.h"

#include <JetSubStructureMomentTools/NSubjettinessRatiosTool.h>
#include <JetSubStructureMomentTools/NSubjettinessTool.h>
#include "JetReclustering/JetReclusteringTool.h"

#include "xAODTracking/TrackParticleContainer.h"

#include "xAODAnaHelpers/HelperFunctions.h"

class MsgStream;

class TARJetAlgo : public EL::Algorithm
{
 protected:
  /** @brief hold the MsgStream object */
  //mutable MsgStream m_msg; //!
  //virtual EL::StatusCode decorate(xAOD::Jet * jet);// override;

 public:
  std::string m_name = "MattLeBlanc";

  /*
  std::string m_inputJetContainer;
  std::string m_inputTrackContainer = "InDetTrackParticles";
  std::string m_outputTrackContainer = "TARTracksOutput";
  std::string m_assocTracksOutName = "TARTracks";
  float m_dRmatch = 0.3;
  std::string m_assocTracksInName = "GhostTrack";
  std::string m_vertexColl = "PrimaryVertices";
  std::string m_trackVtxAssoc = "";
  bool m_doTrackVtxAssoc = true;
  */

  std::string m_inputJetContainer;
  
  std::string m_inTrackColl = "InDetTrackParticles";
  std::string m_inSelTrackColl; // Name of a view container to allow a track preselection
  std::string m_outTrackColl;
  std::string m_assocTracksOutName = "TARTracks";
  std::string m_assocObjsOutName = "TARObjects";
  float m_dRmatch = 0.3;

  std::string m_assocTracksInName = "GhostTrack";
  std::string m_vertexColl = "PrimaryVertices";
  std::string m_trackVtxAssoc;
  bool m_doTrackVtxAssoc = true;
  std::string m_trackSelTool;

  Bool_t m_debug;
    
 private:
  xAOD::TEvent *m_event; //!
  xAOD::TStore *m_store; //!
  
  asg::AnaToolHandle<TARJetTool> m_TARJetTool_handle {"TARJetTool" , this}; //!
  //asg::AnaToolHandle<TARJetTool> m_TARJetTool_handle; //!

  //asg::AnaToolHandle<NSubjettinessRatiosTool> m_NSubjettinessRatiosTool_handle; //!
  //asg::AnaToolHandle<NSubjettinessTool> m_NSubjettinessTool_handle; //!
  //asg::AnaToolHandle<IJetModifier> m_TARDecoTool_handle; //!
  //asg::AnaToolHandle<IJetExecuteTool> m_jetReclTool_handle; //!

 public:
  // this is a standard constructor
  TARJetAlgo ();

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  /**
   * @brief the standard message stream for this algorithm
   */
  //MsgStream& msg () const;
  /**
   * @brief allow ANA_MSG_XXXX macros to be used within algorithms for a given level
   */
  //MsgStream& msg (int level) const;

  // this is needed to distribute the algorithm to the workers
  ClassDef(TARJetAlgo, 1);
};

#endif
