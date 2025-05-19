# LLaMA Translator

**LLaMA Translator** is a lightweight translation tool that leverages Meta's LLaMA language model to perform multilingual translations.  
Designed for simplicity and efficiency, this project provides a foundational framework for integrating LLaMA-based translations into various applications.

## Features

- Utilizes Meta's LLaMA model for translation tasks
- Modular C++ codebase for easy integration and extension
- Includes build scripts for streamlined compilation
- Open-source under the GPL-3.0 license

## Getting Started

### Prerequisites

- C++17 compatible compiler (e.g., GCC, Clang, MSVC)
- CMake 3.10 or higher
- [LLaMA model weights](https://github.com/meta-llama/llama)

### Installation

1. **Clone the Repository**
   ```bash
   git clone https://github.com/fsb3rke/llama-translater.git
   cd llama-translater
   ```

2. **Build the Project**
   ```bash
   ./build.bat
   ```
   *Only For Windows users.*

3. **Configure the Model**

   Install the ollama. Install a LLaMA model in ollama.

## Usage

After building the project, you can run the translator with the following command:

```bash
./llama-translater
```

## Project Structure

- `src/` - Contains the main source code files
- `include/` - Header files for the project
- `lib/` - External libraries and dependencies
- `bin/` - Compiled binaries
- `build.bat` - Windows build script

## Contributing

Contributions are welcome! If you have suggestions for improvements or encounter any issues, please open an issue or submit a pull request.

## License

This project is licensed under the [GNU General Public License v3.0](LICENSE).

---

*Note: Ensure you have the appropriate rights and permissions to use the LLaMA model weights in your project.*

For more information on the LLaMA models, visit the [official Meta LLaMA repository](https://github.com/meta-llama/llama).
