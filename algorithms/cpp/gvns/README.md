# C++14 GVNS Foundation

Status: **prototype — experimental foundation/API draft**.

This module provides solver-independent orchestration primitives in the
`py2cpp4or::gvns` namespace. It is intentionally smaller than a complete GVNS
library: applications still supply the solution representation, objective,
feasibility logic, neighborhoods, local-search moves, shaking behavior, clock,
and random source.

The module uses only the C++14 standard library. It has no modeling-layer or
solver dependency.

## Core interfaces

- `Neighborhood<Solution>` identifies an enabled or disabled neighborhood and
  provides its search callback.
- `NeighborhoodContext` injects RNG, clock, progress, and stopping policy into
  each callback.
- `NeighborhoodOutcome<Solution>` reports exactly one of four states.
- `VndEngine<Solution>` scans the neighborhood registry with strict
  minimization acceptance and restart-on-improvement behavior.
- `StrictMinimizationAcceptance` accepts only finite objective values satisfying
  `candidate < incumbent - tolerance`; maximization is not supported.
- `SequentialNeighborhoodChange` advances an application-provided sequence
  after rejection and resets it after acceptance.
- `NeverStopPolicy`, `DeadlineStopPolicy`, and `FixedWorkStopPolicy` implement
  the current stopping contracts.
- `NullObserver` and `VectorTraceObserver` provide opt-out and in-memory trace
  observation without owning search decisions.
- `OptionalValue<T>` supplies the C++14 nullable value used by trace events. An
  empty value does not construct `T`, so `T` need not be default-constructible.

### Four-state neighborhood outcome

| State | Meaning |
| --- | --- |
| `improved` | A candidate is present and must pass strict-improvement acceptance. |
| `exhausted` | The callback completed its scan without an improving candidate. |
| `interrupted` | The scan did not complete; a non-empty reason is required. |
| `disabled` | The registry entry is intentionally inactive and is not executed. |

An interrupted scan never claims a registry local optimum. A registry with no
enabled neighborhoods terminates separately as `no_enabled_neighborhoods`.
`disabled` is emitted by the engine for disabled registry entries; returning it
from an enabled callback is a contract error.

## VND restart and neighborhood change

`VndEngine` scans neighborhoods in registry order. When an `improved` outcome
passes `StrictMinimizationAcceptance`, the candidate becomes the incumbent and
the next scan restarts at the first registry entry. A local optimum is reported
only after enabled, exhaustible neighborhoods have been scanned without an
accepted improvement. The current acceptance class assumes minimization:
`candidate < incumbent - tolerance`.

`SequentialNeighborhoodChange` is a separate state helper for a caller-owned
sequence of positive neighborhood levels. `on_rejected()` advances and cycles;
`on_accepted()` resets to the first level. The foundation does not prescribe a
sequence or implement a top-level GVNS loop.

## Stopping, RNG, observers, and traces

`Clock::now_ticks()` must be monotonic. The application chooses the tick unit;
every deadline paired with that clock must use the same unit. A
`DeadlineStopPolicy` receives an absolute tick value and stops when
`now_ticks() >= deadline_ticks`. Stopping is checked before each neighborhood
call. A call started before a deadline is atomic: its completed outcome is
processed, then the next call is prevented. A callback can query
`NeighborhoodContext::stop_after()` to project additional work and return
`interrupted` when its own scan must stop.

`RandomSource::next_u64()` may return any value in the inclusive `uint64_t`
range. `uniform_index(n)` must reject `n == 0` and otherwise return in `[0, n)`.
The required zero-bound failure is `std::invalid_argument`.
`draws_consumed()` is a non-decreasing lifetime count of primitive 64-bit draws,
including any rejection draws used by `uniform_index`; the engine records it as
supplied and does not reset it at the start of a run. With identical initial RNG
state and the same call sequence, an implementation must reproduce the same
values and draw count. The foundation itself does not consume RNG; application
callbacks do.

The engine owns its copied registry, callbacks, objective, and acceptance
policy. `run()` borrows RNG, clock, stop policy, and observer for the call;
`NeighborhoodContext` and its references are valid only during the callback.
The caller must synchronize shared callbacks and borrowed dependencies or use
independent instances for concurrent runs; the foundation adds no locking.
Observers run synchronously after decisions and receive a borrowed event. An
observer exception, or an exception from an application callback, objective,
stop policy, clock, or RNG, propagates from `run()` without rollback or a
guaranteed terminal event. Built-in stop decisions and interrupted outcomes
always carry a non-empty reason; custom stopping policies must do the same.

`VectorTraceObserver::golden_trace()` formats deterministic text for snapshot
tests. It does not escape field delimiters and is not an interchange format. The
format remains part of the experimental API draft and has no compatibility
guarantee yet. Trace `work_units` is cumulative for the run, `rng_draws` is the
source's cumulative lifetime count, and `clock_ticks` is the clock's raw
absolute value. `events()` returns a reference valid only while the observer
exists and until its event vector is next mutated.

## Minimal example

```cpp
#include "py2cpp4or/gvns/core.h"

struct ToySolution { int score; };

// Application-defined ToyClock and ToyRandom implement the Clock and
// RandomSource interfaces. Neighborhoods contain only synthetic logic here.
std::vector<py2cpp4or::gvns::Neighborhood<ToySolution>> neighborhoods{
    {"N1-Increment", true,
     [](const ToySolution& current, py2cpp4or::gvns::NeighborhoodContext&) {
         if (current.score > 0) {
             return py2cpp4or::gvns::NeighborhoodOutcome<ToySolution>::improved(
                 ToySolution{current.score - 1}, 1U);
         }
         return py2cpp4or::gvns::NeighborhoodOutcome<ToySolution>::exhausted(1U);
     }},
};

py2cpp4or::gvns::VndEngine<ToySolution> engine{
    std::move(neighborhoods),
    [](const ToySolution& solution) {
        return static_cast<double>(solution.score);
    },
    py2cpp4or::gvns::StrictMinimizationAcceptance{0.0}};

ToyClock clock;
ToyRandom random;
py2cpp4or::gvns::NeverStopPolicy stop;
py2cpp4or::gvns::VectorTraceObserver trace;
const auto result = engine.run(
    ToySolution{3}, "toy-run", random, clock, stop, trace);
```

See the [toy tests](../../../tests/cpp/gvns/toy_gvns_tests.cpp) for complete
synthetic clock, RNG, and neighborhood implementations.

## Build and test

From the repository root:

```console
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

The CMake project requires CMake 3.15 or newer, enforces C++14 without compiler
extensions, and supports GCC and MSVC. Configuration performs no dependency
downloads. A minimal GitHub Actions workflow runs the same configure, build,
and CTest path with GCC.

## Draft limitations

This prototype does not provide a complete top-level GVNS algorithm, a stable
ABI, install/export packaging, a Python counterpart, problem adapters,
maximization acceptance, performance claims, or compatibility guarantees. The
API should be reviewed in real public synthetic integrations before it is
described as verified or supported.
