#include <string>
#include "serialPort/SerialPort.h"
#include <csignal>
#include <iostream>
#include <fstream>
using namespace std;
void menshow();                        // 菜单
void read_position0(int ID);           // 读取左腿位置
void read_position1();                 // 读取右腿位置
int motor_vel_USB0(int ID, float vel); // 左腿速度控制
int motor_vel_USB1(int ID, float vel); // 右腿速度控制
int motor_stop_USB0(int ID);           // 左电机停止
int motor_stop_USB1(int ID);           // 右电机停止
float motor_pos0(int ID, float pos);   // 左边位置控制
float motor_pos1(int ID, float pos);   // 右边位置控制
// double arr[6] = {3.03115, 1.69812, 3.11782, 2.41564, 6.23871, 6.12328}; // 机器人初始位置时电机的位置参数
double arr[6] = {1.31, 2.28, 2.55, 2.61, 2.92, 3.027}; // 机器人初始位置时电机的位置参数
// ofstream outfile("data4.xls");//将输出保存到excel表

int main()
{
    int select = 0;
    float posi = 0.0;
    while (1)
    {
        menshow();
        cin >> select;
        switch (select)
        {
        case 1: //  读取位置
            read_position0(0);
            read_position0(1);
            read_position0(2);
            read_position1();
            break;

        case 2: // 位置控制
        {
            usleep(1000000);
            for (int i = 0; i < 5; i++)
            {
                if (i % 2 == 0)
                {
                    for (int j = 0; j < 80; j++)
                    {
                        arr[0] = arr[0] + 0.02  * 3.141 * 9.1;
                        arr[1] = arr[1] + 0.005  * 3.141 * 9.1;
                        arr[2] = arr[2] + 0.02  * 3.141 * 9.1;
                        arr[3] = arr[3] - 0.005  * 3.141 * 9.1;
                        arr[4] = arr[4] - 0.02  * 3.141 * 9.1;
                        arr[5] = arr[5] - 0.005 * 3.141 * 9.1;
                        motor_pos0(0, arr[0]);
                        motor_pos0(1, arr[1]);
                        motor_pos0(2, arr[2]);
                        motor_pos1(0, arr[3]);
                        motor_pos1(1, arr[4]);
                        motor_pos1(2, arr[5]);
                        usleep(20000);
                    }
                }
                else
                {
                    for (int k = 0; k < 80; k++)
                    {
                        arr[0] = arr[0] + 0.005 * 3.141 * 9.1;
                        arr[1] = arr[1] + 0.02 * 3.141 * 9.1;
                        arr[2] = arr[2] + 0.005 * 3.141 * 9.1;
                        arr[3] = arr[3] - 0.02 * 3.141 * 9.1;
                        arr[4] = arr[4] - 0.005 * 3.141 * 9.1;
                        arr[5] = arr[5] - 0.02 * 3.141 * 9.1;
                        motor_pos0(0, arr[0]);
                        motor_pos0(1, arr[1]);
                        motor_pos0(2, arr[2]);
                        motor_pos1(0, arr[3]);
                        motor_pos1(1, arr[4]);
                        motor_pos1(2, arr[5]);
                        usleep(20000);
                    }
                }
            }

            break;
        }
        case 3: // 速度控制
        {
            motor_vel_USB0(0, -1.0);
            motor_vel_USB0(1, -1.0);
            motor_vel_USB0(2, -1.0);
            motor_vel_USB1(0, 1.0);
            motor_vel_USB1(1, 1.0);
            motor_vel_USB1(2, 1.0);
            usleep(10000000);
            motor_stop_USB0(0);
            motor_stop_USB0(1);
            motor_stop_USB0(2);
            motor_stop_USB1(0);
            motor_stop_USB1(1);
            motor_stop_USB1(2);
            break;
        }
        case 4: // 电机步进
        {
            usleep(1000000);
            for (int j = 0; j < 80; j++)
            {
                arr[0] = arr[0] + 0.005 * j * 3.141 * 9.1;
                arr[1] = arr[1] + 0.005 * j * 3.141 * 9.1;
                arr[2] = arr[2] + 0.005 * j * 3.141 * 9.1;
                arr[3] = arr[3] - 0.005 * j * 3.141 * 9.1;
                arr[4] = arr[4] - 0.005 * j * 3.141 * 9.1;
                arr[5] = arr[5] - 0.005 * j * 3.141 * 9.1;
                posi = motor_pos0(0, arr[0]);
                motor_pos0(1, arr[1]);
                motor_pos0(2, arr[2]);
                motor_pos1(0, arr[3]);
                motor_pos1(1, arr[4]);
                motor_pos1(2, arr[5]);
                usleep(20000);
                cout << posi << endl;
                printf("按  Enter 键继续\n");
                getchar();
            }
            break;
        }
        default:

            return 0;
        }
    }
    return 0;
}

