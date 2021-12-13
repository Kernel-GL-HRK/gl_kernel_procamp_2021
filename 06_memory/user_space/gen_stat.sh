#!/bin/bash

CYCLES=1000

mkdir -p log
rm -f log/*.txt.bz2

make clean
make CALL_ALLOCATOR=1 CALL_FREE=1 all
echo "----------------------------------------------------------------"
echo "|size                |allocate took       |free took           | malloc free cycles=$CYCLES"
echo "----------------------------------------------------------------"

for i in $(seq "$CYCLES"); do ./user-space-memory | bzip2 -9 >> log/malloc_free_log.txt.bz2; done
bzcat log/malloc_free_log.txt.bz2 | grep '|' | grep -v total | grep -v pow | awk -v OFS='\t\t' -v CYCLES="$CYCLES" -F"|" '{a[$3] += $4; b[$3] += $5} END {for (size in a) print size, a[size]/CYCLES, b[size]/CYCLES}'| sort -n

make clean
make CALL_ALLOCATOR=1 CALL_FREE=0 all
echo "----------------------------------------------------------------"
echo "|size                |allocate took       |free took           | malloc no free cycles=$CYCLES"
echo "----------------------------------------------------------------"
for i in $(seq "$CYCLES"); do ./user-space-memory | bzip2 -9 >> log/malloc_nofree_log.txt.bz2; done
bzcat log/malloc_nofree_log.txt.bz2 | grep '|' | grep -v total | grep -v pow | awk -v OFS='\t\t' -v CYCLES="$CYCLES" -F"|" '{a[$3] += $4; b[$3] += $5} END {for (size in a) print size, a[size]/CYCLES, b[size]/CYCLES}'| sort -n

make clean
make CALL_ALLOCATOR=2 CALL_FREE=1 all
echo "----------------------------------------------------------------"
echo "|size                |allocate took       |free took           | calloc free cycles=$CYCLES"
echo "----------------------------------------------------------------"
for i in $(seq "$CYCLES"); do ./user-space-memory | bzip2 -9 >> log/calloc_free_log.txt.bz2; done
bzcat log/calloc_free_log.txt.bz2 | grep '|' | grep -v total | grep -v pow | awk -v OFS='\t\t' -v CYCLES="$CYCLES" -F"|" '{a[$3] += $4; b[$3] += $5} END {for (size in a) print size, a[size]/CYCLES, b[size]/CYCLES}'| sort -n

make clean
make CALL_ALLOCATOR=2 CALL_FREE=0 all
echo "----------------------------------------------------------------"
echo "|size                |allocate took       |free took           | calloc no free cycles=$CYCLES"
echo "----------------------------------------------------------------"
for i in $(seq "$CYCLES"); do ./user-space-memory | bzip2 -9 >> log/calloc_nofree_log.txt.bz2; done
bzcat log/calloc_nofree_log.txt.bz2 | grep '|' | grep -v total | grep -v pow | awk -v OFS='\t\t' -v CYCLES="$CYCLES" -F"|" '{a[$3] += $4; b[$3] += $5} END {for (size in a) print size, a[size]/CYCLES, b[size]/CYCLES}'| sort -n

make clean
make CALL_ALLOCATOR=3 CALL_FREE=0 all
echo "----------------------------------------------------------------"
echo "|size                |allocate took       |free took           | alloca cycles=$CYCLES"
echo "----------------------------------------------------------------"
for i in $(seq "$CYCLES"); do ./user-space-memory | bzip2 -9 >> log/alloca_nofree_log.txt.bz2; done
bzcat log/alloca_nofree_log.txt.bz2 | grep '|' | grep -v total | grep -v pow | awk -v OFS='\t\t' -v CYCLES="$CYCLES" -F"|" '{a[$3] += $4; b[$3] += $5} END {for (size in a) print size, a[size]/CYCLES, b[size]/CYCLES}'| sort -n
