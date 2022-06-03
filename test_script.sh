#!/bin/bash
# make clean
# make HIDDEN_SIZE=30
## *** OOPS --- different than the others. Will not compare to rest of items...
# twinpipe './neural' '/usr/local/bin/gtp-rhino -e 2 -b 3 -m 2 -w 2 -r 0'
# mv gtp_log.txt log_30.txt

make clean
make HIDDEN_SIZE=60
twinpipe './ggl' '/usr/local/bin/gtp-rhino -e 1 -b 3 -m 1 -w 1 -r 0'
mv gtp_log.txt log_60.txt

make clean
make HIDDEN_SIZE=90
twinpipe './ggl' '/usr/local/bin/gtp-rhino -e 1 -b 3 -m 1 -w 1 -r 0'
mv gtp_log.txt log_90.txt

make clean
make HIDDEN_SIZE=120
twinpipe './ggl' '/usr/local/bin/gtp-rhino -e 1 -b 3 -m 1 -w 1 -r 0'
mv gtp_log.txt log_120.txt

make clean
make HIDDEN_SIZE=150
twinpipe './ggl' '/usr/local/bin/gtp-rhino -e 1 -b 3 -m 1 -w 1 -r 0'
mv gtp_log.txt log_150.txt
