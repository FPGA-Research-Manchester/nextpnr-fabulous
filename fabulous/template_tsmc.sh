#!/bin/bash
set -ex
/usr/bin/time -v python3 script_gen.py -f $1 -t $2
/usr/bin/time -v yosys ${2}.ys -l ${2}_log.txt
/usr/bin/time -v nextpnr-fabulous --json ${2}.json --pcf template.pcf --fasm ${2}.fasm  --freq 200 --verbose --debug --log ${2}_npnr_log.txt
/usr/bin/time -v python3 bit_gen_tsmc.py -genBitstream ${2}.fasm meta_data_tsmc.txt ${2}_tsmc_output.bin
