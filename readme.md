HTTP-QT



目的很简单

目前来说仅仅只是为了创建一个UI能够免打开浏览器

就能够进行隔壁LANFileshare项目的需求浏览器进行的所有行为



为了实现这个功能 则需要用到cpp的一个开源库 `httplib.h`

鉴于httplib.h是阻塞性库



进度:

基础功能:

0.整体UI实现

~~0.1基础IP/Port输入控制台与连接&断开行为~~

~~0.2 对于项目item的右键菜单~~





~~?1.下载~~

~~2.上传~~

~~2.1 但是现在只是固定上传~~



\[Problem]:

开发相关:

~~0.想把IP控制台单独做出一个隐藏在上方工具栏的Action触发~~

~~可是我现在还不清楚怎么让另外的一个Widget窗口与主窗口进行数据传输或同步。。~~



将MainWindow的ui指针抛出给外部的ui使用

> mainwindow.h 引入 subwindow.h 以正常打开subwindow
>
> 而subwindow.h 引入 ui_mainwindow.h 以获取 mainwindow的所有ui信息
>
> 然后subwindow.h的构造函数 除了基本的QWidget *parent = nullptr
>
> 还要添加 Ui::MainWindow *[ptr]
>
> 
>
> 最后 subwindow.cpp正常引入subwindow.h
>
> 不过在构造函数的实现里 不仅要与构造函数的传参匹配
>
> 其继承 还需要再添加一份 
>
> sub::sub(QWidget\*,Ui::MainWindow\*):QWidget\*,ui(new Ui::sub),\[ptr]([ptr])
>
> 
>
> 外层指代当前操作的ui 而内层指代哪个ui的信息(也就是`ui_*.h`)
>
> 其中 上文所指的nullptr 即空指针的作用都是为了独立打开一个新窗口 而非依靠其他窗口



~~1.1解决ofstream传输中文名称文件乱码~~

因为采用mingw的原因 

目前最直接的解决方法是直接让QT使用gbk编码编译 而非utf-8



使用c++17 引入的 `std::filesystem::u8path`来代替默认fstream的char

当然 wchar同样也可以处理这种事 wchar天生就是来处理这些unicode字符的



然而 兼容性问题 wchar[]数组不管再怎么样 它的兼容性都不可能比得过`std::string`

那为啥不干脆用`u8path`呢?

****

~~3.执行耗时任务时 mainwindow会直接整个卡死~~



需要将任务抛给子线程 主线程继续响应ui响应

透过QFutureWatcher+QEventLoop+QConcurrent::run/map

来确保线程的创建和回收



QFutureWatcher负责线程工作状态的监控 同时增加真正要执行的异步任务

QEventLoop负责将线程推入工作环境

QFutureWatcher执行完后 emit发送 finished信号

最后透过信号与槽机制退出QEventLoop的工作环境



这样软件也不会变成单核战士了(



~~3.1线程与队列任务一对一~~

听起来抽象

但实际上各种成熟的下载器里面都是标配的功能



比如允许的最大线程数下载

也许听起来很奇怪 但实际上在开发里 如果你不对线程的行为加以管控

即使是顶着最大线程数 SetMaxThreadCount() 的限制



这些工作线程会一直几艘任务船来回跳

而不会乖乖的先把手头上的任务处理完 再去处理下一个

这就需要去手动管理这些线程的行为 不要让它们来回跳



但你又不能直接粗暴的使用QFuture::waitforFinished()

要不然整个ui陪你一起卡 那这样做又有啥用？



后来想到 如果一个EventLoop管控全局的线程

​	那么我在这个EventLoop的生命周期内再开一个inside EventLoop 不就好了?

​	结果还真可以 新建了一个实验用的QT环境 试了下可以实现我想的那种效果

​	然后 inside EventLoop 同样也需要connect 对应的 inside FutureWatcher

​	以执行 inside EventLoop::quit()

以重新返回外部的EventLoop循环



而只要在外部的循环 提取mapList的一部分 变为 subList

​	以让内部的循环执行subList

​	最后subList.clear()

回归主eventLoop 寻复往返直到mapList完毕即可



这样就实现了 任务队列的最大值 表现



3.1.1 错误线程结果处理与队列调整



在以前的任务队列都是以你的任务肯定会成功 为前提



那有没有想过 如果某个线程的任务失败了 给你返回个404

那剩下的线程会怎么样呢?





3.2大文件的分块多线程处理与合并

[前提已解锁]~~但是这只是划分了线程 还没有划分好固定数目的线程~~

遇到大文件想分块下载又怎么办呢 

如果一次性选中几百个文件又咋办呢?



4.任务暂停功能

鉴于httplib是一个阻塞性库 我认为。。这个功能恐怕只能换库才能解决

恐怕只能做出任务取消 而非暂停

因为我实在是不敢想断点续传的适配性。。



~~5.主窗口连体退出~~

目前窗口之间的逻辑都是独立的

只有完整退出所以窗口 整个主程序的事件循环才算终止

然而一般来说 主窗口挂了 其他窗口也得跟着挂 然后程序直接终止

解决方案:

重写Mainwinodw的closeEvent功能 然后关联到qAPP->quit()即可

当然也可以先加入QMessageBox 做一个退出确认



~~6.文件的预创建~~
预先创建文件以避免下载目录变更以影响到cliFileDownload函数

看看httplib有没有这种功能 
还是说我得自己先去指定的地方用ofstream空文件 然后通过httplib直接覆盖就完事



****

\[Enchanted]

~~0.增加了直觉性的 快捷键 操作~~

为列表树 增加了  多选操作 delete Enter 等等



1.任务队列提示

Toaster也好 小红点也好 缩略图动画也好 

不管怎么说 必须要整个提示



1.1任务队列 下载速度返回



****

[UI_design]

1.`IP_controlPanel.ui`内的详细设置

希望可以被折叠



2.Qsplitter 划分出来的窗体大小还是没能理解怎么和整个UI进行同步



****

LOW LEVEL

~~1.窗口创建优先级~~

现在还不知道为什么 似乎是因为主窗口的构造函数没完整执行之前 是不会将mainwindow自身先创建出来的

这就导致了主窗口创建出来时 甚至在子窗口之上。。

延迟IP_controlPanel的show事件

直接用QTimer::singleShot()即可 延迟个几百ms完全足够了 后续也可以做一点淡入特效 然后降低延迟时间



1.1淡入显示美化





2.想办法压缩exe大小

该开始想想release版本的事了..



那些内置dll的exe是怎么搞出来的?



****

Debug mode:

1.不知道为什么 有些ui的头部依赖文件必须要待在那个debug工程文件夹 而非源码文件夹

如果放到源码文件夹会提示无法找到更上层的依赖

也许是因为CMake build的时候把依赖关系全甩到那边去了。。



~~2.尝试使用动态dll代替各种编译库 毕竟Debug文件夹真的有点大~~



目标文件编译出来的未经压缩大小(Debug):24.5MB (?)

目标文件编译出来的内置链接库大小(Release):1.273MB

