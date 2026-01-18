# TailwindCPP

**TailwindCPP** is a **header-only, utility-first styling library for C++ console applications**, inspired by the philosophy of Tailwind CSS and adapted for **ANSI terminals on Windows**.

It allows you to style console output declaratively using simple utility strings instead of manually writing ANSI escape codes or layout logic.

---

## Motivation

Styling console output in C++ usually means:
- Hard-coded ANSI escape sequences
- Platform-specific hacks
- Repeated layout logic
- Poor readability and maintainability

TailwindCPP introduces a **utility-based styling layer** for terminal output, similar to how Tailwind CSS works for the web.

---

## Features

- Header-only (`tailwind.h`)
- Zero external dependencies
- Windows-friendly (ANSI + Windows Console API)
- Utility-first API (`text-red`, `bg-blue`, `p-2`, `border`, etc.)
- Chainable C++ interface
- Declarative styling
- Debug mode for unknown utilities

---

## Supported Utilities

### Text Colors
text-black
text-red
text-green
text-yellow
text-blue
text-magenta
text-cyan
text-white


### Background Colors
bg-black
bg-red
bg-green
bg-yellow
bg-blue
bg-magenta
bg-cyan
bg-white


### Text Modifiers
bold
dim
underline
blink
reverse
hidden


### Padding
p-0
p-1
p-2
p-3
p-4


### Margin
m-0
m-1
m-2


### Borders
border
border-double
border-rounded


### Alignment
left
center
right


### Width
w-20
w-40
w-full


---

## Example

```cpp
#include "tailwind.h"
using namespace tw;

int main() {
    enable_ansi();
    print("Hello TailwindCPP", "text-green bold p-2 border center");
}
Chainable API
Text("Styled Text")
    .tw("text-white bg-blue bold")
    .tw("p-2 border center")
    .print();
```

==Note: The current implementation replaces the style state on each .tw() call.
Style merging is planned.==

# Architecture Overview
- Core Components
- Parser

# Parses utility strings

- Maps tokens to enums and flags

- Ignores or logs unknown utilities (debug mode)

- StyleState

- Central configuration object

- Stores colors, modifiers, padding, margin, border, alignment, width

- Renderer

# Rendering pipeline:

- Width adjustment

- Padding

- Border

- Alignment

- Margin

- ANSI styling (applied last)

- Text

- Public-facing API

+ Holds raw text + style

- Renders to string or prints directly

- Windows Support

==TailwindCPP enables ANSI escape sequence support automatically using:

- ENABLE_VIRTUAL_TERMINAL_PROCESSING

- Windows Console API

- Works out of the box in:

- Windows Terminal

- PowerShell (Windows 10/11)

+ Debug Mode
==Enable debug logging for unknown utilities:

```

#define TW_DEBUG
#include "tailwind.h"
```

# Known Limitations
Multiline alignment is basic

- Borders + alignment are not fully multiline-aware

- Chainable style merging not implemented yet

- Primarily designed for Windows (Linux/macOS not officially tested)

- These are intentional trade-offs for v1.

# Use Cases
- CLI tools

- DevOps utilities

- Debug dashboards

- Internal developer tooling

- Lightweight TUI components

- Log viewers and status outputs

# Roadmap
Planned improvements:

- Style merging for chainable API

- Proper multiline alignment

- Theme system

- Reusable components (alerts, panels, headers)

- Cross-platform support

License
MIT License

Author
Saba Sajaia
Computer Science
Tbilisi State University
