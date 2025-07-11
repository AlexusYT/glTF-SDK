// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <memory>
#include <nlohmann/json_fwd.hpp>

namespace Microsoft
{
    namespace glTF
    {
        class ISchemaLocator
        {
        public:
            virtual ~ISchemaLocator() = default;
            virtual const char* GetSchemaContent(const std::string& uri) const = 0;
        };

        void ValidateDocumentAgainstSchema(const nlohmann::json& d, const std::string& schemaUri, std::unique_ptr<const ISchemaLocator> schemaLocator);
    }
}
