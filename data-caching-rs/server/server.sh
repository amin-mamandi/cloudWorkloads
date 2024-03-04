#!/bin/sh

if [ "$1" = "help" ]; then
    echo "Usage: server.sh [mode] [cores] [threads]"
    echo "      core: core to run memcached server on"
    echo "      mode: mode to run memcached server"
    echo "      ip:   the ip address fo the server"
    exit 0
fi

if [ "$1" = "net" ]; then
    taskset -c "$2" memcached -m 23040 -p 11211 -l $3 
elif [ "$1" = "skt" ]; then
    taskset -c "$2" memcached -s /tmp/memcached.sock -m 23040 
fi

