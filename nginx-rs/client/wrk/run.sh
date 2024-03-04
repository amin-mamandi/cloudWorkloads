#!/bin/bash

# Variables to store cumulative values
total_latency=0
total_requests=0
RUN_CMD="./wrk -t 8 -c 24 -d 5s --latency http://192.168.254.2:80/index.nginx-debian.html"
ITER=5

if [ "$1" == "once" ]; then
    echo "Run once"
    $RUN_CMD

elif [ "$1" == "full" ]; then
    echo "Run full, iterations = $ITER"

    run_wrk() {
        local output
        output=$($RUN_CMD)
        # echo "$output"
        
        local latency=$(echo "$output" | grep "99%" | awk '{print $2}')
        local requests=$(echo "$output" | grep "Requests/sec" | awk '{print $2}')
        
        total_latency=$(awk "BEGIN {print $total_latency + $latency}")
        total_requests=$(awk "BEGIN {print $total_requests + $requests}")
        
        echo "Requests/sec: $requests, 99% Latency: $latency"
        echo "-------------------"
    }

    for run in $(seq 1 $ITER); do
        echo "RUN $run"
        run_wrk
    done

    avg_latency=$(awk "BEGIN {print $total_latency / $ITER}")
    avg_requests=$(awk "BEGIN {print $total_requests / $ITER}")

    echo "Avg Requests/sec: $avg_requests, Avg 99% Latency: $avg_latency""ms"

else
    echo "Invalid option. Please specify either 'once' or 'full' as the first argument."
fi