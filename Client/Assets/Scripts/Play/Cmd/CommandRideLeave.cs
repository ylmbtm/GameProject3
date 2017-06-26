using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//骑乘坐骑移动
public class CommandRideLeave : ICommand
{
    public override Resp Do()
    {
        CmdHandler<CommandRideLeave> call = Del as CmdHandler<CommandRideLeave>;
        return call == null ? Resp.TYPE_NO : call(this);
    }
}
