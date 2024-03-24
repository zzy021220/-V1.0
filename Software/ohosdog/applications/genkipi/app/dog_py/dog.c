#include "dog.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"

#include "pca9685.h"

#include "hi_nvm.h"
#include <hi_nv.h>
#include <hi_flash.h>
#include <hi_partition_table.h>

#include "cJSON.h"
#include "kinematics.h"

// left front leg
#define LF_A    4
#define LF_B    5
// left back leg
#define LB_A    10
#define LB_B    11
// right front leg
#define RF_A    2
#define RF_B    3
// right back leg
#define RB_A    13
#define RB_B    12


#define OFFSET_LF_A     0
#define OFFSET_LF_B     0
#define OFFSET_LB_A     0
#define OFFSET_LB_B     0

#define OFFSET_RF_A     0
#define OFFSET_RF_B     0
#define OFFSET_RB_A     0
#define OFFSET_RB_B     0

int home_rf_a = 165;//右前手臂
int home_rf_b = 60;//右前腿
int home_lf_a = 60;//左前
int home_lf_b = 165;//左前
int home_lb_a = 100;//左后
int home_lb_b = 15;//左后
int home_rb_a = 100;//右后腿
int home_rb_b = 15;//右后手臂

static int port_lf_a = LF_A;        //左前手臂
static int port_lf_b = LF_B;        //左前腿
static int port_lb_a = LB_A;        //左后手臂
static int port_lb_b = LB_B;        //左后腿
static int port_rf_a = RF_A;        //右前手臂
static int port_rf_b = RF_B;        //右前腿
static int port_rb_a = RB_A;        //右后手臂
static int port_rb_b = RB_B;        //右后腿

static int offset_lf_a = OFFSET_LF_A;
static int offset_lf_b = OFFSET_LF_B;
static int offset_lb_a = OFFSET_LB_A;
static int offset_lb_b = OFFSET_LB_B;
static int offset_rf_a = OFFSET_RF_A;
static int offset_rf_b = OFFSET_RF_B;
static int offset_rb_a = OFFSET_RB_A;
static int offset_rb_b = OFFSET_RB_B;


#define BASE_DELAY 10
static int current_angle[8] = {0, 0, 0, 0, 0, 0, 0};

void dog_init() {
    pca9685_servo_reset();
    pca9685_servo_init();
}

void dog_setRealAngle(int joint, int angle) {
    if (joint < 0 || joint > 15) return;
    pca9685_servo_set_angle(joint, angle);
}

void dog_setLeftFrontAngle(int alpha, int beta) {
    dog_setRealAngle(port_lf_a, offset_lf_a + alpha);
    dog_setRealAngle(port_lf_b, offset_lf_b + beta);
    current_angle[0] = alpha;
    current_angle[1] = beta;
}

void dog_setLeftBackAngle(int alpha, int beta) {
    dog_setRealAngle(port_lb_a, offset_lb_a + alpha);
    dog_setRealAngle(port_lb_b, offset_lb_b + beta);
    current_angle[2] = alpha;
    current_angle[3] = beta;
}

void dog_setRightFrontAngle(int alpha, int beta) {
    dog_setRealAngle(port_rf_a, 180 - (offset_rf_a + alpha));
    dog_setRealAngle(port_rf_b, 180 - (offset_rf_b + beta));
    current_angle[4] = alpha;
    current_angle[5] = beta;
}

void dog_setRightBackAngle(int alpha, int beta) {
    dog_setRealAngle(port_rb_a, 180 - (offset_rb_a + alpha));
    dog_setRealAngle(port_rb_b, 180 - (offset_rb_b + beta));
    current_angle[6] = alpha;
    current_angle[7] = beta;
}


static int map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// 趴下
void sithome(){
    dog_setLeftFrontAngle(70, 135);//（5，4）
    dog_setLeftBackAngle(110, 45);//（7,6）
    dog_setRightFrontAngle(70, 135);//（3.2）
    dog_setRightBackAngle(110, 45);//(9,8)
}
//站立
void standhome(){
    dog_setLeftFrontAngle(15, 100);//（手臂，腿）
    dog_setLeftBackAngle(165, 60);//（手臂，腿）
    dog_setRightFrontAngle(15, 120);//(手臂，腿)
    dog_setRightBackAngle(165, 80);//(手臂，腿)
}

