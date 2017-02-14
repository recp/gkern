# 🎥 libgk - Graphics Kernel

Basically this is a rendering library/kernel. The main purpose was render [AssetKit](https://github.com/recp/assetkit) content. Then it became general purpose rendering library. You can also call this "Graphics Kit".

libgk doesn't depend on AssetKit, so there are another library to load AssetKit content to libgk called [assetkit-gl](https://github.com/recp/assetkit-gl), you can see how to load contents to libgk in this library. Because libgk doesn't provide loading contents, in the future maybe it will.

Currently this library only renders using OpenGL, but in the future other apis like `Vulkan`, `DirectX`, `Metal` are considered to be supported!
# Features:
- Utils for loading shaders
- Default shaders/materials and default program
  - You don't have to write shaders (vertex, fragment) but you can if you want
- Cache transforms 
  - cache world transforms (Model Matrix) in nodes
  - cache ViewModel matrix
  - cache ProjectViewModel matrix
- If model matrix doesn't have uniform scale then you must provide `Normal Matrix`, you don't have to do that with `libgk` , libgk checks this situation and sends NormalMatrix to shader if necessary and a boolean about this. This way will improve performance by avoid extra calc (inverse, mul) and copy ops.
- Render scene, node, model
- Common materials
  - Send all materials to shaders (uniform material)
- Lights, Multiple lights
  - Send all lights to shaders (uniform light)
- Built-in trackball

# Todo
- [ ] Don't render models which are outside of view frustum using bounding box
- [ ] Textures
- [ ] ... 

# Build 
todo

# License
MIT. check the LICENSE file

# Notes
- This library is still unshaped, wait this until AssetKit finished. 
- After finished this AssetKit and library, I'll publish documentation and update README.
- Also check [simple-collada-viewer](https://github.com/recp/simple-collada-viewer) repo to see an example to how to use this
