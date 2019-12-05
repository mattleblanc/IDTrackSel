'''
xAH_LundAnalysis.py
An xAODAnaHelpers config to run studies on the jet Lund Plane, for HCW 2018.
Matt LeBlanc (Arizona), matt.leblanc@cern.ch

Cluster-level systematics tool implemented by J. Roloff (Harvard)
(link)

Original Lund Plane code from F. Dreyer
https://github.com/rappoccio/fastjet-tutorial
'''

from xAODAnaHelpers import Config
c = Config()

#c.output("ANALYSIS")
#c.setalg("MessagePrinterAlgo", {"m_sourceWidth": 60})

c.algorithm("BasicEventSelection", {"m_name": "BasicEventSelector",
                                    "m_isMC" : True,
                                    "m_truthLevelOnly": False,
                                    "m_doPUreweighting": False,
                                    "m_doPUreweightingSys" : False,
                                    "m_autoconfigPRW" : False,
                                    "m_periodConfig" : "auto",
                                    "m_vertexContainerName": "PrimaryVertices",
                                    "m_PVNTrack": 2,
                                    "m_applyGRLCut": False,
                                    "m_GRLxml": "GoodRunsLists/data15_13TeV/20170619/data15_13TeV.periodAllYear_DetStatus-v89-pro21-02_Unknown_PHYS_StandardGRL_All_Good_25ns.xml,GoodRunsLists/data16_13TeV/20180129/data16_13TeV.periodAllYear_DetStatus-v89-pro21-01_DQDefects-00-02-04_PHYS_StandardGRL_All_Good_25ns.xml,GoodRunsLists/data17_13TeV/20180619/data17_13TeV.periodAllYear_DetStatus-v99-pro22-01_Unknown_PHYS_StandardGRL_All_Good_25ns_Triggerno17e33prim.xml,GoodRunsLists/data18_13TeV/20180906/data18_13TeV.periodAllYear_DetStatus-v102-pro22-03_Unknown_PHYS_StandardGRL_All_Good_25ns_Triggerno17e33prim.xml",
                                    "m_lumiCalcFileNames" : "GoodRunsLists/data18_13TeV/20180906/physics_25ns_Triggerno17e33prim.lumicalc.OflLumi-13TeV-001.root,GoodRunsLists/data17_13TeV/20180619/physics_25ns_Triggerno17e33prim.lumicalc.OflLumi-13TeV-010.root,GoodRunsLists/data16_13TeV/20180129/PHYS_StandardGRL_All_Good_25ns_297730-311481_OflLumi-13TeV-009.root,GoodRunsLists/data15_13TeV/20170619/PHYS_StandardGRL_All_Good_25ns_276262-284484_OflLumi-13TeV-008.root",
                                    #"m_prwActualMu2017File" : "GoodRunsLists/data17_13TeV/20180619/physics_25ns_Triggerno17e33prim.actualMu.OflLumi-13TeV-010.root",
                                    #"m_prwActualMu2018File" : "GoodRunsLists/data18_13TeV/20190318/physics_25ns_Triggerno17e33prim.actualMu.OflLumi-13TeV-010.root",
                                    "m_useMetaData": False,
                                    "m_triggerSelection": "HLT_j360|HLT_j340|HLT_j380|HLT_j400|HLT_j420|HLT_j450|HLT_j420",
                                    "m_applyTriggerCut": False,
                                    "m_applyEventCleaningCut" : False,
                                    "m_applyCoreFlagsCut"     : True,
                                    "m_applyJetCleaningEventFlag" : True, # This does the event-level jet cleaning.
                                 })

"""
c.setalg("JetCalibrator", { "m_name"                  : "SmallJetCalibratorEMPFlow",
                            "m_inContainerName"       : "AntiKt4EMPFlowJets",
                            "m_outContainerName"      : "AntiKt4EMPFlowJetsCalib",
                            "m_jetAlgo"               : "AntiKt4EMPFlow",
                            "m_outputAlgo"            : "AntiKt4EMPFlow_Calib_Algo",
                            "m_calibConfigFullSim"    : "JES_MC16Recommendation_Consolidated_PFlow_Apr2019_Rel21.config ",
                            "m_calibConfigData"       : "JES_MC16Recommendation_Consolidated_PFlow_Apr2019_Rel21.config ",
                            "m_uncertConfig"          : "rel21/Fall2018/R4_GlobalReduction_SimpleJER.config ",
                            "m_calibSequence"         : "JetArea_Residual_EtaJES_GSC_Smear",
                            "m_pseudoData"            : False,
                            #"m_overrideAnalysisFile"  : "DijetFlavourComp_Run2.root", # if you want to use a custom flavour composition file
                            "m_overrideCalibArea"     : "00-04-82",
                            "m_doCleaning"            : False,
                            "m_systVal"               : 1,
                            "m_systName"              : "All",
                            "m_sort"                  : True                            
                            })

c.algorithm("JetSelector",{"m_name"             : "JetSelectorAlgoEMPFlow",
                           "m_inContainerName"  : "AntiKt4EMPFlowJetsCalib",
                           "m_outContainerName" : "AntiKt4EMPFlowJetsCalibSel",
                           "m_createSelectedContainer" : True,
                           "m_pT_min"           : 25.e3,
                           "m_eta_max"          : 4.9,
                           "m_doJVT"                   :  True,
                           "m_pt_max_JVT"              :  60e3,
                           "m_eta_max_JVT"             :  2.4,
                           "m_WorkingPointJVT" : "Medium"
                           })

c.algorithm("JetReclusteringAlgo", {"m_name"             : "JetReclusteringEMPFlow",
                                    #"m_inputJetContainer" : "AntiKt4EMPFlowJetsCalibSel",
                                    "m_inputJetContainer" : "AntiKt2LCTopoJets",
                                    #"m_outputJetContainer" : "AntiKt08EMPFlowRCJets",                                    
                                    "m_outputJetContainer" : "AntiKt8LCTopoRCJets",
                                    "m_rc_alg" : "AntiKt",  
                                    "m_radius" : 0.8,
                                    "m_ptMin_input" : 25.0, # pt cut on input R=0.4 jets
                                    "m_ptMin_rc" : 100.0,   # RC jet pt cut     
                                    "m_ptFrac" : 0.0       # don't groom the RC jets!                                
                                })

c.algorithm("JetHistsAlgo", { "m_name" : "JetHistsAntiKt8LCTopoRCJets",
                              "m_inContainerName"   : "AntiKt8LCTopoRCJets",
                              "m_detailStr": "kinematic substructure"
                          } )

"""

c.algorithm("InDetTrackSelectionToolAlgo", {
    "m_name": "InDetTrackSelectionToolAlgo_TAR",
    "m_inputTrackContainer": "InDetTrackParticles",
    "m_outputTrackContainer": "SelectedTrackParticles_InDetTrackParticles",
    "m_CutLevel": "Loose"
})

c.algorithm("TARJetAlgo", {
    #"m_inputJetContainer" : "AntiKt8LCTopoRCJets",
    "m_inputJetContainer" : "AntiKt2LCTopoJets",
    "m_inputTrackContainer" : "SelectedTrackParticles_InDetTrackParticles",
    "m_outputTrackContainer" : "OutTracks",
    "m_assocTracksOutName" : "TARTracks",
    "m_dRmatch" : 0.3,
    "m_assocTracksInName" : "GhostTrack",
    "m_vertexColl" : "PrimaryVertices",
    "m_trackVtxAssoc" : "",
    "m_doTrackVtxAssoc" : True,
    #"m_trackSelTool" : ""
})

