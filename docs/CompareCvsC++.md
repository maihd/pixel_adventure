# Pros of C
1. Simple
2. Near machine
3. Mostly no hidden cost of abstraction
4. Mostly no hidden code
5. Most OS were written in C
6. Performance
7. SIMD
8. Inline and inline asm
9. Explicit over implicit
10. Expilicit structure layout, alignment
11. No RAII, RTTI, Exceptions
12. Memory model is simple
13. Union is easier to use
14. Typedef
15. Enum can use as flags
16. Successful libraries were written in C: OpenGL, Vulkan, stb
17. DoD can be easily use
18. Focus on how program work, not abstraction
19. Cross-platform
20. Flexible because you freely to do things: see Cello, an VM that run natively on C

# Cons of C
1. No namespace, no scoped enum
2. No this call
3. Need to write more
4. No operator overloading
5. No function overloading
6. No constexpr
7. No decltype
8. No RAII
9. Less resources to learn gamedev
10. Less use in gamedev, C is widely use in engine, system
11. No Dear Imgui (C binding is awful to use)
12. Some features are compiler-dependent: __declspec(align(#)), __alignof, __forceline
13. Some features are still missing: __vectorcall, __fastcall (deprecated)
14. No constructor and static evaluation make unit tests harder

# Pros of C++
1. Many pros of C
2. Widely use in gamedev, old engine code still use C++
3. Operator overloading, function overloading
4. Namespace, scoped enum
5. This call, shorthand function call
6. Constexpr
7. Decltype
8. Native support for alignas, alignof
9. RAII
10. More resources, libraries for gamedev
11. Dear ImGui, idLib, The Forge and families
12. Use in my professional (but we use modern C++, sucked!)
13. Const reference
14. Less time to startup, but no difference from C if you are masters (someone still use C and constantly release games periodly)
15. Can be coding like wizzard, do spelling on your code.

# Cons of C++
1. Overuse of RAII
2. Overuse of operator overloading
3. Overuse of function overloading
4. RTTI, Exceptions. Can be avoid, but many libaries still use
5. Pure OOP and OOP mindset for all things
6. Scoped enums are not natural be flags
7. Hard to combine RAII with ECS, DoD
8. Hidden cost of abstraction
9. Hidden code
10. Focus more on abstraction instead of how program work
11. Can be coding like wizzard, do spelling on your code. (All thing come with costs, no matter what the fuck the shit it is)
12. This call maybe make function call confusing.
13. Virtual functions
14. Static functions cannot be added on seperated files
15. Missing novtable in many compilers (We need its to create some dynamic patching system, a.k.a interfaces, but without sacrifice performance and shorthand functions call compare with C interfaces)
16. Some cons of C: compiler-dependent features

# Conlusion
1. Choose C: when you need performance, no magic, just code that work on machine, but sacrifice of development speed
2. Choose C++: when you need semi-performance, but also need abstraction, development speed, and call C functions, use of existent libraries
3. Combine C/C++: best of both world, but still have some cons, because it's hard to decise what language you should use to implement the features. Easybrain: choose existent style, Orthordox C++, Insomniac C++, id Software C++.