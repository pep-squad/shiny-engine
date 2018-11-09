sudo rm -rf /usr/bin/ble/
sudo mkdir /usr/bin/ble
while :
do
	echo "scanning..."
	sudo bash ble.sh
	sleep 1
done
