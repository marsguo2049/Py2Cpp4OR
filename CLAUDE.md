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

### When Translating Models
1. Read `agent.md` for current operational rules
2. Analyze Python model structure (pyomo/gurobipy)
3. Generate minimal C++ equivalent following patterns
4. Validate compilation and logic
5. Document non-obvious mappings in `reasoning_traces/`

### Learning and Updates
- Record translation challenges in `learning_log.md`
- Update `agent.md` when new patterns emerge
- Store refined decision logs in `reasoning_traces/`

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

Refer to this log when encountering translation challenges or needing performance guidance.