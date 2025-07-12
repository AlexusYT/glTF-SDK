// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "stdafx.h"

#include <GLTFSDK/GLTF.h>
#include <GLTFSDK/Serialize.h>
#include <GLTFSDK/Deserialize.h>

using namespace glTF::UnitTest;

namespace
{
    const char* c_expectedDefaultDocument = R"({
    "asset": {
        "version": "2.0"
    }
})";

    const char* c_expectedDefaultDocumentAndScene = R"({
    "asset": {
        "version": "2.0"
    },
    "scenes": [
        {}
    ]
})";

    const char* c_expectedDefaultDocumentAndSceneAsDefault = R"({
    "asset": {
        "version": "2.0"
    },
    "scene": 0,
    "scenes": [
        {}
    ]
})";

    const char* c_expectedDefaultDocumentAndNonDefaultScene = R"({
    "asset": {
        "version": "2.0"
    },
    "scenes": [
        {}
    ]
})";

    const char* c_expectedDefaultDocumentAndNonDefaultSceneAsDefault = R"({
    "asset": {
        "version": "2.0"
    },
    "scene": 0,
    "scenes": [
        {}
    ]
})";
}

namespace Microsoft
{
    namespace glTF
    {
        namespace Test
        {
            GLTFSDK_TEST_CLASS(SerializeTests)
            {
                GLTFSDK_TEST_METHOD(SerializeTests, SerializeNodeMatrixTransform)
                {
                    auto originalDoc = Document::create();
                    Scene sc; sc.id = "0";
                    sc.nodes = { "0" };
                    originalDoc->SetDefaultScene(std::move(sc));
                    std::array<float, 16> matrixData; std::fill(matrixData.begin(), matrixData.end(), 1.0f);
                    Matrix4 mat4; mat4.values = matrixData;
                    Node matrixNode; matrixNode.id = "0"; matrixNode.name = "matrixNode";
                    matrixNode.matrix = mat4;
                    originalDoc->nodes.Append(std::move(matrixNode));
                    auto outputString = Serializer::Serialize(originalDoc);
                    auto twoPassDoc = Deserializer::Deserialize(outputString);
                    Assert::IsTrue(*twoPassDoc == *originalDoc);
                }

                GLTFSDK_TEST_METHOD(SerializeTests, SerializeNodeTRSTransform)
                {
                    auto originalDoc = Document::create();
                    Scene sc; sc.id = "0";
                    sc.nodes = { "0" };
                    originalDoc->SetDefaultScene(std::move(sc));
                    Vector3 translation = { 1.0f, 1.0f, 1.0f };
                    Vector3 scaling = { 0.1f, 0.42f, 0.133f };
                    Node trsNode; trsNode.id = "0"; trsNode.name = "trsNode";
                    trsNode.translation = translation;
                    trsNode.scale = scaling;
                    originalDoc->nodes.Append(std::move(trsNode));
                    auto outputString = Serializer::Serialize(originalDoc);
                    auto twoPassDoc = Deserializer::Deserialize(outputString);
                    Assert::IsTrue(*twoPassDoc == *originalDoc);
                }

                GLTFSDK_TEST_METHOD(SerializeTests, SerializeNodeInvalidTransform)
                {
                    Assert::ExpectException<DocumentException>([]()
                    {
                    auto originalDoc = Document::create();
                        Scene sc; sc.id = "0";
                        sc.nodes = { "0" };
                        originalDoc->SetDefaultScene(std::move(sc));
                        Vector3 translation = { 1.0f, 1.0f, 1.0f };
                        Vector3 scaling = { 0.1f, 0.42f, 0.133f };
                        std::array<float, 16> matrixData; std::fill(matrixData.begin(), matrixData.end(), 1.0f);
                        Matrix4 mat4; mat4.values = matrixData;
                        Node badNode; badNode.id = "0"; badNode.name = "badNode";
                        badNode.translation = translation;
                        badNode.scale = scaling;
                        badNode.matrix = mat4;
                        originalDoc->nodes.Append(std::move(badNode));
                        auto outputString = Serializer::Serialize(originalDoc);
                    });
                }

                GLTFSDK_TEST_METHOD(SerializeTests, MatrixNodeTest)
                {
                    Node matrixNode;
                    std::array<float, 16> matrixData; std::fill(matrixData.begin(), matrixData.end(), 1.0f);
                    Matrix4 mat4; mat4.values = matrixData;
                    matrixNode.matrix = mat4;
                    Assert::IsTrue(matrixNode.GetTransformationType() == TransformationType::TRANSFORMATION_MATRIX);
                }

                GLTFSDK_TEST_METHOD(SerializeTests, NoTransformTest)
                {
                    Node defaultNode;
                    Assert::IsTrue(defaultNode.GetTransformationType() == TransformationType::TRANSFORMATION_IDENTITY);
                }

                GLTFSDK_TEST_METHOD(SerializeTests, TRSNodeTest)
                {
                    Node trsNode;
                    Vector3 scale = { 2.0f, 1.1f, 4.0f };
                    trsNode.scale = scale;
                    Assert::IsTrue(trsNode.GetTransformationType() == TransformationType::TRANSFORMATION_TRS);
                }

                GLTFSDK_TEST_METHOD(SerializeTests, InvalidNodeTest)
                {
                    Node badNode;
                    std::array<float, 16> matrixData; std::fill(matrixData.begin(), matrixData.end(), 1.0f);
                    Matrix4 mat4; mat4.values = matrixData;
                    Vector3 scale = { 2.0f, 1.1f, 4.0f };
                    badNode.matrix = mat4;
                    badNode.scale = scale;
                    Assert::IsFalse(badNode.HasValidTransformType());
                }

                GLTFSDK_TEST_METHOD(SerializeTests, ValidNodeTest)
                {
                    Node node;
                    Assert::IsTrue(node.HasValidTransformType());
                }

                GLTFSDK_TEST_METHOD(SerializeTests, PerspectiveCameraTest)
                {
                    Camera cam("0", "", std::make_unique<Perspective>(0.1f, 10.0f, 1.2f, 0.5f));
                    Assert::IsTrue(cam.projection->GetProjectionType() == PROJECTION_PERSPECTIVE);
                }

                GLTFSDK_TEST_METHOD(SerializeTests, DefaultDocument)
                {
                    auto doc = Document::create();

                    const auto output = Serializer::Serialize(doc, true);
                    Assert::AreEqual(output.c_str(), c_expectedDefaultDocument);
                }

                GLTFSDK_TEST_METHOD(SerializeTests, DefaultDocumentAndScene)
                {
                    auto doc = Document::create();
                    doc->scenes.Append(Scene(), AppendIdPolicy::GenerateOnEmpty);

                    const auto output = Serializer::Serialize(doc, true);
                    Assert::AreEqual(output.c_str(), c_expectedDefaultDocumentAndScene);
                }

                GLTFSDK_TEST_METHOD(SerializeTests, DefaultDocumentAndSceneAsDefault)
                {
                    auto doc = Document::create();
                    doc->SetDefaultScene(Scene(), AppendIdPolicy::GenerateOnEmpty);

                    const auto output = Serializer::Serialize(doc, true);
                    Assert::AreEqual(output.c_str(), c_expectedDefaultDocumentAndSceneAsDefault);
                }

                GLTFSDK_TEST_METHOD(SerializeTests, DefaultDocumentAndNonDefaultScene)
                {
                    auto doc = Document::create();
                    Scene scene;
                    scene.id = "foo";
                    doc->scenes.Append(std::move(scene));

                    const auto output = Serializer::Serialize(doc, true);
                    Assert::AreEqual(output.c_str(), c_expectedDefaultDocumentAndNonDefaultScene);
                }

                GLTFSDK_TEST_METHOD(SerializeTests, DefaultDocumentAndNonDefaultSceneAsDefault)
                {
                    auto doc = Document::create();
                    Scene scene;
                    scene.id = "foo";
                    doc->SetDefaultScene(std::move(scene));

                    const auto output = Serializer::Serialize(doc, true);
                    Assert::AreEqual(output.c_str(), c_expectedDefaultDocumentAndNonDefaultSceneAsDefault);
                }

                GLTFSDK_TEST_METHOD(SerializeTests, InvalidDefaultScene)
                {
                    Scene scene;
                    scene.id = "foo";

                    auto doc = Document::create();
                    doc->scenes.Append(std::move(scene));
                    doc->defaultSceneId = "bar";

                    Assert::ExpectException<GLTFException>([&doc]
                    {
                        try
                        {
                            Serializer::Serialize(doc);
                        }
                        catch (const GLTFException& ex)
                        {
                            Assert::AreEqual("key bar not in container", ex.what());
                            throw;
                        }
                    }, L"Expected exception was not thrown");
                }
            };
        }
    }
}
