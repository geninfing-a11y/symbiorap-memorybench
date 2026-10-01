# Contributing

Contributions are welcome when they preserve benchmark neutrality and reproducibility.

## Rules

1. Do not change a published workload in place. Introduce a new version.
2. Do not remove or hide failed repetitions or negative results.
3. Do not compare systems from different semantic classes as if their operations were equivalent.
4. Add tests for parser, schema, or adapter changes.
5. Include environment metadata for submitted benchmark results.
6. Keep proprietary benchmarked-system code outside this repository unless you have the right to publish it.

Pull requests that alter a frozen benchmark definition must explain the motivation and use a new benchmark version identifier.
