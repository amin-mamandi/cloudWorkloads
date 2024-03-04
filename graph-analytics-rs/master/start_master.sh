#!/usr/bin/env bash

# Starts Spark master on the machine this script is executed on.
SPARK_HOME=/opt/spark-3.3.2
SPARK_MASTER_PORT=7077
SPARK_MASTER_IP=192.168.1.2
SPARK_MASTER_WEBUI_PORT=8080

. "$SPARK_HOME/sbin/spark-config.sh"
. "$SPARK_HOME/bin/load-spark-env.sh"

exec "$SPARK_HOME"/bin/spark-class org.apache.spark.deploy.master.Master \
  --ip $SPARK_MASTER_IP --port $SPARK_MASTER_PORT --webui-port $SPARK_MASTER_WEBUI_PORT "$@"

# /usr/lib/jvm/java-17-openjdk-amd64/bin/java \
#   -cp /opt/spark-3.3.2/conf/:/opt/spark-3.3.2/jars/* \
#   -Xmx1g org.apache.spark.deploy.master.Master \
#   --ip 192.168.1.2 --port 7077 --webui-port 8080