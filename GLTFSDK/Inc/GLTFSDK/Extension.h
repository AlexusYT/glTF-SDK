// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <memory>
#include <typeindex>
#include <nlohmann/json_fwd.hpp>

namespace Microsoft
{
    namespace glTF
    {
    class PropertyType;

    class Extension
        {
        public:
            virtual ~Extension() = default;

            virtual std::unique_ptr<Extension> Clone() const = 0;
            virtual bool IsEqual(const Extension& other) const = 0;

            bool operator==(const Extension& rhs) const;
            bool operator!=(const Extension& rhs) const;

            virtual std::string getName() const = 0;

            virtual void serialize(nlohmann::json& json, const PropertyType & pPropertyType) const;

            virtual void deserialize(const nlohmann::json& json);

        protected:
            Extension() = default;
        };
    }
}
