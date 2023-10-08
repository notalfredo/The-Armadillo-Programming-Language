# The Armadillo Programming Language

![Project Logo](./img/logo.png)




##Valid Armadillo programs
```rust
fn gcd( a: integer, b: integer ) -> integer {
    if[2] ( a = 0 ) ( b = 0 ) {
        check 0 {
            return b;
        };
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
