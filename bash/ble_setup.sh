sudo /etc/init.d/bluetooth stop
sudo /etc/init.d/bluetooth start

sudo hciconfig hci0 down
sudo hciconfig hci0 up

sudo hciconfig hci0 leadv 3
