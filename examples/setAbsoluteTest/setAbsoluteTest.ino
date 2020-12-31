#include <StepMot.h>
StepMot myStepMot(2, 3, A2, A1, A0, 48, 10);//создаем объект myStepMot на пинах
//dir_pin=2, step_pin=3, MS1=A2, MS2=A1, MS3=A0, steps_for_rotation = 48, distanse_per_rotation = 10
void setup() {
  Serial.begin(9600);
  myStepMot.Begin();//инициализируем пины подключения мотора
  myStepMot.SetMode(16);//устанавливаем дробление шага 1/16
  myStepMot.SetSpeed(10000);//устанавливаем длительность одного шага 10000 микросекунд
  myStepMot.setAbsolute();//устанавливаем абсолютные координаты
  myStepMot.setPosition(-5);//перемещаемся в точку -5
  myStepMot.setPosition(-5);//еще раз перемещаемся в точку -5
  //однако, двигатель повернется только один раз, так как на момент второго перемещения он уже находится в точке -5 в абсолютной системе координат
}

void loop() {}
