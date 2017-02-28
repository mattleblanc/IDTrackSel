#ifndef IDTrackSel_TightTrackVertexAssociationToolAlgo_H
#define IDTrackSel_TightTrackVertexAssociationToolAlgo_H

#include <EventLoop/Algorithm.h>

#include <xAODRootAccess/Init.h>
#include <xAODRootAccess/TEvent.h>
#include <xAODRootAccess/TStore.h>

#include "TrackVertexAssociationTool/TightTrackVertexAssociationTool.h"

//#include "xAODEventInfo/EventInfo.h"
#include "xAODTracking/VertexContainer.h"
#include "xAODTracking/TrackParticleContainer.h"

class TightTrackVertexAssociationToolAlgo : public EL::Algorithm
{
 public:
  std::string m_name = "MattLeBlanc";
  std::string m_inputTrackContainer;
  std::string m_outputTrackContainer;

  float m_dzSinTheta_cut=-999;
  float m_d0_cut=-999;
  bool m_Applyd0Selection=false;
  bool m_doPV=true;

  Bool_t m_debug=false;

 private:
  xAOD::TEvent *m_event; //!
  xAOD::TStore *m_store; //!

  CP::TightTrackVertexAssociationTool* m_TightTrktoVxTool; //!  

 public:
  // this is a standard constructor
  TightTrackVertexAssociationToolAlgo ();
  
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
  ClassDef(TightTrackVertexAssociationToolAlgo, 1);
};

#endif
