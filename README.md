# SRAM-PUF 物理不可克隆函数实现

## 📖 论文引用

**许嘉鑫,孟坤.一种基于应用驱动的物理不可克隆函数分析方法[J].北京信息科技大学学报(自然科学版),2025,40(4):76-82.**

> **注意：** 最新版本的实现请参考 `domain` 分支。

## 📋 项目简介

本项目实现了基于 SRAM 的物理不可克隆函数（Physical Unclonable Function, PUF），提供了一种基于应用驱动的 PUF 分析方法。项目基于 STM32F407 微控制器平台，集成了多种外设驱动和编码算法。

## 🏗️ 系统架构

### 实现路径图

![SRAM PUF实现路径图](https://github.com/user-attachments/assets/4a8b9b13-b58c-467f-912b-c351b5cb34d5)  
_图 1 SRAM PUF 实现路径图_

### 设备示意图

![SRAM PUF设备示意图](https://github.com/user-attachments/assets/ff1dc2f6-3949-44ce-8e70-81d52c423ded)  
_图 2 SRAM PUF 设备示意图_

### 硬件设计

![设备原理图](https://github.com/user-attachments/assets/0a886e37-a05d-49d1-b7db-aa78b513c82a)  
_图 3 设备原理图_

![设备外观示意图](https://github.com/user-attachments/assets/ba9f75a7-ee64-4d68-911c-c9e23256494a)  
_图 4 设备外观示意图_

## 📁 项目结构

```
├── APP/                    # 应用层驱动模块
│   ├── sram/              # SRAM 操作模块
│   ├── rng/               # 随机数生成器
│   ├── HASH/              # 哈希算法实现
│   └── ...                # 其他外设驱动
├── Coder/                 # 编码算法实现
│   ├── BCH/               # BCH 纠错编码
│   ├── sha512/            # SHA-512 哈希算法
│   └── ...                # 其他编码算法
├── Libraries/             # STM32 标准外设库
├── Public/                # 公共系统文件
└── User/                  # 用户应用代码
```

## 🔧 主要功能

- **SRAM PUF 提取**：基于 SRAM 启动状态的物理特征提取
- **纠错编码**：BCH 编码确保密钥稳定性
- **哈希处理**：SHA-512 算法进行数据摘要
- **多外设支持**：集成 OLED 显示、按键输入、LED 指示等
- **实时监控**：支持温度检测和系统状态监控

## 🚀 快速开始

1. **环境要求**

   - Keil MDK-ARM 开发环境
   - STM32F407 开发板
   - ST-Link 调试器

2. **编译运行**

   ```bash
   # 打开项目文件
   Template.uvprojx

   # 编译项目
   Build -> Build Target

   # 下载到开发板
   Flash -> Download
   ```

## 📊 技术特点

- **高可靠性**：采用 BCH 纠错编码技术
- **低功耗设计**：优化的 SRAM 读取策略
- **实时性能**：快速的 PUF 特征提取
- **可扩展性**：模块化的代码架构

## 📄 许可证

本项目遵循开源许可证，具体请参考项目中的许可证文件。

## 👥 贡献者

- 许嘉鑫
- 孟坤

## 📞 联系方式

如有技术问题或合作意向，请通过 GitHub Issues 联系我们。
