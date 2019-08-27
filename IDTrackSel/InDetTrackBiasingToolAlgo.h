#ifndef IDTrackSel_InDetTrackBiasingToolAlgo_H
#define IDTrackSel_InDetTrackBiasingToolAlgo_H

#include <EventLoop/Algorithm.h>

#include <xAODRootAccess/Init.h>
#include <xAODRootAccess/TEvent.h>
#include <xAODRootAccess/TStore.h>

#include "InDetTrackSystematicsTools/InDetTrackBiasingTool.h"
#include "xAODTracking/TrackParticleContainer.h"

class InDetTrackBiasingToolAlgo : public EL::Algorithm
{
public:
  std::string m_name = "MattLeBlanc";
  std::string m_inputTrackContainer;
  std::string m_outputTrackContainer;
  Bool_t m_isData = false;
  
  // tool parameters go here

  Bool_t m_debug;
    
 private:
  xAOD::TEvent *m_event; //!
  xAOD::TStore *m_store; //!

  InDet::InDetTrackBiasingTool* m_InDetTrackBiasingTool; //!

  CP::SystematicSet m_systSetTrkWeak = {}; //!
  CP::SystematicSet m_systSetTrkWeak2 = {
    InDet::TrackSystematicMap[InDet::TRK_BIAS_QOVERP_SAGITTA_WM]
  }; //!


 public:
  // this is a standard constructor
  InDetTrackBiasingToolAlgo ();

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

  // this is needed to distribute the algorithm to the workers
  ClassDef(InDetTrackBiasingToolAlgo, 1);
};

#endif
