// Single Line Comment

/* 
 * Multi 
 * Line 
 * Comment
 */


/*
 * Supported variable types:
 * 4 byte int
 * 4 byte float
 * string (ASCII 1 byte per character)
 *
 *
 *
 */

// Potential ways to declare and initialize variables
int a := 5;
let a : int := 5;
let a<int> := 5;


float b := 3.2;
string c := "Hello, world!";

// Interates over each element in iterable
for (variable_name in iterable) {
	
}

// Classic for loop
for (int i := 0; i < 10; i := i + 1) {

}

// Classic while loop
while (true) {

}

// if, else if, else construct
if (false) {

} elif (false) {

} elif (true) {

} else {

}

// Multi conditioned if
if[3] (true), (file), (good_format) {
	file := open("some.txt", "r");
	// Condition check can be delayed until inside the body.
	// Condition index starts at 0
	check(1);
	
	for (line in file) {
		good_format = check_format(line);
		check(2); // Condition check can be repeated multiple times
	}

	close(file);

	case 0 {
		// Cases contain unique code and other case blocks are not entered
		// Similar to switch statements except "break" is forced
	}

	case 1 {
		// also similar to anonymous functions since 
		// 	check(1)
		// calls
		// 	case 1 {}	
	}

	case 2 {
		close(file); //Since file exists in the outer scope, we can still access it here.
	}

	// Code that's always executed after the case blocks
}

if (x > 1) {
	int y = get_int();
	x := x - y;
	check(0);

	case 0 {
		// Case 0 is NOT called if first condition fails before entering the if block
		// because it might use variables whose scope is restricted to the if block
		//
		// Case 0 IS called if check(0) is called and fails in the body of the if statement
		// This makes it so you can repeat the initial check at a later point.

		print("{y} is too big\n", y);
		x := x + y;
	}
}

/*
 * Multi Conditioned ifs can be used with elif structure
 *
 */

// Ok
if[2] (bool1) (bool2) {

	case 0 {
	
	}

	case 1 {
	
	}

} elif[3] (bool3) (bool4) (bool5) {

	case 0 {
	
	}

	case 1 {
	
	}

	case 2 {
	
	}

} else {

}









