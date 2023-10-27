# Contributing guidelines

## Git workflow

- Branches **prefixes**:
    - `feature/` for new features;
    - `bugfix/` for bug fixes;
    - `docs/` for documentation changes;
- Include **issue number** in branch name: `feature/42-foo-bar`;
- **Meaningful & short** commit messages: `Add foo bar` instead of `Fix bug`;
- Name PRs **exactly like issue**: `Foo bar`;
- **Squash** commits before merging;
- Use **kanban** board in projects tab.

## Coding style

- Variables & functions names in `snake_case`;
- Structs in `PascalCase`;
- Constants & enums in `SCREAMING_SNAKE_CASE`;
- Curly braces on new lines:

```c
// Good
int main()
{
    printf("Hello World!");
    return 0;
}

// Bad
int main() {
    printf("Hello World!");
    return 0
}
```

- One line statements if possible:

```c
// Good
if (a < b) return a;

// Bad
if (a < b)
{
    return a;
}
```
