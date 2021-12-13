#!/bin/sh

CYCLES=1000

mkdir -p log
rm -f log/*.txt.gz

for i in $(seq "$CYCLES"); do cat /sys/kernel/mem_alloc_mod/kmalloc_table | gzip -9 >> log/kmalloc_log.txt.gz; done
for i in $(seq "$CYCLES"); do cat /sys/kernel/mem_alloc_mod/vmalloc_table | gzip -9 >> log/vmalloc_log.txt.gz; done
for i in $(seq "$CYCLES"); do cat /sys/kernel/mem_alloc_mod/kzalloc_table | gzip -9 >> log/kzalloc_log.txt.gz; done
for i in $(seq "$CYCLES"); do cat /sys/kernel/mem_alloc_mod/free_page_table | gzip -9 >> log/free_page_log.txt.gz; done
