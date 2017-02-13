// https://stackoverflow.com/questions/36716567/is-there-a-rust-equivalent-to-the-clock-function-in-c
extern crate libc;
extern {
    pub fn clock() -> ::libc::clock_t;
}

const SIZE:usize = 512;

// https://stackoverflow.com/questions/13212212/creating-two-dimensional-arrays-in-rust
// https://stackoverflow.com/questions/13241190/passing-two-dimensional-arrays-to-functions-in-rust?rq=1
fn copy_ab(src: &[[i32; SIZE]], dest: &mut [[i32; SIZE]]) {
    for row in 0..SIZE {
        for col in 0..SIZE {
            dest[row][col] = src[row][col];
        }
    }
}

fn copy_ba(src: &[[i32; SIZE]], dest: &mut [[i32; SIZE]]) {
    for row in 0..SIZE {
        for col in 0..SIZE {
            dest[col][row] = src[col][row];
        }
    }
}

fn main() {
    let src = [[0i32; SIZE]; SIZE];
    let mut dest = [[0i32; SIZE]; SIZE];

    println!("SIZE = {}", SIZE);

    let start = unsafe { clock() };
    copy_ab(&src, &mut dest);
    let end = unsafe { clock() };
    let dur = ((end-start) as f32) / 1000.0;
    println!("copyAB - start:{} end:{} duration:{} CPU time used:{} ms", start, end, end-start, dur);

    let start = unsafe { clock() };
    copy_ba(&src, &mut dest);
    let end = unsafe { clock() };
    let dur = ((end-start) as f32) / 1000.0;
    println!("copyBA - start:{} end:{} duration:{} CPU time used:{} ms", start, end, end-start, dur);
}
