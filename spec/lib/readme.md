# Melon Library Specification

**Melon** is a core library specifically meant to be simple and well contained, having a slight bias for games. 

## Foreword(s)

### On specifications

This is intended to be a formal specification. If you have formalphobia or makes you exprience an orgasm, please consider the *upsides and downsides* to a formal specification:

**Good things**

* **Very accruate**: Specs are very accurate and can help you to think about details you might not consider when writing code alone while not having to think about details that do not matter.
* **More abstract**: Specs provide a way to abstract away details more easily than code does, allowing for breifness with clearity.
* **Clear about details**: If you understand what the spec means, you will often be able to figure out the most important details about something and be confident that something is correct.
* **It's just cool**: It's just really cool to convert between to representations of something and have both take a place.

**Bad things**

* **Understanding is obfuscated**: As with abstraction, simple to understand things are often put in to obtuse and/or abstract words.
* **Missing details**: Of course, on the first side of being abstract, some details are missing entirely, allowing non-optimal choosing of details, or scattered in other parts of the specification.
* **Can give formality paralisys**: One result of the first point is that you can sometimes get in at state of paralisys where you don't understnad what you are doing but are playing with abstractions regardless, often without understanding the implications of your actions.
* **They are boring**: Specifications, to some people, are extremely dry and boring. That is fine and valid, and should be considered when thinking about specifications in projects as a whole. (For example, they can't be used as a replacement for documentation.)

## Basic conventions

The key words MUST, MUST NOT, SHOULD, SHOULD NOT, MAY and other keywords (indicated by being writting in ALL CAPS) SHOULD be interpeted as in [RFC 2119](https://www.rfc-editor.org/rfc/rfc2119).

Other basic conventions are set in the *Conventions* section.

## [Next: Conventions](conventions.md)
