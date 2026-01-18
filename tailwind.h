#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cstdint>
#include <memory>
#include <Windows.h>

#ifdef TW_DEBUG
#include <iostream>
#endif

namespace tw {

// Forward declarations
class Text;
class StyleState;
class Parser;

// ANSI Color Codes
enum class Color : uint8_t {
    Black = 0,
    Red = 1,
    Green = 2,
    Yellow = 3,
    Blue = 4,
    Magenta = 5,
    Cyan = 6,
    White = 7,
    Default = 9
};

// Text Modifiers (bitmask)
enum Modifier : uint32_t {
    None        = 0,
    Bold        = 1 << 0,
    Dim         = 1 << 1,
    Underline   = 1 << 2,
    Blink       = 1 << 3,
    Reverse     = 1 << 4,
    Hidden      = 1 << 5
};

// Border Types
enum class BorderType : uint8_t {
    None,
    Single,
    Double,
    Rounded
};

// Alignment Types
enum class Alignment : uint8_t {
    Left,
    Center,
    Right
};

// Padding Levels
enum class Padding : uint8_t {
    P0 = 0,
    P1 = 1,
    P2 = 2,
    P3 = 3,
    P4 = 4
};

// Margin Levels
enum class Margin : uint8_t {
    M0 = 0,
    M1 = 1,
    M2 = 2
};

// StyleState - Central configuration
struct StyleState {
    Color text_color = Color::Default;
    Color bg_color = Color::Default;
    uint32_t modifiers = Modifier::None;
    Padding padding = Padding::P0;
    Margin margin = Margin::M0;
    BorderType border = BorderType::None;
    Alignment alignment = Alignment::Left;
    uint16_t width = 0; // 0 = auto
    bool width_full = false;
    
    bool has_text_color() const { return text_color != Color::Default; }
    bool has_bg_color() const { return bg_color != Color::Default; }
    bool has_border() const { return border != BorderType::None; }
    bool has_modifier(Modifier m) const { return (modifiers & m) != 0; }
};

// Parser - Utility string parser
class Parser {
private:
    static inline std::unordered_map<std::string, Color> color_map = {
        {"black", Color::Black}, {"red", Color::Red}, {"green", Color::Green},
        {"yellow", Color::Yellow}, {"blue", Color::Blue}, {"magenta", Color::Magenta},
        {"cyan", Color::Cyan}, {"white", Color::White}
    };
    
    static inline std::unordered_map<std::string, Padding> padding_map = {
        {"p-0", Padding::P0}, {"p-1", Padding::P1}, {"p-2", Padding::P2},
        {"p-3", Padding::P3}, {"p-4", Padding::P4}
    };
    
    static inline std::unordered_map<std::string, Modifier> modifier_map = {
        {"bold", Modifier::Bold}, {"dim", Modifier::Dim},
        {"underline", Modifier::Underline}, {"blink", Modifier::Blink},
        {"reverse", Modifier::Reverse}, {"hidden", Modifier::Hidden}
    };

public:
    static StyleState parse(const std::string& utility_str) {
        StyleState state;
        std::string token;
        
        auto tokens = split(utility_str, ' ');
        
        for (const auto& token : tokens) {
            apply_token(token, state);
        }
        
        return state;
    }

private:
    static std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        size_t start = 0;
        size_t end = str.find(delimiter);
        
        while (end != std::string::npos) {
            tokens.push_back(str.substr(start, end - start));
            start = end + 1;
            end = str.find(delimiter, start);
        }
        tokens.push_back(str.substr(start));
        return tokens;
    }
    
