# PQ-secure_FHE_ML_PDTE_FiLIP后量子安全的机器学习算法
本项目为基于全同态加密库[FINAL](https://github.com/KULeuven-COSIC/FINAL)和FiLIP流密码实现的机器学习决策树算法以及应用。

## 项目背景
机器学习已成为大数据、物联网和云计算等领域的核心技术。随着数据隐私和安全性的日益重要，传统的机器学习方法在处理敏感数据时受到诸多限制。

我们可以将隐私数据进行全同态加密，将全同态密文应用于机器学习上，由于全同态特殊的数学结
构，使得我们得到密文解密后，与直接将明文进行相同的操作得到的结果相同，因此实现机器学习
中数据的“可用而不可见”。但目前主流的全同态加密方案存在安全性问题、密文扩展大、运行效率
低等问题，使得其落实在现实应用中存在一些困难。为了解决这些问题，我们提出了一种高效的全同态加密方案，并实现了基于该全同态加密方案
的隐私决策树评估系统。该系统可以在密文下正确高效进行数据分类，极大程度的实现了数据的“可
用而不可见”。

## 项目简介
我们研究了基于安全参数下 NTRU 的 FHE 方案——FINAL，它选择的 NTRU 参数位于“延
展攻击”范围之外，因此它能够安全且准确的实现全同态加密。然而，目前的方案仍存在密文扩展
大、运行效率低、噪声增长快的缺点。为了解决这些缺点，我们引入了 FiLIP 流密码、一个新的小
工具分解算法、转用 LWE 来进行自举以及用 GPU 对相关部件进行加速等，从而使其的性能等各
个方面得到了显著提升。

与原有的方案相比，我们通过引入一个新的小工具分解算法，使得我们的噪声增长减小了 10 余
倍；通过在我们的系统引入 FiLIP 流密码，大大减少了因 FHE 密文扩展而导致的通信开销；通过
CUDA 对 FFT 进行加速，使得自举过程比原方案提升了约 50%，密钥生成过程提升了约 40%∼60%，
加解密提升了约 80%；最后通过将 NTRU 密文转换为 LWE 密文进行自举，使得密钥生成几乎无需
时间开销，自举过程提升了约 70%, 加密提升了了约 95%。

我们基于该 FHE 方案实现了隐私决策树评估系统，其可以处理密态下的数据，实现了对客户
端的隐私保护，在医疗、金融等领域具有广阔的应用前景。此外，我们的底层方案在其他方面也具
有广阔的应用前景。

## 环境要求
编译源码运行：
- Linux
- C/C++
- GCC
- CPU/Memory:推荐8C/8G以上

可视化客户端搭建：
- Qtcreator 6.0 以上

FINAL优化版本：
- 硬件：NVIDIA显卡（30系以上）
- 软件：CUDA
## 部署
### 安装环境依赖
该项目基于FINAL实现，已包含其代码。
但仍需安装FINAL的依赖项：NTL\GNU GMP\FFTW

由于我们的FINAL优化是基于CUDA进行的，因此还需要安装CUDA环境
 
安装FFTW

```bash
sudo apt update
sudo apt install libfftw3-dev
```

安装GNU GMP

```bash
sudo apt update
sudo apt install libgmp-dev
```

安装NTL
```bash
sudo apt install libntl-dev
```

安装nvidia-driver
```bash
sudo apt install nvidia-driver-470
```

安装CUDA
```bash
wget https://developer.download.nvidia.com/compute/cuda/11.4.0/local_installers/cuda_11.4.0_470.42.01_linux.run
sudo sh cuda_11.4.0_470.42.01_linux.run
# 添加环境变量
sudo vim ~/.bashrc
export PATH=/usr/local/cuda-11.4/bin:$PATH
source ~/.bashrc
# 验证安装成功
nvcc --version
```
### 获取源码
```bash
git clone https://github.com/bbbbhrrrr/PQ-secure_FHE_ML_PDTE_FiLIP.git
```
### 项目结构
├── build 构建Qt可视化界面

├── gold PDTE+FiLIP

├── icons Qt可视化界面资源

├── style Qt可视化界面资源

├── other Qt可视化界面资源

├── assets 仓库资源文件

## 运行
编译源码运行只需在gold/src/cpp_pdte_transciphering运行make即可。可在源码中改写对应 test_ 文件测试各项功能。

使用Qt打开该项目进行build即可生成可视化界面。

*注：
- 根据需要修改模型等相关文件路径。
- gold/src/cpp_pdte_transciphering/data提供各模型测试数据。
- testdialog.h/testdialog.cpp等文件为Qt可视化界面相关文件。

## 效果预览
- 主界面
![主界面](assets/main.png)

- ML_PDTE
![ML](assets/ml.png)

- FiLIP
![FILIP](assets/filip.png)








