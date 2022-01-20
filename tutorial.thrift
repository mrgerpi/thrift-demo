namespace cpp tutorial
namespace d tutorial
namespace java tutorial
namespace php tutorial
namespace perl tutorial
namespace haxe tutorial

enum Operation {
  ADD = 1,
  SUBTRACT = 2,
  MULTIPLY = 3,
  DIVIDE = 4
}

struct Work {
  1: i32 num1 = 0,
  2: i32 num2,
  3: Operation op,
  4: optional string comment,
}

service Calculator {
   void ping(),

   i32 add(1:i32 num1, 2:i32 num2),

   i32 calculate(1:i32 logid, 2:Work w) ,
}
