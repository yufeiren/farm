#! /bin/bash

iscsiadm -m discovery -t sendtargets -p 192.168.150.11:13261 -I iser
iscsiadm -m discovery -t sendtargets -p 192.168.151.11:13261 -I iser

