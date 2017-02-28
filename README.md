# IDTrackSel

Simple wrappers around ATLAS track selection tools (`InDetTrackSelectionTool`) ...

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
```

## Documentation

`InDetTrackSelectionTool` twiki:

https://twiki.cern.ch/twiki/bin/view/AtlasProtected/InDetTrackSelectionTool
