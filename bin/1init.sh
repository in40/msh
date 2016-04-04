#sudo insmod /lib/modules/3.8.0-35-generic/extra/osif.ko speed=400
#sudo insmod /lib/modules/3.8.0-35-generic/kernel/drivers/i2c/i2c-dev.ko



#echo Connect OSIF  to any USB port now
#sleep 3
#read -p "Hit RETURN key to continue"
#echo Initializing...
#sudo i2cdetect -l
#sudo owfs -d /dev/i2c-6 -m /mnt/1wire
sudo owfs -d /dev/ttyUSB1 -m /mnt/1wire
sudo ls /mnt/1wire

echo Switching RST off...
sudo echo 1 > /mnt/1wire/uncached/29.D6080E000000/strobe 
