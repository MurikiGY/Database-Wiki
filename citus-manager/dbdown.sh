#!/bin/bash

NUM_WORKERS=4

#Shut the workers
for (( i=1; i<=NUM_WORKERS ;i++))
do
  pg_ctl -D citus/worker$i -o "-p 970$i" -l logfiles/worker$1_logfile stop
done

#Shut the coordinator
pg_ctl -D citus/coordinator -o "-p 9700" -l logfiles/coordinator_logfile stop
