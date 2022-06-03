#!/bin/bash
make clean
make HIDDEN_SIZE=60 ETA_VAL=.2 LAMBDA_VAL=0
twinpipe './ggl' '/usr/local/bin/gtp-rhino -e 1 -b 3 -m 1 -w 1 -r 0'
mv gtp_log.txt eta_val_logs/log_30.txt

make clean
make HIDDEN_SIZE=60 ETA_VAL=.2 LAMBDA_VAL=.2
twinpipe './ggl' '/usr/local/bin/gtp-rhino -e 1 -b 3 -m 1 -w 1 -r 0'
mv gtp_log.txt eta_val_logs/log_30.txt

make clean
make HIDDEN_SIZE=60 ETA_VAL=.2 LAMBDA_VAL=.5
twinpipe './ggl' '/usr/local/bin/gtp-rhino -e 1 -b 3 -m 1 -w 1 -r 0'
mv gtp_log.txt eta_val_logs/log_30.txt

make clean
make HIDDEN_SIZE=60 ETA_VAL=.2 LAMBDA_VAL=1
twinpipe './ggl' '/usr/local/bin/gtp-rhino -e 1 -b 3 -m 1 -w 1 -r 0'
mv gtp_log.txt eta_val_logs/log_30.txt