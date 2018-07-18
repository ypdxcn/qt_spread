1  目录结构 
1.1 子目录结构：
  ...CPlusFramwork
  |- Utility 基础模块(.dll/.a)
  |  |- Include 头文件目录
  |  |- Src 源文件目录
  |- NetMgr 网管框架(.dll/.a)
  |  |- Include 头文件目录
  |  |- Src 源文件目录
  |- CommFramwork 通讯框架(.dll/.a)
  |  |- Include 头文件目录
  |  |- Src 源文件目录
  |- FramworkAp	通讯框架接入点(接口协议,共有部分.dll/.a) 
  |  |- Include 头文件目录
  |  |- Src 源文件目录
  |- NetMgrAgent 网管代理，实现的H1/H2接口协议(.dll/.a) 
  |  |- Include
  |  |- Src 源文件目录
  |- Include windows平台外部库包含文件
  |  |- pthread windows平台posix线程库头文件
  |  |- oci windows平台ORACLE OCI头文件
  |- lib windows平台外部库文件
  |  |- pthread windows平台posix线程库库文件
  


1.2 根目录下文件说明
Root_2005.sln  vs2005解决方案文件(包含所有windows平台的应用)
Root_2008.sln  vs2008解决方案文件(包含所有windows平台的应用)
Risk_src.bat  unix平台编译目录源文件拷贝批处理文件
Risk_aix.bat  aix平台make文件拷贝批处理文件
Risk_hpunix.bat hpunix平台make文件拷贝批处理文件
Risk_linux.bat linux平台make文件拷贝批处理文件


2 编译说明
2.1 windows平台
   1 打开VS2005
   2 加载Root_2005.sln解决方案
   3 根据需要编译各个应用工程

2.1.1 工具基础库(Utility)编译说明
    工具基础库工程名Utility。
    编译成功后输出文件:Utility.dll,Utility.lib
    编译成功后输出目录:bin目录 
    注意：winodws平台posix线程库dll/lib的目录位置
    
2.1.2 系统监控接口库(NetMgr)编译说明
    系统监控接口库工程名NetMgr，依赖工程：Utility，已设置好依赖关系，
编译本工程会自动编译依赖工程。
    编译成功后输出文件:NetMgr.dll,NetMgr.lib
    编译成功后输出目录:bin目录 
    注意：winodws平台posix线程库dll/lib的目录位置
    
2.1.3 通讯框架库(CommFramwork)编译说明
    通讯框架库工程名CommFramwork，依赖工程：Utility，NetMgr，已设置好依赖关系，
编译本工程会自动编译依赖工程。
    编译成功后输出文件:CommFramwork.dll,CommFramwork.lib
    编译成功后输出目录:bin目录 
    注意：winodws平台posix线程库dll/lib的目录位置
         
2.1.4 ，基本报文接口库(FramworkAp)编译说明
    通讯框架库工程名FramworkAp，依赖工程：Utility，NetMgr，CommFramwork，
已设置好依赖关系，编译本工程会自动编译依赖工程。
    编译成功后输出文件:FramworkAp.dll,FramworkAp.lib
    编译成功后输出目录:bin目录 
    注意：winodws平台posix线程库dll/lib的目录位置
             
2.1.5 系统监控实现库(NetMgrAgent)编译说明
    通讯框架库工程名NetMgrAgent，依赖工程：Utility，NetMgr，CommFramwork，FramworkAp，
已设置好依赖关系，编译本工程会自动编译依赖工程。
    编译成功后输出文件:NetMgrAgent.dll,NetMgrAgent.lib
    编译成功后输出目录:bin目录 
    注意：winodws平台posix线程库dll/lib的目录位置
     
2.2 Linux平台
1 执行Risk_src.bat(假设存在D盘，否则请修改批处理文件)
2 执行Risk_linux.bat(假设存在D盘，否则请修改批处理文件)
3 使用FTP将D:\Unix_Linux_ver目录下的所有文件上传到Linux编译环境，假设为Root目录
4 进入Root目录
5 执行make命令则自动编译风控及相关库
    注意：工具基础库(Utility)，通讯框架库(CommFramwork)，系统监控库(NetMgr，NetMgrAgent)，
基本报文接口库(FramworkAp)编译输出文件为静态库.a，输出目录为用户目录 ~/lib。

2.3 HP_Unix平台(已在g++ 4.2平台上成功编译，请确保已经正确安装gcc)
1 执行Risk_src.bat(假设存在D盘，否则请修改批处理文件)
2 执行Risk_hpunix.bat(假设存在D盘，否则请修改批处理文件)
3 使用FTP将D:\Unix_Linux_ver目录下的所有文件上传到HP_Unix编译环境，假设为Root目录
4  进入Root目录
5 执行gmake -f mkhp.gcc命令则自动编译风控及相关库
    注意：工具基础库(Utility)，通讯框架库(CommFramwork)，系统监控库(NetMgr，NetMgrAgent)，
基本报文接口库(FramworkAp)编译输出文件为静态库.a，输出目录为用户目录 ~/lib。

2.4 AIX平台(已使用xlC_r 6.0成功编译，请确保已经正确安装IBM对应版本的编译环境)
1 执行Risk_src.bat(假设存在D盘，否则请修改批处理文件)
2 执行Risk_aix.bat(假设存在D盘，否则请修改批处理文件)
3 使用FTP将D:\Unix_Linux_ver目录下的所有文件上传到AIX编译环境，假设为Root目录
4 进入Root目录
5 执行make -f mk.vac命令则自动编译风控及相关库
    注意：工具基础库(Utility)，通讯框架库(CommFramwork)，系统监控库(NetMgr，NetMgrAgent)，
基本报文接口库(FramworkAp)编译输出文件为静态库.a，输出目录
为用户目录 ~/lib。

3 二进制文件配套说明        
    Utility.dll
    NetMgr.dll
    CommFramwork.dll
    FramworkAp.dll
    NetMgrAgent.dll
    



