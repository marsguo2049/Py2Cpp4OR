# Model Migration

## Purpose

Model migration aims to reproduce mathematical meaning across modeling languages and solver APIs. It is not a mechanical replacement of Python syntax with C++ syntax.

This page specifies a future workflow. No translator is implemented in Phase 0.

## Source and target paths

Planned source paths:

- **Pyomo:** a general algebraic modeling layer that can invoke CPLEX, Gurobi, and other supported solvers.
- **gurobipy:** Gurobi's Python API, where the model is built directly against a specific solver.

Planned C++ targets:

- **CPLEX Concert:** first target and first same-solver verification path.
- **Gurobi C++ API:** possible future target, not currently supported.

The modeling layer and solver backend must be recorded separately in every migration report.

## Migration contract

Before translating code, record the following in a solver-neutral model contract:

- sets, index ranges, ordering, and index conversions;
- raw parameters, derived parameters, units, defaults, and missing-value behavior;
- variables, index domains, types, lower bounds, and upper bounds;
- objective sense, components, coefficients, and constant terms;
- constraints, quantification domains, relation signs, coefficients, and conditional activation;
- data preprocessing, filtering, aggregation, and scaling;
- solver options that change mathematical behavior;
- expected outputs and acceptable numeric tolerances.

Hard-coded dimensions should be treated as source facts to extract and parameterize, not as permanent target-code rules.

## Proposed workflow

### 1. Freeze a public toy instance

Use a fully synthetic, small instance whose expected structure can be reviewed manually. Preserve the input data and the exact source revision used for comparison.

### 2. Inventory the source model

Generate or manually review a manifest of sets, parameters, variables, objectives, constraints, preprocessing, and solver configuration. Dynamic construction and conditional constraints require explicit attention.

### 3. Define a neutral mapping

Map source concepts to target concepts without assuming that identical names imply identical semantics. Document index ordering, type conversion, bound conventions, infinities, and expression construction.

### 4. Implement the C++ target

Start with the simplest auditable implementation. Solver lifecycle, expression ownership, exception handling, and resource cleanup should follow the target SDK's documented practices rather than an inherited case-specific style.

### 5. Run structural verification

Compare both generated models before comparing solutions. See [verification.md](verification.md).

### 6. Run same-solver verification

The first planned reproducible path is:

```text
Python + Pyomo + CPLEX
          versus
C++ + CPLEX Concert
```

This isolates language and implementation differences from most backend differences.

### 7. Run cross-solver verification

Only after same-solver verification passes should a Python/Gurobi result be compared with a C++/CPLEX result. Cross-solver checks need explicit tolerances and must account for presolve and multiple optimal solutions.

## What equivalence means

Depending on the model, equivalence may require:

- matching feasible/infeasible/unbounded status;
- objective values and bounds within declared tolerances;
- matching feasibility when a solution from one implementation is fixed or evaluated in the other;
- matching objective decomposition;
- no unexplained differences in variable or constraint structure.

Exact equality of every decision variable is required only when uniqueness has been established. Otherwise, two different feasible solutions with the same valid optimum may both be correct.

## Data handling

The public framework should support explicit schemas and validation rather than mandate one text-file layout or one operating-system path convention. Derived values may be recalculated in each implementation, but their formulas and units must be part of the contract and verified.

Never import a private project merely to manufacture a public example. Public fixtures must be synthetic and independently understandable.

## Expected future artifacts

A verified migration should eventually include:

- source and target code;
- synthetic inputs;
- model manifests;
- structural comparison output;
- solver configuration;
- same-solver and, where applicable, cross-solver results;
- declared tolerances and known limitations;
- commands that reproduce the checks.

These artifacts do not yet exist in the repository.
