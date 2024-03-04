# data-serving-gem5

CloudSuite README

https://github.com/parsa-epfl/cloudsuite/blob/main/docs/benchmarks/data-serving.md

Install Cassandra and OpenJDK 11 for both server and client
```bash
$ sudo apt-get update
$ sudo apt-get install cassandra openjdk-11-jdk-headless
```

Install python3-yaml for server only
```bash
$ sudo apt-get update
$ sudo apt-get install python3-yaml
```

On server's machine,
```bash
Setup environment
$ cd server
$ source ./setup.sh

To start server, need to stop current Cassandra
$ sudo service cassandra stop
$ sudo python ./docker-entrypoint.py --listen-ip=192.168.1.2
```

On client's machine, 
```bash
Setup environment
$ cd client
$ source ./setup.sh

To warm server:
$ ./warmup.sh 192.168.1.2 <record_count> <threads=1>

To load server:
$ ./load.sh 192.168.1.2 <record_count> <target_load> <threads=1> <operation_count=load * 60>
```
