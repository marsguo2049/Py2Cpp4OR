# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is Py2Cpp4OR - an intelligent agent system for translating Python optimization models (pyomo/gurobipy) to efficient C++ code. The project uses an iterative learning framework to continuously improve translation quality.

## Repository Structure

```
.
├── agent.md              # Agent operational rules and LLM context
├── learning_log.md       # Experience summary and gap analysis
├── user_guide.md         # User data preparation guide
├── README.md             # Project documentation
├── CLAUDE.md             # This file - Claude Code guidance
└── reasoning_traces/     # Directory for refined decision logs
```

## Core Architecture

### Iterative Learning Framework
The project follows a continuous improvement cycle:
```
agent.md → Execute → reasoning_traces/ → Feedback → learning_log.md → Update agent.md
```

### Translation Philosophy
- **Simplicity over complexity**: Generate only .cpp and .h files
- **Readability first**: Code must be human-reviewable for non-AI programmers
- **Minimal dependencies**: Avoid external libraries (Eigen, Armadillo, etc.)
- **Consistent style**: Follow supervisor's coding patterns

## Key Translation Requirements

### Data I/O Patterns
The project mandates specific file reading approaches:

**TXT files (parameters with comments):**
```cpp
ifstream file;
file.open("data\\data.txt");
std::getline(file, line);  // Skip comment lines
file >> temp;              // Read value
```

**CSV files (tabular data):**
```cpp
ifstream file;
file.open("data\\centers.csv");
for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
        file >> value;
        if (j < numCols - 1) file >> tempchar;  // Skip comma
    }
}
```

### Code Structure Requirements
- **Required files**: `model.h`, `model.cpp`, `function.cpp`
- **Forbidden files**: .exe, g++, Makefile, CMakeLists.txt (unless explicitly requested)
- **No matrix libraries** for file reading - use raw ifstream approach
- **Header guards** required in all .h files

### C++ Class Design for Optimization Models
Node classes encapsulate optimization model entities:

**Design Rules:**
- **Scalar members**: Attributes belonging to this node only (e.g., `capacity`, `varCost`)
- **Array members**: Relationships to other entities (e.g., `trCostToDestination[d]`)
- **Array index represents node identity**: `node[i]` is the i-th node
- **Getter/setter pattern** for all members

**Memory Management:**
- Manual allocation with `new`/`delete[]` for multi-dimensional arrays
- `init()` method for allocation, `delArr()` for cleanup
- Proper destructor implementation

## Development Workflow

### Specialized Agent-Based Translation Process

**Phase 1: Analysis (Agents: Paramy + Classie)**
1. **Paramy Analysis**: Analyze Python files for hard-coded values and data sources
   - Input: Python model files
   - Output: `reports/parameter_report.md`
   - Focus: File organization strategy, data source identification

2. **Classie Analysis**: Identify entity classes and attribute relationships
   - Input: Python model structure
   - Output: `reports/classes_report.md`
   - Focus: Entity types, scalar vs array classification

**Phase 2: Data Processing (Agent: Convertd)**
3. **Convertd Conversion**: Convert Python data structures to C++-compatible formats
   - Input: `parameter_report.md` + Python data sources
   - Output: `Data_new/` folder with C++-compatible files
   - Focus: Dynamic data extraction, universal templates, no hard-coding

**Phase 3: Code Generation (Agents: Heady + Dataie)**
4. **Heady Headers**: Generate C++ header files based on Classie's analysis
   - Input: `classes_report.md`
   - Output: `.h` files with complete getter/setter patterns
   - Focus: Memory management, naming conventions

5. **Dataie Functions**: Generate C++ data reading functions
   - Input: `Data_new/` files + `.h` files
   - Output: `function.cpp` with comprehensive data loading
   - Focus: Dynamic adaptability, error resilience

**Phase 4: Model Translation (Agent: Mody)**
6. **Mody Translation**: Translate Pyomo models to CPLEX C++ functions
   - Input: Python optimization model + Dataie functions
   - Output: `model.cpp` with complete optimization logic
   - Focus: CPLEX implementation, constraint patterns

**Phase 5: Integration & Performance**
7. **C++ Implementation**: Complete C++ translation using generated components
8. **Performance Integration**: Add CPU timing and debug control
9. **Validation & Optimization**: Test and optimize based on performance analysis
10. **Learning**: Record insights and update agent configurations

