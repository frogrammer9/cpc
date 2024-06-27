#!/bin/bash
cmake . --preset linux-debug;cd out/build/linux-debug;
if ninja ; then 
	./cpc/cpc
fi
