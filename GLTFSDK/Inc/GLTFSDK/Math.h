// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <array>
#include <cmath>
#include <nlohmann/json.hpp>
#include <cstdint>

namespace Microsoft
{
    namespace glTF
    {
        struct Matrix4
        {
            Matrix4();

            bool operator==(const Matrix4& other) const;
            bool operator!=(const Matrix4& other) const;

            std::array<float, 16> values;

            static const Matrix4 IDENTITY;
            friend void to_json(nlohmann::json& json, const Matrix4& pType) {
                json = pType.values;
            }

            friend void from_json(const nlohmann::json& json, Matrix4& pType) {
                json.get_to(pType.values);
            }
        };

        struct Vector2
        {
            Vector2();
            Vector2(float x, float y);

            bool operator==(const Vector2& other) const;
            bool operator!=(const Vector2& other) const;

            float x;
            float y;

            static const Vector2 ZERO;
            static const Vector2 ONE;

            friend void to_json(nlohmann::json& json, const Vector2& pType) {
                json.push_back(pType.x);
                json.push_back(pType.y);
            }

            friend void from_json(const nlohmann::json& json, Vector2& pType) {
                auto ait = json.begin();
                pType.x = ait++->get<float>();
                pType.y = ait->get<float>();
            }
        };

        struct Vector3
        {
            Vector3();
            Vector3(float x, float y, float z);

            bool operator==(const Vector3& other) const;
            bool operator!=(const Vector3& other) const;

            float x;
            float y;
            float z;

            static const Vector3 ZERO;
            static const Vector3 ONE;

            friend void to_json(nlohmann::json& json, const Vector3& pType) {
                json.push_back(pType.x);
                json.push_back(pType.y);
                json.push_back(pType.z);
            }

            friend void from_json(const nlohmann::json& json, Vector3& pType) {
                auto ait = json.begin();
                pType.x = ait++->get<float>();
                pType.y = ait++->get<float>();
                pType.z = ait->get<float>();
            }
        };

        struct Quaternion
        {
            Quaternion();
            Quaternion(float x, float y, float z, float w);

            bool operator==(const Quaternion& other) const;
            bool operator!=(const Quaternion& other) const;

            float x;
            float y;
            float z;
            float w;

            static const Quaternion IDENTITY;
            friend void to_json(nlohmann::json& json, const Quaternion& pType) {
                json.push_back(pType.x);
                json.push_back(pType.y);
                json.push_back(pType.z);
                json.push_back(pType.w);
            }

            friend void from_json(const nlohmann::json& json, Quaternion& pType) {
                auto ait = json.begin();
                pType.x = ait++->get<float>();
                pType.y = ait++->get<float>();
                pType.z = ait++->get<float>();
                pType.w = ait->get<float>();
            }
        };

        namespace Math
        {
            template<class T>
            const T& Clamp(const T& v, const T& lo, const T& hi) {
                return std::min(hi, std::max(v, lo));
            }

            // https://en.wikipedia.org/wiki/SRGB#The_reverse_transformation
            inline float ToLinear(float value)
            {
                if (value < 0.04045f)
                {
                    return value / 12.92f;
                }

                return std::pow((value + 0.055f) / 1.055f, 2.4f);
            }

            // https://en.wikipedia.org/wiki/SRGB#The_forward_transformation_.28CIE_XYZ_to_sRGB.29
            inline float ToGamma(float value)
            {
                if (value <= 0.0031308f)
                {
                    return value * 12.92f;
                }

                return 1.055f * std::pow(value, 1.0f / 2.4f) - 0.055f;
            }

            inline float ByteToFloat(uint8_t value)
            {
                return value / 255.0f;
            }

            inline uint8_t FloatToByte(float value)
            {
                return static_cast<uint8_t>(value * 255.0f + 0.5f);
            }
        }
    }
}
