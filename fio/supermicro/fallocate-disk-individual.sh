#! /bin/bash

for disk in b c d e f g h i j k l m n o p q r s t u v w x y z
do
	fallocate -l 100g /mnt/sd$disk/hg
done

for disk in b c d e f g h i j k l m n o p q r s t u v
do
	fallocate -l 100g /mnt/sda$disk/hg
done

