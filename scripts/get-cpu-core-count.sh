#!/usr/bin/env bash

# usage: ./scripts/get-cpu-core-count.sh $DEFAULT_VALUE

COUNT=`cat /proc/cpuinfo | gawk '/^processor/ {print ($3-0)+1;}' | sort | tail -1`;
[[ -z "$COUNT" ]] && echo "$1" || echo "$COUNT";
