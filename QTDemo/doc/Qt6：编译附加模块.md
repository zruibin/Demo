

```
编译QtBase
mkdir build && cd build
cmake -GNinja -DCMAKE_INSTALL_PREFIX={qt安装的目录} ..
cmake --build . --parallel
cmake --install .

编译QtMultimedia
mkdir build && cd build
cmake -GNinja -DCMAKE_INSTALL_PREFIX={qt安装的目录} ..
cmake --build . --parallel
cmake --install .

目标工程中find_package通过CMAKE_PREFIX_PATH引入
cmake -GXcode -DCMAKE_PREFIX_PATH={qt安装的目录} ..
或CMakeLists.txt中设置
set(CMAKE_PREFIX_PATH {qt安装的目录})
如：set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/deps/")
```

```
Qt: https://download.qt.io/official_releases/qt/6.5/6.5.0/submodules/
QtBase: https://download.qt.io/official_releases/qt/6.5/6.5.0/submodules/qtbase-everywhere-src-6.5.0.tar.xz
QtShadertools: https://download.qt.io/official_releases/qt/6.5/6.5.0/submodules/qtshadertools-everywhere-src-6.5.0.tar.xz
QtMultimedia: https://download.qt.io/official_releases/qt/6.5/6.5.0/submodules/qtmultimedia-everywhere-src-6.5.0.tar.xz
```

> 注意：下载.xz格式

<hr/>

#### 原文：[用VS Code搞Qt6：编译附加模块](https://www.1024sou.com/article/1044967.html)

> **使用qt-configure-module方式编译QtMultimedia可能会报错，可使用以上直接cmake方式**

上一次水文中，老周所介绍的是编译 Qt 的基础模块—— qtbase。一次性编译所有代码可以一劳永逸，但体积相当大，编译时间较长，CPU负载大发热大，风扇转得猛，电费交得多。因此老周更喜欢分开来编译。

qtbase 模块已经能完成一般的 Qt 开发了，毕竟它包含了铁三角—— Core、Gui、Widgets。有这三个硬汉到场，Qt应用就能运行起来。而附加模块是为了特殊需要的，比如 OpenGL。当要用到某模块时再增量编译，这样逼格更高，也省事。

本文老周以编译多媒体模块 qtmultimedia 为例，演示一下如果编辑附加模块。编译附加模块的前提是你已编译并安装好 qtbase 模块（就是老周前一篇水文中说的）。

在编译之前，老周要先说明一个事：在编译qtbase模块时，默认它会编译为 release 模式，即省去一些调试符号文件，使体积更小。但老周通过试验发现，**如果qtbase模块在执行 configure 脚本时没有明确加上 -release 参数的话，那附加模块会编译为 debug 模式的。**所以，如果你之前在编译 qtbase 模块时没有加 -release 参数，那不妨重新编译一下，反正单个模块编译也很快。

所以，配置命令如下：

```
configure -prefix G:\Kits\Qt6\installed -release
```

之后就是跟前面说的一样，`cmake --build . --parallel、cmake --install .`。

编译附加模块无需重新配置，而是通过一个脚本来设置参数。这个脚本位于你最终的安装路径中，比如，老周的是 `G:\Kits\Qt6\installed`，在 bin 子目录下，有个名为 `qt-configure-module` 的脚本。Linux 上无后缀，Windows 上为 .bat。

用法也很简单，直接输入：`qt-configure-module <附加模块源码路径> `。

只有源代码路径是必须参数，之后是可选参数。这些参数是什么取决于你正在编译的模块——也就是说每个模块的选项不同。可以在模块的源码目录下找到一个叫 config_help.txt 的文本文件，里面会有选项说明。比如，qtmultimedia 模块的选项说明如下：

```
Multimedia options:

  -pulseaudio .......... Enable PulseAudio support [auto] (Unix only)
  -alsa ................ Enable ALSA support [auto] (Unix only)
  -no-gstreamer ........ Disable support for GStreamer
  -gstreamer [version] . Enable GStreamer support [auto]
                         With no parameter, 1.0 is tried first, then 0.10.
  -evr ................. Enables EVR in WMF [auto]
```

 这些选项没有很详细的说明，想弄明白的话多数要看脚本代码。当然，没兴趣的话就不用看了，其实这些选项一般不用去配置。

接下来，咱们开始干正事。要编译 qtmultimedia 模块，还要下载 qtshadertools 模块。因为 qtmultimedia 依赖 qtshadertools。说白了，咱们要先编译并安装 qtshadertools 模块。

怎么下载就不说了，还是和上次一样，我们先建一些特定的目录，各司其责，这样好管理。

源码目录：`G:\Kits\Qt6\src`

生成目录（执行build）：`G:\Kits\Qt6\build`

安装目录（最终成品）：`G:\Kits\Qt6\installed`

