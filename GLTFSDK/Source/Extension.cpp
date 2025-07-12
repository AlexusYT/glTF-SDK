// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <GLTFSDK/Extension.h>
#include <GLTFSDK/GLTF.h>


using namespace Microsoft::glTF;

bool Extension::operator==(const Extension& rhs) const
{
    return IsEqual(rhs);
}

bool Extension::operator!=(const Extension& rhs) const
{
    return !operator==(rhs);
}

void Extension::serialize(nlohmann::json &/*json*/, const PropertyType & pPropertyType) const {}

void Extension::deserialize(const nlohmann::json &/*json*/) {}
