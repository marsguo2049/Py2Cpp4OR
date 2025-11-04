# User Guide

## Data File Preparation

### File Format Selection

**txt** - Configuration parameters
- **Why**: Supports inline comments for documentation
- **Use for**: Parameters that need human explanation, grouped settings
- **Benefits**: Easy manual editing, self-documenting

**csv** - Tabular data
- **Why**: Clean matrix format without structural overhead
- **Use for**: Large datasets, matrices, scenarios
- **Benefits**: Excel-compatible, programmatically generated, version control friendly

### Core Principle: Store Base Values Only

Files should contain only raw input data. All calculations belong in C++ code.

**Rationale**:
- Changing formulas requires only code edits, not data regeneration
- Parameters remain transparent and traceable
- Reduces error-prone data preprocessing

**Wrong**: Pre-calculate `fixedCost = capacity * 0.25` in Excel, store both
**Right**: Store only `capacity`, compute `fixedCost` in C++

## File Organization

```
data/
├── data.txt        # Main parameters
├── centers.csv     # Demand matrices
└── label.csv       # Scenario data
```
