// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <memory>
#include <string>

namespace Microsoft::glTF {
class Document;
class Serializer {
public:

    static std::string Serialize(const std::shared_ptr<Document>& gltfDocument, bool pPretty = false);
};
}
