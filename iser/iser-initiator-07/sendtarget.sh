#! /bin/bash

iscsiadm -m discovery -t sendtargets -p 192.168.150.15:3261 -I iser
iscsiadm -m discovery -t sendtargets -p 192.168.151.15:3262 -I iser

