#!/bin/bash
protoc -I=./proto_raw --cpp_out=./protoc-gen-cpp ./proto_raw/*.proto
cp -rf ./protoc-gen-cpp/*.* ../Server/Src/Message 