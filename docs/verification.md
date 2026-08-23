# Verification

## Principle

Migration is complete only when the source and target have been compared at the appropriate semantic level. Compilation and a similar objective value are necessary evidence, but neither is sufficient on its own.

The checks below are requirements for future implementations. Phase 0 contains no verification tool yet.

## Verification sequence

### 1. Structural verification

Compare the model before solving it.

| Area | Required comparison |
|---|---|
| Sets and indices | cardinality, order, ranges, filters, index conversion |
| Parameters | values, units, defaults, scaling, derived values |
| Variables | count, indexing, type, lower and upper bounds |
| Objective | sense, coefficients, constant terms, decomposition |
| Constraints | count, index domain, relation, coefficients, right-hand side |
| Preprocessing | filtering, aggregation, missing values, conditional construction |

Aggregate counts can reveal errors, but a useful verifier should also provide stable identifiers or canonical signatures so that a mismatch can be localized.

### 2. Same-solver verification

The first planned public path is:

```text
Python + Pyomo + CPLEX
          versus
C++ + CPLEX Concert
```

Record and compare:

- normalized solve status;
- incumbent objective;
- best bound and reported gap where applicable;
- feasibility and constraint violations;
- objective components;
- the cost and feasibility of a fixed candidate solution.

Use compatible solver versions and aligned settings where possible. Record any setting that cannot be matched.

### 3. Cross-solver verification

After the same-solver path passes, compare other backend combinations such as Python/Gurobi and C++/CPLEX.

Expected sources of benign variation include presolve, tolerances, solution pools, tie-breaking, and multiple optimal solutions. The comparison should focus on status, feasibility, objective, bound, and declared invariants rather than demanding identical raw solution vectors without a uniqueness argument.

### 4. Algorithm verification

Verify metaheuristics only after the mathematical model and any decoder have passed their relevant checks.

For corresponding Python and C++ implementations, define:

- identical public inputs and representation contracts;
- an explicit random-number contract;
- fixed-work stopping conditions;
- operator- or neighborhood-level golden traces;
- feasibility checks and objective decomposition;
- acceptance and improvement rules;
- termination reason and work counters.

Fixed-time benchmarks come last because wall-clock runs do not guarantee equal search effort.

## Numeric comparison policy

Every verification report should declare both absolute and relative tolerances. A typical comparison shape is:

```text
abs(a - b) <= absolute_tolerance
or
abs(a - b) <= relative_tolerance * max(1, abs(a), abs(b))
```

The actual tolerances must be justified for the model scale and solver settings. Do not silently round values until they agree.

## Multiple optimal solutions

If uniqueness is not established, variable-by-variable equality is too strong. Prefer one or more of the following:

- compare objective values and feasibility;
- fix the source solution in the target and re-evaluate it;
- compare invariant aggregates;
- add a documented secondary objective for deterministic tie-breaking;
- enumerate alternatives only for a sufficiently small toy problem.

## Status normalization

Different APIs expose different status vocabularies. A future verifier should preserve the raw backend status while also mapping it to a small documented set such as:

- optimal;
- feasible but not proven optimal;
- infeasible;
- unbounded;
- infeasible or unbounded;
- no solution / interrupted;
- error.

The mapping must not turn an unknown or interrupted solve into a success.

## Algorithm traces

A golden trace may record, for a fixed-work run:

- seed and RNG specification;
- generated candidates or move identifiers;
- evaluated objective components;
- feasibility result;
- accepted/rejected decision;
- incumbent update;
- neighborhood or generation counter.

When byte-identical RNG streams are impractical, inject deterministic candidate sequences to verify operators separately, then test statistical behavior as a distinct layer.

## Benchmarking

Use two complementary modes:

1. **Fixed work:** same iterations, evaluations, nodes, or neighborhood calls; suitable for semantic and implementation comparisons.
2. **Fixed time:** same wall-clock budget; suitable for practical performance comparisons after correctness is established.

Reports should include hardware, software versions, build type, solver settings, seeds, warm-up policy, repetition count, and summary statistics. Private infrastructure identifiers and unpublished project results must not enter the public repository.

## Minimum evidence for a future “verified” claim

- runnable source and target implementations;
- synthetic test data;
- declared environment and solver configuration;
- structural comparison output;
- same-solver result;
- tolerance policy;
- reproducible commands;
- documented limitations.
