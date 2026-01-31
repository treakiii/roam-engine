# Contributing to Roam Engine

Thank you for your interest in contributing to **Roam Engine** 🚀

Roam Engine is a community-driven, open-source C++ game engine focused on high-performance systems and large-scale open-world simulation. All contributions are welcome — from bug fixes and performance improvements to new engine modules and documentation updates.

---

## Code of Conduct

By participating in this project, you agree to maintain a respectful and professional environment.

* Be constructive and respectful
* Avoid harassment or toxic behavior
* Provide helpful feedback
* Keep discussions technical and productive

Failure to follow these guidelines may result in restricted access to the repository.

---

## Ways You Can Contribute

You can help Roam Engine grow in many ways:

### 🛠 Code Contributions

* Engine core improvements
* Rendering optimizations
* Physics and collision systems
* Memory management
* Multithreading systems
* Platform support
* Performance profiling tools

### 🧪 Testing & Debugging

* Report bugs
* Validate fixes
* Stress test large worlds
* Benchmark engine performance

### 📚 Documentation

* Improve code comments
* Add usage examples
* Write engine architecture guides
* Create setup tutorials

### 💡 Feature Proposals

* Suggest new systems
* Improve existing modules
* Propose optimizations

---

## Getting Started

### 1. Fork the Repository

Click the **Fork** button on GitHub to create your own copy of the repository.

### 2. Clone Your Fork

```bash
git clone https://github.com/YOUR_USERNAME/roam-engine.git
cd roam-engine
```

### 3. Create a Feature Branch

Always create a new branch for your changes:

```bash
git checkout -b feature/my-feature-name
```

---

## Coding Standards

Roam Engine follows strict C++ coding standards to maintain performance and readability.

### General Rules

* Use modern C++ (C++17 or newer)
* Avoid unnecessary heap allocations
* Prefer RAII patterns
* Avoid global state
* Write cache-friendly code
* Minimize dependencies

### Naming Convention

* Classes: `PascalCase`
* Functions: `camelCase()`
* Variables: `camelCase`
* Constants: `UPPER_SNAKE_CASE`
* Files: `snake_case.cpp / snake_case.h`

Example:

```cpp
class WorldStreamer
{
public:
    void loadRegion(int regionId);
};
```

---

## Performance Requirements

Roam Engine is performance-first.

When submitting code:

* Avoid unnecessary memory copies
* Use move semantics where possible
* Profile heavy systems
* Avoid blocking calls on main threads
* Use multithreading safely

Pull requests that significantly degrade performance may be rejected.

---

## Commit Guidelines

Write clear and meaningful commit messages.

Good examples:

* `Added async world streaming system`
* `Optimized physics broadphase collision`
* `Fixed memory leak in renderer`

Bad examples:

* `Update`
* `Fix stuff`
* `Changes`

---

## Submitting a Pull Request

Before submitting:

* Make sure the engine builds successfully
* Run unit tests (if applicable)
* Format your code
* Remove debug logs

### Pull Request Steps

1. Push your branch to GitHub
2. Open a Pull Request
3. Describe your changes clearly
4. Link related issues (if any)

Your PR will be reviewed by maintainers. Feedback may be requested before merging.

---

## Reporting Bugs

When reporting bugs, include:

* Operating system
* Compiler and version
* Build configuration (Debug/Release)
* Steps to reproduce
* Crash logs or error output

Clear bug reports help fix issues faster.

---

## Feature Requests

Feature ideas are welcome.

When submitting a proposal:

* Explain the problem
* Describe the solution
* Include performance impact
* Consider engine architecture compatibility

---

## License Agreement

By contributing to Roam Engine, you agree that your contributions will be licensed under the same license as the project (MIT License).

---

## Thank You

Every contribution helps Roam Engine become a stronger engine.

Thank you for supporting open-source engine development ❤️

> "Performance first. Freedom always." — Roam Engine
