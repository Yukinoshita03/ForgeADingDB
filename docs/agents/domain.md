# Domain docs

How engineering skills consume this repository's domain documentation.

## Before exploring

- Read `CONTEXT.md` at the repository root when it exists.
- Read relevant decisions under `docs/adr/` when they exist.
- If these files do not exist, proceed silently. Domain-modeling creates them when terms or decisions are resolved.

## Layout

This is a single-context repository:

```text
/
|-- CONTEXT.md
|-- docs/adr/
`-- src/
```

## Vocabulary

Use terms as defined in `CONTEXT.md` when naming domain concepts in issues, proposals, hypotheses, tests, and code. If a needed concept is absent, reconsider whether it belongs or record it through domain-modeling.

## ADR conflicts

If proposed work contradicts an existing ADR, identify that conflict explicitly instead of silently overriding the decision.
