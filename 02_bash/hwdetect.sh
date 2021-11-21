#!/bin/bash

allDevices=(ttyUSB i2c sd mmcblk) 

function checkDevicesDiff() {
  device=${1}
  ls /dev | grep ${device} > ${device}_updated.txt
  updated_dev=$(diff ${device}.txt ${device}_updated.txt)

  if [[ -n ${updated_dev} ]]; then
    echo "Devices status changed ${device}: ${updated_dev}"
    cp ${device}_updated.txt ${device}.txt
  fi
}

for device in ${allDevices[@]}; do
  ls /dev | grep ${device} > ${device}.txt

  if [[ -s ${device}.txt ]]; then
    cat ${device}.txt
  else
    echo "No ${device} devices detected"
  fi
done

echo "\">\" - means connected device"
echo "\"<\" - means disconnected device"

while(true); do
  sleep 1 
  for device in ${allDevices[@]}; do
    checkDevicesDiff "${device}"
  done
done

exit 0

