#!/bin/bash

xAH_run.py \
    --config src/xAH_TAR.py \
    --files mc16_13TeV.364705.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ5WithSW.deriv.DAOD_EXOT27.e7142_e5984_s3126_r10201_r10210_p3990 \
    --inputRucio -f \
    --nevents 5000  \
    direct
