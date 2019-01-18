# sudo timeout -s INT 0.5s hcitool lescan --duplicates &
sudo timeout -s INT 1s hcitool lescan --duplicates &
# sudo timeout -s INT 0.5s hcidump --raw | sed -e 's/ //g' | grep '[0-9A-F]*$' >> test.txt &
sudo timeout -s INT 1s hcidump --raw &
