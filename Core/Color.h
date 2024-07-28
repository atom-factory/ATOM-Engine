// Author: Jake Rieger
// Created: 7/26/2024.
//

#pragma once

#include "Types.h"

namespace Atom {
    /**
     * @brief A 32 bit color value in ARGB format
     */
    class Color {
    public:
        Color();
        Color(f32 r, f32 g, f32 b, f32 a = 1.0);
        explicit Color(u32 color);

        /// Returns the color value as a 32-bit unsigned integer (ex. 0xFF000000 for black)
        [[nodiscard]] u32 Value() const;
        /// Gets the luminance value as a 32-bit float
        [[nodiscard]] f32 Luminance() const;
        /// Creates a new color from the source color with the provided alpha
        [[nodiscard]] Color WithAlpha(u32 a) const;
        /// Creates a new color from the source color with the provided alpha
        [[nodiscard]] Color WithAlpha(f32 a) const;
        /// Creates a new color from the source color with the provided red value
        [[nodiscard]] Color WithRed(u32 r) const;
        /// Creates a new color from the source color with the provided red value
        [[nodiscard]] Color WithRed(f32 r) const;
        /// Creates a new color from the source color with the provided green value
        [[nodiscard]] Color WithGreen(u32 g) const;
        /// Creates a new color from the source color with the provided green value
        [[nodiscard]] Color WithGreen(f32 g) const;
        /// Creates a new color from the source color with the provided blue value
        [[nodiscard]] Color WithBlue(u32 b) const;
        /// Creates a new color from the source color with the provided blue value
        [[nodiscard]] Color WithBlue(f32 b) const;
        /// Gets the current color in its greyscale format
        [[nodiscard]] Color Greyscale() const;

        /// Creates a new color by blending two provided colors including their alpha channels
        static Color AlphaBlend(const Color& foreground, const Color& background);
        /// Gets the alpha value as an unsigned integer from the provided opacity value
        static u32 GetAlphaFromOpacity(f32 opacity);
        /// Linearly interpolates between two provided colors by a constant `t`
        static Color Lerp(const Color& a, const Color& b, f32 t);
        /// Gamma corrects specified color component `c`
        static f32 LinearizeComponent(f32 c);

        [[nodiscard]] Color GammaCorrect() const;

        f32 Red {};
        f32 Green {};
        f32 Blue {};
        f32 Alpha {};

    private:
        static f32 LinearToSRGB(f32 c);
    };

    namespace Colors {
        inline static auto White       = Color(0xFFFFFFFF);
        inline static auto Black       = Color(0xFF000000);
        inline static auto Red         = Color(0xFFFF0000);
        inline static auto Yellow      = Color(0xFFFFFF00);
        inline static auto Green       = Color(0xFF00FF00);
        inline static auto Cyan        = Color(0xFF00FFFF);
        inline static auto Blue        = Color(0xFF0000FF);
        inline static auto Magenta     = Color(0xFFFF00FF);
        inline static auto Transparent = Color(0x00000000);
    };  // namespace Colors

    namespace Converters {
        inline void HexToRGBA(const u32 hex, f32& r, f32& g, f32& b, f32& a) {
            const unsigned char alphaByte = (hex >> 24) & 0xFF;
            const unsigned char redByte   = (hex >> 16) & 0xFF;
            const unsigned char greenByte = (hex >> 8) & 0xFF;
            const unsigned char blueByte  = hex & 0xFF;

            a = static_cast<float>(static_cast<u32>(alphaByte) / 255.0);
            r = static_cast<float>(static_cast<u32>(redByte) / 255.0);
            g = static_cast<float>(static_cast<u32>(greenByte) / 255.0);
            b = static_cast<float>(static_cast<u32>(blueByte) / 255.0);
        }

        inline void HexToRGBA(const u32 hex, u32& r, u32& g, u32& b, u32& a) {
            const unsigned char alphaByte = (hex >> 24) & 0xFF;
            const unsigned char redByte   = (hex >> 16) & 0xFF;
            const unsigned char greenByte = (hex >> 8) & 0xFF;
            const unsigned char blueByte  = hex & 0xFF;

            a = static_cast<u32>(alphaByte);
            r = static_cast<u32>(redByte);
            g = static_cast<u32>(greenByte);
            b = static_cast<u32>(blueByte);
        }

        inline u32 RGBAToHex(const f32 r, const f32 g, const f32 b, const f32 a) {
            const auto redByte   = static_cast<unsigned char>(r * 255.0f);
            const auto greenByte = static_cast<unsigned char>(g * 255.0f);
            const auto blueByte  = static_cast<unsigned char>(b * 255.0f);
            const auto alphaByte = static_cast<unsigned char>(a * 255.0f);

            return (alphaByte << 24) | (redByte << 16) | (greenByte << 8) | blueByte;
        }
    }  // namespace Converters

}  // namespace Atom