# raytracerpp
## Simple C++ raytracer

This is a C++ Monte Carlo raytracer that was originally written as a university assignment. As such, it heavily uses OO (heterogeneous collection, the shapes are all descendants of IShape, ISerializable interface), as such, it's not the most performant.

As per the course requirement, it doesn't use STL, so the shared pointer and dynamic array types are all implemented in the program.

Multithreading can be enabled which divides the screen into tiles (which are called regions in the program) based on the thread count.
In the example scene this is very inefficient, as most of the scene consists of the sky.

There are some hungarian comments left in the code, mostly specific quirks of the original testing environment.

The rendering was _heavily_ influenced by Peter Shirley's [Ray-tracing in a weeked](https://www.realtimerendering.com/raytracing/Ray%20Tracing%20in%20a%20Weekend.pdf).

# License
This software can be used/reused by anyone, free of charge.
