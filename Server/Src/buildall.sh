cd ServerEngine && make
cd ..
cd CenterServer && make
cd ..
cd GameServer && make
cd ..
cd HttpServer && make
cd ..
cd LogicServer && make
cd ..
cd LoginServer && make
cd ..
cd ProxyServer && make
cd ..
cd AccountServer && make
cd ..
cd DBServer && make
cd ..
cd LogServer && make
cd ..
cp ./CenterServer/CenterServer ./Linux/
cp ./GameServer/GameServer ./Linux/
cp ./HttpServer/HttpServer ./Linux/
cp ./LogicServer/LogicServer ./Linux/
cp ./LoginServer/LoginServer ./Linux/
cp ./ProxyServer/ProxyServer ./Linux/
cp ./AccountServer/AccountServer ./Linux/
cp ./DBServer/DBServer ./Linux/
cp ./LogServer/LogServer ./Linux/

 
