#!/bin/bash
set -ex
/usr/bin/time -v yosys template.ys -l template_log.txt
/usr/bin/time -v nextpnr-fabulous --json template.json --pcf template.pcf --fasm template.fasm  --freq 200 --verbose --debug
