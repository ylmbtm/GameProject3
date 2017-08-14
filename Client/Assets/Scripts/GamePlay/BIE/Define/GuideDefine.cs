using UnityEngine;
using System.Collections;

namespace BIE
{
    public enum EGuideType
    {
        TYPE_PATH                      =  1,  //路径任务  
        TYPE_OPERATION                 =  2,  //强制UI操作
        TYPE_DIALOGUE                  =  3,  //对话指引
        TYPE_CG                        =  4,  //播放视频
        TYPE_PLOT                      =  5,  //播放剧情
    }

    public enum EGuideState
    {
        TYPE_NONE                      =  0,  
        TYPE_ENTER                     =  1,  //进入引导程序
        TYPE_EXECUTE                   =  2,  //引导任务执行中
        TYPE_FINISH                    =  3,  //完成引导
    }

    public enum EGuideUIOperationType
    {
        TYPE_CLICK                     =  0,
        TYPE_PRESS                     =  1,
        TYPE_SWAP                      =  2,
        TYPE_JOYSTICK                  =  3,
    }

    public enum EGuideConditionRelation
    {
        AND                            =  0,  //所有条件都满足
        OR                             =  1   //只要一个条件满足即可
    }

    public enum EGuideRowType
    {
        TYPE_NONE                      =  0,
        TYPE_UP                        =  1,  //上方
        TYPE_UP_RIGHT                  =  2,  //右上
        TYPE_RIGHT                     =  3,  //右边
        TYPE_DOWN_RIGHT                =  4,  //右下
        TYPE_DOWN                      =  5,  //下方
        TYPE_DOWN_LEFT                 =  6,  //左下
        TYPE_LEFT                      =  7,  //左边
        TYPE_UP_LEFT                   =  8,  //左上
    }

    public enum EGuideBoardType
    {
        TYPE_NONE                      =  0,   
        TYPE_RECTANGLE                 =  1,   //矩形
        TYPE_CIRCLE                    =  2,   //圆形
    }

    public enum EGuideGirlPos
    {
        TYPE_NONE                      =  0,
        TYPE_LEFT                      =  1,
        TYPE_RIGHT                     =  2,
    }

    public enum EGuideCondition
    {
        CheckItemAmount                =  1,   //检查物品数量
        CheckPlayerLevel               =  2,   //检查玩家等级
        CheckPlayerVip                 =  3,   //检查玩家Vip
        CheckOpenUI                    =  4,   //检查打开UI
        CheckHideUI                    =  5,   //检查关闭UI
        CheckMonsterAppear             =  6,   //怪物出现
        CheckMonsterHP                 =  7,   //检查怪物血量
        CheckScene                     =  8,   //检查场景
        CheckWeekday                   =  9,   //检查星期数
        CheckPlayerHP                  =  10,  //检查玩家HP
        CheckPlayerHPPercent           =  11,  //检查玩家HP百分比
    }
}
