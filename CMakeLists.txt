# Declare the package name:
atlas_subdir( IDTrackSel )

# Declare the package's dependencies:
atlas_depends_on_subdirs( PUBLIC
                          xAODAnaHelpers
                          Control/AthContainers
                          Control/AthToolSupport/AsgTools
                          Control/xAODRootAccess
                          DataQuality/GoodRunsLists
                          Event/xAOD/xAODBase
                          Event/xAOD/xAODCaloEvent
                          Event/xAOD/xAODCutFlow
                          Event/xAOD/xAODEgamma
                          Event/xAOD/xAODEventInfo
                          Event/xAOD/xAODJet
                          Event/xAOD/xAODMetaDataCnv
                          Event/xAOD/xAODMissingET
                          Event/xAOD/xAODMuon
                          Event/xAOD/xAODTau
                          Event/xAOD/xAODTracking
                          Event/xAOD/xAODTrigger
                          Event/xAOD/xAODTriggerCnv
                          Event/xAOD/xAODTruth
                          PhysicsAnalysis/D3PDTools/EventLoop
                          PhysicsAnalysis/D3PDTools/EventLoopGrid
                          InnerDetector/InDetRecTools/InDetTrackSelectionTool
			  InnerDetector/InDetRecTools/TrackVertexAssociationTool
			  PhysicsAnalysis/TrackingID/InDetTrackSystematicsTools
                          PhysicsAnalysis/AnalysisCommon/AssociationUtils
                          PhysicsAnalysis/AnalysisCommon/IsolationSelection
                          PhysicsAnalysis/AnalysisCommon/PATInterfaces
                          PhysicsAnalysis/AnalysisCommon/PMGTools
                          PhysicsAnalysis/AnalysisCommon/PileupReweighting
                          PhysicsAnalysis/D3PDTools/RootCoreUtils
                          PhysicsAnalysis/D3PDTools/SampleHandler
                          PhysicsAnalysis/ElectronPhotonID/ElectronEfficiencyCorrection
                          PhysicsAnalysis/ElectronPhotonID/ElectronPhotonFourMomentumCorrection
                          PhysicsAnalysis/ElectronPhotonID/ElectronPhotonSelectorTools
                          PhysicsAnalysis/ElectronPhotonID/IsolationCorrections
                          PhysicsAnalysis/ElectronPhotonID/ElectronPhotonShowerShapeFudgeTool
                          PhysicsAnalysis/ElectronPhotonID/PhotonEfficiencyCorrection
                          PhysicsAnalysis/JetMissingEtID/JetSelectorTools
                          PhysicsAnalysis/JetTagging/JetTagPerformanceCalibration/xAODBTaggingEfficiency
                          PhysicsAnalysis/MuonID/MuonIDAnalysis/MuonMomentumCorrections
                          PhysicsAnalysis/MuonID/MuonIDAnalysis/MuonEfficiencyCorrections
                          PhysicsAnalysis/MuonID/MuonSelectorTools
                          PhysicsAnalysis/TauID/TauAnalysisTools
                          Reconstruction/Jet/JetAnalysisTools/JetTileCorrection
                          Reconstruction/Jet/JetCalibTools
                          Reconstruction/Jet/JetJvtEfficiency
                          Reconstruction/Jet/JetMomentTools
                          Reconstruction/Jet/JetResolution
                          Reconstruction/Jet/JetSubStructureUtils
			  Reconstruction/Jet/JetSubStructureMomentTools
                          Reconstruction/Jet/JetUncertainties
			  Reconstruction/Jet/JetRecTools			  
                          Reconstruction/Jet/JetRec
			  Reconstruction/MET/METInterface
                          Reconstruction/MET/METUtilities
                          Tools/PathResolver
                          Trigger/TrigAnalysis/TrigDecisionTool
                          Trigger/TrigAnalysis/TriggerMatchingTool
                          Trigger/TrigConfiguration/TrigConfxAOD
)

# Find the needed external(s):
find_package( ROOT COMPONENTS Core RIO Hist Tree )

# Component(s) in the package:
atlas_add_root_dictionary (IDTrackSelLib
   IDTrackSelDictSource
   ROOT_HEADERS IDTrackSel/*.h Root/LinkDef.h
   EXTERNAL_PACKAGES ROOT )

# add the library itself:
atlas_add_library (IDTrackSelLib
   IDTrackSel/*.h IDTrackSel/*.ihh Root/*.cxx ${IDTrackSelDictSource}
   PUBLIC_HEADERS IDTrackSel
   INCLUDE_DIRS
   LINK_LIBRARIES EventLoop InDetTrackSystematicsToolsLib InDetTrackSelectionToolLib TrackVertexAssociationToolLib xAODAnaHelpersLib
   PRIVATE_LINK_LIBRARIES )
