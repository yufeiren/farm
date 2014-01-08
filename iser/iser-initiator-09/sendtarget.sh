#! /bin/bash

iscsiadm -m discovery -t sendtargets -p 192.168.150.13:3261 -I iser
iscsiadm -m discovery -t sendtargets -p 192.168.151.13:3262 -I iser

