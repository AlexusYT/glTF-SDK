// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <memory>
#include <string>

namespace Microsoft::glTF {
enum class SerializeFlags
{
    None   = 0x0,
    Pretty = 0x1
};

SerializeFlags  operator| (SerializeFlags lhs,  SerializeFlags rhs);
SerializeFlags& operator|=(SerializeFlags& lhs, SerializeFlags rhs);
SerializeFlags  operator& (SerializeFlags lhs,  SerializeFlags rhs);
SerializeFlags& operator&=(SerializeFlags& lhs, SerializeFlags rhs);

class Document;
class ExtensionSerializer;
class Serializer {
public:

    static std::string Serialize(const std::shared_ptr<Document>& gltfDocument, SerializeFlags flags = SerializeFlags::None);

    static std::string Serialize(const std::shared_ptr<Document>& gltfDocument, const std::shared_ptr<ExtensionSerializer>& extensionHandler, SerializeFlags flags = SerializeFlags::None);
};
}
