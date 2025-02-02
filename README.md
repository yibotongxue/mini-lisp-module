# `Mini-Lisp` 解释器

这个项目作为 C++20 模块的一个实践，实现了[北京大学软件设计实践](https://pku-software.github.io/)的[大作业](https://pku-software.github.io/project-doc/)的 `Mini-Lisp` 解释器，当前实现到了 `Lv.6` ，并通过了 `Lv.2` 到 `Lv.6` 的所有测试，但代码重构后无法兼容测试库 `rjsj` ，我们修改了测试库作为一个模块库，但没有改变测试用例。

## 编译运行

需要安装有 `clang` 和 `llvm` ，最好是最新的版本，我使用的版本是 `19.1.7` ，这个版本可以通过编译，以下的安装流程在 `Ubuntu` 平台安装的是 `19.1.7` 版本，如果要安装其他版本可以类似进行。安装流程由[Kimi](https://kimi.ai)提供。

首先下载预编译的压缩包，使用如下命令

```bash
wget https://github.com/llvm/llvm-project/releases/download/llvmorg-19.1.7/LLVM-19.1.7-Linux-X64.tar.xz
```

然后解压缩

```bash
tar -xf LLVM-19.1.7-Linux-X64.tar.xz
```

移动到 `/usr/local`

```bash
sudo mv LLVM-19.1.7-Linux-X64 /usr/local
```

添加 LLVM 19 的路径到系统环境变量

```bash
echo "export PATH=/usr/local/LLVM-19.1.7-Linux-X64/bin:\$PATH" | sudo tee /etc/profile.d/llvm-19.sh
source /etc/profile.d/llvm-19.sh
```

检查是否安装成功

```bash
clang --version
```

如果安装成功，应该得到输出

```
clang version 19.1.7 (/home/runner/work/llvm-project/llvm-project/clang cd708029e0b2869e80abe31ddb175f7c35361f90)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /usr/local/LLVM-19.1.7-Linux-X64/bin
```

然后可以将 `source /etc/profile.d/llvm-19.sh` 写入 `.bashrc` 中，这样就不用每次使用都要输入这个命令。

如果安装不成功，请自行检查上面的步骤是否有误，特别注意如果你安装的版本不是 `19.1.7` ，那么你需要修改命令中的版本号。如果版本号正确，也可以检查一下链接是否还有效等。以及注意你的平台可能不是 `Linux-X64` ，你需要对应的修改。具体的可以查看[Release](https://github.com/llvm/llvm-project/releases)页面。

然后使用如下命令配置

```bash
xmake f --toolchain=llvm --runtimes=c++_shared -c
```

使用如下命令构建、编译

```bash
xmake
```

使用如下命令运行

```bash
xmake run mini_lisp
```

## 相关说明

### 编译器

C++20 引入了模块编译，改变了 C++ 的编译链接模式，可以说是带来了革命性的变革，当前主流编译器对模块的支持参差不齐， `Visual Studio` 的编译器支持是比较好的， `Clang` 的支持也很好，而 `GCC` 相对没有十分完善的支持。如果在 `Windows` 平台，使用 `Visual Studio` 是最好的选择；如果使用 `macOs` ， `Clang` 是原生的工具链，使用起来也比较方便；如果在 `Linux` 平台，以 `Ubuntu` 为例， `GCC` 是原生工具链，使用 `Clang` 并不方便，但还是推荐使用 `Clang` ，但需要注意的是， `llvm` 包最好自己安装，而不是使用系统安装， `apt` 获取的可能没有模块支持。

### 构建系统

这个项目使用了 `XMake` 作为构建系统， `XMake` 在支持 C++20模块方面做的很好，也比 `CMake` 更为易用，学习难度较低，推荐使用 `XMake` 作为构建系统。

### 编译速度

模块编译一个重要的作用就是提高了编译速度，但本项目并没有体现出来这方面的优势，可能是我的模块组织并不好。我的实验中，使用模块编译从头编译需要 5.4s ，而没有使用模块编译需要 3.545s 。如果对 `token.cppm` 或者 `token.h` 进行改动（我添加了一个空白行），重新编译，模块编译需要 3.534s ，而没有使用模块需要 2.662s 。
