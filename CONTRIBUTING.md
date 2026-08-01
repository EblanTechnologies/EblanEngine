# Contributing to EblanEngine

First of all — thank you for considering contributing!  
We're happy to review any pull requests you send. 🎮

> **Before you start**, please check the [Actions tab](https://github.com/EblanTechnologies/EblanEngine/actions) to see if the CI passes for your platform.  
> If it fails, but you'd like to help us fix platform-specific issues — we'd really appreciate it!

---

## Development Setup

### Windows

1. Install **Visual Studio** with these workloads:
    - **Desktop development with C++**
    - **Game development with C++** (at least the minimum)
    - (Optional) **Linux and embedded development with C++** — for cross-platform support
    - (Optional) **Mobile development with C++**

   Go to the **Individual components** tab and make sure the latest **MSVC** is installed.

2. Install **Xmake**:
   ```bash
   winget install xmake
   ```
   Or download from [xmake.io](https://xmake.io/)

3. Choose your IDE:
   #### Visual Studio:
   ```bash
   xmake project -k vsxmake
   ```
   Open: `/vsxmake2022/EblanEngine.sln`

   (The year may vary — if you have VS 2019, it will be vsxmake2019)
   #### VSCode / CLion:
   * Install the [Xmake plugin](https://xmake.io/guide/extensions/ide-integration-plugins.html)
   * Generate `compile_commands.json`:
     ```bash
     xmake project -k compile_commands
     ```
---

### Linux:
1. Install dependencies:
   ```bash
   sudo apt-get update
   sudo apt-get install -y \
       curl \
       clang-17 \
       libgl1-mesa-dev \
       libglu1-mesa-dev \
       libxext-dev \
       libx11-dev \
       libxrandr-dev \
       libxinerama-dev \
       libxcursor-dev \
       libxi-dev \
       libxfixes-dev \
       libxrender-dev \
       libxss-dev \
       libxtst-dev \
       build-essential \
       pkg-config
   ```
2. Install Xmake:
   ```bash
   curl -fsSL https://xmake.io/shget.text | bash
   ```
3. Set up **VSCode** (example):
   * Install plugins: [clangd](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd), [xmake](https://marketplace.visualstudio.com/items?itemName=tboox.xmake-vscode)
   * After installing clangd, you'll see a popup asking to install the clangd server — click **"Install"**
   * Generate `compile_commands.json`:
     ```bash
     xmake project -k compile_commands
     ```
   * Restart VSCode

---

### Building & Running
```bash
xmake build -v         # Build the engine
xmake run EblanEngineApp  # Run the application
```

For more details, check the [README.md](README.md)
A proper Roadmap will be added in the future — stay tuned!

---

### Questions?
Feel free to open an issue or discussion. We're all learning here!
