# 🎥 libgk - Graphics Kernel

Basically this is a rendering library/kernel. The main purpose was render [AssetKit](https://github.com/recp/assetkit) content. Then it became general purpose rendering library. You can also call this "Graphics Kit".

libgk doesn't depend on AssetKit, so there are another library to load AssetKit content to libgk called [assetkit-gl](https://github.com/recp/assetkit-gl), you can see how to load contents to libgk in this library. Because libgk doesn't provide loading contents, in the future maybe it will.

Currently this library only renders using OpenGL, but in the future other apis like `Vulkan`, `DirectX`, `Metal` are considered to be supported! 

# Features:
todo

# Todo
- [ ] PBR 
- [ ] Transparency in common profile
- [ ] Frustum culling
- [ ] Occlusion Culling
- [ ] Level of Detail for mesh
- [ ] Separate common profile into multiple small shaders to reduce branching in GPU
- [ ] Multithread rendering
- [ ] Textures
   - [x] 2D textures for common profile
- [ ] Instanced Rendering
- [ ] Animations
- [ ] ... 

# Build 
todo

# License
MIT. check the LICENSE file

# Notes
- This library is still unshaped, wait this until AssetKit finished. 
- After finished this AssetKit and library, I'll publish documentation and update README.
- Also check [simple-collada-viewer](https://github.com/recp/simple-collada-viewer) repo to see an example to how to use this
