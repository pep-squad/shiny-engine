# parse the hexadecimal value from the ble packet
COMMAND=$(sudo timeout -s INT 1s hcitool lescan --duplicates | sed -e 's/://g' | grep -o '^[0-9a-fA-F]*')
# turn it into an array
HEX=$(echo $COMMAND | tr " " "\n")
# create a file with the hexadeximal value
for FILE in $HEX
do
	sudo touch /usr/bin/ble/"$FILE"
done
