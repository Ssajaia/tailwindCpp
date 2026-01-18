#include "tailwind.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace tw;

void demo_text_colors() {
    std::cout << "\n=== Text Colors ===\n";
    print("Black text", "text-black");
    print("Red text", "text-red");
    print("Green text", "text-green");
    print("Yellow text", "text-yellow");
    print("Blue text", "text-blue");
    print("Magenta text", "text-magenta");
    print("Cyan text", "text-cyan");
    print("White text", "text-white");
}

void demo_background_colors() {
    std::cout << "\n\n=== Background Colors ===\n";
    print("Black background", "bg-black text-white");
    print("Red background", "bg-red text-white");
    print("Green background", "bg-green text-white");
    print("Yellow background", "bg-yellow text-black");
    print("Blue background", "bg-blue text-white");
    print("Magenta background", "bg-magenta text-white");
    print("Cyan background", "bg-cyan text-black");
    print("White background", "bg-white text-black");
}

void demo_modifiers() {
    std::cout << "\n\n=== Text Modifiers ===\n";
    print("Bold text", "bold");
    print("Underlined text", "underline");
    print("Dim text", "dim");
    print("Blinking text", "blink");
    print("Reversed colors", "reverse text-red bg-white");
    print("Hidden text", "hidden");
}

void demo_padding() {
    std::cout << "\n\n=== Padding ===\n";
    print("No padding", "text-cyan");
    print("p-1", "text-cyan p-1");
    print("p-2", "text-cyan p-2");
    print("p-3", "text-cyan p-3");
    print("p-4", "text-cyan p-4");
}

void demo_borders() {
    std::cout << "\n\n=== Borders ===\n";
    print("Simple border", "border p-2");
    print("Double border", "border-double p-2 text-green");
    print("Rounded border", "border-rounded p-2 text-yellow");
}

void demo_alignment() {
    std::cout << "\n\n=== Alignment ===\n";
    print("Left aligned (default)", "text-blue");
    print("Center aligned", "center text-green");
    print("Right aligned", "right text-magenta");
}

void demo_width() {
    std::cout << "\n\n=== Width Control ===\n";
    print("Auto width", "text-cyan border");
    print("Width 20", "w-20 text-green border");
    print("Width 40", "w-40 text-yellow border");
    print("Full width", "w-full text-magenta border");
}

void demo_combinations() {
    std::cout << "\n\n=== Combinations ===\n";
    print("Styled box", "text-white bg-blue bold border p-2 center");
    print("Alert style", "text-yellow bg-red bold border-double p-3 center");
    print("Success style", "text-green bg-black bold border-rounded p-2");
}

void demo_margin() {
    std::cout << "\n\n=== Margin ===\n";
    print("First line with margin", "m-1 text-cyan border");
    print("Second line with margin", "m-1 text-green border");
}

void demo_invalid_utilities() {
    std::cout << "\n\n=== Error Tolerance ===\n";
    std::cout << "With TW_DEBUG defined, unknown utilities are logged:\n";

    // These will be ignored (or logged in debug mode)
    print("Valid + invalid", "text-red bg-blue invalid-utility another-bad-one");
    print("Only invalid", "not-a-real-style completely-fake");
}

void demo_multi_block_layout() {
    std::cout << "\n\n=== Multi-block Layout ===\n";

    // Header
    print("TAILWINDCPP DEMO", "text-white bg-blue bold border center w-full p-2");

    // Content blocks
    print("Feature 1: Colors", "text-green border p-1 m-1");
    print("Feature 2: Borders", "text-yellow border-double p-1 m-1");
    print("Feature 3: Alignment", "text-cyan border-rounded p-1 m-1 center");

    // Footer
    print("End of demo", "text-white bg-magenta bold border center w-full p-1");
}

void demo_chainable_api() {
    std::cout << "\n\n=== Chainable API ===\n";

    Text("Hello")
        .tw("text-red bold")
        .print();

    std::cout << "\n";

    Text("World")
        .tw("bg-blue text-white")
        .tw("p-2 border") // Can chain multiple calls
        .print();
}

int main() {
    // Enable ANSI support for Windows
    tw::enable_ansi();

    std::cout << "TAILWINDCPP DEMONSTRATION\n";
    std::cout << "=========================\n";

    // Wait a moment for terminal to initialize
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Run all demos
    demo_text_colors();
    demo_background_colors();
    demo_modifiers();
    demo_padding();
    demo_borders();
    demo_alignment();
    demo_width();
    demo_combinations();
    demo_margin();
    demo_invalid_utilities();
    demo_multi_block_layout();
    demo_chainable_api();

    std::cout << "\n\n=== End of Demo ===\n";
    std::cout << "All features demonstrated successfully!\n";

    return 0;
}