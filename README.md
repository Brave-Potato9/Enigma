# Enigma

A modular C++ implementation of an **Enigma-style cipher machine**, featuring configurable rotors, ring settings, rotor positions, turnover positions, reflectors, plugboard pairs, persistent configuration files, random configuration generation, and interactive command-line interfaces.

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
* Reflector support
* Configurable plugboard
* Plugboard pair swapping
* Authentic-style rotor stepping
* Double-stepping behavior
* Random rotor wiring generation
* Configuration save/load system
* Interactive command-line interface
* ANSI-colored Windows terminal interface
* Modular C++ architecture
* Input and configuration validation
* CMake-based build system
* Separate configuration and encryption executables

---

# How It Works

The encryption process follows the classic Enigma-style signal path.

With the plugboard enabled, the complete signal path is:

```text
                    ┌──────────────┐
                    │   Keyboard   │
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
                    │  Plugboard   │
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
                    │   Rotor 1⁻¹  │
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
                    │   Rotor 2⁻¹  │
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
                    │   Rotor 3⁻¹  │
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
                    │  Plugboard   │
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
                    │    Output    │
                    └──────────────┘
```

Before every alphabetic character is encrypted, the rotors advance according to their configured turnover positions.

The middle rotor implements the characteristic **double-stepping mechanism**:

```text
Right rotor
    │
    └── always rotates

Middle rotor
    │
    ├── rotates if right rotor is at its notch
    │
    └── rotates if middle rotor is at its notch

Left rotor
    │
    └── rotates if middle rotor is at its notch
```

The stepping is performed **before** the character enters the plugboard.

---

# Rotor Model

Each `Rotor` contains:

* A permutation of the 26 lowercase English letters
* An inverse permutation
* A ring setting
* A current position
* A turnover position
* A name

A rotor can mathematically be represented as a permutation:

```text
R : {0,1,...,25} → {0,1,...,25}
```

The inverse permutation:

```text
R⁻¹
```

is used when the signal travels back from the reflector.

Conceptually, the forward path is:

```text
Input
  │
  ▼
Position / Ring offset
  │
  ▼
Rotor wiring
  │
  ▼
Inverse offset
  │
  ▼
Output
```

The same `Rotor` class therefore supports both forward and reverse signal propagation.

---

# Reflector

The `Reflector` represents the fixed substitution in the center of the machine.

A valid reflector must behave as an involution:

```text
R(R(x)) = x
```

and must not map a letter to itself:

```text
R(x) ≠ x
```

Therefore its wiring consists of pairs:

```text
a ↔ y
b ↔ r
c ↔ u
...
```

Invalid reflector configurations are rejected.

The project currently provides:

```text
Reflector B
Reflector C
Custom Reflector
```

---

# Plugboard

The project includes a configurable `Plugboard` component.

The plugboard is represented internally as a mapping of the 26 letters.

By default:

```text
a → a
b → b
c → c
...
z → z
```

When a pair is added:

```text
a ↔ b
```

the mapping becomes:

```text
a → b
b → a
```

The plugboard is applied twice during encryption:

```text
Input
  │
  ▼
Plugboard
  │
  ▼
Rotors → Reflector → Inverse Rotors
  │
  ▼
Plugboard
  │
  ▼
Output
```

A letter can belong to at most one plugboard pair.

For example:

```text
ab cd ef
```

is valid, while:

```text
ab ac
```

is rejected because `a` is already connected to another letter.

The plugboard also preserves the case of alphabetic input when used through `EnigmaMachine::totalTransform()`.

---

# Configuration System

The project contains a dedicated configuration application:

```text
enigma_setup
```

It provides an interactive interface for configuring the machine.

## Rotors

Five predefined rotor wirings are available:

```text
I
II
III
IV
V
```

A custom rotor can also be created.

For each rotor, the user can specify:

```text
Name
Wiring
Ring Setting
Initial Position
Turnover Position
```

The setup program prevents the same predefined rotor wiring from being selected more than once.

---

## Reflector

The setup program supports:

```text
1. Reflector B
2. Reflector C
3. Custom Reflector
```

Custom reflector wiring is validated before being accepted.

---

## Plugboard

The plugboard can be configured manually or reset to an empty/default state.

Manual configuration accepts pairs such as:

```text
a b
c d
e f
```

The configuration process ends with:

```text
q q
```

The program automatically rejects:

* Non-alphabetic input
* Pairing a letter with itself
* Reusing a letter already connected to another pair

---

# Random Configuration

The configuration program can generate random rotor wirings.

Each generated wiring is:

```text
• exactly 26 characters
• a permutation of a-z
• different from the wiring of the other configured rotors
```

A random turnover position is also generated for every rotor:

```text
0 ≤ turnover ≤ 25
```

The generated rotor configuration uses:

