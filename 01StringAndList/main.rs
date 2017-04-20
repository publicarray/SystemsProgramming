mod mylist;
use mylist::*;
// use list::{list, addList};

fn main() {
    println!("Hello world");


    // Array
    // Stack
    let mut values = [0f64; 100];
    // Heap
    let mut values2 = Box::new([0f64; 100]);

    // unicode
    let my_str = ":>😇⚡";
    println!("Number of bytes = {}", my_str.len());
    println!("Number of chars = {}", my_str.chars().count());

    // split
    let (part1, part2) = "Hello".split_at(3);
    println!("Part 1 = {}", part1);
    println!("Part 2 = {}", part2);
    //format str
    // let message = format!("The temperature {temp}C is within {percent} of maximum", temp = 104, percent = 99);
    // println!(message.to_string());

    let size = Size::new(10, 20);
    println!("Size = {}", size.area());

    let suff = List::new(10);
    suff.add(13);
    suff.add(34);

}
