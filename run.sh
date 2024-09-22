#!/bin/bash
clear;
cmake . --preset linux-debug;cd out/build/linux-debug;
if ninja ; then 
	cd ../../..;
	./out/build/linux-debug/cpc/cpc $1;
fi
