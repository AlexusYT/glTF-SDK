// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <GLTFSDK/Serialize.h>

#include <GLTFSDK/Document.h>

using namespace Microsoft::glTF;

std::string Serializer::Serialize(const std::shared_ptr<Document> &gltfDocument, bool pPretty) {
    nlohmann::json json = gltfDocument;
    return json.dump(pPretty ? 4 : 0);
}

