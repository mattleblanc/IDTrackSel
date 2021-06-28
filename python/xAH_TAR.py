'''
xAH_TAR.py
An xAODAnaHelpers config to run studies on TAR jets.
Matt LeBlanc (Arizona), matt.leblanc@cern.ch, 2019
'''

from xAODAnaHelpers import Config
c = Config()

#c.output("ANALYSIS")
#c.algorithm("MessagePrinterAlgo", {"m_sourceWidth": 60})

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

c.algorithm("JetCalibrator", { "m_name"                  : "JetCalibrator4PF",
                               "m_inContainerName"       : "AntiKt4EMPFlowJets",
                               "m_outContainerName"      : "AntiKt4EMPFlowJetsCalib",
                               "m_jetAlgo"               : "AntiKt4EMPFlow",
                               "m_outputAlgo"            : "AntiKt4EMPFlow_Calib_Algo",
                               "m_calibConfigFullSim"    : "JES_MC16Recommendation_Consolidated_PFlow_Apr2019_Rel21.config ",
                               "m_uncertConfig"          : "rel21/Fall2018/R4_GlobalReduction_SimpleJER.config ",
                               "m_calibSequence"         : "JetArea_Residual_EtaJES_GSC_Smear",
                               "m_pseudoData"            : False,
                               "m_overrideCalibArea"     : "00-04-82",
                               "m_doCleaning"            : False,
                               "m_systVal"               : 1,
                               "m_systName"              : "nominal",
                               "m_sort"                  : True                            
                           })

