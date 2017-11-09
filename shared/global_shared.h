#ifndef A_SHARED_H
#define A_SHARED_H

#include "input/keyboard.h"
#include "input/mouse.h"
#include "file_reader.h"
#include "graphics/matrix_transform.h"
#include "jsonwax/JsonWax.h"

namespace Graphics {
    class MatrixTransform;
}

namespace Shared {

enum VBOConfig {NoVBO, OneVBOPerAttrib, FromIndexInVBO, FromVBOStart_SequentialAttribs, SequentialAttribs, FromVBOStart_InterleavedAttribs};
enum VBOAttrib {Positions, Normals, Colors, TexCoords, Disabled};
enum CullingMode {CullBack, CullFront, CullFrontAndBack, CullDisabled};
enum VBOUsage {StreamDraw,  StreamRead,  StreamCopy,        // Stream:  It's assumed that you'll be updating the data in the
                                                            //          buffer object often, but then only using it a few times
                                                            //          in drawing or other operations.
               StaticDraw,  StaticRead,  StaticCopy,        // Static:  You specify the data once, but use the values often.
               DynamicDraw, DynamicRead, DynamicCopy};      // Dynamic: You may update the data often and use the data values
                                                            //          in the buffer object many times as well.
                                                            //
                                                            // Drawing: The client specifies data that is used for rendering.
                                                            // Reading: Data values are read from an OpenGL buffer (such as
                                                            //          the framebuffer) and used in the application in various
                                                            //          computations not immediately related to rendering.
                                                            // Copying: Data values are read from an OpenGL buffer and then used
                                                            //          as data for rendering.
class Global
{
public:
    Graphics::MatrixTransform* matrixTransform = nullptr;
    QString applicationPath;
    FileReader fileReader = FileReader();
    double indexGainCondition = 0.9;            // If there will be a memory gain from using indices.
                                                // 0.9 means that, if we are to use indices, the memory requirement of
                                                // using them is allowed to be between 0 and 0.9 times the memory
                                                // requirement of not using indices (1.0 means no gain, anything below uses less memory).
    Input::Keyboard keyboard;
    Input::Mouse mouse;
    int resolutionW = 0;
    int resolutionH = 0;
    JsonWax* jsonSettings = nullptr;
    JsonWax* jsonKeyboard = nullptr;
    bool closeProgram = false;
    QString endMessage;

    void setApplicationPath(QString path);
    float fractionPart( float& value);
    void setIndexGainCondition( float limit);
    void init();
};

extern Global global;
}

#endif // A_SHARED_H
