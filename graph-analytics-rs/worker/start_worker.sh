#!/usr/bin/env bash

# Starts Spark worker on the machine this script is executed on.
SPARK_HOME=/opt/spark-3.3.2
SPARK_WORKER_WEBUI_PORT=8080
MASTER=spark://192.168.1.2:7077

. "$SPARK_HOME/sbin/spark-config.sh"
. "$SPARK_HOME/bin/load-spark-env.sh"


exec "$SPARK_HOME"/bin/spark-class org.apache.spark.deploy.worker.Worker \
  --webui-port $SPARK_WORKER_WEBUI_PORT $MASTER "$@"

# /usr/lib/jvm/java-17-openjdk-arm64/bin/java \
#     -cp /opt/spark-3.3.2/conf/:/opt/spark-3.3.2/jars/* \
#     -Xmx1g org.apache.spark.deploy.worker.Worker \
#     --webui-port 8080 spark://192.168.1.2:7077