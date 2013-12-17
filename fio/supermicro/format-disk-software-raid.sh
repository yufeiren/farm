#! /bin/bash

for disk in b c d e f g h i j k l m n o p q r s t u v w x y z
do
	(echo o; echo n; echo p; echo 1; echo; echo; echo p; echo t; echo fd; echo p; echo w) | fdisk /dev/sd$disk
done

for disk in b c d e f g h i j k l m n o p q r s t u v
do
	(echo o; echo n; echo p; echo 1; echo; echo; echo p; echo t; echo fd; echo p; echo w) | fdisk /dev/sda$disk
done

