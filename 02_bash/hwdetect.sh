#!/bin/bash

echo 'hwdetect started'

DEVICES_FILE=./devices
DEVICES_VAR=""

while : 
do
	if test -f "$DEVICES_FILE"; then
#		echo "---"
		DEVICES_VAR=$(ls /dev)
#		echo "$DEVICES_VAR"
#		echo "........"
		diff <(echo "$DEVICES_VAR") "$DEVICES_FILE" | grep '<' && echo "added"
		diff <(echo "$DEVICES_VAR") "$DEVICES_FILE" | grep '>' && echo "removed"
		echo "$DEVICES_VAR" > "$DEVICES_FILE"
	else
#		echo "else"
		ls /dev > "$DEVICES_FILE"
	fi 
done