```text
Ring Setting = 0
Initial Position = 0
```

The generated configuration is displayed in the terminal before continuing.

---

# Configuration File

The machine configuration can be saved to:

```text
enigma_config.txt
```

A configuration contains:

```text
rotor1
rotor2
rotor3
reflector
plugboard
```

Example:

```text
# Enigma Machine Configuration
# Format: name, ringSetting, position, turnover, wiring

rotor1: I, 0, 0, 16, ekmflgdqvzntowyhxuspaibrcj
rotor2: II, 0, 0, 4, ajdksiruxblhwtmcqgznpyfvoe
rotor3: III, 0, 0, 21, bdfhjlcprtxvznyeiwgakmusqo
reflector: yruhqsldpxngokmiebfzcwvjat
plugboard: badcfeghijklmnopqrstuvwxyz
```

The plugboard line represents its complete 26-character mapping.

For example:

```text
ab
```

being connected results in:

```text
a → b
b → a
```

The encryption program loads the saved configuration when starting.

---

# Configuration Loading

`EnigmaMachine::loadConfigs()` parses the configuration file and reconstructs:

```text
Rotor 1
Rotor 2
Rotor 3
Reflector
Plugboard
```

The loader validates the basic structure of the configuration and reports invalid entries through the terminal.

This allows the machine configuration to persist between executions.

---

# Command-Line Interface

After configuring the machine, run:

```text
enigma_cli
```

The CLI provides several commands.

| Command | Description                                    |
| ------- | ---------------------------------------------- |
| `:h`    | Display help                                   |
| `:s`    | Show current rotor positions and ring settings |
| `:r`    | Reset/reload the machine configuration         |
| `:c`    | Clear the terminal                             |
| `:q`    | Exit the program                               |

Any other input is interpreted as plaintext.

Example:

<img width="532" height="508" alt="enig1" src="https://github.com/user-attachments/assets/fab83d42-7d13-4182-9794-fab21603dc24" />

The machine state changes after every processed character because the rotors step before encryption.

---

# Encryption Process

For every alphabetic character:

### 1. Rotor stepping

The rotor positions are updated.

```text
Left ← Middle notch
Middle ← Right notch OR Middle notch
Right ← Always
```

### 2. Input plugboard

The character passes through:

```text
Plugboard.swap()
```

### 3. Forward rotor traversal

The signal travels:

```text
Rotor 3
   ↓
Rotor 2
   ↓
Rotor 1
```

### 4. Reflection

The signal enters the reflector:

```text
Reflector
```

### 5. Reverse rotor traversal

The signal travels through the inverse rotor mappings:

```text
Rotor 1⁻¹
   ↓
Rotor 2⁻¹
   ↓
Rotor 3⁻¹
```

### 6. Output plugboard

Finally:

```text
Plugboard.swap()
```

is applied again.

The resulting character is returned.

---

# Reciprocal Encryption

An important property of an Enigma-style machine is that the transformation is reciprocal.

For a fixed machine state:

```text
E(E(x)) = x
```

because the signal path uses reversible permutations and a reflector satisfying:

```text
R² = I
```

However, the rotor positions change after every character.

Therefore, to decrypt a message correctly, the receiving machine must begin in the **same initial state** as the encrypting machine.

The following must match:

```text
Rotor wirings
Rotor positions
Ring settings
Turnover positions
Reflector
Plugboard
```

---

# Class Architecture

The project is divided into several classes.

## `Rotor`

Responsible for:

* Rotor wiring
* Inverse wiring
* Position management
* Ring settings
* Turnover detection
* Rotation
* Forward transformation
* Reverse transformation

---

## `Reflector`

Responsible for:

* Reflector wiring
* Reflector validation
* Involution behavior
* Signal reflection

---

## `Plugboard`

Responsible for:

* Maintaining the 26-character mapping
* Adding letter pairs
* Rejecting duplicate connections
* Resetting the mapping
* Swapping characters
* Exporting the mapping as a string

---

## `EnigmaMachine`

Responsible for:

* Managing the three rotors
* Managing the reflector
* Managing the plugboard
* Rotor stepping
* Character transformation
* String transformation
* Loading machine configuration

The main transformation pipeline is:

```text
Plugboard
    ↓
Rotor 3
    ↓
Rotor 2
    ↓
Rotor 1
    ↓
Reflector
    ↓
Rotor 1⁻¹
    ↓
Rotor 2⁻¹
    ↓
Rotor 3⁻¹
    ↓
Plugboard
```

---

## `EnigmaConfigurator`

Responsible for:

* Interactive configuration
* Rotor selection
* Custom rotors
* Reflector selection
* Custom reflectors
* Plugboard configuration
* Random rotor generation
* Input validation
* Configuration saving

---

# Project Architecture

