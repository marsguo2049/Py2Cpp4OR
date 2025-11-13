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
├── classes_report.md     # 类结构分析与报告 | Class structure analysis & reports
├── USER_GUIDE.md         # 用户数据准备指南 | User data preparation guide
├── CLAUDE.md             # Claude Code使用指导 | Claude Code usage guidance
├── .claude/agents/       # 专门化Agent配置 | Specialized agent configurations
│   ├── paramy.md         # 参数分析专家Agent (orange/Sonnet) | Parameter analysis expert agent
│   ├── classie.md        # 类分析专家Agent (purple/Sonnet) | Class analysis expert agent
│   ├── convertd.md       # 数据转换专家Agent (teal/Sonnet) | Data conversion expert agent
│   ├── heady.md          # 代码生成专家Agent (blue/Sonnet) | Code generation expert agent
│   ├── dataie.md         # 数据读取专家Agent (green/Sonnet) | Data reading expert agent
│   └── mody.md           # 模型翻译专家Agent (green/Sonnet) | Model translation expert agent
├── examples/             # Python-C++翻译对照 | Python-C++ translation pairs
├── tests/                # 验证案例 | Validation cases
└── *.h                   # 生成的C++头文件 | Generated C++ header files
```

## 当前进展 | Current Status

### ✅ 完成的里程碑 | Completed Milestones

**智能Agent系统 | Intelligent Agent System:**
- ✅ **完整Agent生态系统**: 6个专门化Agent覆盖从分析到实现的全流程 | Complete Agent Ecosystem: 6 specialized agents covering the full pipeline from analysis to implementation
- ✅ **Agent协作工作流**: Paramy→Classie→Convertd→Heady→Dataie→Mody的结构化协作 | Agent Collaboration Workflow: Structured collaboration Paramy→Classie→Convertd→Heady→Dataie→Mody
- ✅ **专门化配置**: 每个Agent都有明确的职责、输入输出和质量标准 | Specialized Configuration: Each agent has clear responsibilities, inputs/outputs, and quality standards
- ✅ **动态数据支持**: 支持实时数据提取和灵活配置的网络模型 | Dynamic Data Support: Real-time data extraction and flexible network configuration
- ✅ **端到端自动化**: 从Python模型分析到C++优化代码生成的完整流程 | End-to-End Automation: Complete pipeline from Python model analysis to C++ optimization code generation

**性能监控和调试系统 | Performance Monitoring & Debugging System:**
- ✅ **CPU计时功能**: 集成`clock()`计时器，支持详细的性能分析 | CPU Timing: Integrated `clock()` timers supporting detailed performance analysis
- ✅ **模块化调试控制**: 可注释的debug输出，支持开发到生产的无缝切换 | Modular Debug Control: Commentable debug output enabling seamless development-to-production transition
- ✅ **执行时间分解**: 分别跟踪I/O、计算和优化阶段的性能 | Execution Time Breakdown: Separate tracking of I/O, computation, and optimization phases
- ✅ **配置驱动灵活性**: 通过配置文件控制参数，支持可扩展性测试 | Configuration-Driven Flexibility: Parameter control via configuration files supporting scalability testing

**Classie Agent (分析师) | Classie Agent (Analyst):**
- ✅ **模式识别**: 识别共享索引的数组、DataFrame、字典结构 | Pattern Recognition: Identify arrays sharing indices, DataFrames, dictionary structures
- ✅ **实体检测**: 自动识别节点、设施、资源、场景等逻辑实体类型 | Entity Detection: Automatically identify logical entity types like nodes, facilities, resources, scenarios
- ✅ **验证规则**: 每个类≥2个相关属性，代表逻辑实体类型 | Validation Rules: Each class has ≥2 related attributes, representing logical entity types
- ✅ **结构化输出**: 生成包含标量/数组成员分类的详细markdown报告 | Structured Output: Generate detailed markdown reports with scalar/array member classification

**Heady Agent (代码生成师) | Heady Agent (Code Generator):**
- ✅ **类型选择**: 容量/计数→int，成本/排放→double，索引→int | Type Selection: Capacity/Count → int, Cost/Emissions → double, Indices → int
- ✅ **命名规范**: 简洁明了(varCost, emsTrToSP, railMaintCostToDry) | Naming Convention: Concise but clear (varCost, emsTrToSP, railMaintCostToDry)
- ✅ **变量组织**: 按类别分组，包含完整初始化 | Variable Organization: Grouped by category with complete initialization
- ✅ **内存管理**: 指针初始化为nullptr，完整的init/delArr模式 | Memory Management: Pointer initialization to nullptr, complete init/delArr patterns
- ✅ **方法生成**: 一致的get/set模式和参数命名 | Method Generation: Consistent get/set patterns and parameter naming

**C++代码生成 | C++ Code Generation:**
- ✅ **头文件模板**: 验证标准化的.h文件模板，包含完整内存管理 | Header File Templates: Validated standardized .h file templates with complete memory management
- ✅ **类设计哲学**: 确立标量vs数组成员分离的设计原则 | Class Design Philosophy: Established design principles separating scalar vs array members
- ✅ **自动生成**: 实现基于分析的C++头文件自动生成 | Automatic Generation: Achieved analysis-based automatic C++ header file generation

**模型分析能力 | Model Analysis Capability:**
- ✅ **复杂模型支持**: 成功分析多周期、多场景的随机优化模型 | Complex Model Support: Successfully analyzed multi-period, multi-scenario stochastic optimization models
- ✅ **实体识别**: 自动识别和分类优化模型中的实体类型 | Entity Identification: Automatically identify and classify entity types in optimization models
- ✅ **命名优化**: 建立简洁一致的类命名规范 | Naming Optimization: Established concise and consistent class naming conventions

### 🎯 下一步目标 | Next Steps

- 🎯 **端到端翻译**: 实现Python模型到C++代码的完整自动化翻译 | End-to-End Translation: Achieve complete automated translation from Python models to C++ code
- 🎯 **性能验证**: 验证C++代码的性能提升效果 | Performance Validation: Verify performance improvement effects of C++ code
- 🎯 **学习能力**: 持续优化agent的模型识别和代码生成能力 | Learning Capability: Continuously optimize agents' model recognition and code generation capabilities
- 🎯 **工具链成熟化**: 将性能监控和调试系统标准化为翻译流程的必备组件 | Toolchain Maturation: Standardize performance monitoring and debugging systems as essential translation workflow components

## 工作流程 | Workflow

### 增强的协作流程 | Enhanced Collaborative Workflow

```mermaid
graph LR
    A[Python模型<br/>Python Model] --> B[paramy分析<br/>paramy Analysis]
    A --> C[classie分析<br/>classie Analysis]
    B --> D[parameter_report.md]
    C --> E[classes_report.md]
    E --> F[.h头文件生成<br/>.h Header Generation]
    D --> G[convertd数据转换<br/>convertd Data Conversion]
    G --> H[Data_new/文件<br/>Data_new/ Files]
    F --> I[dataie函数生成<br/>dataie Function Generation]
    H --> I
    I --> J[.cpp实现<br/>.cpp Implementation]
    J --> K[mody模型翻译<br/>mody Model Translation]
    K --> L[C++优化代码<br/>C++ Optimization Code]
    L --> M[性能监控集成<br/>Performance Monitoring]
    M --> N[验证与优化<br/>Validation & Optimization]
    N --> O[学习与改进<br/>Learning & Improvement]
    O --> A
