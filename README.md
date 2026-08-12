# Enigma 

A modular C++ implementation of the **Enigma cipher machine**, featuring configurable rotors, ring settings, rotor positions, turnover positions, reflectors, persistent configuration files, random configuration generation, and an interactive command-line interface.

The project is designed with a clean separation between the **cryptographic core**, **configuration system**, and **CLI tools**, making it easy to experiment with the internal mechanics of an Enigma-style machine.

---

## Features

* Three-rotor Enigma-style encryption
* Configurable rotor wiring
* Forward and inverse rotor transformations
* Configurable:

  * Ring settings
  * Initial rotor positions
  * Turnover positions
  * Rotor wiring
* Reflector support with involution validation
* Authentic-style rotor stepping and double-stepping behavior
* Random rotor wiring generation
* Configuration save/load system
* Interactive command-line interface
* ANSI-colored Windows terminal interface
* Modular C++ architecture
* Input and configuration validation
* CMake-based build system
* Separate configuration and encryption executables

---

## How It Works

The encryption process follows the classic Enigma-style signal path:

```text
                    ┌──────────────┐
                    │   Keyboard   │
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
                    │    Rotor 3   │
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
                    │    Rotor 2   │
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
                    │    Rotor 1   │
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
                    │  Reflector   │
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
                    │ Rotor 1⁻¹    │
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
                    │ Rotor 2⁻¹    │
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
                    │ Rotor 3⁻¹    │
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
                    │   Output     │
                    └──────────────┘
```

Before each character is processed, the rotor mechanism advances according to the configured turnover positions.

The middle rotor also implements the characteristic **double-stepping behavior**:

```text
Right rotor  → always rotates

Middle rotor → rotates when:
               • right rotor is at its notch
               OR
               • middle rotor is at its notch

Left rotor   → rotates when:
               • middle rotor is at its notch
```

---

## Rotor Model

Each rotor contains:

* A permutation of the 26 lowercase English letters
* An inverse permutation
* A ring setting
* A current position
* A turnover position
* A name

The forward transformation can be represented conceptually as:

```text
input
  │
  ▼
position/ring offset
  │
  ▼
rotor wiring
  │
  ▼
inverse offset
  │
  ▼
output
```

For the reverse path, the precomputed inverse wiring is used.

This allows the same `Rotor` class to handle both directions of signal propagation.

---

## Reflector

The `Reflector` class validates that its wiring is a valid permutation and satisfies the required reflector properties.

A valid reflector must satisfy:

```text
R(R(x)) = x
```

and must not contain fixed points:

```text
R(x) ≠ x
```

Therefore the reflector wiring forms pairs such as:

```text
a ↔ y
b ↔ r
c ↔ u
...
```

Invalid reflector configurations are rejected during construction.

---

## Configuration System

The project includes a dedicated configuration application:

```text
enigma_setup
```

It allows the user to configure:

### Rotors

Choose from five predefined rotor wirings:

```text
I
II
III
IV
V
```

or create a custom rotor.

For each rotor, the user can specify:

```text
Name
Wiring
Ring Setting
Initial Position
Turnover Position
```

### Reflector

The setup tool supports:

```text
Reflector B
Reflector C
Custom Reflector
```

### Random Configuration

The application can also generate random rotor wirings and turnover positions.

Randomly generated rotor wirings are checked so that the same wiring is not assigned to multiple rotors.

---

## Configuration File

Configurations can be saved to:

```text
enigma_config.txt
```

Example:

```text
# Enigma Machine Configuration
# Format: name, ringSetting, position, turnover, wiring

rotor1: I, 0, 0, 16, ekmflgdqvzntowyhxuspaibrcj
rotor2: II, 0, 0, 4, ajdksiruxblhwtmcqgznpyfvoe
rotor3: III, 0, 0, 21, bdfhjlcprtxvznyeiwgakmusqo
reflector: yruhqsldpxngokmiebfzcwvjat
```

The encryption program loads this configuration when it starts.

The same configuration can also be reloaded during execution using:

```text
:r
```

This provides a simple way to restore the machine to its initial state.

---

## Command-Line Interface

After configuration, run the CLI application:

```text
enigma_cli
```

The interface supports the following commands:

| Command | Description                                    |
| ------- | ---------------------------------------------- |
| `:h`    | Display help                                   |
| `:s`    | Show current rotor positions and ring settings |
| `:r`    | Reset machine to the initial configuration     |
| `:c`    | Clear the terminal                             |
| `:q`    | Exit the program                               |

Any other input is treated as plaintext and passed through the machine.

Example:

<img width="532" height="508" alt="enig1" src="https://github.com/user-attachments/assets/fab83d42-7d13-4182-9794-fab21603dc24" />

The machine state changes after every processed character because the rotors step before encryption.

---

## Project Architecture

The project is divided into three main layers.

```text
EnigmaProject/
│
├── include/
│   ├── Defines.h
│   ├── Rotor.h
│   ├── Reflector.h
│   ├── EnigmaMachine.h
│   └── EnigmaConfigurator.h
│
├── src/
│   ├── Rotor.cpp
│   ├── Reflector.cpp
│   ├── EnigmaMachine.cpp
│   └── EnigmaConfigurator.cpp
│
├── tools/
│   ├── enigma_cli/
│   │   └── EnigmaCLI.cpp
│   │
│   └── enigma_setup/
│       └── EnigmaSetup.cpp
│
├── bin/
│
├── CMakeLists.txt
└── README.md
```

