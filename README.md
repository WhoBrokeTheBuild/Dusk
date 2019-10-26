# Dusk Game/Simulation Engine

[![Build Status](https://travis-ci.org/WhoBrokeTheBuild/Dusk.svg?branch=master)](https://travis-ci.org/WhoBrokeTheBuild/Dusk)
[![Build Status](https://ci.appveyor.com/api/projects/status/github/WhoBrokeTheBuild/Dusk?svg=true)](https://ci.appveyor.com/project/WhoBrokeTheBuild/dusk)
[![Build Status](https://jenkins.stephenlw.com/job/Dusk/badge/icon)](https://jenkins.stephenlw.com/job/Dusk/)

## Building

```
cd Build
cmake ..
cmake --build .
```

## Running Examples

The examples are built to run from their source directories. To facilitate this, there are convenience targets prefixed with `run-`

```
cmake --build . --target run-HelloWorld
```

## Contributing

### Pull Requests

Use the "Branch and Pull Request" model for contributing changes.

### Commit Message Format

```
<type>(<scope>): <title>
<message>
```

**Allowed `<type>`**

* `Release`: A new release, increments Major
* `Feat`: A new feature, increments Minor
* `Fix`: A small fix, increments Patch
* `Chore`: A change to the build process or tooling
* `Test`: Unit / Integration test updates
* `Doc`: Documentation updates
* `Nit`: A small change related to style or snytax
* `Refactor`: A change to refactor code

**Allowed `<scope>`**

Scope is optional, but it should refer to a subsystem of the engine.

**`<subject>`**

Subject should contain a succinct description of the change.

* Use present tense, "change" not "changed" or "changes"
* Capitalize the first letter
* Don't end with a period

**`<message>`**

* Use present tense, as in `<subject>`
* Include whatever details and motivations are relevant to the change

**Examples**

```
Feat(Scene): Add glTF Camera Loading
Updated Scene loading to create Camera objects

Nit: Switch to for(auto& it) from for(int i)

Chore(Build): Change default compiler to GCC
```