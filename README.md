<img src="logo.png" alt="logo" width="800"/>

## Usage

```c
// This macro must be defined once before including the header for the first time
// It will add all the definitions
#define BJSON_IMPLEMENTATION

// Then you can just include the header and start using all the functions
#include <BlueJSON.h>
```
* It is recommended that the file chosen for the "implementation" process is not a header file.
* Later includes of the **BlueJSON.h** header must not have the macro defined.

## Documentation

Available in the repo's [Wiki](https://github.com/mrbru12/BlueJSON/wiki).

## Why I made BlueJSON?

For the sake of doing it. 🙃 Just kidding, actually I always wanted to make my own JSON parser from scratch, since I love kind of reinventing the wheel in C. But every time I started putting things together, I would get unmotivated little by little, specially by bad design choices, until I have given up. But now I started and actually finished it! It might not be the most optimized and well-written JSON parser, but it works (quite well indeed) and I like how it turned out. It was lots of fun to develop, focusing on thinking about efficient solutions by myself. :)

## References
[json.org](https://www.json.org/json-en.html) - All JSON structure, syntax and style information plus some examples. \
[wikipedia.org/wiki/JSON](https://en.wikipedia.org/wiki/JSON) - Some specific details about JSON data types.

## License
This library is licensed under the MIT License, present inside the **BlueJSON.h** file.