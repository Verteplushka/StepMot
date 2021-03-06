#pragma once
#include <Arduino.h>
class StepMot //объявление класса StepMot
{
  public:
    StepMot(byte dir_pin, byte step_pin, byte MS1, byte MS2, byte MS3, float steps_for_rotation = 200, float distanse_per_rotation = 0)
    {
      _MS1 = MS1;//объявление пинов
      _MS2 = MS2;
      _MS3 = MS3;
      _dir_pin = dir_pin;
      _step_pin = step_pin;
      _steps_for_rotation = steps_for_rotation;
      _distanse_per_rotation = distanse_per_rotation;
    }
    void Begin();//функция, отвечающая за инициализацию пинов
    void SetMode(byte mode = 1);//функция, отвечающая за установку дробления шага (1-полный шаг, 2-1/2 шага, 4-1/4 шага, 8-1/8 шага, 16-1/16 шага)
    void SetSpeed(unsigned long sped);//функция, отвечающая за установку скорости вращения двигателя
    //в скобках указывается длительность одного шага в миллисекундах; независимо от выбранного дробления шага скорость вращения двигателя будет одинакова
    //однако, установка слишком малой длительности шага может привести к прекращению работы двигателя, так как он не будет успевать вращаться
    //если двигатель не вращается, возможно, стоит увеличить длительность шага
    void rotationByAngel(int angel);//функция, отвечающая за поворот двигателя на определенный угол
    //угол, значение которого больше 0, будет отложен по часовой стрелке, а угол, значение которого меньше 0 - против часовой стрелки
    void setPosition(float x, float distance = 0);//функция, отвечающая за линейное перемещение двигателя
    //в скобках указывается значение координаты, в которую должен переместиться двигатель
    void setAbsolute();//функция, отвечающая за уставновку абсолютных координат
    void setLocal();//функция, отвечающая за уставновку относительных координат
  private://инициализация переменных
    byte _dir_pin;
    byte _step_pin;
    byte _MS1;
    byte _MS2;
    byte _MS3;
    float _steps_for_rotation;//указание количества шагов двигателя для совершения одного оборота (по умолчанию - 200)
    float _distanse_per_rotation;//указание количества единиц длины, которые шаговый двигатель проходит за один оборот (по умолчанию - 0)
    //выбранная единица измерения длины должна соответствовать единице измерения, использующейся при указании линейного перемещения
    unsigned long _speed = 1000;
    byte _mode;
    float _x = 0;//абсолютная координата двигателя
    bool flag = true;
};
void StepMot::Begin()//инициализация портов
{
  pinMode(_dir_pin, OUTPUT);
  pinMode(_step_pin, OUTPUT);
  pinMode(_MS1, OUTPUT);
  pinMode(_MS2, OUTPUT);
  pinMode(_MS3, OUTPUT);
}
void StepMot::SetSpeed(unsigned long sped) {
  _speed = sped / 2;//длительность шага делится пополам, так как для каждого шага устанавливаются две задержки (см. функции setPosition и rotationByAngel)
}
void StepMot::setPosition(float x, float distance = 0) //x в мм
{
  if (flag)//если задана абсолютная система координат
  {
    distance = x - _x;//перемещение будет равно разности координаты, в которую нужно переместиться и координаты, в которой сейчас находится двигатель
    _x = x;//запоминаем новую координату
  }
  else
  {
    distance = x;//перемещение будет равно координате, в которую нужно переместиться
    _x += x;//суммируем абсолютную координату с координатой, в которую нужно переместиться и запоминаем эту сумму
  }
  if (distance >= 0)//если перемещение положительно
  {
    digitalWrite(_dir_pin, 1);//установка вращения двигателя по часовой стрелке
    for (int i = 0; i <  (distance / _distanse_per_rotation)*_steps_for_rotation * _mode; i++)//перевод перемещения в шаги
    {
      digitalWrite(_step_pin, 1);//эти четыре строчки отвечают за прохождение двигателем одного шага
      delayMicroseconds(_speed / _mode);
      digitalWrite(_step_pin, 0);
      delayMicroseconds(_speed / _mode);
    }
  }
  if (distance < 0)//если перемещение потрицательно
  {
    digitalWrite(_dir_pin, 0);//установка вращения двигателя против часовой стрелки
    for (int i = 0; i >  (distance / _distanse_per_rotation)*_steps_for_rotation * _mode; i--)//перевод перемещения в шаги
    {
      digitalWrite(_step_pin, 1);//эти четыре строчки отвечают за прохождение двигателем одного шага
      delayMicroseconds(_speed / _mode);
      digitalWrite(_step_pin, 0);
      delayMicroseconds(_speed / _mode);
    }
  }

}
void StepMot::rotationByAngel(int angel)
{
  if (angel >= 0)//по часовой, если значение угла положительно
  {
    digitalWrite(_dir_pin, 1);//установка вращения двигателя по часовой стрелке
    for (int i = 0; i < _steps_for_rotation * angel * _mode / 360; i++)//перевод угла в колличество шагов
    {
      digitalWrite(_step_pin, 1);//эти четыре строчки отвечают за прохождение двигателем одного шага
      delayMicroseconds(_speed / _mode) ;
      digitalWrite(_step_pin, 0);
      delayMicroseconds(_speed / _mode) ;
    }
  }
  else//против часовой
  {
    digitalWrite(_dir_pin, 0);//установка вращения двигателя против часовой стрелки
    for (int i = 0; i > _steps_for_rotation * angel * _mode / 360; i--)//перевод угла в колличество шагов
    {
      digitalWrite(_step_pin, 1);//эти четыре строчки отвечают за прохождение двигателем одного шага
      delayMicroseconds(_speed / _mode) ;
      digitalWrite(_step_pin, 0);
      delayMicroseconds(_speed / _mode) ;
    }
  }
}
void StepMot::setAbsolute()//в переменную flag записывается выбранный тип координат (абсолютные или относительные)
{
  flag = true;
}
void StepMot::setLocal()
{
  flag = false;
}
void StepMot::SetMode(byte mode = 1)//в переменную _mode записывается выбранный тип дробления шага
{
  if (mode == 1)
  {
    _mode = 1;
    digitalWrite(_MS1, 0);//эти три строчки устанавливают выбранный тип дробления шага
    digitalWrite(_MS2, 0);
    digitalWrite(_MS3, 0);
  }
  if (mode == 2)
  {
    _mode = 2;
    digitalWrite(_MS1, 1);//эти три строчки устанавливают выбранный тип дробления шага
    digitalWrite(_MS2, 0);
    digitalWrite(_MS3, 0);
  }
  if (mode == 4)
  {
    _mode = 4;
    digitalWrite(_MS1, 0);//эти три строчки устанавливают выбранный тип дробления шага
    digitalWrite(_MS2, 1);
    digitalWrite(_MS3, 1);
  }
  if (mode == 8)
  {
    _mode = 8;
    digitalWrite(_MS1, 1);//эти три строчки устанавливают выбранный тип дробления шага
    digitalWrite(_MS2, 1);
    digitalWrite(_MS3, 0);
  }
  if (mode == 16)
  {
    _mode = 16;
    digitalWrite(_MS1, 1);//эти три строчки устанавливают выбранный тип дробления шага
    digitalWrite(_MS2, 1);
    digitalWrite(_MS3, 1);
  }
}
