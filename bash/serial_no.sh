sudo cat /proc/cpuinfo | grep 'Serial' | sed -e 's/[ \t]//g' | cut -c 16-
