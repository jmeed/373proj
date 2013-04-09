stackusage 
============================
***Project Abstract***

Project Objective
	Keeping track of an application's stack usage is a 
	difficult, yet important task when developing embedded 
	systems applications. Insufficient stack space can 
	result in rare, difficult-to-debug intermittent
	failures cause by the stack overwriting other data
	while the program is running. This sample project will 
	demonstrate some simple functions that could be an
	aide in keeping track of stack usage.
	
Project Overview
	The approach in this project is to initialize the
	entire stack area with a known constant (0xDEADBEEF).
	By counting the number of 0xDEADBEEF constants, it
	is possible to "guess" how much stack is currently
	in use; moreover, to detect how much stack was used
	during its peak demand.
	
	Note: This code is meant to be only an aide. Stack
	consumption can vary on several aspects, including
	but not limited to: the number of interrupts being
	serviced/generated and/or the number of recursive
	function calls, and the scope of their variables.
	
	StackHeapFree()
		This function returns the number of bytes that
		are currently available in the stack. The stack
		grows downward in memory towards the heap. The
		heap grows upward in memory towards the stack.
		This function subtracts the current top-of-heap
		Pointer value from the current stack pointer
		value and returns the amount of free memory
		between the heap and stack.

	
	StackHeapMinFree()
		This function shows the number of bytes left in the
		stack from the point of peak usage. If this
		function returns a low value, then most of the
		stack has been used at some point in time in
		execution. This function works by counting the
		largest contiguous block of 0xDEADBEEF words
		in the stack/heap memory area. It assumes that
		the largest block of memory is free area, and not
		program data initialized to 0xDEADBEEF, or
		allocated but uninitialized stack or heap space.
		Before using this function, StackHeapInit must
		be called to initialize the stack area with
		0xDEADBEEF.


Program Example
	This application example will simply call the
	callme() function, which calls itself recursively.
	During each iteration, the stack's usage stats
	are printed onto the semi-hosting console.
	Through each iteration, it is possible to view
	the stack usage.

***END OF ABSTRACT***
	
