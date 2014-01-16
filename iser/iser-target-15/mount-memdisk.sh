#! /bin/sh

mkdir -p /mnt/tmp0 /mnt/tmp1
mount -t tmpfs -o size=160g tmpfs /mnt/tmp0
mount -t tmpfs -o size=160g tmpfs /mnt/tmp1

