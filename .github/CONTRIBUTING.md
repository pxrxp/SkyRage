# Contributing

Thanks for taking an interest in SkyRage!

This is a small game project, so keep changes focused. For a bigger change, open an issue first so we can talk it through. Bug fixes and small improvements can go straight into a pull request.

## Before you start

- Check the existing issues to see if the bug or idea is already covered.
- For bug reports, include what happened and how to reproduce it. Screenshots or logs help when relevant.
- Please only submit assets you made or have permission to use.

## Building

SkyRage uses C++20, CMake, and SFML. Initialize the submodules first:

```bash
git submodule update --init --recursive
```

Then build with `./build.sh` on Linux or `build.bat` on Windows. The README has more build details.

## Pull requests

- Keep the PR about one thing where possible.
- Say what you changed and how you tested it.
- Include a screenshot or short clip for visible gameplay changes, if useful.
- Don't include generated build files.

The project has a `.clang-format` file; use it for C++ formatting. No need to overthink it—small, understandable changes are best.
