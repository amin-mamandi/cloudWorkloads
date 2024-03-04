#!/bin/bash

if [ "$1" = "help" ]; then
    echo "Usage: clinet.sh [mode] [workers] [connections] [rps]"
    echo "      workers:     number of workers"
    echo "      mode:        mode to run memcached client"
    echo "      connections: number of connections"
    echo "      RPS:         number requests per second"

    exit 0
fi
#sudo killall -9 loader 

# max throughput
if [ "$1" == "th" ]; then
    ./loader -a ./twitter_dataset_64  -s servers.txt -g 0.8 -c $3 -w $2 -T 1 -D 23040 
# rps
elif [ "$1" == "rps" ]; then
    ./loader -a ./twitter_dataset_64  -s servers.txt -g 0.8 -c $3 -w $2 -T 1 -D 23040 -r $4
# warmup
elif [ "$1" == "w" ]; then
    ./loader -a ./twitter_dataset_64  -s servers.txt -c $3 -w $2 -S 1 -T 1 -j -D  23040 
# scale&warmup
elif [ "$1" == "sw" ]; then
    ./loader -a ./twitter_dataset_unscaled -o ./twitter_dataset_64 -s servers.txt -c $3 -w $2 -S 64 -T 1 -j -D  23040

else
    echo "Invalid option. Please specify either 'th' or 'sw' or 'rps' or 'w'  as the first argument."
fi


