# Legacy v1

## Why v1 was replaced

The first repository version captured practical notes from early migration experiments as if they were universal Agent rules and completed public capabilities. The current public tree, however, contained documentation and Agent prompts rather than a runnable translation toolchain.

Some v1 material also embedded case-specific symbols, fixed dimensions, domain-flavored names, path conventions, and implementation preferences. Those details were unsuitable for a general public framework and have been removed from the current tree.

## Reusable lessons retained

The following ideas remain useful after generalization:

- generated C++ must be readable and auditable;
- model structure should be inventoried before translation;
- data preprocessing and derived values are part of model semantics;
- source and target implementations need explicit differential verification;
- debug and trace output should help localize mismatches;
- case-specific knowledge should be separated from reusable framework logic;
- feature claims should be supported by runnable public evidence.

These lessons now appear as requirements in the architecture, migration, verification, and privacy documents rather than as rigid code-generation recipes.

## Rules intentionally retired

v2 does not preserve the following v1 assumptions as universal requirements:

- a mandatory sequence of six named Agents;
- claims of end-to-end automated translation;
- fixed problem dimensions or hard-coded index sizes;
- case-specific class, variable, cost, transport, or facility names;
- one Windows-only path or file-reading convention;
- mandatory raw pointers and manual `new`/`delete[]` ownership;
- a ban on build systems, tests, or CI;
- unverified performance numbers;
- a requirement to generate only a fixed trio of C++ files.

Future implementation choices should follow the public architecture, solver SDK guidance, tests, and demonstrated needs.

## Current-tree cleanup

Phase 0 replaces the old top-level Agent/development rules, removes the old learning log and user guide, and removes the specialized `.claude/agents/` prompts containing legacy case assumptions. Their reusable lessons are summarized here without reproducing private or identifying details.

## Historical commits

This cleanup affects the current tree only. Earlier commits may still contain removed text. The repository history has not been rewritten; see [privacy.md](privacy.md) for the separate process required if historical erasure is ever necessary.
