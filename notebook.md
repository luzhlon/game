# cocos 使用笔记
## 坐标问题：
> - GLView::getFrameSize() 获取的是分辨率大小，而不是cocos坐标系中的大小。应该用GetVisibleSize()
-

## 构建问题:
> - cocos new 时要加-e ENGINE_PATH 选项，这样快而且不易出错。

## Cocos2dx 3.7源代码错误
> CCSSceneReader.h中没包含CocoLoader.h

## Android 构建
> - 资源的名字不能有汉字、空格等字符

## 好的教程
> -  [cocos2dx瓦片地图](http://www.cnblogs.com/liuzhi/p/4017674.html)
- [cocos2dx屏幕适配](http://blog.csdn.net/chinahaerbin/article/details/39586281)
- [cc2dx官方帮助](http://www.cocos.com/doc/article)

## Windows远程连接的问题
> -RDP上的应用程序使用客户端电脑的OpenGL驱动而不是用服务端来绘制
> 然而我用的Linux怎么可能满足的了Windows上应用的需求？在百度上找到一个解决方案，不过是英文的：
> > 原文我不记得了，反正大体意思是在服务端上先开一个OpenGL应用，这样系统就会用服务端的OpenGl驱动，
> > 然后再远程连接Windows，就可以了


## VS2013 不能正确处理UTF-8无BOM编码的文件
> 从Linux移植到Windows上的源代码文件(这些文件之前经过了iconv处理)，在VS2013下编译出错，费了好大劲才找到错误，将UTF-8无BOM编码的文件转换成UTF-8的就好了(用Notepad ++)