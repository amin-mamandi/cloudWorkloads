#!/usr/bin/env bash

SPARK_HOME=/opt/spark-3.3.2
INPUT_FILE=$(pwd)/data/edges.csv
ARG_FILENAME=-file="${INPUT_FILE}"   
WORKLOAD_NAME=pr    
ARG_WORKLOAD=-app="${WORKLOAD_NAME}"
BENCHMARK_JAR=$(pwd)/graph-analytics-2.0.jar

echo "Executing with: $ARG_FILENAME $ARG_WORKLOAD"
exec ${SPARK_HOME}/bin/spark-submit --class GraphAnalytics \
    --driver-memory 8g --executor-memory 8g --master spark://192.168.1.2:7077 \
    ${BENCHMARK_JAR} $ARG_FILENAME $ARG_WORKLOAD