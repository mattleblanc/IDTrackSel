#ifndef IDTrackSel_JetTrackFilterToolAlgo_H
#define IDTrackSel_JetTrackFilterToolAlgo_H

#include <EventLoop/Algorithm.h>

#include <xAODRootAccess/Init.h>
#include <xAODRootAccess/TEvent.h>
#include <xAODRootAccess/TStore.h>

#include "InDetTrackSystematicsTools/JetTrackFilterTool.h"
#include "xAODTracking/TrackParticleContainer.h"

class JetTrackFilterToolAlgo : public EL::Algorithm
{
public:
  std::string m_name = "MattLeBlanc";
  std::string m_JetContainerName = "AntiKt10LCTopoJets";
  std::string m_inputTrackContainer;
  std::string m_outputTrackContainer;

  // put the tool parameters here

  Bool_t m_debug;
    
 private:
  xAOD::TEvent *m_event; //!
  xAOD::TStore *m_store; //!

  InDet::JetTrackFilterTool* m_JetTrackFilterTool; //!

  CP::SystematicSet m_systSetJet = {
    InDet::TrackSystematicMap[InDet::TRK_EFF_LOOSE_TIDE],
    InDet::TrackSystematicMap[InDet::TRK_FAKE_RATE_LOOSE_TIDE]
  };
  
 public:
  // this is a standard constructor
  JetTrackFilterToolAlgo ();

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
  ClassDef(JetTrackFilterToolAlgo, 1);
};

#endif
