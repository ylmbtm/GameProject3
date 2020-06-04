nohup ./AccountServer &
echo "$!" > account.pid

nohup ./LoginServer &
echo "$!" > login.pid

nohup ./LogServer  &
echo "$!" > logsvr.pid

nohup ./CenterServer &
echo "$!" > center.pid

nohup ./DBServer &
echo "$!" > dbsvr.pid

nohup ./LogicServer &
echo "$!" > logic.pid

nohup ./GameServer id=1 port=9008 &
echo "$!" > game1.pid
 