float motor_pos0(int ID, float pos)
{
    // set the serial port name
    SerialPort serial("/dev/ttyUSB0");

    // send message struct
    MOTOR_send motor_run, motor_stop;
    // receive message struct
    MOTOR_recv motor_r;

    // set the id of motor
    motor_run.id = ID;
    // set the motor type, A1Go1 or B1
    motor_run.motorType = MotorType::A1Go1;
    motor_run.mode = 10;
    motor_run.T = 0.00;
    motor_run.W = 0.0;
    motor_run.K_P = 0.05;
    motor_run.K_W = 10;
    motor_run.Pos = pos;
    modify_data(&motor_run);
    serial.sendRecv(&motor_run, &motor_r);
    // decode data from motor states
    extract_data(&motor_r);
    // std::cout << "***********" << std::endl;
    // std::cout << "位置Pos: " << motor_r.Pos << std::endl;
    // outfile << motor_r.Pos << '\t' << motor_r.T << '\t' << motor_r.W << endl;
    //  std::cout << "力矩Torque: " << motor_r.T << std::endl;
    //  std::cout << "速度velocity: " << motor_r.W << std::endl;

    return motor_r.Pos;
}
float motor_pos1(int ID, float pos)
{
    // set the serial port name
    SerialPort serial("/dev/ttyUSB1");

    // send message struct
    MOTOR_send motor_run, motor_stop;
    // receive message struct
    MOTOR_recv motor_r;

    // set the id of motor
    motor_run.id = ID;
    // set the motor type, A1Go1 or B1
    motor_run.motorType = MotorType::A1Go1;
    motor_run.mode = 10;
    motor_run.T = 0.00;
    motor_run.W = 0.0;
    motor_run.K_P = 0.05;
    motor_run.K_W = 10;
    motor_run.Pos = pos;
    modify_data(&motor_run);
    serial.sendRecv(&motor_run, &motor_r);
    // decode data from motor states
    extract_data(&motor_r);
    // std::cout << "***********" << std::endl;
    // std::cout << "位置Pos: " << motor_r.Pos << std::endl;
    //  outfile << motor_r.Pos << '\t' << motor_r.T << '\t' << motor_r.W << endl;
    // std::cout << "力矩Torque: " << motor_r.T << std::endl;
    // std::cout << "速度velocity: " << motor_r.W << std::endl;

    return motor_r.Pos;
}

void menshow()
{
    cout << "\n"
         << endl;
    cout << "***   机器人系统   ***" << endl;
    cout << "***  1.读取当前位置   ***" << endl;
    cout << "***  2.位置控制       ***" << endl;
    cout << "***  3.速度控制       ***" << endl;
    cout << "***  4.步进控制      ***" << endl;
}
// 1、读取当前6个电机位置
void read_position0(int ID)
{
    SerialPort serial("/dev/ttyUSB0");
    // send message struct
    MOTOR_send motor_run;
    // receive message struct
    MOTOR_recv motor_r;
    // set the id of motor
    // int ID = 2;
    motor_run.id = ID;
    // set the motor type, A1Go1 or B1
    motor_run.motorType = MotorType::A1Go1;
    motor_run.mode = 0;
    motor_run.T = 0.0;
    motor_run.W = 0.0;
    motor_run.Pos = 0.0;
    motor_run.K_P = 0.0;
    motor_run.K_W = 0.0;
    modify_data(&motor_run);
    // 发送并接受反馈
    motor_r.motorType = motor_run.motorType;
    serial.sendRecv(&motor_run, &motor_r);
    // decode data from motor states
    extract_data(&motor_r);
    cout << "左:  第" << ID << " 条腿的位置:" << motor_r.Pos << endl;
}

