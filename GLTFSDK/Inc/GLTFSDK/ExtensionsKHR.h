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
            std::shared_ptr<ExtensionSerializer> GetKHRExtensionSerializer();
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

                    friend void to_json(nlohmann::json& json, const PBRSpecularGlossiness& pType) {
                        nlohmann::to_json(json, static_cast<const glTFProperty&>(pType));
                        if (pType.diffuseFactor != Color4(1.0f, 1.0f, 1.0f, 1.0f))
                            json["diffuseFactor"] = pType.diffuseFactor;

                        if (!pType.diffuseTexture.textureId.empty())
                            json["diffuseTexture"] = pType.diffuseTexture;

                        if (pType.specularFactor != Color3(1.0f, 1.0f, 1.0f))
                            json["specularFactor"] = pType.specularFactor;

                        if (pType.glossinessFactor != 1.0)
                            json["glossinessFactor"] = pType.glossinessFactor;

                        if (!pType.specularGlossinessTexture.textureId.empty())
                            json["specularGlossinessTexture"] = pType.specularGlossinessTexture;

                    }

                    friend void from_json(const nlohmann::json& json, PBRSpecularGlossiness& pType) {
                        nlohmann::from_json(json, static_cast<glTFProperty&>(pType));

                        if (auto iter = json.find("diffuseFactor"); iter != json.end())
                            iter.value().get_to(pType.diffuseFactor);

                        if (auto iter = json.find("diffuseTexture"); iter != json.end())
                            iter.value().get_to(pType.diffuseTexture);

                        if (auto iter = json.find("specularFactor"); iter != json.end())
                            iter.value().get_to(pType.specularFactor);

                        pType.glossinessFactor = json.value("glossinessFactor", 1.0f);

                        if (auto iter = json.find("specularGlossinessTexture"); iter != json.end())
                            iter.value().get_to(pType.specularGlossinessTexture);
                    }

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

                nlohmann::json SerializePBRSpecGloss(const PBRSpecularGlossiness& specGloss, const Document& gltfDocument, const ExtensionSerializer& extensionSerializer);
                std::unique_ptr<Extension> DeserializePBRSpecGloss(const nlohmann::json& json, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer);

                constexpr const char* UNLIT_NAME = "KHR_materials_unlit";

                // KHR_materials_unlit
                struct Unlit : Extension, glTFProperty
                {
                    std::unique_ptr<Extension> Clone() const override;
                    bool IsEqual(const Extension& rhs) const override;
                };

                nlohmann::json SerializeUnlit(const Unlit &unlit, const Document &gltfDocument,
                                              const ExtensionSerializer &extensionSerializer);
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

                    void serialize(nlohmann::json& json) const;

                    friend void to_json(nlohmann::json& json, const DracoMeshCompression& pType) {
                        nlohmann::to_json(json, static_cast<const glTFProperty&>(pType));
                        pType.serialize(json);
                        //json["bufferViewId"] = pType.bufferViewId;
                    }

                    friend void from_json(const nlohmann::json& json, DracoMeshCompression& pType) {
                        nlohmann::from_json(json, static_cast<glTFProperty&>(pType));

                        if (auto iter = json.find("bufferView"); iter != json.end())
                            pType.bufferViewId = std::to_string(iter.value().get<uint32_t>());


                        if (auto it = json.find("attributes"); it != json.end()) {
                            if (!it.value().is_object())
                                throw GLTFException("Member attributes of " + std::string(DRACOMESHCOMPRESSION_NAME) + " is not an object.");

                            for (const auto& attribute : it.value().items()) {
                                auto &name = attribute.key();
                                if (!attribute.value().is_number_integer())
                                    throw GLTFException("Attribute " + std::string(name) + " of " + std::string(DRACOMESHCOMPRESSION_NAME) + " is not a number.");

                                pType.attributes.emplace(name, attribute.value().get<uint32_t>());
                            }
                        }
                    }
                };

                nlohmann::json SerializeDracoMeshCompression(const DracoMeshCompression &dracoMeshCompression,
                                                             const Document &gltfDocument,
                                                             const ExtensionSerializer &extensionSerializer);
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

                    friend void to_json(nlohmann::json& json, const TextureTransform& pType) {
                        nlohmann::to_json(json, static_cast<const glTFProperty&>(pType));
                        if (pType.offset != Vector2::ZERO) json["offset"] = pType.offset;
                        if (pType.rotation != 0.0f) json["rotation"] = pType.rotation;
                        if (pType.scale != Vector2::ONE) json["scale"] = pType.scale;
                        if (pType.texCoord) json["texCoord"] = pType.texCoord.Get();
                    }

                    friend void from_json(const nlohmann::json& json, TextureTransform& pType) {
                        nlohmann::from_json(json, static_cast<glTFProperty&>(pType));


                        if (auto iter = json.find("offset"); iter != json.end()){
                            auto &val = iter.value();
                            if (!val.is_array())
                                throw GLTFException("Offset member of " + std::string(TEXTURETRANSFORM_NAME) + " must be an array.");

                            if (val.size() != 2)
                                throw GLTFException("Offset member of " + std::string(TEXTURETRANSFORM_NAME) + " must have two values.");

                            val.get_to(pType.offset);
                        }

                        pType.rotation = json.value("rotation", 0.0f);

                        if (auto iter = json.find("scale"); iter != json.end()) {
                            auto &val = iter.value();
                            if (!val.is_array())
                                throw GLTFException("Scale member of " + std::string(TEXTURETRANSFORM_NAME) + " must be an array.");

                            if (val.size() != 2)
                                throw GLTFException("Scale member of " + std::string(TEXTURETRANSFORM_NAME) + " must have two values.");

                            val.get_to(pType.scale);
                        }

                        if (auto iter = json.find("texCoord"); iter != json.end())
                            pType.texCoord = iter.value().get<size_t>();

                    }
                };

                nlohmann::json SerializeTextureTransform(const TextureTransform &textureTransform,
                                                         const Document &gltfDocument,
                                                         const ExtensionSerializer &extensionSerializer);
                std::unique_ptr<Extension> DeserializeTextureTransform(const nlohmann::json& json, const std::shared_ptr<ExtensionDeserializer>& extensionDeserializer);
            }
        }
    }
}
