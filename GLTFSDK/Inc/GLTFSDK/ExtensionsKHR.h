// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <GLTFSDK/ExtensionHandlers.h>
#include <GLTFSDK/Optional.h>

#include <memory>
#include <string>

namespace Microsoft
{
    namespace glTF
    {
        namespace KHR
        {
        std::shared_ptr<ExtensionDeserializer> GetKHRExtensionDeserializer();

            namespace Materials
            {
                constexpr const char* PBRSPECULARGLOSSINESS_NAME = "KHR_materials_pbrSpecularGlossiness";

                // KHR_materials_pbrSpecularGlossiness
                struct PBRSpecularGlossiness : Extension, glTFProperty
                {
                    PBRSpecularGlossiness();

                    Color4 diffuseFactor;
                    TextureInfo diffuseTexture;
                    Color3 specularFactor;
                    float glossinessFactor;
                    TextureInfo specularGlossinessTexture;

                    std::unique_ptr<Extension> Clone() const override;
                    bool IsEqual(const Extension& rhs) const override;

                    std::string getName() const override {
                        return PBRSPECULARGLOSSINESS_NAME;
                    }

                    void serialize(nlohmann::json &json, const PropertyType & pPropertyType) const override;

                    void deserialize(const nlohmann::json &json) override;


                    void setGltfDocument(Document* pGltfDocument) override {
                        glTFProperty::setGltfDocument(pGltfDocument);
                        diffuseTexture.setGltfDocument(pGltfDocument);
                        specularGlossinessTexture.setGltfDocument(pGltfDocument);
                    }

                    void deserializeExtensions(const std::shared_ptr<ExtensionDeserializer> &pDeserializer) override {
                        glTFProperty::deserializeExtensions(pDeserializer);
                        specularGlossinessTexture.deserializeExtensions(pDeserializer);
                        diffuseTexture.deserializeExtensions(pDeserializer);
                    }
                };

                std::unique_ptr<Extension> DeserializePBRSpecGloss(const nlohmann::json& json, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer);

                constexpr const char* UNLIT_NAME = "KHR_materials_unlit";

                // KHR_materials_unlit
                struct Unlit : Extension, glTFProperty
                {
                    std::unique_ptr<Extension> Clone() const override;
                    bool IsEqual(const Extension& rhs) const override;

                    std::string getName() const override {
                        return UNLIT_NAME;
                    }

                    void serialize(nlohmann::json &json, const PropertyType &pPropertyType) const override;
                };

                std::unique_ptr<Extension> DeserializeUnlit(const nlohmann::json& json, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer);
            }

            namespace MeshPrimitives
            {
                constexpr const char* DRACOMESHCOMPRESSION_NAME = "KHR_draco_mesh_compression";

                // KHR_draco_mesh_compression
                struct DracoMeshCompression : Extension, glTFProperty
                {
                    std::string bufferViewId;

                    std::unordered_map<std::string, uint32_t> attributes;

                    std::unique_ptr<Extension> Clone() const override;

                    bool IsEqual(const Extension& rhs) const override;

                    std::string getName() const override {
                        return DRACOMESHCOMPRESSION_NAME;
                    }

                    void serialize(nlohmann::json& json, const PropertyType & pPropertyType) const override;

                    void deserialize(const nlohmann::json& json) override;

                    friend void from_json(const nlohmann::json& json, DracoMeshCompression& pType) {
                        pType.deserialize(json);
                    }
                };

                std::unique_ptr<Extension> DeserializeDracoMeshCompression(const nlohmann::json& json, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer);
            }

            namespace TextureInfos
            {
                constexpr const char* TEXTURETRANSFORM_NAME = "KHR_texture_transform";

                // KHR_texture_transform
                struct TextureTransform : Extension, glTFProperty
                {
                    TextureTransform();
                    TextureTransform(const TextureTransform&);

                    Vector2 offset;
                    float rotation;
                    Vector2 scale;
                    Optional<size_t> texCoord; // TexCoord is an optional property

                    std::unique_ptr<Extension> Clone() const override;
                    bool IsEqual(const Extension& rhs) const override;

                    std::string getName() const override {
                        return TEXTURETRANSFORM_NAME;
                    }

                    void serialize(nlohmann::json &json, const PropertyType & pPropertyType) const override;

                    void deserialize(const nlohmann::json &json) override;

                    friend void from_json(const nlohmann::json& json, TextureTransform& pType) {
                        pType.deserialize(json);
                    }
                };

                std::unique_ptr<Extension> DeserializeTextureTransform(const nlohmann::json& json, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer);
            }
        }
    }
}
