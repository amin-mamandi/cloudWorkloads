#!/bin/bash

for i in {1..10000}; do
    taskset -c $1 curl -X GET --unix-socket /tmp/nginx.sock -s http://localhost/ > /dev/null &
done

# Wait for all background processes to finish
wait


