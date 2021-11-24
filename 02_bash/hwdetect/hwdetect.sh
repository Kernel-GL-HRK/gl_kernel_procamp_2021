#!/bin/bash

DEV_DIR=/dev

DEVICE_TYPES='ttyUSB|i2c|sd|mmcblk'
devices=($(ls ${DEV_DIR} | egrep ${DEVICE_TYPES}))

function arraydiff()
{
    awk 'BEGIN{RS=ORS=" "}
         {NR==FNR?a[$0]++:a[$0]--}
         END{for(k in a)if(a[k])print k}' <(echo -n "${!1}") <(echo -n "${!2}")
}

echo "Listening devices attached/detached.."
echo "Press [Ctrl+C] to stop.."

while :
do
    new_devices=($(ls ${DEV_DIR} | egrep ${DEVICE_TYPES}))
    diff_devices=($(arraydiff devices[@] new_devices[@]))

    for device in "${diff_devices[@]}"
    do
        if [[ ! " ${devices[*]} " =~ " ${device} " ]]; then
            echo "Attached device: " ${device}
        fi
        if [[ ! " ${new_devices[*]} " =~ " ${device} " ]]; then
            echo "Detached device: " ${device}
        fi
    done

    if [ -n "$diff_devices" ]; then
        devices=(${new_devices[@]})
    fi

    sleep 1
done
