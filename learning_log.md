# Learning Log

## 2025-11-04: Initial Observations

### Performance Validation
**Finding**: C++ drastically outperforms Python in model building  
- Python (pyomo): ~100s
- C++ (CPLEX/Gurobi): <1s
- **Conclusion**: Translation effort is justified

### Code Complexity Issues
**Problem**: AI-generated code is overly complex
- Generates unnecessary files: .exe, g++, Makefile, CMakeLists.txt
- Uses heavy libraries (Eigen, Armadillo) for simple tasks
- Supervisor cannot easily review the code

**Root Cause**: AI tries to replicate Python's matrix operations directly
- Python uses numpy/pandas for efficiency
- C++ equivalents add unnecessary complexity

**Solution**: 
- Restrict output to .cpp and .h files only
- Use simple readline() for data I/O
- Avoid matrix libraries unless absolutely necessary

### Data I/O Strategy
**Observation**: Supervisor's code uses readline approach
- More transparent than library-based matrix loading
- Potentially faster for sequential reading
- Easier to debug

**Action Item**: Modify data files to support line-by-line reading

### Current Status
- ✅ Model translation complete (in function.cpp)
- 🔄 Refining data loading approach
- 🔄 Simplifying AI-generated .h files

### Next Steps
1. Establish clear file structure rules in agent.md
2. Test readline data loading performance
3. Document standard translation patterns
