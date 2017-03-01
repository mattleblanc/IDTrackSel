# IDTrackSel

Simple wrappers around ATLAS track selection tools (`InDetTrackSelectionTool` and `TrackVertexAssociationTool`) ...

## Setup

```
setupATLAS
rcSetup Base,2.4.28
git clone https://github.com/mattleblanc/IDTrackSel.git
rc build
```

## Usage

These wrappers are meant for use with [xAODAnaHelpers](https://github.com/UCATLAS/xAODAnaHelpers). To use it, you could specify part of a config such as ...

```
c.setalg("InDetTrackSelectionToolAlgo", {
    "m_debug": False,
    "m_name": "InDetTrackSelectionToolAlgo",
    "m_inputTrackContainer": "InDetTrackParticles",
    "m_outputTrackContainer": "SelectedTrackParticles",
    "m_CutLevel": "Loose"
})

c.setalg("TightTrackVertexAssociationToolAlgo", {
        "m_debug": False,
        "m_name": "TightTrackVertexAssociationToolAlgo",
        "m_inputTrackContainer": "SelectedTrackParticles",
        "m_outputTrackContainer": "PVTrackParticles",
        "m_dzSinTheta_cut": 3,
        "m_doPV": True
})

c.setalg("TreeAlgo", {"m_debug": False,
                      "m_name": "rcRes",
                      "m_trackParticlesContainerName": "PVTrackParticles",
                      "m_trackParticlesDetailStr": "kinematic trackpars numbers vertex",
                    })

```

## Documentation

`InDetTrackSelectionTool` twiki:

https://twiki.cern.ch/twiki/bin/view/AtlasProtected/InDetTrackSelectionTool

`TrackVertexAssociationTool` twiki:

https://twiki.cern.ch/twiki/bin/view/AtlasProtected/TrackVertexAssociationTool