//初始化动作
void initial()
{
    sithome();
    usleep(300* 1000);
    dog_setRightFrontAngle(70,175);
    usleep(300* 1000);
    dog_setRightFrontAngle(70,135);
    usleep(300* 1000);
    dog_setRightBackAngle(110,85);
    usleep(300* 1000);
    dog_setRightBackAngle(110,45);
    usleep(300* 1000);
    dog_setLeftBackAngle(110,5);
    usleep(300* 1000);
    dog_setLeftBackAngle(110,45);
    usleep(300* 1000);
    dog_setLeftFrontAngle(70,95);
    usleep(300* 1000);
    dog_setLeftFrontAngle(70,135);
    usleep(300* 1000);
    standhome();
    usleep(300* 1000);
}

//初始化动作
// void initial(){
//   sithome();
//   usleep(300* 1000);//300ms延时
//   pca9685_servo_set_angle(port_rf_b, 95);
//   usleep(300* 1000);//300ms延时
//   pca9685_servo_set_angle(port_rf_b, home_rf_b);
//   usleep(300* 1000);//300ms延时
//   pca9685_servo_set_angle(port_rb_b, 10);
//   usleep(300* 1000);//300ms延时
//   pca9685_servo_set_angle(port_rb_b, home_rb_a);
//   usleep(300* 1000);//300ms延时
//   pca9685_servo_set_angle(port_lb_b, 10);
//   usleep(300* 1000);//300ms延时
//   pca9685_servo_set_angle(port_lb_b, home_lf_a);
//   usleep(300* 1000);//300ms延时
//   pca9685_servo_set_angle(port_lf_b, 170);
//   usleep(300* 1000);//300ms延时
//   pca9685_servo_set_angle(port_lf_b, home_lb_a);
//   usleep(300* 1000);//300ms延时

//   pca9685_servo_set_angle(port_rf_a, 165);
//   pca9685_servo_set_angle(port_lf_a, 15);
//   pca9685_servo_set_angle(port_lb_a, 165);
//   pca9685_servo_set_angle(port_rb_a, 15);
// }

void stand1(void) {
    dog_setLeftFrontAngle(15, 100);//（7,6）
    dog_setLeftBackAngle(165, 60);//（5，4）
    dog_setRightFrontAngle(165, 70);//(9,8)
    dog_setRightBackAngle(15, 120);//（3.2）
    usleep(1000* 1000);//一秒延时
    sithome();
    usleep(1000* 1000);//一秒延时
    initial();
    usleep(1000* 1000);//一秒延时
    forward(5);
    usleep(1000* 1000);//一秒延时
    backward(5);
    usleep(1000* 1000);//一秒延时
    rightturn(5);
    usleep(1000* 1000);//一秒延时
    leftturn(5);
    usleep(1000* 1000);//一秒延时
    twist();
    usleep(1000* 1000);//一秒延时
}

// 前进
void forward(unsigned int step){
  while (step-- > 0){
  pca9685_servo_set_angle(port_rf_a, home_rf_b+30);
  pca9685_servo_set_angle(port_lf_a, home_lb_b+30);
  usleep(200* 1000);//100ms延时
  pca9685_servo_set_angle(port_rf_b, home_rf_a+30);
  pca9685_servo_set_angle(port_rb_b, home_rb_a-30);
  pca9685_servo_set_angle(port_lb_b, home_lf_a);
  pca9685_servo_set_angle(port_lf_b, home_lb_a);
  usleep(200* 1000);//100ms延时
  pca9685_servo_set_angle(port_rf_a, home_rf_b);
  pca9685_servo_set_angle(port_lf_a, home_lb_b);
  usleep(200* 1000);//100ms延时
  
  pca9685_servo_set_angle(port_lb_a, home_lf_b-30);
  pca9685_servo_set_angle(port_rb_a, home_rb_b-30);
  pca9685_servo_set_angle(port_rf_b, home_rf_a);
  pca9685_servo_set_angle(port_rb_b, home_rb_a);
   usleep(200* 1000);//100ms延时
  pca9685_servo_set_angle(port_lb_b, home_lf_a-30);
  pca9685_servo_set_angle(port_lf_b, home_lb_a+30);
  usleep(200* 1000);//100ms延时

  pca9685_servo_set_angle(port_lb_a, home_lf_b);
  pca9685_servo_set_angle(port_rb_a, home_rb_b);
  usleep(200* 1000);//100ms延时
  }
}

