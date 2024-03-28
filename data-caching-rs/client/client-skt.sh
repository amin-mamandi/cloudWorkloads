#!/bin/bash

#  sudo killall -9 loader 
# max throughput
if [ "$1" == "th" ]; then
    ./loader -a ./twitter_dataset_2x  -s unixScoket.txt -g 0.8 -c 1 -w 1 -T 1 -D 512 
# rps
elif [ "$1" == "rps" ]; then
    ./loader -a ./depFile  -s unixScoket.txt -g 1 -c 8 -w 1 -T 1 -D 512 -r  240000 
# scale&warmup
elif [ "$1" == "sw" ]; then
    ./loader -o ./depFile -s unixScoket.txt -c 1 -w 1 -S 1 -T 1 -j -D 512 -k 8000000
else
    echo "Invalid option. Please specify either 'th' or 'sw' or 'rps' as the first argument."
fi


# Finally, we find that thread-pinning and interrupt routing based on network flow-affinity resolve much of memcached’s vulnerability to load imbalance