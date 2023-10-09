# The Armadillo Programming Language

![Project Logo](./img/logo.png)



## What exactly is Armadillo?
Armadillo was a program written by Alfredo, David, Nate. Armadillo was built during hack uta 2023 as a 
cool little project to mess around with. Altough the project was incomplete most of the proof of concept
is in the code.


## Valid Armadillo programs
```rust
fn gcd( a: integer, b: integer ) -> integer {
    if[2] ( a = 0 ) ( b = 0 ) {
        //Checks if a = 0
        check 0 {
            return b;
        };
        //Checks if b = 0
        check 1 {
            return a;
        };
    }

    while ( b != 0 ) {
        r := a % b;
        a := b;
        b := y;
    }
    return a;
}

fn main() {
    let result <integer> := gcd( 7, 31 ); 
    shout("%d", result);
}
```



```rust
scope myVars = {
    varOne <Integer> := 12;
    varTwo <String> := "testing";
}

fn main(){
    for i in 0..12 {
        //myVars.varOne gets declared here
        for j in 0..12 {
       
            //Copy and pastes the
            //contents of myVars
            //Into here
            myVars;

            //Get the contents of 
            //the value varTwo
            let myString <String> := myVars.varTwo;
        }
    }
}
```

```rust
fn main() {
    if( test = 12 ){
        //varFoo gets declared in the
        //above scope
        scope varFoo <integer> := 12 1;

        if( pho < 3 ){
            //Lives for 3 more scopes
            outlive bar <real> := 0.1 3;
        }
    }
}
```


## References
Although we did not finish we would have not gotten as far as we did without the following references. Thank you for the authors mentioned below. Finally although not explicit mentioned Dr.Dalio the professor for UT Arlington CSE-4305 (Compilers class) assignments also served as a good reference. 

https://llvm.org/doxygen/index.html

https://llvm.org/docs/tutorial/

https://github.com/lsegal/my_toy_compiler/tree/master

https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl03.html#chapter-3-introduction
