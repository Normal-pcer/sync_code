use rand::Rng;
use std::{cmp::Ordering, io};


fn main() {
    let mut line = String::new();
    io::stdin().read_line(&mut line).expect("Cannot read line.");

    let mut s = line.trim().split(' ');
    let a: i32 = s
        .next()
        .expect("Cannot get item.")
        .parse()
        .expect("Cannot get number.");
    let b: i32 = s
        .next()
        .expect("Cannot get item.")
        .parse()
        .expect("Cannot get number.");

    let rand_num = rand::thread_rng().gen_range(a ..= b);
    println!("rand_num = {rand_num}");

    match rand_num.cmp(&5) {
        Ordering::Equal => println!("Equal"),
        Ordering::Less => println!("Less"),
        Ordering::Greater => println!("Greater")
    }
}
