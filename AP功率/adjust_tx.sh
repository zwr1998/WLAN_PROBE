sleep 40
/remount.sh
rmmod /lib/modules/4.1.51/extra/dhd.ko
sleep 3
insmod /lib/modules/4.1.51/extra/dhd.ko firmware_path=/etc/wlan/dhd/43684b0/rtecdc.bin nvram_path=/DHD/sdk+7+huoshuo5.txt
sleep 5
/DHD/sys-restart
sleep 15
wl -i eth5 txpwr1 -o -d 18
sleep 1
wl -i eth6 txpwr1 -o -d 18