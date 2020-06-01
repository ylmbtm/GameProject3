# GameProject3 Linux Build Steps

## Build server from source

```txt

git clone https://github.com/ylmbtm/GameProject3.git

cd GameProject3/Server/Src

sh buildall.sh

```

* if build success, all build server bin will output to `GameProject3/Server/Src/Linux`

## Initialization database

```txt

cd GameProject3/Server

mysql -uroot -p -h 127.0.0.1 < db_create.sql

```



## How to run the game server ?

```txt

[root@iZbp1fwykdvz81j5qum40vZ Linux]# cd GameProject3/Server/Src/Linux
[root@iZbp1fwykdvz81j5qum40vZ Linux]# ll
-rwxr-xr-x 1 root root  9737352 6月   1 16:51 AccountServer
-rwxr-xr-x 1 root root  9457848 6月   1 16:51 CenterServer
-rwxr-xr-x 1 root root 10550496 6月   1 16:51 DBServer
-rwxr-xr-x 1 root root 16602696 6月   1 16:51 GameServer
-rwxr-xr-x 1 root root 23407640 6月   1 16:51 LogicServer
-rwxr-xr-x 1 root root 10287864 6月   1 16:51 LoginServer
-rwxr-xr-x 1 root root  9358672 6月   1 16:51 LogServer
-rwxr-xr-x 1 root root  9532608 6月   1 16:51 ProxyServer
-rwxr-xr-x 1 root root  9449040 6月   1 20:08 WatchServer
[root@iZbp1fwykdvz81j5qum40vZ Linux]# sh startall.sh # all server will be start as backend job

``` 


## Build protobuf from source

```txt
git clone https://github.com/protocolbuffers/protobuf.git
cd protobuf/
./configure
make && make install
```
* protoc output dir `/usr/local/bin/protoc`
* use protoc to generate protocol code 






