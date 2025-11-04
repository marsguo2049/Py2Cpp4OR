# Py2Cpp4OR

Intelligent agent for translating Python optimization models (gurobipy/pyomo) to efficient C++ code, with iterative learning capabilities.

## Background

**Problem**: Python model building is too slow for large-scale OR problems (100s+ for model construction). While alternatives exist (HPC distributed computing, Cython), C++ offers dramatic speedup (<1s) and aligns with supervisor's expertise.

**Challenge**: Initial AI-generated C++ code is overly complex, difficult to review, and potentially error-prone. This project aims to build a reliable agent that produces simple, stable, and safe C++ code.

## Purpose

Create a controlled translation framework that:
- Generates clean, minimal C++ code (only .cpp and .h files)
- Maintains code readability for human review
- Learns from feedback to improve translation quality
- Bridges Python prototyping and C++ production

## Architecture

**Iterative Learning Framework:**
```
agent.md → Execute → reasoning_traces/ → Feedback → learning_log.md → Update agent.md
```

**Repository Structure:**
```
.
├── agent.md              # Agent operational rules (LLM context)
├── learning_log.md       # Experience summary & gap analysis
├── reasoning_traces/     # Distilled decision logs
├── examples/             # Python-C++ translation pairs
└── tests/                # Validation cases
```

## Current Status

✅ Model translation complete (pyomo → C++ in function.cpp)  
🔄 Refining data I/O approach (numpy/pandas → readline)  
🔄 Simplifying code structure (removing unnecessary complexity)

## Workflow

1. **Execute**: Agent reads `agent.md` and translates models
2. **Record**: Log key decisions in `reasoning_traces/`
3. **Learn**: Extract patterns into `learning_log.md`
4. **Evolve**: Refine `agent.md` based on experience

## Philosophy

This repository stores refined knowledge and serves as daily progress tracker. Local development (Cursor/Claude Code/Codex) handles experimentation; GitHub contains the distilled best practices.

## License

MIT

# Py2Cpp4OR

智能Agent，用于将Python优化模型（gurobipy/pyomo）翻译为高效的C++代码，具备迭代学习能力。

## 项目背景

**问题**：大规模运筹优化问题中，Python建模耗时过长（100秒以上）。虽然存在其他方案（HPC分布式计算、Cython），但C++能带来显著加速（<1秒），且符合导师技术栈。

**挑战**：初期AI生成的C++代码过于复杂，难以审查，存在潜在错误风险。本项目旨在构建可靠的Agent，生成简洁、稳定、安全的C++代码。

## 项目目标

建立可控的翻译框架：
- 生成简洁的C++代码（仅.cpp和.h文件）
- 保持代码可读性，便于人工审查
- 从反馈中学习，持续优化翻译质量
- 连接Python原型开发与C++生产部署

## 架构设计

**迭代学习框架：**
```
agent.md → 执行 → reasoning_traces/ → 反馈 → learning_log.md → 更新agent.md
```

**仓库结构：**
```
.
├── agent.md              # Agent操作规范（LLM上下文）
├── learning_log.md       # 经验总结与差距分析
├── reasoning_traces/     # 精炼的决策日志
├── examples/             # Python-C++翻译对照
└── tests/                # 验证案例
```

## 当前进展

✅ 模型翻译完成（pyomo → C++，位于function.cpp）  
🔄 优化数据I/O方式（numpy/pandas → readline）  
🔄 简化代码结构（移除不必要的复杂性）

## 工作流程

1. **执行**：Agent读取`agent.md`并翻译模型
2. **记录**：在`reasoning_traces/`中记录关键决策
3. **学习**：提炼模式到`learning_log.md`
4. **演进**：基于经验完善`agent.md`

## 设计理念

本仓库存储精炼的知识，作为每日进展追踪工具。本地开发（Cursor/Claude Code/Codex）处理实验性工作；GitHub保存提炼的最佳实践。

## 开源协议

MIT
