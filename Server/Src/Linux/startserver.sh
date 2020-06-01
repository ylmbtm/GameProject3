nohup ./ProxyServer >> proxy.log 2>&1 &
echo "$!" > proxy.pid

nohup ./AccountServer >> account.log 2>&1 &
echo "$!" > account.pid

nohup ./LoginServer >> login.log 2>&1 &
echo "$!" > login.pid

nohup ./LogServer >> log_svr.log 2>&1 &
echo "$!" > logsvr.pid

nohup ./CenterServer >> center.log 2>&1 &
echo "$!" > center.pid

nohup ./DBServer >> db.log 2>&1 &
echo "$!" > dbsvr.pid

nohup ./LogicServer >> logic.log 2>&1 &
echo "$!" > logic.pid

nohup ./GameServer id=1 port=9008 >> game1.log 2>&1 &
echo "$!" > game1.pid
 
