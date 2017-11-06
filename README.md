# krapyl-engine
Krapyl Engine will eventually become a GPLv3 licensed OpenGL 3D engine written in Qt C++, using SDL2 instead of Qt 3D.

Current state:
- Not in a good state for any actual use. Has bugs. Is not streamlined. It can barely be called an engine.
- Purely OpenGL 2.1.
- It can load meshes from .obj files (requires that they have been exported with indices).
- Textured and multi-colored 3d objects can be exported from Blender, and viewed (it reads the .mtl and image file).
- Translate and rotate scenes and entities.
- Meshes can be put in VBO's if chosen.
- You can apply shaders on entities, and update uniforms per frame (however, this hasn't been optimized).
- Currently you have to create at least one VBO per mesh, but you may choose between sequential / interleaved attributes.
- It can generate a simple 3d mesh programmatically.
- You can create the texture from a QImage. You can also write a QString to the texture, and update it every frame.
- Configure basic graphical settings via a JSON document:
• Mipmaps • bilinear/trilinear texture filtering • MSAA • fullscreen • vsync • resolution • number of windows to display • FPS limiting.
- Multiple windows with OpenGL context sharing (allowed through SDL).

Things to create next:
- Proper VBO and data management.
- A powerful event system that scales.
- A proper ECS system.
- Proper camera functions.
- Programmable keyboard/mouse/gamepad link to events.
- Cull entities that are out of view frustrum.
- Proper shader management / shader inheritance.
- Light and shadows.
- Generate, load and update Meshes at runtime. 
- Generate Meshes and Textures from data specified in JSON (could be really cool).
- Flip y axis, so that minus is down instead of up... It's really annoying right now.
- Simplify but keep flexible.

Project goals:
- OpenGL 2.1 graphics all the way (probably). This must be able to run on the vast majority of PC's from the last 10 years. At some point it must be made to run on a Raspberry Pi.
- High integration with JSON, so that all world data may be written to JSON files (Model-View-Controller system).
- Decent performance, but focus on flexibility and being dynamic.
- A good system for applying shaders.
- Support for multiple players on one PC, each having their own 3d window. I really like this feature, it's very useful for couch-coop games.
- Fully integrated P2P local network system.
- Support both large and small 3D worlds (stream the world).
- Programmable world generation.
- 3D world editor.
- Animation support through collada format.

Frameworks/libraries used: Qt, SDL2, GLEW, Glu (glu is barely used, and will be removed).
