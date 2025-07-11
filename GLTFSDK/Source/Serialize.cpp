// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <GLTFSDK/Serialize.h>

#include <GLTFSDK/Document.h>
#include <GLTFSDK/ExtensionHandlers.h>
#include <GLTFSDK/GLTF.h>

using namespace Microsoft::glTF;

bool HasFlag(SerializeFlags flags, SerializeFlags flag)
{
    return ((flags & flag) == flag);
}
std::string Serializer::Serialize(const std::shared_ptr<Document>& gltfDocument, SerializeFlags flags) {
    return Serialize(gltfDocument, nullptr, flags);
}

std::string Serializer::Serialize(const std::shared_ptr<Document>& gltfDocument, const std::shared_ptr<ExtensionSerializer>& extensionSerializer, SerializeFlags flags) {
    gltfDocument->serializer = extensionSerializer;
    nlohmann::json json = gltfDocument;
    gltfDocument->serializer = nullptr;
    return json.dump(HasFlag(flags, SerializeFlags::Pretty) ? 4 : 0);
}

SerializeFlags Microsoft::glTF::operator|(SerializeFlags lhs, SerializeFlags rhs)
{
    const auto result =
        static_cast<std::underlying_type_t<SerializeFlags>>(lhs) |
        static_cast<std::underlying_type_t<SerializeFlags>>(rhs);

    return static_cast<SerializeFlags>(result);
}

SerializeFlags& Microsoft::glTF::operator|=(SerializeFlags& lhs, SerializeFlags rhs)
{
    lhs = lhs | rhs;
    return lhs;
}

SerializeFlags Microsoft::glTF::operator&(SerializeFlags lhs, SerializeFlags rhs)
{
    const auto result =
        static_cast<std::underlying_type_t<SerializeFlags>>(lhs) &
        static_cast<std::underlying_type_t<SerializeFlags>>(rhs);

    return static_cast<SerializeFlags>(result);
}

SerializeFlags& Microsoft::glTF::operator&=(SerializeFlags& lhs, SerializeFlags rhs)
{
    lhs = lhs & rhs;
    return lhs;
}