```

1. **参数分析 | Parameter Analysis**: paramy agent分析Python文件中的硬编码值和数据源 | paramy agent analyzes hard-coded values and data sources in Python files
2. **实体分析 | Entity Analysis**: classie agent分析Python模型，识别实体类和属性关系 | classie agent analyzes Python models to identify entity classes and attribute relationships
3. **数据转换 | Data Conversion**: convertd agent将Python数据结构转换为C++兼容格式 | convertd agent converts Python data structures to C++-compatible formats
4. **头文件生成 | Header Generation**: heady agent基于分析生成完整的.h文件 | heady agent generates complete .h files based on analysis
5. **函数生成 | Function Generation**: dataie agent生成C++数据读取函数 | dataie agent generates C++ data reading functions
6. **模型翻译 | Model Translation**: mody agent翻译Pyomo模型为CPLEX C++代码 | mody agent translates Pyomo models to CPLEX C++ code
7. **C++实现 | C++ Implementation**: 使用生成的组件完成完整的C++实现 | Use generated components to complete C++ implementation
8. **性能监控集成 | Performance Monitoring**: 集成CPU计时和调试控制功能 | Integrate CPU timing and debug control features
9. **验证与优化 | Validation & Optimization**: 基于性能分析进行代码优化 | Optimize code based on performance analysis
10. **记录与学习 | Record & Learn**: 记录关键决策和性能洞察，提炼最佳实践 | Record key decisions and performance insights, distill best practices

### Agent协作机制 | Agent Collaboration

**主Agent (agent.md) | Main Agent (agent.md)**:
- 负责整体翻译策略和C++代码生成 | Responsible for overall translation strategy and C++ code generation
- 协调6个专门化Agent的工作流程 | Coordinate 6 specialized agents' workflows
- 维护项目编码标准和最佳实践 | Maintain project coding standards and best practices

**专门化Agent团队 | Specialized Agent Team**:

**Paramy Agent (参数分析师 - orange/Sonnet) | Paramy Agent (Parameter Analyst - orange/Sonnet)**:
- 分析Python文件中的硬编码值和外部数据源 | Analyze hard-coded values and external data sources in Python files
- 推荐文件组织策略(TXT vs CSV) | Recommend file organization strategy (TXT vs CSV)
- 生成`parameter_report.md`参数分析报告 | Generate `parameter_report.md` analysis reports

**Classie Agent (实体分析师 - purple/Sonnet) | Classie Agent (Entity Analyst - purple/Sonnet)**:
- 专门负责Python模型的结构分析 | Specialized in Python model structural analysis
- 识别实体类型和属性关系 | Identify entity types and attribute relationships
- 生成结构化的`classes_report.md`分析报告 | Generate structured `classes_report.md` analysis reports

**Convertd Agent (数据转换师 - teal/Sonnet) | Convertd Agent (Data Converter - teal/Sonnet)**:
- 将Python数据结构转换为C++兼容格式 | Convert Python data structures to C++-compatible formats
- 强调动态导入和实时数据提取 | Emphasize dynamic import and live data extraction
- 生成`Data_new/`文件夹中的C++兼容文件 | Generate C++-compatible files in `Data_new/` folder

**Heady Agent (代码生成师 - blue/Sonnet) | Heady Agent (Code Generator - blue/Sonnet)**:
- 专门负责C++头文件(.h)的生成 | Specialized in C++ header file (.h) generation
- 应用项目编码标准和模板 | Apply project coding standards and templates
- 确保内存安全和代码质量 | Ensure memory safety and code quality

**Dataie Agent (数据读取专家 - green/Sonnet) | Dataie Agent (Data Reading Expert - green/Sonnet)**:
- 生成C++数据读取函数 | Generate C++ data reading functions
- 提供动态网络索引和错误弹性 | Provide dynamic network indexing and error resilience
- 确保与转换数据文件的兼容性 | Ensure compatibility with converted data files

**Mody Agent (模型翻译师 - green/Sonnet) | Mody Agent (Model Translator - green/Sonnet)**:
- 翻译Pyomo优化模型为CPLEX C++代码 | Translate Pyomo optimization models to CPLEX C++ code
- 应用学习的C++实现模式 | Apply learned C++ implementation patterns
- 生成完整的优化逻辑和约束 | Generate complete optimization logic and constraints

**协作接口 | Collaboration Interface**:
- **Paramy → Convertd**: 通过`parameter_report.md`传递数据组织策略 | Pass data organization strategy through `parameter_report.md`
- **Classie → Heady**: 通过`classes_report.md`传递实体分析结果 | Pass entity analysis results through `classes_report.md`
- **Convertd → Dataie**: 提供`Data_new/`转换后的数据文件 | Provide converted data files in `Data_new/`
- **Heady → Dataie**: 生成标准化.h文件供数据读取函数使用 | Generate standardized .h files for data reading functions
- **Dataie → Mody**: 提供完整的数据加载函数供模型使用 | Provide complete data loading functions for model use
- **质量保证**: 各Agent都有独立的验证清单和输出标准 | Quality Assurance: Each agent has independent validation checklists and output standards
- **持续改进**: 通过反馈循环优化各Agent能力和协作效率 | Continuous Improvement: Optimize agent capabilities and collaboration efficiency through feedback loops

## 设计理念 | Philosophy

本仓库存储精炼的知识，作为每日进展追踪工具。本地开发（Cursor/Claude Code/Codex）处理实验性工作；GitHub保存提炼的最佳实践。

This repository stores refined knowledge and serves as daily progress tracker. Local development (Cursor/Claude Code/Codex) handles experimentation; GitHub contains the distilled best practices.

### 核心创新 | Core Innovation

**智能Agent协作系统 | Intelligent Agent Collaboration System:**
- **专门化分工**: Classie(分析师)专注结构分析，Heady(代码师)专注头文件生成，主agent专注翻译实现 | Specialized Division: Classie (analyst) focuses on structural analysis, Heady (coder) focuses on header generation, main agent focuses on translation implementation
- **Agent配置化**: 完整的Agent配置体系，指定模型、颜色标识和专门功能 | Agent Configuration: Complete agent configuration system specifying models, color identifiers, and specialized functions
- **结构化学习**: 通过classes_report.md积累模型识别经验 | Structured Learning: Accumulate model recognition experience through classes_report.md
- **模板化生成**: 标准化的.h文件模板确保代码一致性 | Template-based Generation: Standardized .h file templates ensure code consistency
- **质量保证**: 每个Agent都有独立的验证清单和质量标准 | Quality Assurance: Each agent has independent validation checklists and quality standards
- **迭代改进**: 从每个翻译案例中学习并优化各Agent能力 | Iterative Improvement: Learn and optimize agent capabilities from each translation case

**类设计哲学 | Class Design Philosophy:**
- **实体中心**: 每个类代表优化模型中的逻辑实体 | Entity-centric: Each class represents a logical entity in optimization models
- **标量vs数组**: 标量成员属于节点自身，数组成员表示与其他节点的关系 | Scalar vs Array: Scalar members belong to the node itself, array members represent relationships with other nodes
- **索引即身份**: 数组索引代表目标节点的身份 | Index as Identity: Array indexes represent the identity of target nodes
- **内存安全**: 完整的init/delArr内存管理模式 | Memory Safety: Complete init/delArr memory management pattern

## 技术栈 | Technology Stack

- **分析引擎**: Claude Code + classie Agent | Analysis Engine: Claude Code + classie Agent
- **建模框架**: Pyomo → C++ (手工优化) | Modeling Framework: Pyomo → C++ (manually optimized)
- **数据I/O**: ifstream readline方法（无外部依赖） | Data I/O: ifstream readline method (no external dependencies)
- **内存管理**: 手动new/delete（符合项目标准） | Memory Management: Manual new/delete (project standard compliant)
- **编译器**: 标准C++11兼容 | Compiler: Standard C++11 compatible

## 开源协议 | License

Apache-2.0 license