    static void apply_token(const std::string& token, StyleState& state) {
        // Text colors
        if (token.substr(0, 5) == "text-") {
            auto color_name = token.substr(5);
            if (color_map.find(color_name) != color_map.end()) {
                state.text_color = color_map[color_name];
            } else {
                log_unknown_utility(token);
            }
        }
        // Background colors
        else if (token.substr(0, 3) == "bg-") {
            auto color_name = token.substr(3);
            if (color_map.find(color_name) != color_map.end()) {
                state.bg_color = color_map[color_name];
            } else {
                log_unknown_utility(token);
            }
        }
        // Padding
        else if (padding_map.find(token) != padding_map.end()) {
            state.padding = padding_map[token];
        }
        // Modifiers
        else if (modifier_map.find(token) != modifier_map.end()) {
            state.modifiers |= modifier_map[token];
        }
        // Border
        else if (token == "border") {
            state.border = BorderType::Single;
        }
        else if (token == "border-double") {
            state.border = BorderType::Double;
        }
        else if (token == "border-rounded") {
            state.border = BorderType::Rounded;
        }
        // Alignment
        else if (token == "left") {
            state.alignment = Alignment::Left;
        }
        else if (token == "center") {
            state.alignment = Alignment::Center;
        }
        else if (token == "right") {
            state.alignment = Alignment::Right;
        }
        // Width
        else if (token == "w-full") {
            state.width_full = true;
        }
        else if (token.substr(0, 2) == "w-") {
            try {
                state.width = static_cast<uint16_t>(std::stoi(token.substr(2)));
            } catch (...) {
                log_unknown_utility(token);
            }
        }
        else if (token != "") {
            log_unknown_utility(token);
        }
    }
    
    static void log_unknown_utility(const std::string& token) {
        #ifdef TW_DEBUG
        std::cerr << "[TW_DEBUG] Unknown utility: " << token << std::endl;
        #endif
    }
};

// Renderer - Handles the rendering pipeline
class Renderer {
private:
    // Border characters
    struct BorderChars {
        char top_left, top_right, bottom_left, bottom_right;
        char horizontal, vertical;
    };
    
    static BorderChars get_border_chars(BorderType type) {
        switch(type) {
            case BorderType::Double:
                return {'╔', '╗', '╚', '╝', '═', '║'};
            case BorderType::Rounded:
                return {'╭', '╮', '╰', '╯', '─', '│'};
            case BorderType::Single:
            default:
                return {'┌', '┐', '└', '┘', '─', '│'};
        }
    }

public:
    static std::string render(const std::string& text, const StyleState& style) {
        std::string result;
        
        // 1. Apply width constraints
        std::string width_adjusted = apply_width(text, style);
        
        // 2. Apply padding
        std::string padded = apply_padding(width_adjusted, style);
        
        // 3. Apply border
        std::string bordered = apply_border(padded, style);
        
        // 4. Apply alignment
        std::string aligned = apply_alignment(bordered, style);
        
        // 5. Apply margin
        result = apply_margin(aligned, style);
        
        // 6. Apply ANSI styling (always last)
        result = apply_ansi_styling(result, style);
        
        return result;
    }

private:
    static std::string apply_width(const std::string& text, const StyleState& style) {
        if (style.width == 0 && !style.width_full) return text;
        
        size_t target_width = style.width;
        if (style.width_full) {
            target_width = get_terminal_width();
        }
        
        if (text.length() <= target_width) {
            // Pad right
            std::string result = text;
            result.append(target_width - text.length(), ' ');
            return result;
        } else {
            // Truncate
            return text.substr(0, target_width - 3) + "...";
        }
    }
    
    static std::string apply_padding(const std::string& text, const StyleState& style) {
        if (style.padding == Padding::P0) return text;
        
        uint8_t pad_size = static_cast<uint8_t>(style.padding);
        std::string pad_str(pad_size, ' ');
        
        // Horizontal padding only (vertical would require line-by-line)
        return pad_str + text + pad_str;
    }
    
