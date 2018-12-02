sudo timeout -s INT 5s hcitool lescan --duplicates &
sudo timeout -s INT 2s hcidump --raw | sed -e 's/ //g' | grep '[0-9A-F]*$' # &
