# meatscript

Mobile, easy and typed scripting language
```
var greeting = "Hello World!"
printn "$greeting"
```
```
$> meatscript -e 'printn "hello World!"'
```

## Variables and functions

Variables are declared by using the `var` keyword or by using its type `var|type_name var_name = ...`

Constants can be declared by using the `const` keyword `const var number = 3`
### Built-in types
- `struct`
- `function`
- `string`
- `[T]`    -> Array of Type T
- `[#T T]` -> Hashmap with key `#T` and value type of `T`


- `int` `int8` ... `int64`
- `uint` `uint8` ... `uint64`
- `float` `float8` ... `float64`

## Small example:
###

```
[#string int] continent_count = 0

var planet ?= ENV("planet") : "World"
print "Hello $planet!\n"

func ask_continent() -> void:
        print "Which continent are you from?: "
        var continent = read_line(:echo_on) -> String.to_lower() -> String.trim()
        continent_count[continent]+= 1
end

func dump_answers() -> void:
        for var answer in continent_count do
			printn "$answer.key: $answer.value"
        end
end

ask_continent
dump_answers

```
