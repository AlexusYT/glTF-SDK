// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "stdafx.h"
#include <GLTFSDK/Deserialize.h>
#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/GLBResourceWriter.h>
#include <GLTFSDK/Serialize.h>
#include "TestUtils.h"

using namespace glTF::UnitTest;

namespace Microsoft
{
    namespace glTF
    {
        namespace Test
        {
            GLTFSDK_TEST_CLASS(GLBResourceWriterTests)
            {
                GLTFSDK_TEST_METHOD(GLBResourceWriterTests, WriteBufferView_Empty_Bin)
                {
                    auto streamWriter = std::make_shared<const StreamReaderWriter>();
                    GLBResourceWriter writer(streamWriter);
                    std::string uri = "foo.glb";

                    // Serialize Default Document -> Json string -> Stream
                    auto doc = Document::create();
                    const auto serialiedJson = Serializer::Serialize(doc, SerializeFlags::None);
                    writer.Flush(serialiedJson, uri);
                    auto stream = streamWriter->GetInputStream(uri);

                    // Deserialize Stream -> Document
                    GLBResourceReader resourceReader(streamWriter, stream);
                    auto roundTrippedDoc = Deserializer::Deserialize(resourceReader.GetJson());

                    Assert::IsFalse(stream->fail());
                    Assert::IsTrue(*doc == *roundTrippedDoc);
                }
            };
        }
    }
}
