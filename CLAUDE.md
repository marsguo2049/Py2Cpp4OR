# CLAUDE.md

This file provides concise working guidance for Claude Code and other coding assistants in the public Py2Cpp4OR repository.

## Project definition

Py2Cpp4OR is a documentation-first framework for migrating mathematical optimization models and metaheuristics from Python to C++, with explicit cross-language verification.

The current repository is at **Phase 0**. It contains documentation and repository rules only. It does not yet contain a translator, model implementations, algorithms, solver adapters, runnable examples, tests, benchmarks, CMake, or CI.

## Required reading

Before changing the repository, read:

1. `agent.md` — repository-wide AI working rules;
2. `docs/privacy.md` — material that may and may not be published;
3. `docs/architecture.md` — target boundaries and status vocabulary;
4. `docs/verification.md` — required evidence and comparison order;
5. `docs/roadmap.md` — current and future phase boundaries.

## Phase 0 instructions

- Limit work to documentation, architecture descriptions, privacy boundaries, and development rules.
- Do not create placeholder Python/C++ implementations, examples, tests, CMake, CI, or benchmark claims.
- Label future directories and capabilities as planned.
- Keep `README.md` synchronized with the current tree.
- Use an independent branch and pull request; do not merge without owner review.

## Privacy guardrail

Use only public repository content and fully synthetic material that was created for public use.

Do not access or infer from private repositories, local research projects, unpublished papers, proprietary data, solver license files, or earlier case-specific material. Do not reproduce private code with renamed identifiers. If publication rights are uncertain, omit the material.

Anonymous case summaries are restricted to `docs/case-studies.md`; do not expand them with a domain, formulas, dimensions, data, results, or performance figures.

## Technical guardrails

- Keep Pyomo distinct from solver backends; it can call multiple solvers.
- Keep gurobipy identified as the Gurobi-specific Python API.
- Treat CPLEX Concert as the first planned C++ target.
- Treat Gurobi C++ as roadmap-only until a runnable verified path exists.
- Validate models before algorithms and same-solver behavior before cross-solver behavior.
- Treat multiple optimal solutions and numeric tolerances explicitly.
- Keep case representation, data, decoder, feasibility, objectives, and specialized neighborhoods outside generic algorithm cores.

## Working procedure

1. Inspect the current branch and relevant documents.
2. State the exact file list and intended claim changes.
3. Make a focused change.
4. Review the full diff for overclaiming and privacy leakage.
5. Check Markdown links and status terminology.
6. Report what is implemented, what remains planned, and what validation was actually run.

The more detailed and authoritative rules are in `agent.md`.
