#!/bin/bash

cp $1 tmp.cpp
make
rm tmp.cpp