### When Translating Models (Agent-Assisted Approach)
1. **Invoke specialized agents**: Start with Paramy and Classie analysis
2. **Follow agent outputs**: Use generated reports as blueprints
3. **Leverage generated components**: Use `.h` files, data functions, model functions
4. **Integrate performance features**: Add timing and debug controls
5. **Validate complete pipeline**: Ensure all components work together
6. **Document agent performance**: Record which agents performed well

### Performance and Debugging Workflow
1. **Development Phase**: Enable detailed debug output for validation
2. **Testing Phase**: Use CPU timing to identify performance bottlenecks
3. **Production Phase**: Comment out debug sections, keep timing summaries
4. **Optimization Phase**: Adjust parameters via configuration files
5. **Documentation**: Record performance insights in `learning_log.md`
6. **Agent Performance Review**: Evaluate which agents need updates

### Learning and Updates
- Record translation challenges in `learning_log.md`
- Update `agent.md` when new patterns emerge
- Store refined decision logs in `reasoning_traces/`
- Document performance optimization strategies
- **Agent Improvement**: Update `.claude/agents/*.md` files based on performance feedback
- **Workflow Optimization**: Refine agent collaboration based on project experience

## Data Preparation Guidelines

### File Format Philosophy
- **TXT files**: Configuration parameters with inline comments
- **CSV files**: Tabular data without structural overhead
- **Core principle**: Store base values only, compute derived values in C++ code

### Validation Checklist
- [ ] No matrix libraries (Eigen, Armadillo) for file reading
- [ ] Derived values computed in code, not read from files
- [ ] Comments preserved in txt using `std::getline()`
- [ ] CSV delimiters handled with `tempchar`
- [ ] Only .cpp/.h files generated
- [ ] Code compiles without warnings
- [ ] Readable by non-AI programmer
- [ ] **Performance monitoring included** (CPU timing with `clock()`)
- [ ] **Debug output controllable** (commented sections for production)
- [ ] **Flexible parameter support** (configuration-driven scalability)
- [ ] **Memory management complete** (proper init/delArr patterns)
- [ ] **Agent workflow followed**: All 6 specialized agents properly invoked
- [ ] **Generated components integrated**: .h files, function.cpp, model.cpp used
- [ ] **Data conversion validated**: Data_new/ files compatible with C++ reading
- [ ] **Agent outputs reviewed**: parameter_report.md, classes_report.md checked

## Important Constraints

1. **No Build Systems**: Do not generate Makefile, CMakeLists.txt, or compilation scripts
2. **Minimal Dependencies**: Avoid external libraries unless absolutely necessary
3. **Human-Readable Code**: Priority on code reviewability over clever optimizations
4. **Base Data Only**: Never store pre-calculated values in data files

## Learning Log Usage

The `learning_log.md` contains validated patterns and performance insights:
- C++ dramatically outperforms Python (<1s vs 100s for model building)
- AI-generated code tends toward unnecessary complexity
- Readline approach is preferred for transparency and debuggability
- **Performance monitoring is essential for optimization**
- **Flexible parameter control enables scalable testing**
- **Modular debug control supports development-to-production workflow**

Refer to this log when encountering translation challenges or needing performance guidance.

## Performance and Debugging Best Practices

### CPU Timing Implementation
```cpp
#include <ctime>
clock_t startTime = clock();
// ... code section ...
clock_t endTime = clock();
double executionTime = double(endTime - startTime) / CLOCKS_PER_SEC;
```

### Debug Control Strategy
- **Development**: Keep detailed debug output active
- **Testing**: Add performance timing measurements
- **Production**: Comment out debug, keep timing summaries
- **Documentation**: Record performance characteristics

### Configuration-Driven Flexibility
- Use central configuration files (data.txt) for parameter control
- Support variable data sizes for scalability testing
- Enable different execution modes without code changes

### Agent-Driven Development Best Practices
- **Sequential agent invocation**: Follow Paramy→Classie→Convertd→Heady→Dataie→Mody order
- **Output dependency management**: Each agent's output is the next agent's input
- **Quality gate validation**: Validate each agent's output before proceeding
- **Performance feedback loop**: Record agent performance for future improvements
- **Template adherence**: Follow agent-specific templates and patterns
- **Error handling**: Each agent should handle errors gracefully and provide meaningful feedback
