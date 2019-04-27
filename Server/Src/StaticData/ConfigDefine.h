#ifndef __CONFIG_DEFINE_H__
#define __CONFIG_DEFINE_H__

enum ETaskEvent
{
	TE_NONE = 0,
	TE_PASS_MAIN_COPY, //挑战主线
};


enum ETaskStatue
{
	TASK_UNFINISHED = 0, //! 未完成
	TASK_FINISHED, //! 已完成
	TASK_ONGOING //! 已领取
};


#endif //__CONFIG_DEFINE_H__