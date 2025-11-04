# Py2Cpp4OR

Intelligent agent for translating Python optimization models (gurobipy/pyomo) to efficient C++ code, with iterative learning capabilities.

## Purpose

Bridge Python OR research and C++ production performance through automated translation and continuous improvement.

## Architecture

**Iterative Learning Framework:**
```
agent.md → Execute → reasoning_traces/ → Feedback → learning_log.md → Update agent.md
```

**Repository Structure:**
```
.
├── agent.md              # Agent operational rules (LLM context)
├── learning_log.md       # Experience summary & gap analysis
├── reasoning_traces/     # Distilled decision logs
├── examples/             # Python-C++ translation pairs
└── tests/                # Validation cases
```

## Workflow

1. **Execute**: Agent reads `agent.md` and translates models
2. **Record**: Log key decisions and issues in `reasoning_traces/`
3. **Learn**: Extract patterns into `learning_log.md`
4. **Evolve**: Refine `agent.md` based on accumulated experience

## Philosophy

GitHub stores refined knowledge; local development (Cursor/Claude Code) handles experimentation. This repository serves as both documentation and daily progress tracker.

## License

MIT
