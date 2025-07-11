// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <iostream>
#include <GLTFSDK/Document.h>
#include <GLTFSDK/Schema.h>

namespace Microsoft::glTF {
class ExtensionDeserializer;
class Deserializer {
public:
    static std::shared_ptr<Document> Deserialize(const std::string& json, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer, SchemaFlags schemaFlags = SchemaFlags::None);
    static std::shared_ptr<Document> Deserialize(const std::string& json, SchemaFlags schemaFlags = SchemaFlags::None) {
        return Deserialize(json, nullptr, schemaFlags);
    }

    static std::shared_ptr<Document> Deserialize(std::istream& jsonStream, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer, SchemaFlags schemaFlags = SchemaFlags::None);
    static std::shared_ptr<Document> Deserialize(std::istream& jsonStream, SchemaFlags schemaFlags = SchemaFlags::None) {
        return Deserialize(jsonStream, nullptr, schemaFlags);
    }


private:
    static std::shared_ptr<Document> DeserializeInternal(const nlohmann::json& document, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer, SchemaFlags schemaFlags);

};
}
