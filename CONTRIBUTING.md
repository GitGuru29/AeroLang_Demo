# Contributing to AeroLang

Thank you for your interest in contributing to AeroLang! 🚀

## 🔒 Code Access Levels

AeroLang has different access levels:

- **Public**: Compiler core, runtime library, documentation
- **Limited**: Advanced optimization algorithms (request access)
- **Private**: Commercial features, proprietary modules

## 🤝 How to Contribute

### Bug Reports
- Use GitHub Issues
- Provide `.aero` code that reproduces the bug
- Include compiler output and error messages

### Feature Requests
- Open an issue with `[Feature Request]` tag
- Describe the use case
- Provide example AeroLang syntax

### Pull Requests

1. **Fork the repository**
2. **Create a feature branch**: `git checkout -b feature/amazing-feature`
3. **Make your changes**
4. **Test thoroughly**: `cmake --build build && ctest`
5. **Commit**: `git commit -m 'Add amazing feature'`
6. **Push**: `git push origin feature/amazing-feature`
7. **Open a Pull Request**

### Code Style

- **C++**: Follow C++17 standards
- **Comments**: Document complex logic
- **Naming**: Use `camelCase` for variables, `PascalCase` for classes
- **Format**: Use clang-format (provided config)

## 📝 Development Setup

```bash
# Clone repository
git clone https://github.com/yourusername/AeroLang.git
cd AeroLang

# Build
mkdir build && cd build
cmake ..
cmake --build .

# Test
./aeroc ../examples/HelloWorld.aero
```

## 🧪 Testing

- Add tests for new features in `tests/`
- All tests must pass before PR approval
- Test both successful compilation and error cases

## 📜 License

By contributing, you agree that your contributions will be licensed under the MIT License.

## ❓ Questions?

- **Discord**: [Coming soon]
- **Email**: aerolang@example.com
- **Issues**: GitHub Issues for public questions

---

**Note**: For access to advanced modules, contact the maintainers with your use case.
