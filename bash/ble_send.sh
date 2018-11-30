sudo hcitool -i hci0 cmd 0x08 0x0008 1E 02 01 1A 1A FF 4C 00 02 15 63 6F 3F 8F 64 91 4B EE 95 F7 D8 CC 64 A8 63 B5 00 00 00 00 C8 00
# http://www.wadewegner.com/2014/05/create-an-ibeacon-transmitter-with-the-raspberry-pi/
# sudo hcitool -i hci0 cmd 0x08 0x0008 1E 02 01 1A 1A FF 4C 00 02 15 63 6F 3F 8F 64 91 4B EE 95 F7 D8 CC 64 A8 63 B5 00 00 00 00 C8 00
# ox08    # OGF
# 0x0008  # OCF
# 1E
# 02  	# Number of bytes that follow in first AD structure
# 01  	# Flags AD type
# 1A  	# Flags value 0x1A = 000011010
#          bit 0 (OFF) LE Limited Discoverable Mode
#          bit 1 (ON) LE General Discoverable Mode
#          bit 2 (OFF) BR/EDR Not Supported
#          bit 3 (ON) Simultaneous LE and BR/EDR to Same Device Capable (controller)
#          bit 4 (ON) Simultaneous LE and BR/EDR to Same Device Capable (Host)
# 1A    	# Number of bytes that follow in second (and last) AD structure
# FF    	# Manufacturer specific data AD type
# 4C 00 	# Company identifier code (0x004C == Apple)
# 02    	# Byte 0 of iBeacon advertisement indicator
# 15    	# Byte 1 of iBeacon advertisement indicator
# 63 6F 3F 8F 64 91 4B EE 95 F7 D8 CC 64 A8 63 B5 # our iBeacon proximity uuid
# 00 00 	# Major
# 00 00 	# Minor
# C8 00 	# Calibrated Tx power
