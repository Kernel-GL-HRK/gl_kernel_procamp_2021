DEV=${1:-/dev/}
STATE=$(ls ${DEV})

while true; do
    NEW_STATE=$(ls ${DEV})
    
    if [ "$STATE" != "$NEW_STATE" ]; then
        DIFF=$(diff <(echo "$STATE") <(echo "$NEW_STATE"))
        echo "$DIFF"
    fi
    
    STATE=$NEW_STATE
done

