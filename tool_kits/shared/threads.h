#ifndef SHARED_THREADS_H_
#define SHARED_THREADS_H_

// thread ids
enum ThreadId
{
	kThreadBegin = -1,//线程ID定义开始
	kThreadUI,			//UI线程（主线程）
	kThreadDatabase,	//写DB线程
	kThreadGlobalMisc,	//全局Misc线程（比如：打开url）
	kThreadDuilibHelper,//Duilib专用，UI辅助线程
	kThreadLiveStreaming,//直播线程
	kThreadScreenCapture,//取屏线程
	kThreadApp,//用户自定义应用线程
	kThreadEnd,//线程ID定义结束
};

#endif // SHARED_THREADS_H_