c.algorithm("JetSelector",{"m_name"             : "JetSelector4PF",
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

c.algorithm("JetReclusteringAlgo", {"m_name"             : "JetReclustering4PF",
                                    "m_inputJetContainer" : "AntiKt4EMPFlowJetsCalibSel",
                                    "m_outputJetContainer" : "AntiKt10EMPFlowRC4Jets",
                                    "m_rc_alg" : "AntiKt",
                                    "m_radius" : 1.0,
                                    "m_ptMin_input" : 25.0, # pt cut on input jets
                                    "m_ptMin_rc" : 100.0,   # RC jet pt cut
                                    "m_trim_ptFrac" : 0.0       # don't groom the RC jets!
                                })

c.algorithm("TARJetAlgo", {
    "m_inputJetContainer" : "AntiKt10EMPFlowRC4Jets",
    "m_outputJetContainer" : "AntiKt10EMPFlowRC4TARJets",
    "m_inTrackColl" : "InDetTrackParticles",    
    "m_outTrackColl" : "OutTracksPFRC",
    "m_assocTracksOutName" : "TARTracksPFRC",
    "m_dRmatch" : 0.3,
    "m_assocTracksInName" : "GhostTrack",
    "m_vertexColl" : "PrimaryVertices",
    "m_trackVtxAssoc" : "",
    "m_doTrackVtxAssoc" : True,
    #"m_trackSelTool" : ""
})

# AntiKt4TruthJets
c.algorithm("JetSelector",{"m_name"             : "JetSelector4TR",
                           "m_inContainerName"  : "AntiKt4TruthJets",
                           "m_outContainerName" : "AntiKt4TruthJetsSel",
                           "m_createSelectedContainer" : True,
                           "m_pT_min"           : 25.e3,
                           "m_eta_max"          : 4.9,
                           "m_doJVT"                   :  False,
                           #"m_pt_max_JVT"              :  60e3,
                           #"m_eta_max_JVT"             :  2.4,
                           #"m_WorkingPointJVT" : "Medium"
                           })

c.algorithm("JetReclusteringAlgo", {"m_name"             : "JetReclustering4TR",
                                    "m_inputJetContainer" : "AntiKt4TruthJetsSel",
                                    "m_outputJetContainer" : "AntiKt10TruthRC4Jets",
                                    "m_rc_alg" : "AntiKt",
                                    "m_radius" : 1.0,
                                    "m_ptMin_input" : 25.0, # pt cut on input jets
                                    "m_ptMin_rc" : 100.0,   # RC jet pt cut
                                    "m_trim_ptFrac" : 0.0       # don't groom the RC jets!
                                })

# R=0.2 >> R=1.0 jets for TAR
c.algorithm("JetCalibrator", { "m_name"                  : "JetCalibrator2LC",
                               "m_inContainerName"       : "AntiKt2LCTopoJets",
                               "m_outContainerName"      : "AntiKt2LCTopoJetsCalib",
                               "m_jetAlgo"               : "AntiKt2LCTopo",
                               "m_outputAlgo"            : "AntiKt2LCTopo_Calib_Algo",
                               "m_calibConfigFullSim"    : "JES_2015_2016_data_Rscan2LC_18Dec2018_R21.config",
                               "m_calibSequence"         : "JetArea_Residual_EtaJES_GSC",
                               "m_overrideCalibArea"     : "00-04-82",
                               "m_doCleaning"            : False,
                               "m_systVal"               : 1,
                               "m_systName"              : "nominal",
                               "m_sort"                  : True
                           })

c.algorithm("JetSelector",{"m_name"             : "JetSelector2LC",
                           "m_inContainerName"  : "AntiKt2LCTopoJetsCalib",
                           "m_outContainerName" : "AntiKt2LCTopoJetsCalibSel",
                           "m_createSelectedContainer" : True,
                           "m_pT_min"           : 25.e3,
                           "m_eta_max"          : 4.9,
                           "m_doJVT"                   :  False,
                           #"m_pt_max_JVT"              :  60e3,
                           #"m_eta_max_JVT"             :  2.4,
                           #"m_WorkingPointJVT" : "Medium"
                           })

c.algorithm("JetReclusteringAlgo",  {"m_name"             : "JetReclustering2LC",
                                    "m_inputJetContainer" : "AntiKt2LCTopoJetsCalibSel",
                                    "m_outputJetContainer" : "AntiKt10LCTopoRC2Jets",
                                    "m_rc_alg" : "AntiKt",
                                    "m_radius" : 1.0,
                                    "m_ptMin_input" : 25.0, # pt cut on input jets
                                    "m_ptMin_rc" : 100.0,   # RC jet pt cut
                                    "m_trim_ptFrac" : 0.0       # don't groom the RC jets!
                                })

c.algorithm("TARJetAlgo", {
    "m_inputJetContainer" : "AntiKt10LCTopoRC2Jets",
    "m_outputJetContainer" : "AntiKt10LCTopoRC2TARJets",
    "m_inTrackColl" : "InDetTrackParticles",
    "m_outTrackColl" : "OutTracksLCRC",
    "m_assocTracksOutName" : "TARTracksLCRC",
    "m_dRmatch" : 0.3,
    "m_assocTracksInName" : "GhostTrack",
    "m_vertexColl" : "PrimaryVertices",
    "m_trackVtxAssoc" : "",
    "m_doTrackVtxAssoc" : True,
#"m_trackSelTool" : ""
})

c.algorithm("JetCalibrator", { "m_name"                  : "JetCalibrator10LC",
                               "m_inContainerName"       : "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets",
                               "m_outContainerName"      : "AntiKt10LCTopoTrimmedPtFrac5SmallR20JetsCalib",
                               "m_jetAlgo"               : "AntiKt10LCTopoTrimmedPtFrac5SmallR20",
                               "m_outputAlgo"            : "AntiKt10LCTopoTrimmedPtFrac5SmallR20_Calib_Algo",
                               "m_calibConfigFullSim"    : "JES_MC16recommendation_FatJet_Trimmed_JMS_comb_17Oct2018.config ",
                               "m_calibSequence"         : "EtaJES_JMS",
                               "m_overrideCalibArea"     : "00-04-82",
                               "m_doCleaning"            : False,
                               "m_systVal"               : 1,
                               "m_systName"              : "nominal",
                               "m_sort"                  : True,
                               "m_useLargeRTruthLabelingTool" : False
                           })

c.algorithm("JetSelector",{"m_name"             : "JetSelector10LC",
                           "m_inContainerName"  : "AntiKt10LCTopoTrimmedPtFrac5SmallR20JetsCalib",
                           "m_outContainerName" : "AntiKt10LCTopoTrimmedPtFrac5SmallR20JetsCalibSel",
                           "m_createSelectedContainer" : True,
                           "m_pT_min"           : 100.e3,
                           "m_eta_max"          : 2.0,
                           "m_doJVT"                   :  False,
                           #"m_pt_max_JVT"              :  60e3,
                           #"m_eta_max_JVT"             :  2.4,
                           #"m_WorkingPointJVT" : "Medium"
                           })


c.algorithm("JetCalibrator", { "m_name"                  : "JetCalibrator10UF",
                               "m_inContainerName"       : "AntiKt10UFOCSSKSoftDropBeta100Zcut10Jets",
                               "m_outContainerName"      : "AntiKt10UFOCSSKSoftDropBeta100Zcut10JetsCalib",
                               "m_jetAlgo"               : "AntiKt10UFOCSSKSoftDropBeta100Zcut10",
                               "m_outputAlgo"            : "AntiKt10UFOCSSKSoftDropBeta100Zcut10_Calib_Algo",
                               "m_calibConfigFullSim"    : "JES_MC16recommendation_R10_UFO_CSSK_SoftDrop_JMS_01April2020.config" ,
                               "m_calibSequence"         : "EtaJES_JMS",
                               "m_overrideCalibArea"     : "00-04-82",
                               "m_doCleaning"            : False,
                               "m_systVal"               : 1,
                               "m_systName"              : "nominal",
                               "m_sort"                  : True,
                               "m_useLargeRTruthLabelingTool" : False
                           })

c.algorithm("JetSelector",{"m_name"             : "JetSelector10UF",
                           "m_inContainerName"  : "AntiKt10UFOCSSKSoftDropBeta100Zcut10JetsCalib",
                           "m_outContainerName" : "AntiKt10UFOCSSKSoftDropBeta100Zcut10JetsCalibSel",
                           "m_createSelectedContainer" : True,
                           "m_pT_min"           : 100.e3,
                           "m_eta_max"          : 2.0,
                           "m_doJVT"                   :  False,
                           #"m_pt_max_JVT"              :  60e3,
                           #"m_eta_max_JVT"             :  2.4,
                           #"m_WorkingPointJVT" : "Medium"
                           })

c.algorithm("InDetTrackSelectionToolAlgo", {
    "m_name": "InDetTrackSelectionToolAlgo_TAR",
    "m_inputTrackContainer": "InDetTrackParticles",
    "m_outputTrackContainer": "SelectedTrackParticles_InDetTrackParticles",
    "m_CutLevel": "Loose"
})

c.algorithm("JetSelector",{"m_name"             : "JetSelector2TR",
                           "m_inContainerName"  : "AntiKt2TruthJets",
                           "m_outContainerName" : "AntiKt2TruthJetsSel",
                           "m_createSelectedContainer" : True,
                           "m_pT_min"           : 25.e3,
                           "m_eta_max"          : 4.9,
                           "m_doJVT"                   :  False,
                           #"m_pt_max_JVT"              :  60e3,
                           #"m_eta_max_JVT"             :  2.4,
                           #"m_WorkingPointJVT" : "Medium"
                           })

c.algorithm("JetReclusteringAlgo", {"m_name"             : "JetReclustering2TR",
                                    "m_inputJetContainer" : "AntiKt2TruthJetsSel",
                                    "m_outputJetContainer" : "AntiKt10TruthRC2Jets",
                                    "m_rc_alg" : "AntiKt",
                                    "m_radius" : 1.0,
                                    "m_ptMin_input" : 25.0, # pt cut on input jets
                                    "m_ptMin_rc" : 100.0,   # RC jet pt cut
                                    "m_trim_ptFrac" : 0.0       # don't groom the RC jets!
                                })

c.algorithm("TreeAlgo",{"m_name": "TreeAlgoMLB",
                        "m_fatJetContainerName": "AntiKt10LCTopoTrimmedPtFrac5SmallR20JetsCalibSel AntiKt10UFOCSSKSoftDropBeta100Zcut10JetsCalibSel AntiKt10EMPFlowRC4TARJets AntiKt10LCTopoRC2TARJets AntiKt10TruthSoftDropBeta100Zcut10Jets AntiKt10TruthTrimmedPtFrac5SmallR20Jets AntiKt10TruthRC4Jets AntiKt10TruthRC2Jets ",
                        "m_fatJetBranchName":    "AntiKt10LCTopoTrimmedPtFrac5SmallR20JetsCalibSel AntiKt10UFOCSSKSoftDropBeta100Zcut10JetsCalibSel AntiKt10EMPFlowRC4TARJets AntiKt10LCTopoRC2TARJets AntiKt10TruthSoftDropBeta100Zcut10Jets AntiKt10TruthTrimmedPtFrac5SmallR20Jets AntiKt10TruthRC4Jets AntiKt10TruthRC2Jets",
                        "m_fatJetDetailStr": "kinematic tar",

                        #"m_truthFatJetContainerName": "AntiKt10TruthSoftDropBeta100Zcut10Jets AntiKt10TruthTrimmedPtFrac5SmallR20Jets AntiKt10TruthRC4Jets AntiKt10TruthRC2Jets",
                        #"m_truthFatJetDetailStr": "kinematic",

                        ## TODO ADD TRUTH JETS

                        #"m_METContainerName": "NewMETRefFinal",
                        #"m_evtDetailStr": "pileup",
                        #"m_trigDetailStr": "basic menuKeys passTriggers",
                        
                    })
