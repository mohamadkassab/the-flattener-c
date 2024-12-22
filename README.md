# The Flattener

**Programming Language**: C

## Purpose

The main goal of this project is to process and transform a nested JSON object into a flat JSON object by iterating over all its components.

## Key Features

- **JSON Iteration**: Efficient traversal of JSON structures regardless of their depth or complexity.
- **Transformation**: Converts a deeply nested JSON object into a single-level flat structure.
- **Performance**: Highly optimized for large JSON objects.

## Performance Benchmarks

- A JSON object with **10,000 fields**: **0.043 seconds**
- A JSON object with **20,000 fields**: **0.09 seconds**
- A JSON object with **100,000 fields**: **0.8 seconds**

## Technology Highlights

- **C Programming**: Used for its performance and memory efficiency, especially for processing large datasets.
- **Library Management**: Leveraged **vcpkg** to simplify the installation and management of necessary libraries, ensuring a smooth development process.
- **Efficient Parsing**: Optimized algorithms for JSON iteration and transformation.

## Advantages

- **High Performance**: The project is exceptionally fast, making it suitable for processing large and complex JSON objects in minimal time.

## Disadvantages

- **Error Handling**: Since C lacks native `try-catch` constructs, errors must be handled very carefully. This requires additional code to check for and manage potential issues like null pointers or invalid JSON structures.

