// https://stackoverflow.com/questions/36716567/is-there-a-rust-equivalent-to-the-clock-function-in-c
extern crate libc;
extern {
    pub fn clock() -> ::libc::clock_t;
}

const SIZE:usize = 512;

fn main() {
    println!("SIZE = {}", SIZE);

    let src: [[i32; SIZE]; SIZE] = [[0; SIZE]; SIZE];
    let mut dest = [[0i32; SIZE]; SIZE];

    let start = unsafe { clock() };
    copy_ab(&src, &mut dest);
    let end = unsafe { clock() };
    println!("AB - start:{} end:{} duration:{}", start, end, end - start);
    let start = unsafe { clock() };
    copy_ba(&src, &mut dest);
    let end = unsafe { clock() };
    println!("BA - start:{} end:{} duration:{}", start, end, end - start);

}

// https://stackoverflow.com/questions/13212212/creating-two-dimensional-arrays-in-rust
// https://stackoverflow.com/questions/13241190/passing-two-dimensional-arrays-to-functions-in-rust?rq=1
fn copy_ab(src: &[[i32; SIZE]], dest: &mut [[i32; SIZE]]) {
    for row in 0..SIZE {
        for col in 0..SIZE {
            // println!("src[{}][{}] = {}", row, col, src[row][col]);
            dest[row][col] = src[row][col];
        }
    }
}

fn copy_ba(src: &[[i32; SIZE]; SIZE], dest: &mut [[i32; SIZE]; SIZE]) {
    for row in 0..SIZE {
        for col in 0..SIZE {
            // println!("src[{}][{}] = {}", col, row, src[col][row]);
            dest[col][row] = src[col][row];
        }
    }
}
