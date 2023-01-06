# argvparse
A C++ library for easier parsing of command line arguments

# Example Usage
Define a list of desired arguments with a description and then you call `auto a = get_arguments(possible_arguments, argc, argv);`. 
You can then check if an argument has been provided by looking up the results like so: `myargs["long-name-look-up"]`. 
The result from `get_arguments` is an `std::unordered_map` with the long name as the key.

## Full Code Example
See [main.cpp](src/main.cpp) for a full example. This is also available as a cmake build target: `example_usage`

# Install
For compiling this library, a C++17 compatible compiler is required. The library also uses standard library components, so if you don't like that feel free to submit a pull request with custom 'unordered_map', 'vector' and 'std::for_each' implementations.
```
mkdir bin && cd bin
cmake .. -DCMAKE_BUILD_TYPE=Release 
sudo make install
```
For uninstall
```
xargs rm < install_manifest.txt
```
Or just manually delete the files listed in `install_manifest.txt`. They are likely these:
```
/usr/local/lib/libargvparse.so
/usr/local/include/arguments.h
```
