#! /bin/bash

for disk in b c d e f g h i j k l m n o p q r s t u v w x y z
do
	mkdir -p /mnt/sd$disk
	mount /dev/sd$disk /mnt/sd$disk
	chown ren:ren /mnt/sd$disk
done

for disk in b c d e f g h i j k l m n o p q r s t u v
do
	mkdir -p /mnt/sda$disk
	mount /dev/sda$disk /mnt/sda$disk
	chown ren:ren /mnt/sda$disk
done

