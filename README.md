# The Armadillo Programming Language

![Project Logo](./img/logo.png)




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
