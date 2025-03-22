#!/usr/bin/env bash

#source ~/PREX/setup_japan_SBS.tcsh
#cd ~/PREX/japan_feedback-SBS
lastrun=${1}

PGLARGEDIR=/adaq3/data1/apar/pgautam
QW_DATA=/adaq2/data1/apar
QW_ROOTFILES=${PGLARGEDIR}/test
LOGDIR=${PGLARGEDIR}/log
MY_LOG_FILE=${LOGDIR}/autoana_$lastrun

MYDIR=${HOME}/pgautam
JAPANDIR=${MYDIR}/sft/japan/japan-SBS

BUILDDIR=${JAPANDIR}/build
QWPARITY=${BUILDDIR}/qwparity
CFGPATH=${MYDIR}/simana/japan/analysis/Parity/prminput
export QW_PRMINPUT=${CFGPATH}

cmd="
${QWPARITY} --QwLog.logfile  ${MY_LOG_FILE}\
    --config prexinj.conf\
    --blinder.force-target-out=1\
    --rootfiles=${QW_ROOTFILES}\
     --run $lastrun 
"

echo ${cmd}

${cmd}


