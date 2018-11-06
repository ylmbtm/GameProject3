cd ServerEngine && make -j2
cd ..
cd CenterServer && make -j2
cd ..
cd GameServer && make -j2
cd ..
cd LogicServer && make -j2
cd ..
cd LoginServer && make -j2
cd ..
cd ProxyServer && make -j2
cd ..
cd AccountServer && make -j2
cd ..
cd DBServer && make -j2
cd ..
cd LogServer && make -j2
cd ..
cp ./CenterServer/CenterServer ./Linux/
cp ./GameServer/GameServer ./Linux/
cp ./LogicServer/LogicServer ./Linux/
cp ./LoginServer/LoginServer ./Linux/
cp ./ProxyServer/ProxyServer ./Linux/
cp ./AccountServer/AccountServer ./Linux/
cp ./DBServer/DBServer ./Linux/
cp ./LogServer/LogServer ./Linux/

 
