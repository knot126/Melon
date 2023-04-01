# Conventions

This is a list of conventions and notations used in this specification in no piticular order.

## Naming

* All globally accessable function names SHOULD be prefixed with `Dg`.
* All globally accessable functions SHOULD be in pascal case (`PascalCase`), not considering the prefix.
* All type names must follow the same restrictions as global function names. This includes structure tags, structure names, classes and typedefs.
* Enumeration values SHOULD use capital snake case (`CAPITAL_SNAKE_CASE`) and SHOULD be prefixed `DG_`.
* All globals SHOULD be in pascal case (`PascalCase`), prefixed with `g`. They MAY not be exposed.
* All local variables SHOULD use snake case (`snake_case`).

## Specification Language

**INFORMATIONAL**: Other than the notes listed below, you can assume that the programming language used is the C programming language. This does not mean that Melon is exclusive to the C programming language, and suggentions may be given about how to interoperate or implement melon in other languages in ways that are better adapted to these languages.

### Structures

* Structures and records SHALL use the `struct` keyword.
* Structures and records MAY have implied padding and MUST NOT be directly read or written from files.

## [Prev: Readme](readme.md)
## [Next: Module `alloc`](alloc.md)
