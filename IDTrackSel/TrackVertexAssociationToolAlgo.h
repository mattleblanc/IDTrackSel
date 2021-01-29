#ifndef IDTrackSel_TrackVertexAssociationToolAlgo_H
#define IDTrackSel_TrackVertexAssociationToolAlgo_H

#include <EventLoop/Algorithm.h>

#include <xAODRootAccess/Init.h>
#include <xAODRootAccess/TEvent.h>
#include <xAODRootAccess/TStore.h>

#include "TrackVertexAssociationTool/TrackVertexAssociationTool.h"

#include "xAODTracking/VertexContainer.h"
#include "xAODTracking/TrackParticleContainer.h"

class TrackVertexAssociationToolAlgo : public EL::Algorithm
{
 public:
  std::string m_name = "MattLeBlanc";
  std::string m_inputTrackContainer;
  std::string m_outputTrackContainer;
  std::string m_workingPoint;

  Bool_t m_debug=false;

 private:
  xAOD::TEvent *m_event; //!
  xAOD::TStore *m_store; //!

  CP::TrackVertexAssociationTool* m_TrktoVxTool; //!  

 public:
  // this is a standard constructor
  TrackVertexAssociationToolAlgo ();
  
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
  ClassDef(TrackVertexAssociationToolAlgo, 1);
};

#endif
