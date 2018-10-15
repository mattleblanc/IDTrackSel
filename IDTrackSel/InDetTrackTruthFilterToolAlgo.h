#ifndef IDTrackSel_InDetTrackTruthFilterToolAlgo_H
#define IDTrackSel_InDetTrackTruthFilterToolAlgo_H

#include <EventLoop/Algorithm.h>

#include <xAODRootAccess/Init.h>
#include <xAODRootAccess/TEvent.h>
#include <xAODRootAccess/TStore.h>

#include "InDetTrackSystematicsTools/InDetTrackTruthFilterTool.h"
#include "xAODTracking/TrackParticleContainer.h"

class InDetTrackTruthFilterToolAlgo : public EL::Algorithm
{
public:
  std::string m_name = "MattLeBlanc";
  std::string m_inputTrackContainer;
  std::string m_outputTrackContainer;
  
  Bool_t m_debug;
    
 private:
  xAOD::TEvent *m_event; //!
  xAOD::TStore *m_store; //!

  InDet::InDetTrackTruthFilterTool* m_InDetTrackTruthFilterTool; //!

 public:
  // this is a standard constructor
  InDetTrackTruthFilterToolAlgo ();

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
  ClassDef(InDetTrackTruthFilterToolAlgo, 1);
};

#endif
