# Yet another vector library for C language

## Introduction

This vector library provides functions for C language.

This library is characterized as following:

- no source codes, only a header
- correspond to any type
- APIs of library are similar to Vector's one in Java

And this library is the following restrictions:

- create only one type vector in your one source code
- all library APIs are implemented on macro

These restrictions are due to the need to respond any type at this library.

## License

This library is free software distributed under the terms of the MIT license
reproduced here.

## Install

Copy vector.h file to your source code tree.

## How to use

Please see API document of doc folder.

## History

### Ver 0.2.0: March 31, 2013
- Added #to_array function
- Changed specification of vector capacity
- Changed #vector_remove_internal to static (bug fix)

### Ver 0.1.0: March 29, 2013
- First release
