using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//跳上骑乘坐骑
public class CommandRideBegin : ICommand
{
    public override Resp Do()
    {
        CmdHandler<CommandRideBegin> call = Del as CmdHandler<CommandRideBegin>;
        return call == null ? Resp.TYPE_NO : call(this);
    }
}
