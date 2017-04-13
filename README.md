# Route11
C++ flow-based metaprogramming library

Route11 is a powerful C++ flow-based metaprogramming library that allows you to create and route complex dataflow systems at compile-time.

Route11 uses Template Metaprogramming to construct complex "process systems" defined by static relationships between "process primitives". A process primitive is a generic host class that inherits a "process policy". The process policy defines the specific behavior of a process primitive. The general term: "process" can refer to either one of these abstractions (I.e. "2 processes connected in series" may refer to 2 primitives, 2 systems, or one of each)

NOTE: Route11 was written using many new C++11 features, so it is required that a C++11 compatible compiler (e.g Clang or GCC) is used when building this project.
