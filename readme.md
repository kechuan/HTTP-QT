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

2.1 但是现在只是固定上传



Problem:

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

疑似阻塞性库问题

上文提到过的httplib库是 阻塞性 库



一开始开发倒也没问题 直到尝试去用它来处理大文件

直接能将整个mainwindow干无响应不说 还让ui的更新队列拖延到 **下载完成** 之后 才去着手进行更新



无响应这个还能解决 让副线程(async)去处理就好了



但是下载进度显示的延后又是不能接受的

最神秘的是 qDebug 本身却完全不会受这种阻塞性的影响

一点都不会被延时 也不会卡死



而emit/signal机制导致的ui更新则会

难道要让我写一个重头写一个监听事件 然后extern一个外部变量吗。。





****

Debug mode:

1.不知道为什么 有些ui的头部依赖文件必须要待在那个debug工程文件夹 而非源码文件夹

如果放到源码文件夹会提示无法找到更上层的依赖

也许是因为CMake build的时候把依赖关系全甩到那边去了。。



2.尝试使用动态dll代替各种编译库 毕竟Debug文件夹真的有点大