    static std::string apply_border(const std::string& text, const StyleState& style) {
        if (!style.has_border()) return text;
        
        auto border_chars = get_border_chars(style.border);
        size_t content_width = text.length();
        
        std::string result;
        
        // Top border
        result += border_chars.top_left;
        result.append(content_width + 2, border_chars.horizontal);
        result += border_chars.top_right;
        result += '\n';
        
        // Middle with text
        result += border_chars.vertical;
        result += ' ' + text + ' ';
        result += border_chars.vertical;
        result += '\n';
        
        // Bottom border
        result += border_chars.bottom_left;
        result.append(content_width + 2, border_chars.horizontal);
        result += border_chars.bottom_right;
        
        return result;
    }
    
    static std::string apply_alignment(const std::string& text, const StyleState& style) {
        if (style.alignment == Alignment::Left || text.find('\n') != std::string::npos) {
            return text;
        }
        
        size_t term_width = get_terminal_width();
        size_t text_width = text.length();
        
        if (text_width >= term_width) return text;
        
        size_t left_padding = 0;
        if (style.alignment == Alignment::Center) {
            left_padding = (term_width - text_width) / 2;
        } else if (style.alignment == Alignment::Right) {
            left_padding = term_width - text_width;
        }
        
        return std::string(left_padding, ' ') + text;
    }
    
    static std::string apply_margin(const std::string& text, const StyleState& style) {
        if (style.margin == Margin::M0) return text;
        
        uint8_t margin_size = static_cast<uint8_t>(style.margin);
        std::string margin_str(margin_size, '\n');
        
        return margin_str + text + margin_str;
    }
    
    static std::string apply_ansi_styling(const std::string& text, const StyleState& style) {
        std::string result;
        
        // Open ANSI sequence
        result += "\033[";
        
        // Modifiers
        if (style.has_modifier(Modifier::Bold)) result += "1;";
        if (style.has_modifier(Modifier::Dim)) result += "2;";
        if (style.has_modifier(Modifier::Underline)) result += "4;";
        if (style.has_modifier(Modifier::Blink)) result += "5;";
        if (style.has_modifier(Modifier::Reverse)) result += "7;";
        if (style.has_modifier(Modifier::Hidden)) result += "8;";
        
        // Text color
        if (style.has_text_color()) {
            result += std::to_string(30 + static_cast<uint8_t>(style.text_color)) + ";";
        }
        
        // Background color
        if (style.has_bg_color()) {
            result += std::to_string(40 + static_cast<uint8_t>(style.bg_color)) + ";";
        }
        
        // Remove trailing semicolon and close
        if (result.back() == ';') result.pop_back();
        result += "m";
        
        result += text;
        result += "\033[0m"; // Reset
        
        return result;
    }
    
    static size_t get_terminal_width() {
        static size_t cached_width = 80; // Default fallback
        
        #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            cached_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        }
        #endif
        
        return cached_width;
    }
};

// Main Text class - Public API
class Text {
private:
    std::string raw_text;
    StyleState style;
    
public:
    explicit Text(const std::string& text) : raw_text(text) {}
    
    // Chainable method
    Text& tw(const std::string& utility_str) {
        auto parsed = Parser::parse(utility_str);
        // Merge with existing style (last wins)
        style = parsed; // Simple for now
        return *this;
    }
    
    // Render to string
    std::string render() const {
        return Renderer::render(raw_text, style);
    }
    
    // Print directly
    void print() const {
        std::cout << render() << std::flush;
    }
};

// Enable Windows ANSI support
inline void enable_ansi() {
    #ifdef _WIN32
    static bool ansi_enabled = false;
    if (!ansi_enabled) {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            GetConsoleMode(hOut, &dwMode);
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
            ansi_enabled = true;
        }
    }
    #endif
}

// Free function API
inline std::string tw(const std::string& text, const std::string& utility_str = "") {
    enable_ansi();
    return Text(text).tw(utility_str).render();
}

inline void print(const std::string& text, const std::string& utility_str = "") {
    enable_ansi();
    std::cout << tw(text, utility_str) << std::flush;
}

} // namespace tw