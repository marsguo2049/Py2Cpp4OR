# Anonymous Case Studies

## Reading note

These cases record high-level experience that motivated the v2 design. They are intentionally stripped of application domain, private formulation, data, dimensions, results, and implementation details.

They do **not** demonstrate an automated Py2Cpp4OR translation and are not public reproducible examples.

## Case A: model migration and exact-solver hybridization

### Confirmed high-level path

- The Python mathematical model used Pyomo.
- The Python workflow used both Gurobi and CPLEX at different points.
- A later mathematical-model implementation used C++ with CPLEX Concert.
- The C++ algorithm work included NSGA-II.
- It also included a Hybrid GA in which the GA selected part of a solution and an exact solver handled the remaining subproblem.

### Important limitation

An early Python GA candidate existed, but the available version history does not establish that the current C++ GA was translated directly from that Python implementation operator by operator. The public repository must therefore not claim that this case proves automated or direct Python-GA-to-C++-GA translation.

### General lesson

Model equivalence and algorithm equivalence are separate claims. A migrated model can be validated across languages without proving that every heuristic operator was translated from an earlier implementation. Hybrid designs also need a clean boundary between the generic evolutionary core and the exact subproblem solver.

## Case B: route-first metaheuristic validation path

### Confirmed high-level path

- The main mathematical model used Python with Pyomo and called Gurobi through Pyomo.
- The main model was not built directly with gurobipy; gurobipy appeared only in limited diagnostic or lower-level supporting work.
- The Python algorithm work included VND and a route-first GVNS.
- The corresponding C++ mathematical model, decoder, and GVNS were not complete at the time of this Phase 0 review.

### General lesson

Documentation must keep the modeling layer distinct from the solver backend. For a future C++ port, the mathematical model and decoder should be validated before the GVNS. Route representation, decoding, feasibility, objective evaluation, and problem-specific neighborhoods belong behind adapters rather than inside a generic GVNS core.

## What remains private

No application domain, business terminology, formulas, variable or constraint names, data structures, instance sizes, parameter values, computational results, performance figures, repositories, paths, or unpublished conclusions from either case belong in this public repository.

Internal validation against these cases may be useful in the future, but any such work must remain outside the public tree.
