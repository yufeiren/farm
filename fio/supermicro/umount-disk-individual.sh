#! /bin/bash

for disk in b c d e f g h i j k l m n o p q r s t u v w x y z
do
	umount /mnt/sd$disk
done

for disk in b c d e f g h i j k l m n o p q r s t u v
do
	umount /mnt/sda$disk
done

