// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <GLTFSDK/GLTF.h>
#include <GLTFSDK/IndexedContainer.h>

#include <unordered_set>

namespace Microsoft
{
    namespace glTF
    {
        class Document : public glTFProperty
        {
        public:

            Asset asset;

            IndexedContainer<const Accessor> accessors;
            IndexedContainer<const Animation> animations;
            IndexedContainer<const Buffer> buffers;
            IndexedContainer<const BufferView> bufferViews;
            IndexedContainer<const Camera> cameras;
            IndexedContainer<const Image> images;
            IndexedContainer<const Material> materials;
            IndexedContainer<const Mesh> meshes;
            IndexedContainer<const Node> nodes;
            IndexedContainer<const Sampler> samplers;
            IndexedContainer<const Scene> scenes;
            IndexedContainer<const Skin> skins;
            IndexedContainer<const Texture> textures;

            std::unordered_set<std::string> extensionsUsed;
            std::unordered_set<std::string> extensionsRequired;

            std::string defaultSceneId;

            //should be removed
            std::shared_ptr<ExtensionSerializer> serializer;
        protected:
            Document();
            Document(Asset&& asset);
        public:

            static std::shared_ptr<Document> create() {

                auto ptr =  std::shared_ptr<Document>(new Document());

                ptr->asset.setGltfDocument(ptr.get());
                ptr->accessors.setGltfDocument(ptr.get());
                ptr->animations.setGltfDocument(ptr.get());
                ptr->buffers.setGltfDocument(ptr.get());
                ptr->bufferViews.setGltfDocument(ptr.get());
                ptr->cameras.setGltfDocument(ptr.get());
                ptr->images.setGltfDocument(ptr.get());
                ptr->materials.setGltfDocument(ptr.get());
                ptr->meshes.setGltfDocument(ptr.get());
                ptr->nodes.setGltfDocument(ptr.get());
                ptr->samplers.setGltfDocument(ptr.get());
                ptr->scenes.setGltfDocument(ptr.get());
                ptr->skins.setGltfDocument(ptr.get());
                ptr->textures.setGltfDocument(ptr.get());
                ptr->setGltfDocument(ptr.get());
                return ptr;
            }

            bool operator==(const Document& rhs) const;

            bool IsExtensionUsed(const std::string& extension) const;
            bool IsExtensionRequired(const std::string& extension) const;

            bool HasDefaultScene() const;
            const Scene& GetDefaultScene() const;
            const Scene& SetDefaultScene(Scene&& scene, AppendIdPolicy policy = AppendIdPolicy::ThrowOnEmpty);


            void serialize(nlohmann::json& json) const;

            void deserialize(const nlohmann::json& json);

            friend void to_json(nlohmann::json& json, const std::shared_ptr<Document>& type) {
                nlohmann::to_json(json, static_cast<const glTFProperty&>(*type));
                type->serialize(json);
            }

            friend void from_json(const nlohmann::json& json, std::shared_ptr<Document>& pType) {

                pType = create();
                nlohmann::from_json(json, static_cast<glTFProperty&>(*pType));
                if (auto iter = json.find("asset"); iter != json.end()) {
                    iter.value().get_to(pType->asset);
                }
                if (auto iter = json.find("accessors"); iter != json.end()) iter.value().get_to(pType->accessors);
                if (auto iter = json.find("animations"); iter != json.end()) iter.value().get_to(pType->animations);
                if (auto iter = json.find("buffers"); iter != json.end()) iter.value().get_to(pType->buffers);
                if (auto iter = json.find("bufferViews"); iter != json.end()) iter.value().get_to(pType->bufferViews);
                if (auto iter = json.find("cameras"); iter != json.end()) iter.value().get_to(pType->cameras);
                if (auto iter = json.find("images"); iter != json.end()) iter.value().get_to(pType->images);
                if (auto iter = json.find("materials"); iter != json.end()) iter.value().get_to(pType->materials);
                if (auto iter = json.find("meshes"); iter != json.end()) iter.value().get_to(pType->meshes);
                if (auto iter = json.find("nodes"); iter != json.end()) iter.value().get_to(pType->nodes);
                if (auto iter = json.find("samplers"); iter != json.end()) iter.value().get_to(pType->samplers);
                if (auto iter = json.find("scenes"); iter != json.end()) iter.value().get_to(pType->scenes);
                if (auto iter = json.find("skins"); iter != json.end()) iter.value().get_to(pType->skins);
                if (auto iter = json.find("textures"); iter != json.end()) iter.value().get_to(pType->textures);

                if (auto iter = json.find("scene"); iter != json.end()) {
                    pType->defaultSceneId = std::to_string(iter.value().get<unsigned int>());
                }

                if (auto iter = json.find("extensionsUsed"); iter != json.end())
                    iter.value().get_to(pType->extensionsUsed);

                if (auto iter = json.find("extensionsRequired"); iter != json.end())
                    iter.value().get_to(pType->extensionsRequired);
            }

            void deserializeExtensions(const std::shared_ptr<ExtensionDeserializer> &pDeserializer) override {
                glTFProperty::deserializeExtensions(pDeserializer);

                asset.deserializeExtensions(pDeserializer);
                accessors.deserializeExtensions(pDeserializer);
                animations.deserializeExtensions(pDeserializer);
                buffers.deserializeExtensions(pDeserializer);
                bufferViews.deserializeExtensions(pDeserializer);
                cameras.deserializeExtensions(pDeserializer);
                images.deserializeExtensions(pDeserializer);
                materials.deserializeExtensions(pDeserializer);
                meshes.deserializeExtensions(pDeserializer);
                nodes.deserializeExtensions(pDeserializer);
                samplers.deserializeExtensions(pDeserializer);
                scenes.deserializeExtensions(pDeserializer);
                skins.deserializeExtensions(pDeserializer);
                textures.deserializeExtensions(pDeserializer);
            }
        };
    }
}
