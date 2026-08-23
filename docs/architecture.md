# Architecture

## Status

This document defines the intended v2 architecture. In Phase 0 it is a design contract, not a description of implemented software.

## Design principles

1. **Semantics before syntax.** A migration is successful only when the target preserves the source model or algorithm contract.
2. **Models before algorithms.** Validate the mathematical model and decoder before using either inside a metaheuristic.
3. **Same solver before different solvers.** Remove backend variation before diagnosing language or implementation variation.
4. **Generic core, problem adapter.** Reusable algorithms must not contain business-specific variables, data, feasibility rules, or neighborhoods.
5. **Evidence-based status.** A feature is implemented only when its source, runnable test, and reproducible verification are present.
6. **Privacy by construction.** Public artifacts use synthetic problems and generic names from the start.

## Conceptual layers

```text
Public toy source model
        │
        ▼
Model description / intermediate contract
        │
        ├── Python source adapters: Pyomo, gurobipy
        ├── C++ target adapters: CPLEX Concert, future Gurobi C++
        └── Verification: structural, same-solver, cross-solver

Generic algorithm core
        │
        ├── problem adapter: representation, evaluation, feasibility
        ├── optional decoder / exact subproblem solver
        ├── RNG, stopping policy, observer, trace
        └── Python/C++ differential verification
```

No component in this diagram is implemented in Phase 0.

## Planned model-migration layout

```text
model_migration/
├── source_models/
│   ├── pyomo/
│   └── gurobipy/
├── cpp_targets/
│   ├── cplex_concert/
│   └── gurobi_cpp/          # roadmap only
└── verification/
    ├── structural/
    ├── same_solver/
    └── cross_solver/
```

Pyomo is a solver-independent modeling layer, while gurobipy is Gurobi-specific. CPLEX Concert is the first planned C++ backend. These distinctions must remain visible in APIs, documentation, and tests.

## Planned algorithm boundaries

### GA and NSGA-II

The generic core may define population and individual contracts, initialization, selection, crossover, mutation, single- or multi-objective policies, non-dominated sorting, crowding distance, RNG, stopping, observation, and tracing.

It must not define a private problem's chromosome, objective, feasibility rules, or data model.

### Exact-solver Hybrid GA

Hybrid GA should extend the generic GA through an injected interface rather than embedding a commercial solver or a case model in the core.

```text
Generic GA core
      +
ExactSubproblemSolver
├── public toy/mock backend
├── optional CPLEX adapter
└── future solver adapters
```

Commercial solver support must remain optional so that the public core can be inspected and tested without a commercial license.

### VND and GVNS

The generic core may coordinate shaking, local search or VND, neighborhood change, acceptance, stopping, RNG, and traces. Solution representation, decoding, feasibility, evaluation, and problem-specific neighborhoods belong behind a problem adapter.

## Target top-level layout

```text
Py2Cpp4OR/
├── docs/
├── model_migration/         # future
├── algorithms/              # future
│   ├── python/
│   │   ├── ga/
│   │   ├── hybrid_ga/
│   │   └── gvns/
│   └── cpp/
│       ├── ga/
│       ├── hybrid_ga/
│       └── gvns/
├── solver_adapters/         # future
├── examples/                # future; synthetic only
├── tests/                   # future
└── benchmarks/              # future
```

The current repository intentionally creates only the `docs/` portion of this layout.

## Dependency direction

- A generic algorithm may depend on a public interface, never on a case-specific implementation.
- A solver adapter may depend on its solver SDK, but the generic core must not.
- Verification may consume outputs from source and target implementations; neither implementation should depend on the verifier.
- Examples may assemble adapters and cores, but production/private projects must remain outside this public repository.

## Version-status vocabulary

- **Documented:** a design or requirement exists.
- **Prototype:** runnable code exists but lacks the full verification contract.
- **Verified:** runnable tests demonstrate the stated equivalence within declared tolerances.
- **Supported:** verified behavior is maintained as part of a released public interface.

Documentation must use these terms precisely.
