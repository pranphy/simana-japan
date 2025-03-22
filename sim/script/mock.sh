#!/usr/bin/env bash
# -*- coding: utf-8 -*-
# vim: ai ts=4 sts=4 et sw=4 ft=sh
# author : Prakash [प्रकाश]
# date   : 2024-04-15 10:32

JAPANDIR=${HOME}/sft/japan/japan-MOLLER
BUILDDIR=${JAPANDIR}/../build/japan
GENPATH=${BUILDDIR}/qwmockdatagenerator
QWPARITY=${BUILDDIR}/qwparity
CFGPATH=${JAPANDIR}/Parity/prminput
export QW_PRMINPUT=${JAPANDIR}/Parity/prminput
export QWANALYSIS=${JAPANDIR}


CMD="${GENPATH} -r 4 -e 1:2000 --config ${CFGPATH}/qwparity_simple.conf --detectors ${CFGPATH}/mock_newdets.map --data ./output/"
CMDN="${QWPARITY} -r 4 -e 1:2000 --config ${CFGPATH}/qwparity_simple.conf --detectors ${CFGPATH}/mock_newdets.map  --data-handle  ${CFGPATH}/mock_datahandlers.map --data ./output/ --rootfiles ./output/root/"

${CMD}
echo "DONE"
${CMDN}

