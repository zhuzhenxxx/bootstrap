# bootstrap
# C++ 虚拟机项目
### heap memory
```
    08 05 00 01 02 00 00 00
    ^   ^     ^  ^
    struct_size opcode opcode_size operand_count
```

- staticlly typed
- functions are going to be 1st class
- namespaces
- *probaly* package manager
- c-like syntax with some pascal flavor
- consistent primitive types:

```
consistent primitive types
    s4        u4
    s8        u8
    s16       u16
    s32       u32
    s64       u64
    s128      u128
    
    f32
    f64
    string(len prefixed)
    smart arrays (with size/iterator)
    
    pointers
    control over allocation model
```
- "preprocessor" but based on recursively running Basecode within Basecode
- flow control: if/else if/else/something switch-like
- while loops/for loops
- labelled jumps
- structs
- enums
- attributes
- support for dynamic/shared library invocation
- static linking.....
- include / import
----------------------------------------------------

```c
names := string:[];
adder := fn(a:u16, b:u16) :u16 {
    a + b
    
    for name in names {
        
    }
}

std::foo::bar::adder := fn(a:u32, b:u32):u32 {
   a += b;
   -- or --
   return a += b;
}
```