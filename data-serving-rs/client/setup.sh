#!/bin/sh

YCSB_VERSION=0.17.0

wget -q --show-progress --progress=bar:force \
  "https://github.com/brianfrankcooper/YCSB/releases/download/$YCSB_VERSION/ycsb-$YCSB_VERSION.tar.gz" \
  -O "./ycsb-$YCSB_VERSION.tar.gz"

tar -xzf "./ycsb-$YCSB_VERSION.tar.gz"
rm "./ycsb-$YCSB_VERSION.tar.gz"
mv "./ycsb-$YCSB_VERSION" ./ycsb

export CASSANDRA_HOME=/opt/cassandra
export CASSANDRA_CONFIG=/etc/cassandra
export PATH=$PATH:/opt/cassandra/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

