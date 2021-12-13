#!/bin/bash

CYCLES=1000

echo "--------------------------------------------------------------| kmalloc free cycles=$CYCLES"
zcat raw_log/kmalloc_log.txt.gz | grep '|' | grep -v total | grep -v pow | awk -v OFS='\t' -v CYCLES="$CYCLES" -F"|" '{a[$3] += $4; b[$3] += $5} END {{printf "%-20s|%-20s|%-20s|\n", "size", "allocate took", "free took"} for (size in a) printf "%-20s|%-20.2f|%-20.2f|\n", size, a[size]/CYCLES, b[size]/CYCLES}'| sort -n

echo "--------------------------------------------------------------| vmalloc free cycles=$CYCLES"
zcat raw_log/vmalloc_log.txt.gz | grep '|' | grep -v total | grep -v pow | awk -v OFS='\t' -v CYCLES="$CYCLES" -F"|" '{a[$3] += $4; b[$3] += $5} END {{printf "%-20s|%-20s|%-20s|\n", "size", "allocate took", "free took"} for (size in a) printf "%-20s|%-20.2f|%-20.2f|\n", size, a[size]/CYCLES, b[size]/CYCLES}'| sort -n

echo "--------------------------------------------------------------| kzalloc free cycles=$CYCLES"
zcat raw_log/kzalloc_log.txt.gz | grep '|' | grep -v total | grep -v pow | awk -v OFS='\t' -v CYCLES="$CYCLES" -F"|" '{a[$3] += $4; b[$3] += $5} END {{printf "%-20s|%-20s|%-20s|\n", "size", "allocate took", "free took"} for (size in a) printf "%-20s|%-20.2f|%-20.2f|\n", size, a[size]/CYCLES, b[size]/CYCLES}'| sort -n

echo "--------------------------------------------------------------| free_page cycles=$CYCLES"
zcat raw_log/free_page_log.txt.gz | grep '|' | grep -v total | grep -v pow | awk -v OFS='\t' -v CYCLES="$CYCLES" -F"|" '{a[$3] += $4; b[$3] += $5} END {{printf "%-20s|%-20s|%-20s|\n", "size", "allocate took", "free took"} for (size in a) printf "%-20s|%-20.2f|%-20.2f|\n", size, a[size]/CYCLES, b[size]/CYCLES}'| sort -n