// 動作2
void stand2(){
  sithome();
  pca9685_servo_set_angle(port_rf_b, 175);
  pca9685_servo_set_angle(port_lb_b, 5);
  pca9685_servo_set_angle(port_lf_b, 175);
  pca9685_servo_set_angle(port_rb_b, 5);
   usleep(600* 1000);//600ms延时
    
  pca9685_servo_set_angle(port_rf_b, home_rf_a);
  pca9685_servo_set_angle(port_lb_b, home_lf_a);
  pca9685_servo_set_angle(port_lf_b, home_lb_a);
  pca9685_servo_set_angle(port_rb_b, home_rb_a);
  usleep(600* 1000);//600ms延时
  
     pca9685_servo_set_angle(port_rf_a, home_rf_b);
  pca9685_servo_set_angle(port_lf_a, home_lb_b);
  pca9685_servo_set_angle(port_lb_a, home_lf_b);
  pca9685_servo_set_angle(port_rb_a, home_rb_b);
}

// 伸懒腰
void stand3()
{ 
  sithome();
  int i;
  int j = 90;
  int k = 90;
  pca9685_servo_set_angle(port_rf_b, home_rf_a);
  pca9685_servo_set_angle(port_lb_b, home_lf_a);
  pca9685_servo_set_angle(port_lf_b, home_lb_a);
  pca9685_servo_set_angle(port_rb_b, home_rb_a);
  for(i = 90; i < 165; i++)
  {
    pca9685_servo_set_angle(port_lb_a, i);
    j = j - 1;
    pca9685_servo_set_angle(port_rf_a, j);
     usleep(20* 1000);//20ms延时
  }
  
  for(i = 115; i < 165; i++)
  {
      pca9685_servo_set_angle(port_rb_a, i);
    k = k - 1;
      pca9685_servo_set_angle(port_lf_a, k);
     usleep(20* 1000);//20ms延时
  }
}

// 跳跃
void downaction(unsigned int step){
  while (step-- > 0){
  sithome();
   usleep(100* 1000);//100ms延时
  standhome();
  usleep(100* 1000);//100ms延时
  }
}

// 握手
void wink(unsigned int step){
  standhome();
    pca9685_servo_set_angle(port_lb_b, home_lf_a -30);
  while (step-- > 0){
  pca9685_servo_set_angle(port_lb_b, 30);
     pca9685_servo_set_angle(port_rb_a, home_rb_b- 100);
   usleep(300* 1000);//300ms延时
     pca9685_servo_set_angle(port_rb_a, home_rb_b+ 5);
   usleep(300* 1000);//300ms延时
  }
}

// 扭身子
void twist(){
   pca9685_servo_set_angle(port_rf_a, home_rf_b);
  pca9685_servo_set_angle(port_lf_a, home_lb_b);
  pca9685_servo_set_angle(port_lb_a, home_lf_b);
  pca9685_servo_set_angle(port_rb_a, home_rb_b);
  
  for(int right=90;right<170;right++){
  pca9685_servo_set_angle(port_rf_b, right);
  pca9685_servo_set_angle(port_lf_b, right);
  pca9685_servo_set_angle(port_lb_b, right-90);
  pca9685_servo_set_angle(port_rb_b, right-90);
    usleep(10* 1000);//100ms延时
  }

   for(int right=170;right>90;right--){
  pca9685_servo_set_angle(port_rf_b, right);
  pca9685_servo_set_angle(port_lf_b, right);
  pca9685_servo_set_angle(port_lb_b, right-90);
  pca9685_servo_set_angle(port_rb_b, right-90);
     usleep(10* 1000);//100ms延时
  }

}

// 退后
void backward(unsigned int step){
  while (step-- > 0){
  pca9685_servo_set_angle(port_rf_a, home_rf_b+30);
  pca9685_servo_set_angle(port_lf_a, home_lb_b+30);
  usleep(100* 1000);//100ms延时
  pca9685_servo_set_angle(port_rf_b, home_rf_a);
  pca9685_servo_set_angle(port_rb_b, home_rb_a);
  pca9685_servo_set_angle(port_lb_b, home_lf_a-30);
  pca9685_servo_set_angle(port_lf_b, home_lb_a+30);
  usleep(100* 1000);//100ms延时
  pca9685_servo_set_angle(port_rf_a, home_rf_b);
  pca9685_servo_set_angle(port_lf_a, home_lb_b);
  usleep(100* 1000);//100ms延时
  
  pca9685_servo_set_angle(port_lb_a, home_lf_b-30);
  pca9685_servo_set_angle(port_rb_a, home_rb_b-30);
  usleep(100* 1000);//100ms延时
  pca9685_servo_set_angle(port_rf_b, home_rf_a+30);
  pca9685_servo_set_angle(port_rb_b, home_rb_a-30);
  pca9685_servo_set_angle(port_lb_b, home_lf_a);
  pca9685_servo_set_angle(port_lf_b, home_lb_a);
  usleep(100* 1000);//100ms延时
  pca9685_servo_set_angle(port_lb_a, home_lf_b);
  pca9685_servo_set_angle(port_rb_a, home_rb_b);
  usleep(100* 1000);//100ms延时
  }
}

