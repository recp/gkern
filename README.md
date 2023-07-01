# 🫧 gkern: Graphics Kernel

`gkern` is a flexible, highly configurable, and extensible render engine. It's designed for both realtime and offline rendering, with a focus on robustness, performance, and ease of use. It is written in `C` so it is easy to use with other languages.

📌 Currently the library renders using OpenGL. I'm trying to integrate with `Metal`, `Vulkan`, `DirectX`

HISTORY: The main purpose was render [AssetKit](https://github.com/recp/assetkit) content. Then it became general purpose rendering and graphics library.

# Features:
- Similar scene data structure with COLLADA/glTF 
- Shader generator for common materials (phong, blinn, lambert, const) 
   - This generator also supports textures, shadowmaps ... all feeatures to render scene perfectly and fast
- Shader manager (currently for common profile)
- Transform caching
- Uniform location caching
- Built-in trackball
- Multi-pass rendering 
- Rendering to Texture
- Multiple Render Targets
- Textures
- State Manager / Caching
- Frustum Culling
- Configurable shadowmaps: Yes you can select which technique to use! More techniques may be supportted by time
   - Basic/Simple Shadow Maps
   - Cascaded Shadow Maps (PSSM)
- Order Independent Transparency
   - Weighted, Blended OIT
   - Some other techniques aill also be supported natively
- PBR materials
   - Metallic-Roughness Workflow
   - Specular-Glossiness Workflow
- Simple animations
- Skeletal animations
- Morph animations
- ... 
- and some built-in helpers for many common tasks and more by fetures time...

# Todo
- [ ] Real-Time Path Tracing
- [ ] Order objects before rendering
- [x] PBR
- [x] Transparency
- [ ] Occlusion Culling
- [ ] Level of Detail for mesh
- [ ] Multithread rendering
- [ ] Instanced Rendering
- [ ] Animations
  - [x] Autoreserve animations
  - [x] Play count...
  - [x] Simple Animations
  - [ ] Keyframe animations
    - [x] Animate float/vec3/vec4
    - [ ] Interpolations
       - [x] STEP
       - [x] LINEAR
       - [x] CUBIC BEZIER SPLINE
       - [ ] CUBIC HERMITE SPLINE
       - [ ] B-SPLINE
       - [ ] CARDINAL SPLINE
  - [x] Skeletal animations (working on this)
  - [x] Morph animations
- [ ] ... 
 
# Screenshots

![](DamagedHelmet_gltf.jpg)

# Build 
todo

# Notes
- This library is still unshaped, wait this until AssetKit finished. 
- After finished this AssetKit and library, I'll publish documentation and update README.
- Also check [simple-collada-viewer](https://github.com/recp/simple-collada-viewer) repo to see an example to how to use this
