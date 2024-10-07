#!/bin/bash

RUN_DIR='runs'
mkdir -p "$RUN_DIR"
LAST_RUN=$(ls "$RUN_DIR" | sed -r 's/run(.*)/\1/' | sort -rnu | head -1)
RUN_NUM=$(bc<<<"$LAST_RUN + 1")
# echo 'running...'
# ./qphH.sh > "${RUN_DIR}/run${RUN_NUM}"
OUT="${RUN_DIR}/run${RUN_NUM}"
touch $OUT && cat $OUT > $OUT

bash scripts/tcl/postgres/tproch/pg_tproch.sh | tee out.tmp

SF=1
S=1

DATA=$(cat out.tmp | grep -E "seconds|mean" | head -n 26)

QUERIES=$(echo "$DATA" | head -n -3 | tail -n -22 | cut -d' ' -f3,5,6,7| sort -n -k1)
QUERY_TIME=$(echo "$QUERIES" | cut -d' ' -f 3 | awk '{s+=$1} END {print s}')
NEW_SALES_TIME=$(echo "$DATA" | head -n 1 | cut -d' ' -f 7)
OLD_SALES_TIME=$(echo "$DATA" | tail -n 1 | cut -d' ' -f 7)
TOTAL_TIME=$(echo "scale=3; $QUERY_TIME+$NEW_SALES_TIME+$OLD_SALES_TIME" | bc)
GEO_MEAN=$(echo "$DATA" | tail -n 2 | head -n 1 | cut -d' ' -f 11)

echo "ORIGINAL" | tee -a "$OUT"
echo "$DATA" | tee -a "$OUT"
echo "QUERIES" | tee -a "$OUT"
echo "$QUERIES" | tee -a "$OUT"

echo "TOTAL QUERY TIME" | tee -a "$OUT"
echo "$QUERY_TIME" | tee -a "$OUT"
echo "NEW SALES TIME" | tee -a "$OUT"
echo "$NEW_SALES_TIME" | tee -a "$OUT"
echo "OLD SALES TIME" | tee -a "$OUT"
echo "$OLD_SALES_TIME" | tee -a "$OUT"
echo "TOTAL TIME" | tee -a "$OUT"
echo "$TOTAL_TIME" | tee -a "$OUT"
echo "GEO MEAN" | tee -a "$OUT"
echo "$GEO_MEAN" | tee -a "$OUT"

# POWER
REFRESH_GEO_MEAN=$(echo "scale=3; sqrt($NEW_SALES_TIME*$OLD_SALES_TIME)" | bc -l)
POWER=$(echo "scale=3; (3600*$SF)/($GEO_MEAN*$REFRESH_GEO_MEAN)" | bc)

echo "POWER METRIC" | tee -a "$OUT"
echo "$POWER" | tee -a "$OUT"

# THROUGHPUT
THROUGHPUT=$(echo "scale=3; ($S*22*3600)/($TOTAL_TIME*$SF)" | bc)

echo "THROUGHPUT" | tee -a "$OUT"
echo "$THROUGHPUT" | tee -a "$OUT"

# QPHH
QPHH=$(echo "scale=3; sqrt($POWER*$THROUGHPUT)" | bc -l)

echo "QPHH" | tee -a "$OUT"
echo "$QPHH" | tee -a "$OUT"