// 右移
void rightturn(unsigned int step){
  while (step-- > 0){
  pca9685_servo_set_angle(port_lb_a, home_lf_b-30);
  pca9685_servo_set_angle(port_rb_a, home_rb_b-30);
  usleep(100* 1000);//100ms延时
  pca9685_servo_set_angle(port_rf_b, home_rf_a+30);
  pca9685_servo_set_angle(port_rb_b, home_rb_a-30);
   pca9685_servo_set_angle(port_lb_b, home_lf_a-30);
  pca9685_servo_set_angle(port_lf_b, home_lb_a+30);
   usleep(100* 1000);//100ms延时
   pca9685_servo_set_angle(port_lb_a, home_lf_b);
  pca9685_servo_set_angle(port_rb_a, home_rb_b);
   usleep(100* 1000);//100ms延时
   pca9685_servo_set_angle(port_rf_a, home_rf_b+30);
  pca9685_servo_set_angle(port_lf_a, home_lb_b+30); 
  usleep(100* 1000);//100ms延时
  pca9685_servo_set_angle(port_rf_b, home_rf_a);
  pca9685_servo_set_angle(port_rb_b, home_rb_a);
  pca9685_servo_set_angle(port_lb_b, home_lf_a);
  pca9685_servo_set_angle(port_lf_b, home_lb_a);
 usleep(100* 1000);//100ms延时
  pca9685_servo_set_angle(port_rf_a, home_rf_b);
  pca9685_servo_set_angle(port_lf_a, home_lb_b);
  usleep(100* 1000);//100ms延时
  }
}

// 左移
void leftturn(unsigned int step){
  while (step-- > 0){
  pca9685_servo_set_angle(port_rf_a, home_rf_b+30);
  pca9685_servo_set_angle(port_lf_a, home_lb_b+30);
  usleep(100* 1000);//100ms延时
  pca9685_servo_set_angle(port_rf_b, home_rf_a+30);
  pca9685_servo_set_angle(port_rb_b, home_rb_a-30);
  pca9685_servo_set_angle(port_lb_b, home_lf_a-30);
  pca9685_servo_set_angle(port_lf_b, home_lb_a+30);
  usleep(100* 1000);//100ms延时
  pca9685_servo_set_angle(port_rf_a, home_rf_b);
  pca9685_servo_set_angle(port_lf_a, home_lb_b);
  usleep(100* 1000);//100ms延时
  
  pca9685_servo_set_angle(port_lb_a, home_lf_b-30);
  pca9685_servo_set_angle(port_rb_a, home_rb_b-30);
  usleep(100* 1000);//100ms延时
  pca9685_servo_set_angle(port_rf_b, home_rf_a);
  pca9685_servo_set_angle(port_rb_b, home_rb_a);
  pca9685_servo_set_angle(port_lb_b, home_lf_a);
  pca9685_servo_set_angle(port_lf_b, home_lb_a);
  usleep(100* 1000);//100ms延时
  pca9685_servo_set_angle(port_lb_a, home_lf_b);
  pca9685_servo_set_angle(port_rb_a, home_rb_b);
  usleep(100* 1000);//100ms延时
  }
}

void dog_execAction(dog_action_t *action) {

    for (int i = 0; i < action->exec_count; ++i) {
        for (int j = 0; j < action->len; ++j) {
            dog_cmd_t cmd = action->cmds[j];
            int duration = cmd.duration;

            int delay_counter = duration / BASE_DELAY;
            for (int k = 0; k < delay_counter; ++k) {
                int v[8] = {0};
                for (int l = 0; l < 8; ++l) {
                    int current = current_angle[l];
                    int dst = cmd.angle[l];

                    v[l] = current;
                    if (current > dst) {
                        int value = map(BASE_DELAY * k, 0, duration, 0, current - dst);

                        if (current - value >= dst) {
                            v[l] = current - value;
                        }
                    } else if (current < dst) {
                        int value = map(BASE_DELAY * k, 0, duration, 0, dst - current);

                        if (current + value <= dst) {
                            v[l] = current + value;
                        }
                    }
                }

                dog_setLeftFrontAngle(v[0], v[1]);
                dog_setLeftBackAngle(v[2], v[3]);
                dog_setRightFrontAngle(v[4], v[5]);
                dog_setRightBackAngle(v[6], v[7]);
                usleep(BASE_DELAY * 1000);
            }
        }
    }

}

