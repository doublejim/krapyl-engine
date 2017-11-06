# krapyl-engine
Krapyl Engine will eventually become a GPLv3 licensed OpenGL 3D engine for Qt, using SDL2 instead of Qt 3D.

Current state:

- Not in a good state for any actual use. Has bugs. Is not streamlined. It can barely be called an engine.
- Purely OpenGL 2.1.
- It can load meshes from .obj files (requires that they have been exported with indices).
- Textured and multi-colored 3d objects can be exported from Blender, and viewed (it reads the .mtl file).
- Translate and rotate scenes and entities (I haven't tested scaling yet).
- Meshes can be put in VBO's if chosen.
- You can apply shaders on entities, and update uniforms per frame (however, this hasn't been optimized).
- Currently you have to create at least one VBO per mesh, but you may choose between sequential / interleaved attributes.
- It can generate a simple 3d mesh programmatically.
- You can create the texture from a QImage. You can also write a QString to the texture, and update it every frame.
- Configure basic graphical settings via a JSON document:
• Mipmaps • bilinear/trilinear texture filtering • MSAA • fullscreen • vsync • resolution • number of windows to display • FPS limiting.
- Multiple windows with OpenGL context sharing (allowed through SDL).

Things to create next:
- Proper VBO management.
- An event system.
- A proper ECS system.
- Proper camera functions.
- Programmable keyboard/mouse link to events.
- Cull entities that are out of view frustrum.
- Proper shader management / shader inheritance.
- Light and shadows.
- Generate Meshes and Textures from data specified in JSON.
- Flip y axis, so that minus is down instead of up... It's really annoying right now.
- Simplify but keep flexible.

Project goals:
- OpenGL 2.1 graphics all the way. This must be able to run on the vast majority of PC's from the last 10 years. At some point it must be made to run on a Raspberry Pi.
- High integration with JSON, so that all world data may be written to JSON files (Model-View-Controller system).
- Support for multiple players on one PC, each having their own 3d window.
- Support both large and small 3D worlds.
- 3D world editor.

Frameworks/libraries used: Qt, SDL2, GLEW, Glu (glu is barely used, and will be removed).
