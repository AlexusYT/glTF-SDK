// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <GLTFSDK/Deserialize.h>

#include <GLTFSDK/ExtensionHandlers.h>
#include <GLTFSDK/GLTF.h>
#include <GLTFSDK/SchemaValidation.h>

using namespace Microsoft::glTF;


std::shared_ptr<Document> Deserializer::DeserializeInternal(const nlohmann::json &document, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer, SchemaFlags schemaFlags) {
    ValidateDocumentAgainstSchema(document, SCHEMA_URI_GLTF, GetDefaultSchemaLocator(schemaFlags));

    auto gltfDocument = document.get<std::shared_ptr<Document>>();

    gltfDocument->deserializeExtensions(extensionDeserializer);

    return gltfDocument;
}
std::shared_ptr<Document> Deserializer::Deserialize(const std::string& json, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer, SchemaFlags schemaFlags){
    nlohmann::json document;

    try {
        document = nlohmann::json::parse(json);
    }catch (...) {
        // The input is not valid JSON.
        throw GLTFException("The document is invalid due to bad JSON formatting");
    }

    return DeserializeInternal(document, extensionDeserializer, schemaFlags);
}

std::shared_ptr<Document> Deserializer::Deserialize(std::istream& jsonStream, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer, SchemaFlags schemaFlags){
    nlohmann::json document;
    try {
        jsonStream >> document;
    }catch (...) {
        // The input is not valid JSON.
        throw GLTFException("The document is invalid due to bad JSON formatting");
    }

    return DeserializeInternal(document, extensionDeserializer, schemaFlags);
}