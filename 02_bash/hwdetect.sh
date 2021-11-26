#!/bin/bash

STR_ATTACHED='attached'
STR_DETACHED='removed'
DIR=/dev

STR_RES=''
DEVICES_LIST=(
  "ttyUSB"
  "i2c"
  "sd"
  "mmcblk"
)

# Check that inotify utility is installed
if ! which inotifywait > /dev/null
then
  echo "inotify utility could not be found. Please install it"
  exit
fi

while true; do

  RES=$(inotifywait -q -e create,delete $DIR)

  if echo "$RES" | grep -q "CREATE"
  then
    STR_RES=$STR_ATTACHED
  else
    STR_RES=$STR_DETACHED
  fi

  for i in "${DEVICES_LIST[@]}"
  do
    SUBSTRING=${RES:13:10}

    if echo "$RES" | grep -q "${i}"
    then
      echo "${SUBSTRING} ${STR_RES}"
    fi
  done

  sleep 1

done
