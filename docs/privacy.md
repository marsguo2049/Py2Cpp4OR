# Privacy and Publication Boundaries

## Purpose

Py2Cpp4OR is a public framework. Reusable ideas may be published, but private research, business context, data, code, and infrastructure must remain outside this repository.

Privacy review is required before committing an example, test, trace, benchmark, screenshot, report, or agent instruction.

## Allowed public material

- generic interfaces, pseudocode, state machines, and verification procedures;
- independently written explanations of common migration patterns;
- fully synthetic toy problems with no private business semantics;
- synthetic datasets that cannot be linked back to a real case;
- generic Python/C++ mapping rules;
- public test harnesses, build files, CI, and benchmark frameworks when they are real and runnable;
- high-level anonymous case-study summaries limited to the facts in [case-studies.md](case-studies.md).

## Prohibited public material

- private project names, repository names, directory layouts, or internal file paths;
- industry, client, location, network, or operational context that could identify a case;
- private formulas, variable names, constraint names, schemas, or data structures;
- instances, raw or transformed data, parameter values, results, or performance numbers;
- logs, commit hashes from private repositories, machine names, cluster details, or local paths;
- unpublished papers, reports, reviews, examination material, or conclusions;
- private source code or substantial code reproduced with superficial renaming;
- supervisor-, collaborator-, employer-, or client-provided code without explicit publication rights;
- third-party code with unclear or incompatible licensing;
- commercial solver libraries, proprietary model files, credentials, tokens, or license files.

## Synthetic means synthetic

Changing names in a private instance is not sufficient. A public toy problem should be designed independently, use invented data, be small enough to explain completely, and avoid distinctive structure that could reveal its source.

Do not connect to, import, or inspect a private repository in order to “generalize” it for publication.

## Anonymous case studies

Anonymous cases may communicate a high-level migration path, for example that a Pyomo model was later implemented with a C++ solver API. They must not include the application domain, organization, geography, private mathematical formulation, data shape, parameter values, results, or unpublished conclusions.

Case studies document motivation and validation needs; they do not prove that Py2Cpp4OR has automated the described migration.

## Pre-publication checklist

- [ ] Every example and dataset was created specifically for public use.
- [ ] No private names, paths, symbols, schemas, dimensions, or business terms remain.
- [ ] No proprietary solver files, credentials, or license material are present.
- [ ] No performance or completion claim exceeds the runnable public evidence.
- [ ] Third-party material has a known compatible license and attribution.
- [ ] Logs and metadata have been checked for usernames, hosts, hashes, and infrastructure details.
- [ ] Anonymous cases remain non-identifying and match only the approved high-level facts.
- [ ] `git diff` and the complete current tree have both been reviewed.

## Git history warning

Deleting a file from the current branch does not remove it from earlier commits. Phase 0 cleans the current tree but does not rewrite repository history.

If historical content later requires permanent removal, stop and perform a separate, explicitly authorized process with backups, collaborator coordination, force-push planning, and credential rotation where relevant. Do not rewrite history as a routine documentation change.

## Handling uncertainty

If publication rights or identifiability are uncertain, do not commit the material. Replace it with a newly created synthetic example or keep the validation local and private.
