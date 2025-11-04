# Agent Configuration

## Objective
Translate Python optimization models (pyomo/gurobipy) to clean, minimal C++ code.

## Core Principles
- **Simplicity over complexity**: Generate only .cpp and .h files
- **Readability first**: Code must be human-reviewable
- **Minimal dependencies**: Avoid unnecessary libraries
- **Consistent style**: Follow supervisor's coding patterns

## Code Structure Requirements

### File Organization
- `model.h`: Class declarations, function prototypes
- `model.cpp`: Implementation
- `function.cpp`: Model building logic
- **Forbidden**: .exe, g++, Makefile, CMakeLists.txt (unless explicitly requested)

### Data I/O Requirements

**Reading Pattern - txt**:
```cpp
ifstream file;
file.open("data\\data.txt");
std::getline(file, line);  // Skip comment lines
file >> temp;              // Read value
```

**Reading Pattern - csv**:
```cpp
ifstream file;
file.open("data\\centers.csv");
for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
        file >> value;
        if (j < numCols - 1) file >> tempchar;  // Skip comma
    }
}
file.close();
```

**Validation Checklist**:
- [ ] No matrix libraries (Eigen, Armadillo) for file reading
- [ ] Derived values computed in code, not read from files
- [ ] Comments preserved in txt using `std::getline()`
- [ ] CSV delimiters handled with `tempchar`

**Code Pattern**:
```cpp
// Wrong: Read pre-calculated value
file >> fixedCost;

// Right: Read base, compute in code
file >> capacity;
hub[i].setfixedCost(capacity * 0.25);
```

See `USER_GUIDE.md` for data preparation specifications.

### Translation Rules
**Variables**:
```cpp
// pyomo: model.x = Var(domain=NonNegativeReals)
IloNumVarArray x(env, n, 0, IloInfinity);
```

**Constraints**:
```cpp
// pyomo: model.c = Constraint(rule=constraint_rule)
IloRangeArray constraints(env);
model.add(constraints);
```

**Objective**:
```cpp
// pyomo: model.obj = Objective(expr=..., sense=minimize)
model.add(IloMinimize(env, objective_expr));
```

## Workflow
1. Analyze Python model structure
2. Generate minimal C++ equivalent
3. Validate compilation and logic
4. Document non-obvious mappings

## Output Checklist
- [ ] Only .cpp/.h files generated
- [ ] Code compiles without warnings
- [ ] Readable by non-AI programmer
- [ ] Data I/O uses readline pattern
- [ ] No unnecessary libraries imported

## C++ Class Design for Nodes

### Why Use Classes
- Encapsulate node attributes (costs, emissions, demand)
- Type safety (different node types don't mix)
- Uniform getter/setter pattern
- Easy to extend with new node types

### Header File Structure
```cpp
#ifndef NODE_H
#define NODE_H

class Node {
public:
    Node() {}
    
    void init(int size1, int size2, int size3) {
        // Allocate 1D arrays
        array1D = new double[size1];
        
        // Allocate 2D arrays
        array2D = new double*[size1];
        for (int i = 0; i < size1; i++) {
            array2D[i] = new double[size2];
        }
        
        // Allocate 3D arrays
        array3D = new double**[size1];
        for (int i = 0; i < size1; i++) {
            array3D[i] = new double*[size2];
            for (int j = 0; j < size2; j++) {
                array3D[i][j] = new double[size3];
            }
        }
    }
    
    // Getters for scalars (single-index attributes)
    int getCapacity() { return capacity; }
    double getVarCost() { return varCost; }
    
    // Getters for arrays (multi-index attributes)
    double get1D(int i) { return array1D[i]; }
    double get2D(int i, int j) { return array2D[i][j]; }
    double get3D(int i, int j, int k) { return array3D[i][j][k]; }
    
    // Setters
    void setCapacity(int v) { capacity = v; }
    void setVarCost(double v) { varCost = v; }
    void set1D(int i, double val) { array1D[i] = val; }
    void set2D(int i, int j, double val) { array2D[i][j] = val; }
    void set3D(int i, int j, int k, double val) { array3D[i][j][k] = val; }
    
    void delArr(int size1, int size2) {
        delete[] array1D;
        
        for (int i = 0; i < size1; i++) {
            delete[] array2D[i];
        }
        delete[] array2D;
        
        for (int i = 0; i < size1; i++) {
            for (int j = 0; j < size2; j++) {
                delete[] array3D[i][j];
            }
            delete[] array3D[i];
        }
        delete[] array3D;
    }
    
    ~Node() {}

private:
    // Single-index: scalar members (attributes of this node only)
    int capacity = 0;
    double varCost = 0.0;
    
    // Multi-index: array members (relationships to other entities)
    double* array1D;    // to destinations
    double** array2D;   // indexed by [destination][attribute]
    double*** array3D;  // indexed by [dest1][dest2][attribute]
};

#endif
```

### Member Design Rules

**Array index represents node identity**:
```cpp
node = new Node[numNodes];  // node[i] is the i-th node
node[i].getCapacity();      // i identifies which node
```

**Scalar vs Array members**:
- **Scalar**: Attribute belongs to this node only (single-index parameter)
  - Example: `capacity`, `varCost`
  - Usage: `node[i].getCapacity()`
- **Array**: Relationship to other entities (multi-index parameter)
  - Example: `trCostToDestination[d]`, `demandMatrix[period][scenario]`
  - Usage: `node[i].getTrCost(d)`

**Rule**: If parameter depends only on current entity → scalar member. If it relates to destinations/periods/scenarios → array member.

### Naming Convention
- Class: `Cities`, `Terminal`
- One .h file per node type
- See `examples/cities.h` for complete reference

## Learning
Record translation challenges in `learning_log.md`. Update this file when new patterns emerge.
