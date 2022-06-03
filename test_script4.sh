#!/bin/bash
mkdir lambda_variation_logs
make clean
make HIDDEN_SIZE=128 HIDDEN_SIZE_2=64 ETA_VAL=.2 LAMBDA_VAL=1 ENABLE_VAR_LAMBDA=1
twinpipe './ggl' '/usr/local/bin/gtp-rhino -e 1 -b 3 -m 1 -w 1 -r 0'
mv gtp_log.txt lambda_variation_logs/log_1.txt
