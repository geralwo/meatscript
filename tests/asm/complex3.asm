asm
{
	JMP :main

	:program
	218 30 0
	JMP :end

	:main
	JMP :program

	:end
	HALT
}