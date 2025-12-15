# .varlist #

the .varlist specification is straightforward, line-oriented data format similar to the .env format. All variables are key-value pairs and are either implicitly typed based on its literal form or explicitly typed via a type specifier.

For the exact specification details, refer to spec.txt

#### Example ####

```
#VARLIST 2025.12 
The #VARLIST header is an optional line to specify the version that the .varlist file adheres to, in year.month format.
Any line which is not in the
key : value
format will be ignored by the parser.

with_spaces : values can contain spaces.

; the varlist standard permits explicit type coercion to any of the following types: 
; str, i32, i64, f32, f64.
forced_str<str> : 128
forced_i64<i64> : 256

hex: 0xDAF
octal: -0o5
pi: 3.14159265358979
key: my key
numeric: 57
ip: 127.0.0.1
```

#### Parsing Engine ####
A .varlist parser written in C99 & x86-64 assembly is included. See tests/test.c for example usage.

#### Todo ####
- Write an odin boilerplate for the C99 parsing engine.