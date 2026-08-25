# Repository Agent Rules

## Purpose

These rules govern AI-assisted work in the public Py2Cpp4OR repository. They protect the project's accuracy, privacy, and verification-first scope.

Read these documents before proposing a change:

- `README.md`
- `docs/architecture.md`
- `docs/model-migration.md`
- `docs/verification.md`
- `docs/privacy.md`
- `docs/roadmap.md`

## Current maturity

The Phase 0 documentation baseline is complete. The tree also contains one
experimental C++14 GVNS foundation/API draft with synthetic toy tests and a
minimal CMake build and GCC CI check. Treat it as a prototype, not a complete
GVNS library. Model translators, solver adapters, GA/Hybrid GA implementations,
benchmarks, and broader CI coverage are not present. Do not describe any
capability as present unless the corresponding runnable public artifact exists
in the current tree.

## Non-negotiable rules

### 1. Protect private material

- Work only from public repository content and explicitly approved synthetic inputs.
- Do not search, connect to, inspect, copy, or infer details from private repositories or local projects.
- Do not publish private names, paths, formulas, symbols, schemas, dimensions, data, parameter values, results, performance figures, logs, hashes, infrastructure details, or unpublished material.
- Do not “anonymize” private source by merely renaming identifiers.
- Do not add solver binaries, proprietary files, credentials, or license material.

Follow the full policy in `docs/privacy.md`.

### 2. State capabilities honestly

Use the status vocabulary from `docs/architecture.md`: documented, prototype, verified, supported.

- A document is not an implementation.
- Compiling is not equivalence verification.
- A high-level case study is not an automated translation demonstration.
- A roadmap item is not current support.
- A performance claim needs runnable public evidence and a reproducible environment.

### 3. Preserve technical distinctions

- Pyomo is a general Python modeling layer and can call multiple solvers.
- gurobipy is Gurobi's Python API.
- CPLEX Concert is the first planned C++ backend.
- Gurobi C++ support is roadmap-only until implemented and verified.

Do not bind a modeling layer and solver into an assumed universal pair.

### 4. Verify in the required order

For future implementation work:

1. validate model structure;
2. validate Python/Pyomo/CPLEX against C++/CPLEX Concert;
3. validate cross-solver behavior;
4. validate algorithms only after models and decoders pass;
5. use fixed-work traces before fixed-time benchmarks.

Do not demand identical decision vectors when multiple optima may exist. Compare status, feasibility, objective, bounds, decomposition, and declared invariants using explicit tolerances.

### 5. Keep generic and case-specific code separate

- Generic GA/NSGA-II code may own population mechanics, operators, policies, RNG, stopping, observation, and tracing.
- Hybrid GA must receive exact subproblem solving through an interface.
- Generic VND/GVNS may own search orchestration, but representation, decoder, feasibility, evaluation, and problem-specific neighborhoods belong in adapters.
- Public examples must be independently created synthetic problems.

## Change workflow

1. Confirm the requested work is within the current roadmap phase.
2. Inspect the current tree and relevant documentation.
3. List the files to add, modify, or delete before editing.
4. Make the smallest coherent change.
5. Update capability/status language together with any implementation change.
6. Review the complete diff for private or identifying content.
7. Check relative Markdown links and terminology consistency.
8. For code phases, run the documented tests and report exact commands and results.
9. Use a branch and pull request; do not merge without owner review.

## Phase transition scope

The Phase 0 documentation-only restriction is retained as project history. New
runnable work must be an explicitly approved roadmap increment with real source,
synthetic tests, honest status language, and synchronized documentation. Do not
add placeholder source files, empty examples, fictional tests, CMake, or CI
merely to make the repository appear more complete.

## Review checklist

- [ ] Claims match files and runnable evidence in the current tree.
- [ ] Modeling layers and solver backends are described correctly.
- [ ] Model verification precedes algorithm verification.
- [ ] Same-solver verification precedes cross-solver verification.
- [ ] Public/private boundaries are respected.
- [ ] Generic cores contain no case-specific semantics.
- [ ] New links resolve.
- [ ] Roadmap features remain labeled as planned.
- [ ] No Git history rewrite was performed without separate authorization.
