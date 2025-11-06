# Py2Cpp4OR

智能Agent，用于将Python优化模型（gurobipy/pyomo）翻译为高效的C++代码，具备迭代学习能力。

Intelligent agent for translating Python optimization models (gurobipy/pyomo) to efficient C++ code, with iterative learning capabilities.

## 项目背景 | Background

**问题 | Problem**: 大规模运筹优化问题中，Python建模耗时过长（100秒以上）。虽然存在其他方案（HPC分布式计算、Cython），但C++能带来显著加速（<1秒），且符合导师技术栈。

Python model building is too slow for large-scale OR problems (100s+ for model construction). While alternatives exist (HPC distributed computing, Cython), C++ offers dramatic speedup (<1s) and aligns with supervisor's expertise.

**挑战 | Challenge**: 初期AI生成的C++代码过于复杂，难以审查，存在潜在错误风险。本项目旨在构建可靠的Agent，生成简洁、稳定、安全的C++代码。

Initial AI-generated C++ code is overly complex, difficult to review, and potentially error-prone. This project aims to build a reliable agent that produces simple, stable, and safe C++ code.

## 项目目标 | Purpose

建立可控的翻译框架 | Create a controlled translation framework that:
- 生成简洁的C++代码（仅.cpp和.h文件） | Generates clean, minimal C++ code (only .cpp and .h files)
- 保持代码可读性，便于人工审查 | Maintains code readability for human review
- 从反馈中学习，持续优化翻译质量 | Learns from feedback to improve translation quality
- 连接Python原型开发与C++生产部署 | Bridges Python prototyping and C++ production

## Python → C++ 关键差异 | Key Python → C++ Differences

### 数据导入：readline方法 | Data I/O: readline Approach
**Python**: 高级库（pandas/numpy） | High-level libraries (pandas/numpy)  
**C++**: 原生`ifstream`逐行读取 | Raw `ifstream` line-by-line reading

**为什么用"笨"方法 | Why "simple" approach**:
- 透明可控，便于调试 | Transparent and debuggable
- 无外部依赖（避免Eigen/Armadillo） | No external dependencies
- 符合导师代码风格 | Matches supervisor's coding style
- 完全控制数据解析 | Full control over data parsing

### 头文件（.h） | Header Files (.h)
**Python**: 类定义和实现一起 | Class definition and implementation together  
**C++**: 声明(.h)和实现(.cpp)分离 | Separated declaration (.h) and implementation (.cpp)

**分离的目的 | Purpose of separation**:
- 接口文档 | Interface documentation
- 编译效率（修改.cpp不需重编译依赖者） | Compilation efficiency
- 通过header guard避免重复定义 | Prevent duplicate definitions

## 架构设计 | Architecture

**迭代学习框架 | Iterative Learning Framework:**
```
agent.md → 执行 | Execute → reasoning_traces/ → 反馈 | Feedback → learning_log.md → 更新 | Update agent.md
```

**仓库结构 | Repository Structure:**
```
.
├── agent.md              # Agent操作规范（LLM上下文） | Agent operational rules (LLM context)
├── learning_log.md       # 经验总结与差距分析 | Experience summary & gap analysis
├── reasoning_traces/     # 精炼的决策日志 | Distilled decision logs
├── USER_GUIDE.md         # 用户数据准备指南 | User data preparation guide
├── examples/             # Python-C++翻译对照 | Python-C++ translation pairs
└── tests/                # 验证案例 | Validation cases
```

## 当前进展 | Current Status

✅ 模型翻译完成 | Model translation complete (pyomo → C++ in function.cpp)  
🔄 优化数据I/O方式 | Refining data I/O approach (numpy/pandas → readline)  
🔄 简化代码结构 | Simplifying code structure (removing unnecessary complexity)

## 工作流程 | Workflow

1. **执行 | Execute**: Agent读取`agent.md`并翻译模型 | Agent reads `agent.md` and translates models
2. **记录 | Record**: 在`reasoning_traces/`中记录关键决策 | Log key decisions in `reasoning_traces/`
3. **学习 | Learn**: 提炼模式到`learning_log.md` | Extract patterns into `learning_log.md`
4. **演进 | Evolve**: 基于经验完善`agent.md` | Refine `agent.md` based on experience

## 设计理念 | Philosophy

本仓库存储精炼的知识，作为每日进展追踪工具。本地开发（Cursor/Claude Code/Codex）处理实验性工作；GitHub保存提炼的最佳实践。

This repository stores refined knowledge and serves as daily progress tracker. Local development (Cursor/Claude Code/Codex) handles experimentation; GitHub contains the distilled best practices.

## 开源协议 | License

MIT