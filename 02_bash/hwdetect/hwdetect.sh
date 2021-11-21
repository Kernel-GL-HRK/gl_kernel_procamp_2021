#!/bin/bash

FOLDER=${1:-/dev}
echo "Polling folder: $FOLDER"

PREV=$(stat --terse --format="%n %z" ${FOLDER}/*|sort -n)

while true
do
    NEW=$(stat --terse --format="%n %z" ${FOLDER}/*|sort -n)
    DELTA=$(diff <(echo "$PREV") <(echo "$NEW") | grep '^>')

    if [[ "${#DELTA}" != 0 ]]
    then
        echo "$DELTA"
    fi
    PREV=$NEW
    sleep 1
done