void read_position1()
{
    SerialPort serial("/dev/ttyUSB1");
    // send message struct
    MOTOR_send motor_run;
    // receive message struct
    MOTOR_recv motor_r;
    for (int ID = 0; ID < 3; ID++)
    {
        // set the id of motor
        motor_run.id = ID;
        // set the motor type, A1Go1 or B1
        motor_run.motorType = MotorType::A1Go1;
        motor_run.mode = 0;
        motor_run.T = 0.0;
        motor_run.W = 0.0;
        motor_run.Pos = 0.0;
        motor_run.K_P = 0.0;
        motor_run.K_W = 0.0;
        modify_data(&motor_run);
        // 发送并接受反馈
        motor_r.motorType = motor_run.motorType;
        serial.sendRecv(&motor_run, &motor_r);
        // decode data from motor states
        extract_data(&motor_r);
        cout << "右:   第 " << ID << "   条腿位置  " << motor_r.Pos << endl;
    }
}
int motor_vel_USB0(int ID, float vel)
{
    SerialPort serial("/dev/ttyUSB0");
    // send message struct
    MOTOR_send motor_run, motor_stop;
    // receive message struct
    MOTOR_recv motor_r;

    // set the id of motor
    motor_run.id = ID;
    // set the motor type, A1Go1 or B1
    motor_run.motorType = MotorType::A1Go1;
    motor_run.mode = 10;
    motor_run.T = 0.0;
    motor_run.W = vel * 9.1;
    motor_run.Pos = 0.0;
    motor_run.K_P = 0.0;
    motor_run.K_W = 25.0;
    modify_data(&motor_run);
    // 发送并接受反馈
    motor_r.motorType = motor_run.motorType;
    serial.sendRecv(&motor_run, &motor_r);
    // decode data from motor states
    // extract_data(&motor_r);
    return 0;
}

int motor_vel_USB1(int ID, float vel)
{
    SerialPort serial("/dev/ttyUSB1");
    // send message struct
    MOTOR_send motor_run, motor_stop;
    // receive message struct
    MOTOR_recv motor_r;

    // set the id of motor
    motor_run.id = ID;
    // set the motor type, A1Go1 or B1
    motor_run.motorType = MotorType::A1Go1;
    motor_run.mode = 10;
    motor_run.T = 0.0;
    motor_run.W = vel * 9.1;
    motor_run.Pos = 0.0;
    motor_run.K_P = 0.0;
    motor_run.K_W = 25.0;
    modify_data(&motor_run);
    // 发送并接受反馈
    motor_r.motorType = motor_run.motorType;
    serial.sendRecv(&motor_run, &motor_r);
    return 0;
    // decode data from motor states
    // extract_data(&motor_r);
}
int motor_stop_USB0(int ID)
{
    SerialPort serial("/dev/ttyUSB0");
    // send message struct
    MOTOR_send motor_run, motor_stop;
    // receive message struct
    MOTOR_recv motor_r;

    // set the id of motor
    motor_run.id = ID;
    // set the motor type, A1Go1 or B1
    motor_run.motorType = MotorType::A1Go1;
    motor_run.mode = 0;
    motor_run.T = 0.0;
    motor_run.W = 0;
    motor_run.Pos = 0.0;
    motor_run.K_P = 0.0;
    motor_run.K_W = 0;
    modify_data(&motor_run);
    // 发送并接受反馈
    motor_r.motorType = motor_run.motorType;
    serial.sendRecv(&motor_run, &motor_r);
    // decode data from motor states
    // extract_data(&motor_r);
    return 0;
}
int motor_stop_USB1(int ID)
{
    SerialPort serial("/dev/ttyUSB1");
    // send message struct
    MOTOR_send motor_run, motor_stop;
    // receive message struct
    MOTOR_recv motor_r;

    // set the id of motor
    motor_run.id = ID;
    // set the motor type, A1Go1 or B1
    motor_run.motorType = MotorType::A1Go1;
    motor_run.mode = 0;
    motor_run.T = 0.0;
    motor_run.W = 0;
    motor_run.Pos = 0.0;
    motor_run.K_P = 0.0;
    motor_run.K_W = 0;
    modify_data(&motor_run);
    // 发送并接受反馈
    motor_r.motorType = motor_run.motorType;
    serial.sendRecv(&motor_run, &motor_r);
    return 0;
    // decode data from motor states
    // extract_data(&motor_r);
}
