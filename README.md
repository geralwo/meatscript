# meats
## mobile, easy and type scripting language

## example hello world:
```
const str = "hello "
var name = io.read_line
name.strip! '\n'
printn "hello $name"
```

### Todo:


- Full implementation of instruction set [⭕]
	- Data Manipulation
		- MOV  	[✅]
		- MOVI  [✅]
		- MOVE  [✅]
		- MOVER [✅]

	- Arithmetic 	[⭕]
		- ADD 	[✅]
		- SUB 	[✅]
		- MUL 	[✅]
		- DIV 	[✅]
		- MOD 	[✅]
		- ADDR	[✅]
		- SUBR 	[✅]
		- MULR 	[✅]
		- DIVR 	[✅]
		- MODR 	[✅]
		- ADDF 	[❌]
		- SUBF 	[❌]
		- MULF 	[❌]
		- DIVF 	[❌]
		- MODF 	[❌]
	- Jump [✅]
		- JMP [✅]
		- JMPZ [✅]
		- JMPNZ [✅]
		- JMPE [✅]
	- Function Calls [❌]
   	- Stack Manipulation [✅]
		- PUSH [✅]
		- POP  [✅]
	- IO [⭕]
		- PRINT [✅]
		- SCAN  [❌]


- ASM Parser [✅]
	- ASM Labels :LABEL [✅]
	- Comment Token ';' [✅]
- Parser [⭕]
   	- Keywords [⭕]
   	  	- asm [✅]
- Floating Point Numbers [❌]
- Memory Management [❌]
	- Allocation [❌]
   	- Deallocation [❌]
- Strings [⭕]
	- storing strings [✅]
		- very basic
	- retrieving strings [✅]
		- very basic
	- strlen [❌]
- Error Reporting [🤷]
