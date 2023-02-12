*Flavo Engine 2, mid-term goals and proposals*

*General*
- Language: C++, all C++20/C++23 features allowed, except for modules. Please refer to the list of supported features first: https://en.cppreference.com/w/cpp/compiler_support
- Compiler: MSVC 19.33 or newer (Visual Studio 2022)
- Version control: Git
- Project generator: CMake 3.23 or newer

*Rendering*
- API: Vulkan
- Mode: Async render thread, may spawn multiple subtasks for batches of render items
- Game<->Render thread synchronization: Pipelining
    - Whenever any object needs to be accessed by both Game and Render threads, it should be duplicated
    - Templated helper struct should be used to do that -> GameRenderData<T>
    - Game data should always contain current object value
    - Render data should initially contain object value from previous frame - it may be further modified by Render thread, though
    - Synchronization should be done at the end of a frame - all GameRenderData<T> objects should have their Game data copied to Render data
    - Upsides: lockless data access for both Game and Render
    - Downsides: increased memory usage, additional time to copy buffers during Sync window, Render is 1 frame behind
- Example frame timeline:
    START FRAME N |-----|_________________Game______________________| Sync window | END FRAME N
    START FRAME N |----------|___________Render_________________|---|-------------| END FRAME N
    START FRAME N |-----------|_RenderTask_|---|_RenderTask_|---|---|-------------| END FRAME N
    START FRAME N |---------------|_RenderTask_|-|__RenderTask__|---|-------------| END FRAME N

*Physics*
- API: ???

*Low-level engine*
- Architecture: Archetype-based Entity-Component-System. See link: https://ajmmertens.medium.com/building-an-ecs-2-archetypes-and-vectorization-fe21690805f9
    - Systems contain actual methods to operate on data, batch processing
    - Components contain data, may have some helpful simple methods, though
    - Entites are just handles
    - Systems may use other systems directly - no frigging events and hidden dependencies
    - However, it is heavily recommended to reduce number of dependencies as much as possible
    - Async execution - it may be either on the system-level (e.g. PhysicsSystem and AudioSystem) or entity level (e.g. 1000 entities with component Rigidbody processed simultanously)
        - Imporant! Order of systems and their (possible) async execution should be done in specific GAME code
- Standard library - usage is of course allowed, but should be aliased e.g. `namespace ftl { using std::string; }`
- Meshes, animations and textures - should be imported offline and converted into more optimized, custom binary forms

*Used libraries*
- Assimp - asset importer
- ImGUI - immediate mode UI system. Should be used in Editor, maybe even in game itself
