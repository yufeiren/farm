#! /bin/bash

for disk in b c d e f g h i j k l m n o p q r s t u v w x y z
do
	mkfs.ext4 /dev/sd$disk
done

for disk in b c d e f g h i j k l m n o p q r s t u v
do
	mkfs.ext4 /dev/sda$disk
done

