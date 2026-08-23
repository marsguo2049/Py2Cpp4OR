# Roadmap

## Status convention

- **Complete:** the stated artifact exists in the public repository and has been reviewed against its acceptance criteria.
- **Planned:** no public implementation should be inferred.

## Phase 0 — v2 documentation and privacy foundation

**Status:** complete in the Phase 0 documentation pull request, pending merge.

- correct capability and maturity statements;
- define architecture and migration boundaries;
- define structural, same-solver, cross-solver, and algorithm verification;
- document privacy and publication rules;
- retain only high-level anonymous case studies;
- replace project-specific Agent rules with generic repository guidance;
- remove legacy project-specific content from the current tree;
- leave Git history unchanged.

## Phase 1 — public toy model migration

**Status:** planned.

- design a completely synthetic small optimization model;
- implement it with Python, Pyomo, and CPLEX;
- implement the corresponding C++ CPLEX Concert model;
- build structural comparison and same-solver differential checks;
- record tolerances, status mapping, objective decomposition, and fixed-solution checks;
- keep CPLEX optional so documentation and license-free parts remain usable without a commercial installation.

Phase 1 is not complete until the example and checks are runnable and documented.

## Phase 2 — GA, NSGA-II, and Hybrid GA

**Status:** planned.

- define corresponding generic Python and C++ GA interfaces;
- add single-objective policies and NSGA-II policies;
- define RNG, stopping, observer, and trace contracts;
- add an `ExactSubproblemSolver` interface;
- require every exact-subproblem call to return raw and normalized status, optional incumbent/objective, best bound, a gap with documented normalization, runtime, requested/consumed work budget and counters, and `proven_optimal`;
- define GA fitness and acceptance behavior for `optimal`, `limit-with-incumbent`, `limit-without-incumbent`, `infeasible`, `unbounded`, `numeric failure`, and `error`, without labeling time-limited incumbents exact or optimal;
- provide a public toy or mock backend and an optional CPLEX adapter;
- verify Hybrid GA calls by aligning subproblem inputs, solver settings, and per-call budgets, using proven-optimal toy solves, a deterministic mock/cached oracle, or comparable solver-work limits;
- require cross-solver verification claims to publish results, backend configuration, normalized comparison policy, and tolerances;
- verify with synthetic problems and fixed-work traces;
- keep all private chromosomes, models, data, and results outside the repository.

## Phase 3 — VND and GVNS

**Status:** planned.

- define corresponding Python and C++ VND/GVNS orchestration;
- separate problem representation, decoder, feasibility, evaluation, and neighborhoods behind adapters;
- specify shaking, local search, neighborhood change, acceptance, RNG, and stop policies;
- add neighborhood-level golden traces and fixed-work verification;
- use private cases only for local internal acceptance tests.

## Phase 4 — engineering and release

**Status:** planned.

- add CMake and CTest for implemented code;
- add CI that runs license-free checks and clearly skips unavailable optional solvers;
- add fixed-work and fixed-time benchmark harnesses;
- document public APIs and supported configurations;
- publish versioned releases with evidence-backed status notes.

## Out of scope until explicitly planned

- one-click translation of arbitrary Python projects;
- guaranteed performance improvement for every model;
- automatic publication or extraction of private cases;
- bundled commercial solver SDKs or licenses;
- support claims for a backend without a runnable public path.
