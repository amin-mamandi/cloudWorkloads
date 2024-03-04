#!/bin/sh
sudo systemctl stop nginx
taskset -c $1 nginx -g 'daemon off;' -c $2 # -c <path to custom nginx.conf file>