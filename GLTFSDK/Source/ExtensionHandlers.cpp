// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <GLTFSDK/ExtensionHandlers.h>

#include <GLTFSDK/Exceptions.h>
#include <GLTFSDK/GLTF.h>

using namespace Microsoft::glTF;

Detail::TypeKey Detail::MakeTypeKey(const Extension& extension, const glTFProperty& property)
{
    return { typeid(extension), typeid(property) };
}

Detail::NameKey Detail::MakeNameKey(const std::string& name, const glTFProperty& property)
{
    return { name, typeid(property) };
}

std::unique_ptr<Extension> ExtensionDeserializer::Deserialize(const ExtensionPair& extensionPair, const glTFProperty& property)
{
    auto it = nameToType.find(Detail::MakeNameKey(extensionPair.name, property));

    if (it == nameToType.end())
    {
        it = nameToType.find(Detail::MakeNameKey<glTFPropertyAll>(extensionPair.name));
    }

    if (it == nameToType.end())
    {
        throw GLTFException("No handler registered to deserialize the specified extension name");
    }

    return Process({ it->second, it->first.second }, extensionPair.value, shared_from_this());
}
