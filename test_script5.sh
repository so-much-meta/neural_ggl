#!/bin/bash
mkdir hidden_size_2_logs

make clean
make HIDDEN_SIZE=90 ETA_VAL=.2 LAMBDA_VAL=.3 HIDDEN_SIZE_2=4
twinpipe './ggl' '/usr/local/bin/gtp-rhino -e 1 -b 3 -m 1 -w 1 -r 0'
mv gtp_log.txt hidden_size_2_logs/log_4.txt

make clean
make HIDDEN_SIZE=90 ETA_VAL=.2 LAMBDA_VAL=.3 HIDDEN_SIZE_2=8
twinpipe './ggl' '/usr/local/bin/gtp-rhino -e 1 -b 3 -m 1 -w 1 -r 0'
mv gtp_log.txt hidden_size_2_logs/log_8.txt

make clean
make HIDDEN_SIZE=90 ETA_VAL=.2 LAMBDA_VAL=.3 HIDDEN_SIZE_2=16
twinpipe './ggl' '/usr/local/bin/gtp-rhino -e 1 -b 3 -m 1 -w 1 -r 0'
mv gtp_log.txt hidden_size_2_logs/log_16.txt
