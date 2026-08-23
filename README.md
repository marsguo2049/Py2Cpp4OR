# Py2Cpp4OR

**Py2Cpp4OR is a documentation-first framework for migrating mathematical optimization models and metaheuristics from Python to C++, with explicit cross-language verification.**

**Py2Cpp4OR 是一个文档优先的运筹优化迁移框架，用于将 Python 数学模型和元启发式算法可靠地迁移到 C++，并通过明确的跨语言验证保证一致性。**

> **Project status / 项目状态:** Phase 0 documentation prototype. This repository currently contains design guidance, verification rules, privacy boundaries, and a roadmap. It does **not** yet contain a model translator, algorithm library, solver adapters, runnable examples, tests, benchmarks, CMake, or CI.

## Why this project exists / 为什么需要它

Moving an operations research implementation from Python to C++ is not a syntax-replacement exercise. A reliable migration must preserve:

- sets, indices, parameters, and derived data;
- variable types, bounds, and domains;
- objective direction and coefficients;
- constraint semantics and feasibility;
- algorithm operators, random-number behavior, stopping rules, and traces.

The framework therefore treats **verification as part of migration**, not as a final optional step.

## Scope / 范围

Py2Cpp4OR is being designed around three complementary tracks:

1. **Model migration** — document and eventually support migration from Python modeling APIs to C++ solver APIs.
2. **Algorithm references** — provide corresponding Python and C++ implementations of generic GA/NSGA-II, exact-solver Hybrid GA, VND, and GVNS components.
3. **Cross-language verification** — compare structure, solver behavior, algorithm traces, feasibility, and objective values.

All implementations in tracks 1–3 are roadmap items unless a future release explicitly marks them as available and provides runnable tests.

## Modeling layers and solver backends

The project does not assume a fixed Python-modeling/solver pairing.

| Component | Role | Status in this repository |
|---|---|---|
| Pyomo | General Python algebraic modeling layer; can call multiple solvers | Documented source path |
| gurobipy | Gurobi-specific Python API | Documented source path |
| CPLEX Concert | C++ API and first planned C++ target | Roadmap |
| Gurobi C++ API | Possible future C++ target | Roadmap only |

## Verification order

The intended order is deliberately conservative:

1. **Structural verification** — compare model entities, domains, bounds, objectives, constraints, and preprocessing.
2. **Same-solver verification** — first compare `Python + Pyomo + CPLEX` with `C++ + CPLEX Concert`.
3. **Cross-solver verification** — only after the same-solver path passes, compare configurations such as Python/Gurobi and C++/CPLEX.
4. **Algorithm verification** — validate RNG contracts, fixed-work traces, operators or neighborhoods, feasibility, and objective values before wall-clock benchmarking.

Different optimal variable assignments are not automatically an error: multiple optimal solutions, tolerances, and presolve can produce different representations with the same valid objective.

## Documentation

- [Architecture](docs/architecture.md)
- [Model migration](docs/model-migration.md)
- [Verification](docs/verification.md)
- [Privacy and publication boundaries](docs/privacy.md)
- [Anonymous case studies](docs/case-studies.md)
- [Legacy v1 notes](docs/legacy-v1.md)
- [Roadmap](docs/roadmap.md)

## Planned repository structure

The directories below describe the target architecture; directories that are not present in the current tree have not been implemented yet.

```text
Py2Cpp4OR/
├── README.md
├── LICENSE
├── docs/
├── model_migration/       # future
├── algorithms/            # future
│   ├── python/
│   └── cpp/
├── solver_adapters/       # future
├── examples/              # future; public toy problems only
├── tests/                 # future
└── benchmarks/            # future
```

## Privacy and reproducibility

Public examples must be synthetic and free of private business context, unpublished research material, proprietary data, credentials, solver license files, and lightly renamed private source code. High-level anonymous case studies may describe migration patterns, but they are not evidence that an automated capability already exists.

See [docs/privacy.md](docs/privacy.md) before contributing any example, trace, benchmark, or case study.

## Contributing today

At the current phase, useful contributions are limited to improving the documentation, identifying ambiguous verification requirements, and proposing fully synthetic future test problems. Do not add placeholder implementations or claim support without executable evidence.

Repository working rules are in [agent.md](agent.md) and [CLAUDE.md](CLAUDE.md).

## License

Licensed under the [Apache License 2.0](LICENSE).