### Core Library

The cryptographic implementation is contained in:

```text
enigma_core
```

It consists of:

```text
Rotor
Reflector
EnigmaMachine
EnigmaConfigurator
```

### Executables

Two independent executables are generated:

```text
enigma_setup
enigma_cli
```

This separation keeps the configuration interface independent from the encryption interface.

---

## Class Overview

### `Rotor`

Responsible for:

* Rotor wiring
* Inverse wiring
* Position management
* Ring settings
* Turnover detection
* Rotation
* Forward transformation
* Reverse transformation

### `Reflector`

Responsible for:

* Reflector wiring
* Wiring validation
* Symmetry/involution validation
* Signal reflection

### `EnigmaMachine`

Responsible for:

* Managing the three rotors
* Managing the reflector
* Rotor stepping
* Encryption/decryption transformation
* Configuration loading
* Processing complete strings

### `EnigmaConfigurator`

Responsible for:

* Interactive configuration
* Rotor selection
* Reflector selection
* Custom components
* Random configuration
* Configuration validation
* Saving configuration files

---

## Encryption and Decryption

One of the important properties of an Enigma-style machine is that encryption is reciprocal.

For a fixed machine state:

```text
E(E(x)) = x
```

provided that the machine starts from the same rotor configuration before both operations.

In practical use, the sender and receiver therefore need to start with identical:

```text
Rotor wirings
Rotor positions
Ring settings
Turnover positions
Reflector
```

The rotor state also changes during encryption, so the starting configuration is part of the cryptographic key.

---

## Validation

The project performs validation at several levels.

### Rotor Wiring

A rotor wiring must:

```text
• contain exactly 26 characters
• contain every letter a-z exactly once
```

### Reflector Wiring

A reflector must additionally:

```text
• contain no fixed points
• be an involution
• map every pair symmetrically
```

### Numeric Configuration

The following values are restricted to:

```text
0 ≤ value < 26
```

for:

```text
Ring Setting
Position
Turnover Position
```

Invalid user input is rejected and requested again.

---

## Requirements

* C++11 or later
* CMake 3.10+
* A C++ compiler
* Windows for the current ANSI/console implementation

The project currently uses:

```cpp
CMAKE_CXX_STANDARD 11
```

and relies on the Windows API for enabling Virtual Terminal Processing.

---

## Building

Clone the repository:

```bash
git clone https://github.com/YOUR_USERNAME/EnigmaProject.git
cd EnigmaProject
```

Create a build directory:

```bash
mkdir build
cd build
```

Configure the project:

```bash
cmake ..
```

Build:

```bash
cmake --build .
```

The executables are configured to be placed in:

```text
bin/
```

After a successful build:

```text
bin/
├── enigma_cli.exe
└── enigma_setup.exe
```

---

## Usage

### 1. Create a configuration

Run:

```text
enigma_setup.exe
```

Configure:

```text
Rotor 1
Rotor 2
Rotor 3
Reflector
```

Then select:

```text
Save configuration
```

The default configuration filename is:

```text
enigma_config.txt
```

---

### 2. Start the machine

Run:

```text
enigma_cli.exe
```

The program automatically attempts to load:

```text
enigma_config.txt
```

A custom configuration file can also be supplied:

```text
enigma_cli.exe my_config.txt
```

---

### 3. Encrypt a message

Simply type:

```text
> attackatdawn
```

The machine returns the transformed text.

Spaces, punctuation, and other non-alphabetic characters are preserved.

Alphabetic input is normalized to lowercase.

---

## Design Goals

This project was built with several goals in mind:

1. **Understand the internal mechanics of the Enigma machine**
2. Practice object-oriented programming in C++
3. Separate cryptographic logic from user interface code
4. Implement permutation-based transformations
5. Work with inverse mappings
6. Implement stateful transformations
7. Practice configuration serialization
8. Build a reusable C++ core library
9. Provide both interactive setup and encryption interfaces

Rather than treating the Enigma machine as a single encryption function, the project models its individual components as independent objects.

---

## Technical Notes

The machine operates on permutations of the 26-letter alphabet.

Each rotor can be viewed mathematically as a permutation:

```text
R : {0,1,...,25} → {0,1,...,25}
```

The reverse traversal uses:

```text
R⁻¹
```

The complete signal transformation can therefore be represented schematically as:

```text
R₃ → R₂ → R₁ → F → R₁⁻¹ → R₂⁻¹ → R₃⁻¹
```

where `F` is the reflector permutation.

Rotor positions and ring settings modify these permutations through cyclic offsets.

This makes the implementation a useful practical example of:

* Permutations
* Inverse functions
* Modular arithmetic
* Stateful transformations
* Object-oriented design

---

## Disclaimer

This project is primarily an **educational implementation of an Enigma-style cipher machine**.

It should not be considered a modern secure cryptographic system. The original Enigma cipher is historically significant but cryptographically obsolete.

The purpose of this project is to explore its mechanics, implementation, and software architecture rather than provide real-world security.

---

## License

This project is intended for educational and experimental use.

If you add a specific open-source license to the repository, replace this section with the corresponding license text.

---

## Author

**Mohammadreza Ahmadi**

GitHub: [Brave-Potato9](https://github.com/Brave-Potato9)
