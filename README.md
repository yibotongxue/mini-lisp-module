# `Mini-Lisp` 解释器

这是使用 C++20 模块实现的 `Mini-Lisp` 解释器，关于 `Mini-Lisp` 解释器可以参考[这里](https://pku-software.github.io/project-doc/)。

## 编译运行

需要安装有 `clang` 和 `llvm` ，在 `Ubuntu` 系统可以用如下命令安装

```bash
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 18
```

然后使用如下命令配置

```bash
xmake f --toolchain=clang --sdk=/usr/lib/llvm-18
```

使用如下命令构建、编译

```bash
xmake
```

使用如下命令运行

```bash
xmake run mini_lisp
```