```text
EnigmaProject/
│
├── include/
│   ├── Defines.h
│   ├── Rotor.h
│   ├── Reflector.h
│   ├── Plugboard.h
│   ├── EnigmaMachine.h
│   └── EnigmaConfigurator.h
│
├── src/
│   ├── Rotor.cpp
│   ├── Reflector.cpp
│   ├── Plugboard.cpp
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

---

# Core Library

The core implementation is organized around the following components:

```text
Rotor
Reflector
Plugboard
EnigmaMachine
EnigmaConfigurator
```

The cryptographic logic is separated from the user-facing applications.

This makes the core components reusable independently of the CLI.

---

# Validation

The project performs validation at several levels.

## Rotor Wiring

A rotor wiring must:

```text
• contain exactly 26 characters
• contain unique letters
```

---

## Reflector Wiring

A reflector must:

```text
• contain 26 valid letters
• be a valid permutation
• contain no fixed points
• map pairs symmetrically
```

---

## Plugboard

A plugboard pair must:

```text
• contain two alphabetic characters
• connect two different letters
• use letters that are not already paired
```

---

## Numeric Settings

The following values are restricted to:

```text
0 ≤ value ≤ 25
```

for:

```text
Ring Setting
Initial Position
Turnover Position
```

Invalid input is rejected and requested again.

---

# Requirements

* C++11 or later
* CMake 3.10+
* A C++ compiler
* Windows for the current terminal implementation

The project currently uses:

```cmake
CMAKE_CXX_STANDARD 11
```

The terminal interface uses Windows console functionality for ANSI / Virtual Terminal Processing support.

---

# Building

Clone the repository:

```bash
git clone https://github.com/Brave-Potato9/Enigma.git
cd EnigmaProject
```

Create a build directory:

```bash
mkdir build
cd build
```

Configure:

```bash
cmake ..
```

Build:

```bash
cmake --build .
```

After a successful build, the executables are placed in:

```text
bin/
```

Expected output:

```text
bin/
├── enigma_cli.exe
└── enigma_setup.exe
```

---

# Usage

## 1. Configure the machine

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
Plugboard
```

Then select:

```text
Save configuration
```

The default configuration file is:

```text
enigma_config.txt
```

---

## 2. Start the Enigma machine

Run:

```text
enigma_cli.exe
```

The program loads:

```text
enigma_config.txt
```

as its machine configuration.

A different configuration file can also be supplied if supported by the CLI:

```text
enigma_cli.exe my_config.txt
```

---

## 3. Encrypt text

Enter a message:

```text
> attackatdawn
```

The machine returns the transformed message.

For example:

```text
> attackatdawn
> ...
```

The exact result depends on:

```text
Rotor configuration
Rotor positions
Ring settings
Reflector
Plugboard
```

and therefore changes with the machine configuration.

---

# Design Goals

This project was built to explore both the mathematics and implementation of an Enigma-style machine.

Main goals:

1. Understand Enigma's internal mechanics
2. Practice object-oriented programming in C++
3. Model cryptographic components independently
4. Work with permutations and inverse mappings
5. Implement state-dependent transformations
6. Implement rotor stepping
7. Implement plugboard substitution
8. Practice configuration serialization
9. Build a reusable C++ core
10. Separate configuration and encryption interfaces

Instead of treating encryption as one large function, the project models the machine as a collection of interacting components.

---

# Technical Notes

The machine can be viewed mathematically as a composition of permutations.

For a fixed rotor state, the transformation has the form:

```text
P ∘ R₃⁻¹ ∘ R₂⁻¹ ∘ R₁⁻¹ ∘ F ∘ R₁ ∘ R₂ ∘ R₃ ∘ P
```

where:

```text
P  = Plugboard permutation
Rᵢ = Rotor permutation
F  = Reflector permutation
```

The reflector satisfies:

```text
F² = I
```

and the plugboard is also an involution:

```text
P² = I
```

The inverse rotor mappings satisfy:

```text
Rᵢ⁻¹ ∘ Rᵢ = I
```

Rotor positions and ring settings introduce state-dependent cyclic offsets into these permutations.

This makes the implementation a practical example of:

* Permutations
* Inverse functions
* Modular arithmetic
* Function composition
* Stateful transformations
* Object-oriented design

---

# Disclaimer

This project is primarily an **educational implementation of an Enigma-style cipher machine**.

It should not be considered a modern secure cryptographic system.

The original Enigma cipher is historically significant but cryptographically obsolete.

The purpose of this project is to explore:

```text
Cryptography
Algorithms
Permutations
State machines
Object-oriented design
Software architecture
```

rather than provide real-world security.

---

# License

This project is intended for educational and experimental use.

If an open-source license is added to the repository, replace this section with the corresponding license text.

---

# Author

**Mohammadreza Ahmadi**

GitHub: [Brave-Potato9](https://github.com/Brave-Potato9)