再次强调下，要以 release 模式编译，qtbase 模块要加 -release 参数。假设你已经编译且安装好 qtbase 模块。在 G:\Kits\Qt6\installed\bin 目录下会有相关的二进制文件。

1、打开 x64 Native Tools Command Prompt for VS 2022（Linux 下不需要）

2、CD 到 build 目录下。cmake 命令在这个目录下运行，就会将文件输出到该目录。

```
cd G:\Kits\Qt6\build
```

3、执行 qt-configure-module 脚本，配置 qtshadertools 模块。

```
G:\Kits\Qt6\installed\bin\qt-configure-module.bat G:\Kits\Qt6\src\qtshadertools
```

4、老样子，先 build 后 install。

```
cmake --build .
cmake --install .
```

5、现在，qtshadertools 模块已就绪。注意，一定要执行 install，这样编译 qtmultimedia 模块时，才会找到相关的库。

-------------------------------------------------------------------------------------------

下面该到 qtmultimedia 模块了。把 build 目录中的文件全删除，确保当前目录仍然是 build。

1、配置 qtmultimedia 模块。

```
G:\Kits\Qt6\installed\bin\qt-configure-module G:\Kits\Qt6\src\qtmultimedia
```

2、一样，执行这两条 cmake 命令。

```
cmake --build .
cmake --install .
```

>【注意：这些 cmake 命令的最后都有个点号“.”，表示当前目录，即 build 目录】

好了，多媒体模块 qtmultimedia 编译成功了。再次回到 installed\bin 目录，就能看到它了。

有个带“Widgets”结尾的，它包含 UI 元素（也叫控件），主要是用来呈现视频，要不然用户只能看个寂寞。

========================================================

编译完成后，我们要检验一下是否正确。写一个示例播放个视频试试。

在 VS Code 中随便打开一个目录作为工作区，然后建一个文本文件 CMakeLists.txt。

```
cmake_minimum_required(VERSION 3.10.0)
project(testApp LANGUAGES CXX)

# 导入库
find_package(Qt6 REQUIRED COMPONENTS
            Core 
            Gui 
            Widgets 
            Multimedia 
            MultimediaWidgets)
# 设置变量
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# 添加源文件并链接库
add_executable(testApp main.cpp)
target_link_libraries(testApp PRIVATE
            Qt6::Core
            Qt6::Gui
            Qt6::Widgets
            Qt6::Multimedia
            Qt6::MultimediaWidgets
)
```

除了铁三角，还要导入 Multimedia 和 MultimediaWidgets 两个库。

接着新建代码文件 main.cpp，以下是完整的 C++ 代码。

```
#include <QApplication>
#include <QWidget>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QUrl>
#include <QAudioOutput>

int main(int argc, char* argv[]) 
{
    QApplication app(argc, argv);

    // 要播放的文件
    QUrl file = QUrl::fromLocalFile("D:\\大山的女儿\\大山的女儿第1集.mp4");
    // 播放器
    QMediaPlayer *player=new QMediaPlayer;
    // 设置文件源
    player -> setSource(file);
    // 设置音频输出
    QAudioOutput *audoutput = new QAudioOutput;
    // 音量
    audoutput -> setVolume(0.5F);
    player -> setAudioOutput(audoutput);

    // 播放控件
    QVideoWidget vd;
    // 将此控件设置为视频输出
    player -> setVideoOutput(&vd);
    // 设置窗口位置
    vd.setGeometry(612, 440, 725, 540);

    // 显示窗口
    vd.show();
    // 开始播放
    player -> play();

    return app.exec();
}
```

file 变量调用静态方法 fromLocalFile 加载本地视频文件，你需要改成你自己的文件路径。

哦，对了，Qt 这个媒体库所支持的解码器是根据系统所安装的解码器来决定的，如果想播放更多格式的视频，可以安装解码包，比如著名的 K-Lite Codec Pack。说白了，WMP 能播放的它就能播放。不要说人家暴风、 PotPlayer 啥都能播放，那些是内置的解码器，.dll 是在安装目录下的，不是安装到系统中的。

另外，在 QMediaPlayer 实例设置 Source 后，不要忘了设置 QAudioOutput，不设置的话，只有画面没有声音的。device 属性不需要设置，它会查找系统默认的音频输出设备。音量可以适当设置一下。

注意音量的值是 float 类型，值在 0 到 1，也就是说，100% 音量是 1.0。官方文档在概览小节中给的示例是错误的，音量值不是 0 - 100，而是 0 - 1。

setVideoOutput 方法引用 QVideoWidget 实例，表示将视频画面输出到这个控件上，不然，啥都看不见。

最后，运行程序，如果看到视频播放，并且有声音，那就OK了。前提是你要用系统能播放的格式。

好了，这个演示就到这儿了，其他模块的编译也是一样的操作。重点是知道用 qt-configure-module 脚本就行了。

完结，收工，开饭。