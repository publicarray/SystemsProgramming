pub struct Size {
  pub width: i32,
  pub height: i32,
}

impl Size {
  pub fn new(width: i32, height: i32) -> Size {
    Size { width: width, height: height, }
  }

  pub fn area(&self) -> i32 {
    self.width * self.height
  }
}


// https://stackoverflow.com/questions/21152429/rust-how-to-implement-linked-list
// pub enum List<T> {
//     Node(T, ~List<T>),
//     Nil
// }

// impl<T> List<T> {
//     fn new(vector: &[T]) -> List<T> { Nil }

//     fn add(&mut self, item: T) {
//         match *self {
//             Node(_, ref mut next) => next.add(item),
//             Nil => *self = Node(item, ~Nil)
//         }
//     }
// }

pub struct List {
    pub data: *mut,
    pub length: i32,
    pub bufferSize: i32,
}

impl List {
  pub fn new() -> List {
    List { data: Box::new([0i32; 100]), length: 0,  bufferSize: 100, }
  }

  pub fn add(&self, data:i32) -> i32 {
    self.length = self.length + 1
    self.data[self.length] = self.data
    self.data
  }
  pub fn print(&self) -> () {
    for (i, x) in self.data.enumerate() {
        println!("Iteration {} is value {}", i, x);
    }
  }
}

// pub fn list() {

// }
// // pub const SOME_VALUE = 20;
// pub fn addList(value: i32) {

// }



// void initList(List* self) {
//     self->length = 0;
//     self->bufferLength = 50;
//     self->data = (void* *) malloc(sizeof(void *) * self->bufferLength); // TODO: error checking
// }
// void* addList(List* self, void* ptr, int sizeOfInput) {
//     // automatically grow and shrink the list
//     if (self->length >= self->bufferLength) {
//         self->bufferLength *= 2;
//         self->data = (void* *) realloc(self->data, sizeof(void *) * self->bufferLength); // TODO: error checking
//     }
//     // copy item to the list
//     void* newData = malloc(sizeOfInput);
//     memcpy(newData, ptr, sizeOfInput);
//     self->data[self->length] = newData;
//     self->length++;
//     return newData;
//     // self->data[self->length++] = (void*) malloc (sizeOfInput);
//     // memcpy(self->data[self->length-1], ptr, sizeOfInput);
// }

// void* getList(List* self, int index) {
//     return self->data[index];
// }

// void freeList(List* self) {
//     for(int i=0; i<self->length; i++) {
//         free(self->data[i]);
//     }
//     free(self->data);
// }
