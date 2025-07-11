// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <GLTFSDK/SchemaValidation.h>
#include <GLTFSDK/Exceptions.h>

#include <unordered_map>
#include <GLTFSDK/valijson_nlohmann_bundled.hpp>

using namespace Microsoft::glTF;

namespace
{
    class RemoteSchemaDocumentProvider
    {
    public:
        RemoteSchemaDocumentProvider(std::unique_ptr<const ISchemaLocator> schemaLocator) : schemaLocator(std::move(schemaLocator))
        {
            assert(this->schemaLocator);
        }

        nlohmann::json getJson(const std::string& uri) {

            try {
                return nlohmann::json::parse(schemaLocator->GetSchemaContent(uri));
            }catch (...) {
                throw GLTFException("Schema document at " + uri + " is not valid JSON");
            }
        }

        valijson::Schema GetRemoteDocumentStr(const std::string& uri)
        {

            valijson::Schema schema;

            try {
                nlohmann::json document = getJson(uri);
                valijson::SchemaParser parser(valijson::SchemaParser::kDraft4);
                const valijson::adapters::NlohmannJsonAdapter schemaDocumentAdapter(document);
                parser.populateSchema(schemaDocumentAdapter, schema, [this](const std::string &uri) {
                    return new nlohmann::json(nlohmann::json::parse(schemaLocator->GetSchemaContent(uri)));;
                },[](const nlohmann::json* json) {
                    delete json;
                });
            }catch (...) {
                throw GLTFException("Schema document at " + uri + " is not valid JSON");
            }

            return schema;
        }

        const std::unique_ptr<const ISchemaLocator> schemaLocator;

    private:
    };
}

void Microsoft::glTF::ValidateDocumentAgainstSchema(const nlohmann::json& document, const std::string& schemaUri, std::unique_ptr<const ISchemaLocator> schemaLocator)
{
    if (!schemaLocator)
    {
        throw GLTFException("ISchemaLocator instance must not be null");
    }

    RemoteSchemaDocumentProvider provider(std::move(schemaLocator));

    const auto &schemaDocument = provider.GetRemoteDocumentStr(schemaUri);

    valijson::Validator validator(valijson::Validator::kStrongTypes);
    valijson::ValidationResults results;
    const valijson::adapters::NlohmannJsonAdapter targetDocumentAdapter(document);
    if (!validator.validate(schemaDocument, targetDocumentAdapter, &results)) {
        valijson::ValidationResults::Error validationError;
        while (results.popError(validationError)) {

            std::string context;
            for (auto &itr: validationError.context) { context += itr; }

            throw ValidationException("Schema violation at " + context + " due to " + validationError.description);

        }
    }

}