void dog_execJson(char *buf, int len) {
    cJSON *recv_json;
    recv_json = cJSON_ParseWithLength(buf, len);

    int type = cJSON_GetObjectItem(recv_json, "type")->valueint;
    if (type == 1) {
        stand1();
    } else if (type == 2) {
        // int index = cJSON_GetObjectItem(recv_json, "index")->valueint;

        // cJSON *data_json;
        // data_json = cJSON_GetObjectItem(recv_json, "data");
        // int alpha = cJSON_GetArrayItem(data_json, 0)->valueint;
        // int beta = cJSON_GetArrayItem(data_json, 1)->valueint;

        // if (index == 0) {
        //     dog_setLeftFrontAngle(alpha, beta);
        // } else if (index == 1) {
        //     dog_setLeftBackAngle(alpha, beta);
        // } else if (index == 2) {
        //     dog_setRightFrontAngle(alpha, beta);
        // } else if (index == 3) {
        //     dog_setRightBackAngle(alpha, beta);
        // }
    } else if (type == 3) {
        // int execute_count = 1;
        // if (cJSON_HasObjectItem(recv_json, "count")) {
        //     execute_count = cJSON_GetObjectItem(recv_json, "count")->valueint;
        // }

        // dog_action_t *action = malloc(sizeof(dog_action_t));
        // action->exec_count = execute_count;

        // // 发送多条
        // cJSON *list_json;
        // list_json = cJSON_GetObjectItem(recv_json, "list");
        // int count = cJSON_GetArraySize(list_json);

        // action->len = count;
        // action->cmds = malloc(sizeof(dog_cmd_t) * count);

        // for (int i = 0; i < count; i++) {
        //     cJSON *item_json;
        //     item_json = cJSON_GetArrayItem(list_json, i);

        //     dog_cmd_t cmd;
        //     cmd.angle[0] = cJSON_GetArrayItem(item_json, 0)->valueint;
        //     cmd.angle[1] = cJSON_GetArrayItem(item_json, 1)->valueint;
        //     cmd.angle[2] = cJSON_GetArrayItem(item_json, 2)->valueint;
        //     cmd.angle[3] = cJSON_GetArrayItem(item_json, 3)->valueint;
        //     cmd.angle[4] = cJSON_GetArrayItem(item_json, 4)->valueint;
        //     cmd.angle[5] = cJSON_GetArrayItem(item_json, 5)->valueint;
        //     cmd.angle[6] = cJSON_GetArrayItem(item_json, 6)->valueint;
        //     cmd.angle[7] = cJSON_GetArrayItem(item_json, 7)->valueint;
        //     cmd.duration = cJSON_GetArrayItem(item_json, 8)->valueint;

        //     action->cmds[i] = cmd;
        // }
        // dog_execAction(action);
            twist();
        usleep(1000* 1000);//一秒延时
        sithome();
        usleep(1000* 1000);//一秒延时

        initial();
        usleep(1000* 1000);//一秒延时

        stand2();
        usleep(1000* 1000);//一秒延时

        stand3();
        usleep(1000* 1000);//一秒延时

        wink(10);
        usleep(1000* 1000);//一秒延时
        }
    cJSON_Delete(recv_json);
}

void dog_leg_move(int index, double x, double y) {
    struct Point_t point = {x, y};
    double *angle = inverse(point);

    if (angle) {
        double alpha = angle[0];
        double beta = angle[1];
        dog_leg_angle(index, alpha, beta);
    } else {
        printf("inverse failed!\n");
    }
    free(angle);
}

void dog_leg_angle(int index, double alpha, double beta) {
    if (index == 0) {
        dog_setLeftFrontAngle((int) alpha, (int) beta);
    } else if (index == 1) {
        dog_setLeftBackAngle((int) alpha, (int) beta);
    } else if (index == 2) {
        dog_setRightFrontAngle((int) alpha, (int) beta);
    } else if (index == 3) {
        dog_setRightBackAngle((int) alpha, (int) beta);
    }
}